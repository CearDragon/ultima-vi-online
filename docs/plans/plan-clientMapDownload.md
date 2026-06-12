# Client Map-Data Download Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before starting the next phase.

> Source of truth: this file. Reference phase IDs in PR titles/descriptions
> (e.g. "MDD-P2.1: add chunked map-file request/response messages").
>
> When resuming work in a new session/context window: read **§ Session
> Handoff** at the bottom first, then jump to the lowest-numbered phase
> that is not ✅. Every phase has a "What to read first" list so context
> can be rebuilt cheaply.
>
> Companion doc: [`docs/rendering/map-client-host-sync/README.md`](../rendering/map-client-host-sync/README.md)
> explains the current pipeline this plan replaces.

---

## Goal

Have the client **download the baked map-data files from the host at
connect time** instead of relying on whatever copies happen to ship in its
local `.\dr\` folder:

- `objfixed.bin` — fixed/static objects (always loaded locally today; **not**
  streamed — the staleness trap).
- `tobjfix.bin` — "top"/transparent fixed objects (same).
- `bt.bin` — base tiles (only loaded locally in the no-host path).

After this work, a client that connects to a host always renders **that
host's current** static geometry, eliminating the manual "regenerate and
redistribute the `.bin` files" step and the class of bug where a client with
a stale `.\dr\` draws map geometry the host no longer has (the shop that
"won't disappear").

## Non-goals

- Not changing how the host **builds** the world (`house()` +
  `assets/map_patches/*.txt` stay as-is).
- Not changing the **live** streaming of base tiles (scene-update message
  type `31`) or dynamic objects (`sobj`/`mover` windows).
- Not removing the **local-file fallback**: single-player / no-host
  (`NEThost==NULL`) must keep reading `.\dr\*.bin` as today
  (`setup_client.inc:919, 924, 930`).
- Not redesigning the socket/thread layer (`function_both.cpp`); we add a new
  message type on top of the existing `txt`-based framing.

## Wire-change notice (read before any code)

This feature **adds new wire message types**, so per
`.github/copilot-instructions.md` it **is** a wire change:

- Encoder **and** decoder change together in **both** client and host.
- Mirror every change into the `.inc` copy of each `.cpp` that has one
  (`loop_host.{cpp,inc}`, `loop_client.{cpp,inc}`).
- **Bump `U6O_VERSION`** in `src/common/define_both.h` (currently **13**) in
  the same commit that lands the new messages.
- Old↔new mixes must degrade gracefully (see MDD-P5.2), not misdecode.

---

## Background — current state (captured 2026-06-12)

**The three files and their sizes** (so chunking math is grounded):

| File | Contents | Bytes |
|---|---|---|
| `bt.bin` | `bt[1024][2048]` (u16) | `1024*2048*2` = 4,194,304 |
| `objfixed.bin` | `objfixed_index[2048][1024]` (u16) + `objfixed_type[65536]` (u16) | `2048*1024*2 + 65536*2` = 4,325,376 |
| `tobjfix.bin` | `tobjfixed_index[2048][1024]` (u16) + `tobjfixed_type[65536]` (u16) | 4,325,376 |

Total ≈ **12.6 MB** uncompressed — too big for one message; must be chunked,
and ideally only re-sent when changed.

**Where the host bakes them** (`src/server/host_setup.h`, right after
`house()` at `setup_host.inc:251`):

- `objfixed.bin` write — lines ~1067–1079.
- `tobjfix.bin` write — lines ~1133–1144.
- `bt.bin` write — lines ~1188–1199 (comment already says
  "later we will compress and encrypt this").

**Where the client loads them** (`src/client/setup_client.inc`):

- `objfixed.bin` — line 919 (**always**).
- `bt.bin` — lines 922–928 (**only** when `NEThost==NULL`).
- `tobjfix.bin` — line 930 (**always**).

**The connect handshake** (where the download will be sequenced):

- Client sends a setup message `t->d2[0]=255` carrying `U6O_VERSION`
  (`loop_client.cpp:4183–4186`).
- Host validates it: `if (t5->ds[0] != U6O_VERSION) { reject }`
  (`loop_host.cpp:4307–4314`).
- After acceptance the host streams the initial world; the manifest +
  download slot in right here.

**Framing & I/O primitives to reuse:**

- Message framing: `txt` objects with a `d2[0]` type byte; bit-packing via
  `BITSput`/`BITSget` (`function_both.cpp:29`); transport via `sockets_send`
  (`:86`) / `sockets_receive` (`:113`).
- File I/O: `open2()` (`myfile.cpp:52`), `get()` (`:96`), `put()` (`:101`),
  declared in `myfile.h` — already portable to the Linux host.

## Key design decisions

1. **Manifest-then-chunks, pull model.** Host sends a small **manifest**
   (`{fileId, length, checksum}` per file). Client compares to its cached
   copy and **pulls only changed files** in fixed-size chunks. Avoids
   re-sending 12.6 MB on every login.
2. **Checksum for freshness + integrity.** A cheap content hash
   (CRC32 or FNV-1a over the file bytes) computed host-side at bake time and
   client-side on the cached copy. Equal hash ⇒ skip download.
3. **Per-host cache, local fallback preserved.** Downloads land in a
   host-scoped cache dir (e.g. `.\dr\hostcache\<hostkey>\`). The shipped
   `.\dr\*.bin` remain the no-host / first-run fallback.
4. **Chunked & interleaved.** Chunk size ~32–48 KB so a transfer never blocks
   the gameplay packet stream; the client requests the next chunk only after
   the previous arrives (flow-controlled by the existing send/receive loop).
5. **Compression optional, deferred.** Negotiated in the manifest; matches the
   existing host TODO. Ship correctness first (MDD-P1–P4), compress in P5.
6. **Graceful cross-version degradation.** Whole feature gated on
   `U6O_VERSION`. No manifest ⇒ client uses shipped local files exactly as
   today; new host ⇒ old client simply never asks.
7. **Headless host parity.** The Linux host already writes the `.bin` files;
   serving bytes is graphics-free, so no `plat_*` work beyond file reads.

---

## MDD-P0 — Design lock-in & safety net

- ⬜ MDD-P0.1 Commit this plan; lock the manifest + chunked-pull + per-host
  cache decisions above.
- ⬜ MDD-P0.2 Baseline: record current `.\dr\` file sizes and a clean
  login packet capture (the interop oracle for later phases).
- ⬜ MDD-P0.3 Reserve the new message-type IDs and the chunk size in
  `define_both.h` with a comment block documenting the wire coupling
  (mirror the `MV_TX_*` / `kPanelHide*` comment style).
- ⬜ MDD-P0.4 Pick the checksum algorithm (CRC32 vs FNV-1a) and the cache
  directory layout (`<hostkey>` derived from host address/port).
- **What to read first:** this Background; `define_both.h:1–160`;
  `function_both.cpp:29–160`.
- **Exit:** plan committed; IDs/chunk size reserved; baseline captured;
  checksum + cache layout chosen.

## MDD-P1 — Manifest message (host → client), no transfer yet

- ⬜ MDD-P1.1 Host: at bake time (`host_setup.h`) compute and store each
  file's length + checksum in memory (a small `MapFileManifest` table).
- ⬜ MDD-P1.2 Host: after the version check / login accept
  (`loop_host.cpp` ~4307), send a **manifest** message
  (`d2[0]=<MAPMANIFEST>`); mirror into `loop_host.inc`.
- ⬜ MDD-P1.3 Client: receive & parse the manifest into a struct, log it,
  take no rendering action yet; mirror into `loop_client.inc`.
- ⬜ MDD-P1.4 Bump `U6O_VERSION` (wire change) in `define_both.h`; rebuild
  `host`, `both`, and `client`.
- **What to read first:** `loop_host.cpp:4290–4350`;
  `loop_client.cpp:4176–4220`; `host_setup.h:1060–1200`.
- **Exit:** a connecting client logs the host's manifest; rendering unchanged;
  build green on all targets.

## MDD-P2 — Chunked request/response protocol

- ⬜ MDD-P2.1 Define a **request** message (client→host:
  `{fileId, offset, length}`) and a **response** message (host→client:
  `{fileId, offset, byteCount, …bytes}`) using `BITSput`/`BITSget` plus a raw
  payload; add encoder+decoder on both sides, both `.cpp` and `.inc`.
- ⬜ MDD-P2.2 Host handler: read the requested slice via `open2`/`get`
  (`myfile.cpp`), bounds-check `offset`/`length` against the manifest length,
  reply with the bytes.
- ⬜ MDD-P2.3 Client driver: loop requesting chunks until each file is
  complete; assemble into a temp buffer; verify the assembled checksum against
  the manifest.
- **What to read first:** MDD-P1 message handlers; `function_both.cpp:86–160`
  (send/receive loops); `myfile.cpp:52–110`.
- **Exit:** client can pull a full file into memory and checksum-match it
  (behind a debug flag); no rendering wired yet.

## MDD-P3 — Cache & load-path integration

- ⬜ MDD-P3.1 Client: write verified files atomically to
  `.\dr\hostcache\<hostkey>\` (temp file + rename; never load a half-written
  file).
- ⬜ MDD-P3.2 Client setup: load `objfixed.bin` / `tobjfix.bin` (and `bt.bin`)
  from the cache when present **and** manifest-current; else fall back to the
  shipped `.\dr\` files at `setup_client.inc:919 / 924 / 930`.
- ⬜ MDD-P3.3 Skip downloading any file whose cached checksum already matches
  the manifest (fast reconnects → ~0 bytes transferred).
- **What to read first:** `setup_client.inc:905–931`; MDD-P2 client driver.
- **Exit:** a client with an **empty or stale** `.\dr\` renders the host's
  current fixed objects correctly after connecting.

## MDD-P4 — Timing, UX, robustness

- ⬜ MDD-P4.1 Sequence the download **before** the fixed-object load /
  main render loop; show a "Syncing map data…" progress indicator
  (reuse the splash/status surface).
- ⬜ MDD-P4.2 Handle mid-transfer disconnect / timeout with retry and
  partial-file cleanup; corrupt or incomplete files are discarded, never
  loaded.
- ⬜ MDD-P4.3 Interleave/throttle chunk requests so the transfer never starves
  gameplay packets on the existing send loop.
- **What to read first:** `loop_client.cpp` connect/setup region;
  `splash.{h,cpp}`; MDD-P3 cache code.
- **Exit:** clean connect over a slow link; no UI hang; resilient to drops.

## MDD-P5 — Compression, cross-version hardening, docs (final)

- ⬜ MDD-P5.1 Optional payload compression negotiated in the manifest
  (gzip or simple RLE), matching the `host_setup.h` "compress and encrypt"
  TODO.
- ⬜ MDD-P5.2 Cross-version matrix: old client ↔ new host and new client ↔ old
  host both fall back to shipped local files (no manifest ⇒ no download).
  Add the result to a version-compat table here.
- ⬜ MDD-P5.3 Linux headless host parity: serve bytes with no graphics; verify
  in the container build (`Dockerfile.linux`).
- ⬜ MDD-P5.4 Update `docs/rendering/map-client-host-sync/README.md`: document
  the download path and retire the manual "redistribute the `.bin` files"
  step.
- **What to read first:** MDD-P1 manifest format; `plan-linuxHost.md`
  Session Handoff; the companion README.
- **Exit:** feature on by default; manual redistribution retired; docs updated;
  cross-version matrix green.

---

## Cross-cutting tracking

| Metric | Baseline | Current | Target |
|---|---|---|---|
| Bytes transferred, fresh login | n/a (files shipped) | TBD | ≤ 12.6 MB (or compressed) |
| Bytes transferred, unchanged reconnect | n/a | TBD | ~0 (manifest match) |
| New wire message types | 0 | TBD | 3 (manifest, request, response) |
| `U6O_VERSION` | 13 | 13 | bumped once (MDD-P1.4) |
| Manual `.bin` redistribution step | required | required | retired (MDD-P5.4) |

Update this table at the end of every phase.

## Risks & open questions

- ⬜ Does any current message type byte collide with the IDs we want to
  reserve? (Audit `t->d2[0]==N` dispatch in both loops before MDD-P0.3.)
- ⬜ Is `bt.bin` worth downloading at all, given base tiles stream live in
  multiplayer? (It is only read when `NEThost==NULL`; may be **client-cache
  only** for the local path, or skipped in MDD-P3 to save ~4 MB.)
- ⬜ Cache key: host IP+port is simplest, but a host that rebuilds its map
  keeps the same address — rely on the **checksum**, not the key, for
  freshness.
- ⬜ Security: payloads are unauthenticated today; defer signing/encryption
  with the compression work (MDD-P5.1) unless threat model says otherwise.

## Session Handoff

- **2026-06-12 (MDD-P0 draft)** — Plan created from the
  `map-client-host-sync` investigation. Pipeline confirmed: host bakes
  `bt.bin`/`objfixed.bin`/`tobjfix.bin` in `host_setup.h` after `house()`;
  client loads `objfixed`/`tobjfix` **unconditionally** and `bt.bin` only when
  `NEThost==NULL` (`setup_client.inc:919/924/930`); fixed objects are **not**
  streamed, which is the staleness bug this plan removes. Handshake slot
  identified: client setup msg `d2[0]=255`+`U6O_VERSION`
  (`loop_client.cpp:4186`), host check (`loop_host.cpp:4307`). Framing
  (`BITSput`/`BITSget`, `sockets_send/receive` in `function_both.cpp`) and
  file I/O (`open2`/`get`/`put` in `myfile.cpp`) are reusable as-is.
  **Resume at MDD-P0.1.**

To pick up cleanly:

1. `git log --grep "MDD-P"` to see what's landed.
2. Scan this file top-to-bottom for the first non-✅ checkbox; that's the
   resume point.
3. Remember the wire-change rule: encoder + decoder, `.cpp` + `.inc` mirror,
   `U6O_VERSION` bump — all in one PR (MDD-P1.4).
4. `loop_host.cpp` is too large for the editor tools; edit it via the
   PowerShell `ReadAllText`/`IndexOf`/`WriteAllText` approach and mirror to
   `loop_host.inc`.

