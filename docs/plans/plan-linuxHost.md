# Linux Host Portability Plan (Trackable)

Status legend: ⬜ todo · 🟡 in-progress · ✅ done · ⏭ deferred · ❌ blocked
Update the checkbox + add a short note (date, commit, author) when status changes.
Each phase has an **Exit criteria** that must be green before starting the next phase.

> Source of truth: this file. Reference phase IDs in PR titles/descriptions
> (e.g. "LH-P3.2: replace ioctlsocket with fcntl in the accept thread").
>
> When resuming work in a new session/context window: read **§ Session
> Handoff** at the bottom first, then jump to the lowest-numbered phase
> that is not ✅.

---

## Goal

Make the **`host`** binary OS-independent so it builds with GCC/Clang and
runs **headless** in a Linux Docker container on Kubernetes — without a
graphical subsystem, without Win32, and **without changing the wire or
save formats** so existing Windows clients keep connecting to a Linux
host (and vice-versa).

## Non-goals

- Not porting the **client** to Linux (it stays Windows/DirectX-only).
- Not changing gameplay, the authoritative world model, the wire
  protocol, or the on-disk `.sav` formats. **`U6O_VERSION` does NOT
  change in this work** (it is a non-wire effort — see
  `.github/copilot-instructions.md`).
- Not modernizing the host's code style (that's `plan-serverRefactor.md`,
  `P*`). We add the smallest possible portability seams.
- Not removing the Windows/MSVC build. The MSVC path in `CMakeLists.txt`
  must stay byte-for-byte equivalent at every phase.

## Strategy & key decisions

