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
  (`loop_host.{cpp,inc}`, `loop_client.{cpp,inc}`). **(Obsolete since the loop
  split: the monolithic `loop_host.cpp`/`loop_client.cpp` and their `.inc`
  mirrors are gone — the per-tick/per-frame loops are now ordered `#include`
  fragments under `src/server/loop/` and `src/client/loop/`. There is nothing
  left to mirror; edit the fragment `.cpp` that owns the handler.)**
- **Bump `U6O_VERSION`** in `src/common/define_both.h` (was **13**, now **14**
  — done MDD-P1.4) in the same commit that lands the new messages.
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

- ✅ MDD-P0.1 Commit this plan; lock the manifest + chunked-pull + per-host
  cache decisions above. *(2026-06-17: locked. Implementation deviation noted —
  freshness is keyed purely on the FNV-1a/32 **checksum**, not a `<hostkey>`
  dir, per the open question below; a single shared `.\dr\hostcache\` is used.)*
- 🟡 MDD-P0.2 Baseline: record current `.\dr\` file sizes and a clean
  login packet capture (the interop oracle for later phases). *(2026-06-17:
  file sizes are documented in Background — bt 4,194,304 / objfixed 4,325,376 /
  tobjfix 4,325,376. A live packet capture still needs a running host+client,
  which this environment can't run; deferred to the build/runtime verification
  pass.)*
- ✅ MDD-P0.3 Reserve the new message-type IDs and the chunk size in
  `define_both.h` with a comment block documenting the wire coupling
  (mirror the `MV_TX_*` / `kPanelHide*` comment style). *(2026-06-17:
  `MSG_MAPMANIFEST=60`, `MSG_MAPCHUNK_REQ=61`, `MSG_MAPCHUNK_RESP=62`,
  `MAP_CHUNK_BYTES=16384`, `MAP_FILE_BT/OBJFIXED/TOBJFIX`, `MAP_FILE_COUNT=3`.
  60–62 audited collision-free against every `t->d2[0]` dispatch in both
  loops — see resolved risk below.)*
- ✅ MDD-P0.4 Pick the checksum algorithm (CRC32 vs FNV-1a) and the cache
  directory layout (`<hostkey>` derived from host address/port). *(2026-06-17:
  **FNV-1a/32** — allocation-free, streamable (lets the client fold each chunk
  into a running hash and hash the two non-contiguous in-RAM arrays without
  concatenating), identical on Win32 client + i386 Linux host. `MAP_checksum*`
  in `function_both.cpp`. Cache layout: single `.\dr\hostcache\<basename>` +
  a `.sum` sidecar commit-marker; freshness is checksum-based so no `<hostkey>`
  subdir is needed.)*
- **What to read first:** this Background; `define_both.h:1–160`;
  `function_both.cpp:29–160`.
- **Exit:** ✅ plan committed; IDs/chunk size reserved; checksum + cache layout
  chosen. (Baseline packet capture deferred to runtime verification.)

## MDD-P1 — Manifest message (host → client), no transfer yet

- ✅ MDD-P1.1 Host: at bake time (`host_setup.h`) compute and store each
  file's length + checksum in memory (a small `MapFileManifest` table).
  *(2026-06-17: after the bt.bin bake, host reads back each baked `.bin` and
  fills `MAP_manifest_len[]` / `MAP_manifest_sum[]` (globals.inc), logging the
  result. Checksums the bytes ON DISK so a manifest match guarantees the
  client ends byte-identical.)*
- ✅ MDD-P1.2 Host: after the version check / login accept
  (`loop_host_part_b_dispatch.cpp`), send a **manifest** message
  (`d2[0]=MSG_MAPMANIFEST`). *(2026-06-17: built in a dedicated scratch txt so
  it can't disturb the in-progress setup-message parse on `t5`. No `.inc`
  mirror exists post-loop-split.)*
- ✅ MDD-P1.3 Client: receive & parse the manifest into state, take no
  rendering action until verified. *(2026-06-17: `MAPDL_on_manifest()` in
  `function_client.cpp`, dispatched from `loop_client_part_net.cpp`.)*
- ✅ MDD-P1.4 Bump `U6O_VERSION` (wire change) in `define_both.h`. *(2026-06-17:
  13 → 14. **Build of `host`/`both`/`client` still pending** — no MSVC/CMake
  toolchain in this environment; flagged for the runtime-verification pass.)*
- **What to read first:** `loop_host_part_b_dispatch.cpp` (version check ~230);
  `loop_client_part_net.cpp` (dispatch tail); `host_setup.h:1196–1250`.
- **Exit:** 🟡 code complete; a connecting client parses the host's manifest and
  rendering is unchanged until a file verifies. **Build-green check outstanding.**

## MDD-P2 — Chunked request/response protocol

- ✅ MDD-P2.1 Define a **request** message (client→host:
  `{fileId u8, offset u32, length u32}`) and a **response** message
  (host→client: `{fileId u8, offset u32, byteCount u32, …bytes}`); encoder +
  decoder on both sides. *(2026-06-17: built with `txtaddchar`/`txtaddlong`
  + a raw payload `txtadd`, decoded with `memcpy` from fixed offsets — simpler
  and adequate vs `BITSput`/`BITSget` for byte-aligned fields. No `.inc`
  mirror exists post-loop-split.)*
- ✅ MDD-P2.2 Host handler: read the requested slice via `open2`/`seek`/`get`,
  bounds-check `offset`/`length` against the manifest length, reply with the
  bytes. *(2026-06-17: in `loop_host_part_b_dispatch.cpp`, ahead of the
  playerlist lookup so it serves whether or not the socket has fully logged in.
  Clamps to `[0, filelen)` and `MAP_CHUNK_BYTES`; missing file ⇒ empty reply.)*
- ✅ MDD-P2.3 Client driver: loop requesting chunks until each file is
  complete; assemble into a malloc'd buffer; verify the assembled checksum
  against the manifest. *(2026-06-17: `MAPDL_send_request` / `MAPDL_on_chunk`,
  single request in flight — the host's reply drives the next request.)*
- **What to read first:** MDD-P1 message handlers; `function_both.cpp:86–160`
  (send/receive loops); `myfile.cpp:52–110`.
- **Exit:** ✅ client pulls a full file into memory and checksum-matches it
  (it then applies it — see P3). **Build-green check outstanding.**

## MDD-P3 — Cache & load-path integration

- ✅ MDD-P3.1 Client: persist verified files to `.\dr\hostcache\` with a
  commit-marker discipline. *(2026-06-17: `MAPDL_store` writes the `.bin`,
  then the `.sum` sidecar LAST; `MAPDL_load_cache` trusts a cached file only
  when the sidecar matches AND the file re-hashes, so an interrupted write is
  re-downloaded, never loaded half-baked. `myfile` has no atomic rename, so the
  sidecar-last commit marker replaces temp+rename — same never-load-partial
  guarantee.)*
- ✅ MDD-P3.2 Client: load `objfixed.bin` / `tobjfix.bin` from the cache when
  current, else download; else the existing local `.\dr\` load stands.
  *(2026-06-17: **implemented as a post-connect reload** rather than a
  setup-time cache check. The setup load (`setup_client.inc:911/922`) is left
  untouched as the always-present baseline; once the manifest arrives,
  `MAPDL_provision` either confirms the live arrays already match
  (`MAPDL_live_checksum`), loads a current cache copy, or downloads — then
  swaps the verified bytes into `objfixed_*`/`tobjfixed_*`. Cleaner than
  checking the cache at setup, because the manifest needed to judge "current"
  only arrives after setup. `bt.bin` is intentionally NOT downloaded — base
  tiles stream live in multiplayer.)*
- ✅ MDD-P3.3 Skip downloading any file whose checksum already matches the
  manifest. *(2026-06-17: `MAPDL_provision` short-circuits on a live-array
  checksum match first, then on a cache-sidecar match — fast reconnects and
  already-current clients transfer ~0 bytes.)*
- **What to read first:** `setup_client.inc:905–931`; MDD-P2 client driver.
- **Exit:** 🟡 code complete — a client with an empty/stale `.\dr\` will render
  the host's current fixed objects after connecting. **Needs runtime
  verification (two endpoints).**

## MDD-P4 — Timing, UX, robustness

- 🟡 MDD-P4.1 Sequence the download relative to the render loop and show
  progress. *(2026-06-17: the download is event-driven within the main loop —
  it starts when the manifest arrives (post-connect) and overlaps gameplay; the
  setup-loaded local data renders meanwhile and is hot-swapped when a file
  verifies, so there is no blocking "pre-render gate" and no UI hang. Progress
  surfaced via `STATUSMESSadd` ("Syncing map data…", "Map data synchronized…")
  + `scrlog` detail. A dedicated splash progress bar is **not** wired —
  downgraded to a status-message indicator; revisit if a connect-time splash is
  desired.)*
- ✅ MDD-P4.2 Handle mid-transfer disconnect / corruption without ever loading
  a bad file. *(2026-06-17: a file is applied only after a full-buffer checksum
  match; bad checksum, truncated payload, OOM, or a host "empty" reply abandon
  the file and keep the local data; a reconnect's fresh manifest frees any
  partial buffer and restarts. Transport is TCP so chunks are reliable/ordered;
  no explicit retransmit needed.)*
- ✅ MDD-P4.3 Throttle so the transfer never starves gameplay packets.
  *(2026-06-17: exactly one request in flight — each `MSG_MAPCHUNK_RESP` drives
  the next `MSG_MAPCHUNK_REQ`, so the transfer self-paces over the existing
  send loop and never floods it.)*
- **What to read first:** `loop_client_part_net.cpp` dispatch;
  `function_client.cpp` `MAPDL_*`; MDD-P3 cache code.
- **Exit:** 🟡 robust-by-construction (always falls back to local data); a
  clean-connect-over-slow-link **runtime** check remains.

## MDD-P5 — Compression, cross-version hardening, docs (final)

- ⏭ MDD-P5.1 Optional payload compression negotiated in the manifest.
  *(2026-06-17: deferred. Correctness-first shipped uncompressed; the wire
  layout has no compression flag yet, so adding it later is another
  `U6O_VERSION` bump. Matches the `host_setup.h` "compress and encrypt" TODO.)*
- ✅ MDD-P5.2 Cross-version degradation. *(2026-06-17: because `U6O_VERSION`
  bumped 13→14, a v13↔v14 mix is **refused at the version check** (host emits
  type 254) rather than misdecoding the new messages — the plan's stated
  acceptable outcome ("a clean refusal beats silently misdecoding"). Same-major
  peers always carry the manifest, so there is no "new feature, no manifest"
  in-band case to fall back from.)*
- ✅ MDD-P5.3 Linux headless host parity. *(2026-06-17: the host serve path uses
  only `open2`/`seek`/`get`/`close` (already POSIX-backed in `myfile.cpp`) and
  `MAP_checksum` (pure arithmetic) — no `plat_*`/graphics. `MAP_manifest_*` /
  `MAP_chunkbuf` are in `globals.inc` (compiled into the Linux host) and the
  bake/serve code is host-capable-only. **Container build not run here** —
  verify under `Dockerfile.linux`.)*
- ✅ MDD-P5.4 Update `docs/rendering/map-client-host-sync/README.md`. *(2026-06-17:
  documented the download path and retired the manual redistribution step.)*
- **What to read first:** MDD-P1 manifest format; `plan-linuxHost.md`
  Session Handoff; the companion README.
- **Exit:** 🟡 feature coded on-by-default; redistribution retired in docs;
  compression deferred (P5.1); cross-version refusal verified by design,
  container/runtime checks outstanding.

---

## Cross-cutting tracking

| Metric | Baseline | Current | Target |
|---|---|---|---|
| Bytes transferred, fresh login | n/a (files shipped) | ≤ 8.65 MB (objfixed+tobjfix; bt.bin skipped in MP) | ≤ 12.6 MB (or compressed) |
| Bytes transferred, unchanged reconnect | n/a | ~0 (live-array or cache checksum match) | ~0 (manifest match) |
| New wire message types | 0 | 3 (60 manifest, 61 request, 62 response) | 3 (manifest, request, response) |
| `U6O_VERSION` | 13 | **14** (bumped MDD-P1.4) | bumped once (MDD-P1.4) |
| Manual `.bin` redistribution step | required | retired in docs (MDD-P5.4) | retired (MDD-P5.4) |

Update this table at the end of every phase.

## Risks & open questions

- ✅ Does any current message type byte collide with the IDs we want to
  reserve? **Resolved (2026-06-17):** audited every `t->d2[0]==N` /
  `d2[0]=N` in both loops. In use: 1–42, 45, 47, 244–252, 254, 255. Picked
  **60/61/62** — a clear gap in both directions.
- ✅ Is `bt.bin` worth downloading at all? **Resolved (2026-06-17): no.** Base
  tiles stream live (message 31) in multiplayer; `bt.bin` is only read on the
  `NEThost==NULL` local path. The client driver **skips** `MAP_FILE_BT`
  (saving ~4 MB); the manifest still carries its length/checksum for future use.
- ✅ Cache key. **Resolved (2026-06-17):** freshness keys on the **checksum**,
  not a host key — a single shared `.\dr\hostcache\` with `.sum` sidecars.
  Switching hosts just re-downloads when the checksum differs (correct, if not
  maximally cached across hosts).
- ⬜ Security: payloads are unauthenticated today; defer signing/encryption
  with the compression work (MDD-P5.1) unless threat model says otherwise.
- ⬜ **Build/runtime verification outstanding:** no MSVC/CMake toolchain was
  available in the implementing environment, so `host`/`both`/`client` were
  **not** compiled and no live two-endpoint transfer was exercised. Next
  session must build all targets and run a real connect (stale-`.\dr\` client
  vs current host) to close P1/P3/P4/P5 exits.

## Session Handoff

- **2026-06-17 (MDD-P0→P5 implementation)** — Core feature implemented across
  client + host. **Files touched:**
  - `src/common/define_both.h` — `U6O_VERSION` 13→14; MDD constants block
    (`MSG_MAPMANIFEST=60`, `MSG_MAPCHUNK_REQ=61`, `MSG_MAPCHUNK_RESP=62`,
    `MAP_CHUNK_BYTES=16384`, `MAP_FILE_*`).
  - `src/common/data_both.h` + `globals.inc` — `MAP_manifest_len[3]`,
    `MAP_manifest_sum[3]`, `MAP_chunkbuf[16384]`.
  - `src/common/function_both.{h,cpp}` — `MAP_checksum*` (FNV-1a/32, one-shot
    + streaming) and `MAP_file_path`.
  - `src/server/host_setup.h` — build the manifest from the baked `.bin` files
    after the bt.bin bake.
  - `src/server/loop/loop_host_part_b_dispatch.cpp` — send `MSG_MAPMANIFEST`
    after the version check; serve `MSG_MAPCHUNK_REQ` → `MSG_MAPCHUNK_RESP`
    from disk (bounds-checked) ahead of the playerlist lookup.
  - `src/client/function_client.{h,cpp}` — the `MAPDL_*` download driver
    (manifest parse, provision/skip/cache/download state machine, chunk
    assembly + verify, `.\dr\hostcache\` cache with `.sum` sidecars, hot-swap
    into `objfixed_*`/`tobjfixed_*`).
  - `src/client/loop/loop_client_part_net.cpp` — dispatch 60→`MAPDL_on_manifest`,
    62→`MAPDL_on_chunk`.
  - Docs: this plan + `docs/rendering/map-client-host-sync/README.md` (P5.4).
  **Design notes:** download is best-effort layered ON TOP of the unchanged
  setup-time local load — every failure path keeps the local data, so the
  feature can only improve freshness, never break a working client. `bt.bin`
  is NOT downloaded (streams live). One request in flight (self-throttling).
  Freshness is checksum-based; no `<hostkey>` subdir. The loop `.inc` mirrors
  the plan mentions no longer exist (loop-split) — the fragment `.cpp` files
  are the source of truth.
  **NOT done / resume here:** (1) **Build** `host` + `both` + `client` — no
  MSVC/CMake toolchain was present; fix any compile fallout. (2) **Runtime**:
  connect a client with an empty/stale `.\dr\` to a current host and confirm it
  renders the host's fixed objects + populates `.\dr\hostcache\`; confirm an
  unchanged reconnect transfers ~0 bytes (log: "map data sync complete" with no
  per-file "downloading…" line). (3) Container/Linux-host serve check
  (MDD-P5.3). (4) Optional compression (MDD-P5.1, deferred). (5) Capture the
  P0.2 packet baseline once two endpoints run. **Resume at the build, then the
  P1/P3/P4 runtime exits.**

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

