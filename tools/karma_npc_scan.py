#!/usr/bin/env python3
"""
karma_npc_scan.py - Detect which NPCs / conversation instructions raise or
lower Karma in the Ultima VI Online host data.

Background (see docs/gameplay/karma.md):
  The host loads three files for the conversation VM:
    host/NPC.BIN   - instruction table, each instruction = 4 little-endian
                     int32s {l1=opcode, l2, l3, l4}.
    host/NPCI.BIN  - npci[]: per-NPC first-instruction index (int32 each).
    host/NPC.INF   - text/data blob (string operands index into this).
  An NPC's code starts at npci[n] and runs through the shared instruction
  stream (jumps via opcode 3 / branch targets in l4).

  Karma is the conversation variable KARMA = 65536+15 = 65551 (see CON_gv /
  CON_sv in function_host.cpp). FLG_KARMA = 65536+14 = 65550.

  Opcodes that write a destination operand (l2) - from the VM dispatch in
  loop_host.cpp:
    4  '='   CON_sv(l2, l3)
    9  '+='  CON_sv(l2, gv(l2)+gv(l3))
    10 '-='  CON_sv(l2, gv(l2)-gv(l3))
    11 '*='  CON_sv(l2, gv(l2)*gv(l3))
    12 '/='  CON_sv(l2, gv(l2)/gv(l3))

  So any instruction whose opcode is in {4,9,10,11,12} and whose l2 == 65551
  modifies the player's karma. (Opcode 4 with the FLG_KARMA gate sets it; the
  arithmetic opcodes nudge it.)

Usage:
    python tools/karma_npc_scan.py
"""

import struct
import sys
from pathlib import Path

HOST = Path(__file__).resolve().parent.parent / "assets" / "game_files" / "host" / "host"
NPC_BIN = HOST / "NPC.BIN"
NPCI_BIN = HOST / "NPCI.BIN"
NPCNAME = HOST / "NPCNAME.TXT"

KARMA = 65536 + 15      # 65551
FLG_KARMA = 65536 + 14  # 65550

WRITE_OPS = {4: "=", 9: "+=", 10: "-=", 11: "*=", 12: "/="}


def load_instructions(path: Path):
    data = path.read_bytes()
    n = len(data) // 16
    insns = []
    for i in range(n):
        l1, l2, l3, l4 = struct.unpack_from("<4i", data, i * 16)
        insns.append((l1, l2, l3, l4))
    return insns


def load_npci(path: Path):
    data = path.read_bytes()
    n = len(data) // 4
    return list(struct.unpack_from("<%di" % n, data, 0))


def load_names(path: Path):
    names = []
    if path.exists():
        for line in path.read_text(encoding="latin-1").splitlines():
            names.append(line.strip())
    return names


def fmt_operand(v: int) -> str:
    """Decode a conversation operand for readability."""
    if v == KARMA:
        return "KARMA"
    if v == FLG_KARMA:
        return "FLG_KARMA"
    if (v & 0xFFFFFF00) == 262144:
        return f"GFLAG[{v & 0xFF}]"
    if (v & 0xFFFFFF00) == 131072:
        return f"REG[{v & 0xFF}]"
    if v == 65536 + 4:
        return "RANDOM"
    if v == 65536 + 5:
        return "NUMBER"
    if v >= 65536:
        return f"VAR(65536+{v - 65536})"
    return str(v)


def main():
    insns = load_instructions(NPC_BIN)
    npci = load_npci(NPCI_BIN)
    names = load_names(NPCNAME)

    # Build sorted unique start indices to bound each NPC's instruction span.
    starts = sorted(set(npci))
    next_start = {}
    for i, s in enumerate(starts):
        next_start[s] = starts[i + 1] if i + 1 < len(starts) else len(insns)

    print(f"Loaded {len(insns)} instructions, {len(npci)} NPC entries, "
          f"{len(names)} names.\n")

    total_hits = 0
    per_npc = []

    for npc_id, start in enumerate(npci):
        end = next_start.get(start, len(insns))
        name = names[npc_id] if npc_id < len(names) else f"<npc {npc_id}>"
        hits = []
        for x in range(start, min(end, len(insns))):
            l1, l2, l3, l4 = insns[x]
            if l1 in WRITE_OPS and l2 == KARMA:
                op = WRITE_OPS[l1]
                rhs = fmt_operand(l3)
                # Determine direction where statically knowable.
                direction = "?"
                if l1 == 10:
                    direction = "LOWER"
                elif l1 == 9:
                    direction = "RAISE"
                elif l1 == 4 and isinstance(l3, int) and l3 < 65536:
                    direction = "SET"
                hits.append((x, f"KARMA {op} {rhs}", direction, (l1, l2, l3, l4)))
        if hits:
            per_npc.append((npc_id, name, start, end, hits))
            total_hits += len(hits)

    if not per_npc:
        print("No direct KARMA-write instructions (opcodes 4/9/10/11/12 with "
              "l2==KARMA) found in NPC.BIN.")
    else:
        for npc_id, name, start, end, hits in per_npc:
            print(f"NPC {npc_id:3d}  {name:<18} "
                  f"(instr {start}..{end - 1})")
            for x, desc, direction, raw in hits:
                print(f"    [{x:6d}] {desc:<24} -> {direction:<6} raw={raw}")
            print()

    print(f"TOTAL: {total_hits} karma-write instruction(s) across "
          f"{len(per_npc)} NPC(s).")

    # Also scan the WHOLE instruction table for KARMA writes that may live in
    # shared/!jumped code regions not owned by a single NPC start.
    global_hits = [
        (x, insns[x]) for x in range(len(insns))
        if insns[x][0] in WRITE_OPS and insns[x][1] == KARMA
    ]
    print(f"\nGlobal KARMA-write instructions in NPC.BIN (any region): "
          f"{len(global_hits)}")
    for x, (l1, l2, l3, l4) in global_hits:
        print(f"    [{x:6d}] op={l1}({WRITE_OPS[l1]}) "
              f"KARMA {WRITE_OPS[l1]} {fmt_operand(l3)}  raw={(l1, l2, l3, l4)}")


if __name__ == "__main__":
    sys.exit(main())