1. **32-bit data model first (`-m32`).** The `txt` union (`d`/`dl`/`ds`/
   `df` in `src/common/mytxt.h`), the per-player `struct player` buffers,
   and the `.sav` files all assume `sizeof(long) == 4` and 32-bit
   pointers (today's `/MACHINE:X86` build). Building the Linux host as
   **i386** preserves struct layout and wire/save compatibility with zero
   field audits. A 64-bit build is a later, optional `LH-P9`.
2. **Thin shim, not a rewrite.** Win32 calls are wrapped behind a small
   `src/common/platform/` layer. On `_WIN32` the shims are pass-throughs
   (the existing Windows headers/behavior — **zero** change to the MSVC
   build). On non-Windows they map to POSIX. The goto-heavy thread procs
   and fixed arrays are kept as-is.
3. **Sever graphics from the host.** The host currently compiles
   `myddraw.cpp` / `frame.cpp` / `dmusic.cpp` and calls `setupddraw()`
   (`src/server/setup_host.inc:31`). The Linux host compiles **none** of
   these; the few host call-sites become headless no-ops.
4. **Keep both clients interoperable.** Acceptance = a Windows client
   connecting to a Linux host with identical behavior to a Windows host.

---

## Background — the Win32 surface the host touches

Captured 2026-06-10 from a full read of the host compile path
(`u6o7.cpp` → `#include "loop_host.cpp"`; `setup_host.inc`; `host.inc`;
`function_host.cpp`; `function_both.cpp`; `myfile.cpp`). Grep the cited
symbols to refresh this list before each phase.

| Area | Symbols / sites | Target mapping |
|---|---|---|
| **Toolchain** | MSVC-only `CMakeLists.txt` (`WIN32`, `/MACHINE:X86`, RC compiler, MASM `.asm` — `both` only); links `ws2_32 winmm user32 gdi32 ddraw dsound dplay dplayx dxguid gdiplus wininet dbghelp` + prebuilt `mylib.lib`, `fdv_dmvr.lib` | GCC/Clang `-m32`; drop import libs; locate/stub the two prebuilt libs |
| **Entry / loop** | `_tWinMain`, `MyRegisterClass`, `LoadAccelerators`, `PeekMessage`/`GetMessage`/`TranslateMessage`/`DispatchMessage`, `ExitProcess` (`u6o7.cpp`) | POSIX `int main()` + headless tick loop |
| **Graphics** | `setupddraw()` (`setup_host.inc:31`, `myddraw.cpp:176`), `hWnd`..`hWnd4` (`data_host.h:269`), `frame`, `dmusic` | compile-out; host call-sites → no-op |
| **Console** | `AllocConsole`, `GetStdHandle`, `_cprintf`, `SetConsoleTitle`, `GetConsoleWindow`, `RemoveMenu` (CONSOLE-gated) | `printf`/`fputs`; drop window calls |
| **Sockets** | `WSAStartup`/`WSACleanup`, `socket`/`bind`/`listen`/`accept`/`setsockopt`, `ioctlsocket(FIONBIO)`, `send`/`recv`/`shutdown`, `closesocket`, `INVALID_SOCKET`, `SOCKET_ERROR`, `WSAGetLastError`, `WSAEWOULDBLOCK` (`function_both.cpp`, `setup_host.inc`, `function_host.cpp`) | BSD sockets shim |
| **Threads** | `CreateThread`/`ExitThread`/`TerminateThread`, `DWORD WINAPI` procs (`sockets_accept`, `sockets_send`, `sockets_receive`, `sockets_disconnect`, `revive_infiniteloopexit`), `HANDLE`/`DWORD` ids | pthreads shim |
| **Timing** | `timeGetTime` (`host.inc:3`), `SleepEx`, `GetLocalTime`+`SYSTEMTIME` (`setup_host.inc`, `u6o7.cpp`) | `clock_gettime`/`nanosleep`/`localtime_r` |
| **File I/O** | `OpenFile`/`_hread`/`_hwrite`/`_llseek`/`_lclose`, `OFSTRUCT`, `HFILE`, `OF_*`, `FILE_BEGIN/CURRENT/END`, `MessageBox` on error (`myfile.cpp`) | `fopen`/`fread`/`fwrite`/`fseek`; errors → `LOGadd` |
| **Crash** | `SetUnhandledExceptionFilter`, `MiniDumpWriteDump`, `dbghelp`, `CaptureStackBackTrace` (`u6o7.cpp`) | POSIX signal handler + `backtrace()` or no-op |
| **Inline asm** | `__asm { fld / fistp }` in `roundfloat` (`function_host.cpp:4764`) | `lrintf` |
| **Misc types** | `DWORD`, `WORD`, `LPVOID`, `LPCSTR`, `HANDLE`, `BOOL`, `ZeroMemory`, `_snprintf`, `MAKEWORD` | `plat_types.h` aliases |

---

## P0 — Strategy lock-in & safety net

- ✅ LH-P0.1 Create this tracked plan; lock the 32-bit / thin-shim / sever-graphics
  decisions above. _(2026-06-10, initial draft.)_
- ⬜ LH-P0.2 Record a baseline: Windows client ↔ Windows host smoke run
  (login, walk, pick up item, save, reconnect). This is the interop oracle
  for every later phase.
- 🟡 LH-P0.3 **Blocker check:** enumerate exactly which symbols the host
  pulls from `mylib.lib` and `fdv_dmvr.lib`. If the host links real code
  from them and no source exists, raise as ❌ before P5. _(See R2.)_
  _(2026-06-10 partial: `fdv_dmvr` is a **DirectMusic** helper reached only
  through `dmusic.cpp` (README L81); it's severed with audio in LH-P5.2, so
  it is **not** a Linux-host blocker. `mylib` is "vendored" with **no
  in-repo source** and only DirectMusic/DX7 headers under `include/`.
  Resolution: the Linux link simply **omits both libs**; any resulting
  undefined symbols are the exact, authoritative audit — reimplement that
  (expected small) set in the platform shim. Definitive symbol list lands
  with the first Linux link in LH-P6.)_
- **Exit:** plan committed; baseline run recorded; prebuilt-lib risk resolved or flagged.

## P1 — Platform shim skeleton (no behavior change)

- ✅ LH-P1.1 Add `src/common/platform/` header layer: `platform.h` (umbrella),
  `plat_types.h`, `plat_sockets.h`, `plat_threads.h`, `plat_time.h`. On
  `_WIN32` every shim is a pass-through to the existing Windows headers.
  _(2026-06-10. Header-only; nothing includes them on the Win32 path yet, so
  zero MSVC impact. POSIX branches validated by inspection only — compiled
  for real in LH-P6.)_
- ✅ LH-P1.2 Prove the seam with one isolated conversion: `roundfloat`
  (`function_host.cpp`) — Windows keeps the `__asm`, non-Windows uses
  `lrintf`. Guarded by `#ifdef _WIN32`. _(2026-06-10. Used
  `__builtin_lrintf` (no extra include); `roundfloat_l` global preserved on
  both paths; `get_errors` shows no new diagnostics at the edit site.)_
- 🟡 LH-P1.3 Map the full Win32-symbol inventory (table above) to shim
  entries; leave call-sites untouched this phase. _(2026-06-10. Sockets,
  base types/macros, threads, and timing mapped in the new headers. Still to
  map: file I/O (`HFILE`/`OF_*` — folded into LH-P2), console (`_cprintf`/
  `SetConsoleTitle`), and the crash/`dbghelp` surface (LH-P5.3).)_
- **Exit:** new headers compile on MSVC with zero diff to host behavior;
  `get_errors` clean on any touched compiled file.

## P2 — Portable file I/O (`myfile.cpp`)

- ⬜ LH-P2.1 Reimplement `open`/`open2`/`get`/`put`/`seek`/`close`/`lof`/
  `loadfile`/`waitforfile`/`deletefile` over `<cstdio>` on non-Windows;
  `HFILE`→portable handle; `OF_*` flags → fopen modes.
- ⬜ LH-P2.2 Route file-not-found to `LOGadd` instead of `MessageBox`.
- ⬜ LH-P2.3 Verify byte-identical `.sav`/`house.sav`/`guardianobjs.sav`
  round-trip vs. a Windows-written fixture.
- **Exit:** host reads/writes the same bytes on both platforms.

## P3 — Portable sockets

- ⬜ LH-P3.1 Replace Winsock in `function_both.cpp` (`sockets_send`,
  `sockets_receive`, `sockets_disconnect`, `NET_send`) via `plat_sockets.h`.
- ⬜ LH-P3.2 Replace Winsock in `setup_host.inc` (`WSAStartup`/`socket`/
  `bind`/`listen`/`setsockopt`) and `function_host.cpp` (`sockets_accept`,
  `ioctlsocket(FIONBIO)`→`fcntl`, `closesocket`, `shutdown`).
- ⬜ LH-P3.3 `errno`/`EWOULDBLOCK` for `WSAGetLastError`/`WSAEWOULDBLOCK`;
  `-1` for `INVALID_SOCKET`/`SOCKET_ERROR`.
- **Exit:** Linux host accepts a TCP connection and exchanges a signature.

## P4 — Portable threads & timing

- ⬜ LH-P4.1 `plat_threads.h`: map `CreateThread`/`ExitThread`/
  `TerminateThread` + `DWORD WINAPI` proc signature to pthreads; keep the
  existing thread-proc bodies (gotos intact).
- ⬜ LH-P4.2 `plat_time.h`: `timeGetTime`→monotonic ms, `SleepEx`→
  `nanosleep`, `GetLocalTime`/`SYSTEMTIME`→`localtime_r`.
- ⬜ LH-P4.3 `ZeroMemory`/`_snprintf`/`MAKEWORD` shims.
- **Exit:** Linux host runs the accept + per-client send/receive threads.

## P5 — Headless entry point & graphics severance

- ⬜ LH-P5.1 Add a non-Windows `int main()` that does the host setup +
  tick loop (replacing `_tWinMain` + message pump). Windows keeps
  `_tWinMain`.
- ⬜ LH-P5.2 Compile-out `myddraw`/`frame`/`dmusic` from the Linux host;
  make `setupddraw()` and any host graphics call-sites headless no-ops.
- ⬜ LH-P5.3 Crash filter → POSIX signal handler writing a text backtrace
  (or no-op); drop `dbghelp`/`MiniDumpWriteDump` on Linux.
- **Exit:** Linux host boots to "listening on port" and serves a client.

## P6 — Linux toolchain (CMake)

- ⬜ LH-P6.1 Gate every Win32-specific block in `CMakeLists.txt` behind
  `if(MSVC)`; add a GCC/Clang path building only `host` with `-m32`, no
  `.rc`/MASM, no import libs.
- ⬜ LH-P6.2 Add a `CMakePresets.json` `linux-host` preset; reuse the
  existing Ninja/`compile_commands.json` flow.
- **Exit:** `cmake --preset linux-host && cmake --build` produces an ELF host.

## P7 — Container image & Kubernetes

- ⬜ LH-P7.1 Add a Linux multi-stage `Dockerfile.linux` (build → slim
  runtime with 32-bit libs); copy host binary + `assets/` + `ultima6/`
  map data + `dns.txt`.
- ⬜ LH-P7.2 Update `k8s/deployment.yaml`: drop the Windows `nodeSelector`,
  fix the PVC `mountPath` to the Linux save path, keep the Service/port.
- **Exit:** `kubectl apply` runs the host pod on a Linux node; client connects.

## P8 — Validation & CI

- ⬜ LH-P8.1 Cross-play test: **Windows client ↔ Linux host** matches the
  LH-P0.2 baseline (login, walk, pick-up, save, reconnect, karma message).
- ⬜ LH-P8.2 `.sav` round-trip: Linux-written save loads on a Windows host
  and vice-versa.
- ⬜ LH-P8.3 CI job compiling the Linux host on every PR.
- **Exit:** green cross-play + save interop + CI.

---

## Risks & mitigations

| ID | Risk | Mitigation |
|---|---|---|
| R1 | 64-bit `long`/pointer widths silently change `.sav`/wire layout. | Build **i386 (`-m32`)** (LH-P0.1 / LH-P6.1). 64-bit is a separate later phase with a field audit. |
| R2 | `mylib.lib` / `fdv_dmvr.lib` have no source and the host links real symbols from them. | LH-P0.3 symbol audit up front; if real, find source or reimplement before P5. |
| R3 | A shim subtly changes Windows behavior. | Every shim is `#ifdef _WIN32` pass-through; MSVC build must stay diff-free; `get_errors` after each edit. |
| R4 | Non-blocking-socket semantics differ (`WSAEWOULDBLOCK` vs `EAGAIN`/`EWOULDBLOCK`). | Centralize in `plat_sockets.h`; test the accept/recv loop in LH-P3 exit. |
| R5 | `TerminateThread` has no clean POSIX equal. | Map to `pthread_cancel` + document the semantic gap; the disconnect path already waits before forcing. |
| R6 | Endianness if ever run big-endian. | Out of scope — target is little-endian x86 only; note in `plat_types.h`. |
| R7 | Drift between this branch and `plan-serverRefactor.md` P4 (.inc → .cpp). | Both touch `setup_host.inc`/`function_both.cpp`; sequence: land LH-P2..P5 seams first, or rebase carefully. Reference phase IDs in PRs. |

## File touch-map (initial estimate, refine in LH-P0.3)

| File | Phase | Notes |
|------|-------|-------|
| `src/common/platform/*` *(new)* | P1, P3, P4 | the shim layer |
| `src/common/myfile.cpp` / `.h` | P2 | portable file I/O |
| `src/common/function_both.cpp` | P3, P4 | net + thread procs |
| `src/server/setup_host.inc` | P3, P4, P5 | winsock setup, GetLocalTime, setupddraw |
| `src/server/function_host.cpp` | P1, P3, P4 | `roundfloat`, `sockets_accept` |
| `src/server/host.inc` | P4 | `timeGetTime` seed |
| `src/server/data_host.h` | P5 | `hWnd*` host stubs |
| `src/common/u6o7.cpp` | P5 | entry point, main loop, crash filter |
| `src/client/myddraw.cpp` | P5 | `setupddraw` no-op for host |
| `CMakeLists.txt` | P6 | Linux toolchain path |
| `CMakePresets.json` *(new)* | P6 | `linux-host` preset |
| `Dockerfile.linux` *(new)* | P7 | image |
| `k8s/deployment.yaml` | P7 | Linux node + mount path |

---

## Session handoff

> **READ THIS FIRST when picking up the plan in a new session.**

- **2026-06-10 (initial draft)** — Plan committed. Win32 surface inventory
  captured from a full read of the host compile path (see Background
  table). Decisions locked: **i386 build**, **thin `#ifdef _WIN32` shim**,
  **graphics severed from host**, **no `U6O_VERSION` bump**. Implementation
  started at **LH-P1.1** (platform shim skeleton) + **LH-P1.2**
  (`roundfloat` proof-of-seam). Resume at the lowest non-✅ checkbox.
- **2026-06-10 (LH-P1 skeleton landed)** — Created
  `src/common/platform/{platform,plat_sockets,plat_types,plat_threads,plat_time}.h`
  (header-only; `#ifdef _WIN32` pass-through, so the MSVC build is untouched —
  nothing on the Win32 path `#include`s them yet). Converted `roundfloat`
  (`function_host.cpp`) to a guarded seam (`__asm` on Windows,
  `__builtin_lrintf` elsewhere); `get_errors` reports only the file's
  pre-existing repo-wide clang-tidy style warnings, none at the edit site.
  **Resume at LH-P0.2/P0.3** (baseline run + prebuilt-lib symbol audit) then
  **LH-P2** (portable `myfile.cpp`). Note: the POSIX branches of the new
  headers have NOT been compiler-checked yet — first real compile is LH-P6;
  treat them as drafts until then.

To pick up cleanly:

1. `git log --grep "LH-P"` to see what's landed.
2. Scan this file top-to-bottom for the first non-✅ checkbox; that's the
   resume point.
3. Re-grep the Background table's symbols to confirm the surface hasn't
   drifted since 2026-06-10.
4. The MSVC build is the regression oracle: any edit to a compiled host
   file must keep `get_errors` clean and the Windows behavior identical.

### Open questions to resolve before P5

- ⬜ Does the host link any real code from `mylib.lib` / `fdv_dmvr.lib`,
  or only the client? (LH-P0.3)
- ⬜ Is `setupddraw()`'s primary surface used anywhere on the host path,
  or is it purely vestigial? (LH-P5.2)
- ⬜ Cross-compile on a Windows builder, or build natively on a Linux
  runner? (LH-P6 / P8.3)
</content>
</invoke>

