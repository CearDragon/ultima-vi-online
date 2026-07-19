# Crash Report - Linux host SIGSEGV cluster (setup parse + socket teardown)

- **Date analyzed**: 2026-07-19
- **Exception Code**: SIGSEGV / SIGABRT (Linux fatal signal 11 / 6)
- **Exception Address (runtime)**: N/A (text backtraces only)
- **EXE-relative RVA**: N/A
- **Faulting symbol**:
  - `_Z9txtNEWLENP3txtl+0x3c` called from `main+0x3ac3/0x3acc`
  - `_Z12sockets_sendPv+0xc9` and `_Z18sockets_disconnectPv+0x1d6`
- **Build under test**:
  - EXE: `bin/host/debug/Ultima VI Online Host.exe` (Windows verification build)
  - PDB: `build/Ultima VI Online Host.pdb`
- **Dump source**:
  - `tools/crash/crash-reports/crash_20260712_054538.txt`
  - `tools/crash/crash-reports/crash_20260713_093937.txt`
  - `tools/crash/crash-reports/crash_20260717_155235.txt`
- **Status**: Mitigated

## Stack (top frames, symbolized names from host logs)

| # | Symbol | Notes |
|---|--------|-------|
| 0 | `main+0x3ac3/0x3acc` -> `txtNEWLEN+0x3c` | setup/login message parse path with length-prefixed reads |
| 0 | `sockets_send+0xc9` | send worker dereferenced per-connection state during/after disconnect |
| 1 | `sockets_disconnect+0x1d6` -> `free(txt*)` -> `cfree` | teardown hit invalid heap state after prior corruption/race |

## Register evidence

Unavailable (Linux text backtrace only; no minidump/register context was captured by these reports).

## Root cause

1. **Malformed/truncated setup packet parsing in host dispatch**: login/setup parsing consumes many length-prefixed fields (`name`, `username`, `password`, optional portrait upload) without verifying enough bytes remain before each `txtNEWLEN` + `memcpy`. A short/malformed packet can drive OOB reads and memory corruption, observed as crashes near `txtNEWLEN` from `main+0x3ac3/0x3acc`.

2. **Socket worker safety gaps under disconnect pressure**: send/receive workers dereference `socketclient_si[]` / `socketclient_ri[]` without defensive null/state checks during disconnect races. Receive buffering also accepted `recv(..., 32768)` blindly at `t->l` offset without clamping to remaining staging capacity, allowing potential overwrite/corruption that later surfaces in send/disconnect/free paths.

## Fix

- File(s) changed:
  - `src/server/loop/loop_host_part_b_dispatch.cpp`
  - `src/common/function_both.cpp`
- Change type:
  - bounds guard / packet validation
  - null/state checks in worker threads
  - receive buffer capacity clamp
- Plan bridge:
  - hardening in host network/dispatch path; no wire-format changes

## Verification

- Rebuilt target: `host`
- Build command used: `tools/Enter-DevBuildEnv.ps1 -Build host`
- Result: build/link succeeded.
- Runtime repro validation: pending (requires Linux host repro environment and original traffic pattern).

## Recurrence log

- 2026-07-12 - first observed `txtNEWLEN`/`main+0x3acc` SIGSEGV
- 2026-07-13 - observed `sockets_send` SIGSEGV + `sockets_disconnect/free` SIGABRT
- 2026-07-17 - repeat `main+0x3ac3` SIGSEGV

