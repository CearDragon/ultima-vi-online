#!/usr/bin/env python3
"""
combat_stats_dump.py - Dump real weapon/armor/monster combat stats from the
host data files so docs can use accurate numbers.

Reads:
  dr/U6SPR.LNK   - sprlnk[]: base-type (0..) -> starting index in obji[] (u16).
  dr/OBJINFO.U6O - obji[]: objectinfo records, 24 bytes each:
                     flags(u32), v1..v8(u16 x8), weight(u32)

Discovers entries by scanning each base type 0..1023, resolving
obji[sprlnk[type]] and classifying via flags/values (no guessed type IDs).
"""
import struct
import sys
from pathlib import Path

DR = Path(__file__).resolve().parent.parent / "assets" / "game_files" / "host" / "dr"
SPRLNK = DR / "U6SPR.LNK"
OBJINFO = DR / "OBJINFO.U6O"
REC = 24

WEAPON_BIT = 8
TWOH_BIT = 16
ARMOR_BITS = 1 + 2 + 4 + 32 + 64  # helm/neck/armour/ring/boots


def load_sprlnk():
    data = SPRLNK.read_bytes()
    return list(struct.unpack_from("<%dH" % (len(data) // 2), data, 0))


def load_obji():
    data = OBJINFO.read_bytes()
    out = []
    for i in range(len(data) // REC):
        flags, v1, v2, v3, v4, v5, v6, v7, v8, weight = struct.unpack_from(
            "<I8HI", data, i * REC)
        out.append(dict(flags=flags, v1=v1, v2=v2, v3=v3, v4=v4, v5=v5,
                        v6=v6, v7=v7, v8=v8, weight=weight))
    return out


def main():
    sprlnk = load_sprlnk()
    obji = load_obji()
    print(f"sprlnk entries={len(sprlnk)}  obji records={len(obji)}")

    weapons, armors, monsters = [], [], []
    for tid in range(min(1024, len(sprlnk))):
        idx = sprlnk[tid]
        if idx == 0 or idx >= len(obji):
            continue
        o = obji[idx]
        f = o["flags"]
        if (f & WEAPON_BIT) and o["v1"] > 0:
            weapons.append((tid, o))
        if (f & ARMOR_BITS) and o["v2"] > 0:
            armors.append((tid, o))
        if o["v4"] and ((o["v8"] & 255) > 0 or (o["v8"] >> 8) > 0):
            monsters.append((tid, o))

    print("\n=== WEAPONS (weapon bit, v1>0) ===")
    print(f"{'type':>5} {'v1dmg':>6} {'v5type':>7} {'2H':>3} {'wt':>6}")
    for tid, o in sorted(weapons, key=lambda t: t[1]["v1"]):
        twoh = "Y" if o["flags"] & TWOH_BIT else ""
        print(f"{tid:5d} {o['v1']:6d} {o['v5']:7d} {twoh:>3} {o['weight']/100:6.1f}")

    print("\n=== ARMOR (armor slot bit, v2>0) ===")
    print(f"{'type':>5} {'v2def':>6} {'slot':>10} {'wt':>6}")
    for tid, o in sorted(armors, key=lambda t: t[1]["v2"]):
        slot = []
        if o["flags"] & 1: slot.append("helm")
        if o["flags"] & 2: slot.append("neck")
        if o["flags"] & 4: slot.append("body")
        if o["flags"] & 32: slot.append("ring")
        if o["flags"] & 64: slot.append("boot")
        print(f"{tid:5d} {o['v2']:6d} {'/'.join(slot):>10} {o['weight']/100:6.1f}")

    print("\n=== MONSTERS (v4 set, has attack/hp) ===")
    print(f"{'type':>5} {'atk':>5} {'hp~':>5} {'dodge':>6} {'armor':>6}")
    for tid, o in sorted(monsters, key=lambda t: (t[1]['v8'] >> 8)):
        print(f"{tid:5d} {o['v8'] & 255:5d} {(o['v8'] >> 8) * 4:5d} "
              f"{o['v6'] >> 8:6d} {o['v7'] >> 8:6d}")


if __name__ == "__main__":
    sys.exit(main())
