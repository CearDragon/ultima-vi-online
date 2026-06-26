# Memory Management & Leak Remediation — root causes & solutions (DONE)

**Status:** ✅ Complete. The in-session memory climb is resolved — an idle/solo
client no longer grows committed private memory on either Intel (no leak to
begin with) or NVIDIA (the hardware where the dominant leak reproduced).

This folder is the archived record of the memory-leak investigation that ran
**2026-06-23 → 2026-06-26**. It collects the trackable plan, the discovery
reports, and the quick-reference maps that were produced while chasing the
"game gets sluggish and bloats to ~500 MB after ~10 minutes" symptom.

## Contents

| File | What it is |
|------|------------|
| [`plan-memoryManagement.md`](plan-memoryManagement.md) | The trackable, phased plan (MM-P1 … MM-P9.6). The authoritative phase-by-phase record. |
| [`MM_P9_GAME_LOOP_LEAKS.md`](MM_P9_GAME_LOOP_LEAKS.md) | Discovery report for the game-loop / per-message allocations (MM-P9.1–9.3). |
| [`MEMORY_LEAK_QUICK_REFERENCE.md`](MEMORY_LEAK_QUICK_REFERENCE.md) | File/line map of each early-phase leak site to its fix (MM-P2 … MM-P6). |
| [`MEMORY_LEAK_INVESTIGATION_SUMMARY.md`](MEMORY_LEAK_INVESTIGATION_SUMMARY.md) | Plain-language summary, incl. the history of the `-l` font workaround. |

Two related records live **outside** this folder and stay where they are:

- [`docs/modernization/MM-P9.5-cached-text-dc.md`](../../../modernization/MM-P9.5-cached-text-dc.md)
  — the cpp-modernizer execution record for the cached-text-DC fix.
- [`tools/crash/crash-reports/2026-06-25_memory-growth_idle-solo.md`](../../../../tools/crash/crash-reports/2026-06-25_memory-growth_idle-solo.md)
  — the concise dump-comparison evidence record (lives with crash triage).

---

## The headline outcome

The reported symptom ("idle/solo client climbs to ~500 MB and gets choppy in
~10 minutes") had **two distinct root causes**, only the second of which was the
true dominant in-session leak:

1. A pile of **genuine but mostly bounded / one-shot leaks** accumulated over 20
   years of legacy C (DirectDraw surfaces, GDI fonts, sockets, DirectSound /
   DirectMusic, lighting buffers). These were fixed first (MM-P2 … MM-P7) and
   are real correctness wins, but none of them explained the *continuous idle*
   climb on its own.
2. The continuous idle climb turned out to be **NVIDIA legacy-DirectDraw
   emulation leaking a fixed ~6 KB per `IDirectDrawSurface::GetDC` call**, and
   the client was doing one or more DDraw `GetDC`s **every frame** (per drawn
   text string, plus one for the window present). Intel's DDraw emulation does
   not leak, which is why it only reproduced on NVIDIA. Fixed in MM-P9.5 /
   MM-P9.6 by caching the on-surface DC and presenting through it instead of a
   fresh per-frame `GetDC`.

The investigation famously **mis-attributed** the dominant leak twice (first to
the chat/name lists, then to portrait reload, then to DirectMusic) before
self-correlating instrumentation (`commitKB` vs every counted pool) localized it
to the per-frame DDraw `GetDC` on NVIDIA. That detective story is preserved in
`plan-memoryManagement.md` (MM-P9) and the crash report — it is a useful lesson
in *measuring* rather than guessing.

---

## Root causes & solutions by phase

| Phase | Root cause | Fix shipped |
|-------|-----------|-------------|
| **MM-P2** DirectDraw surfaces | Primary surf `malloc` never freed (free was commented out); `purgesurfaces()` released the COM surface but leaked the `surf*` struct; no DDraw device teardown on exit. | Free the temp surf after pixel-format query; `free(surflist[i])` in `purgesurfaces()`; added `ddrawshutdown()` wired into client `WM_QUIT`. |
| **MM-P3** Fonts & the `-l` hack | Fonts were left **selected into a DC** when deleted, so the legacy `-l` flag "fixed" it by deleting+recreating all 7 fonts **every frame** (~420 alloc/s). | Save/restore every `SelectObject`/`GetTextExtentPoint32` so fonts deselect before `ReleaseDC`; create fonts **once** at startup; hard-disable the `-l` per-frame recreate path. |
| **MM-P4** Sockets | `sockets_disconnect()` closed the socket but never freed the `socketclient_ri[]`/`socketclient_si[]` structs (or their nested `->d[]`/`->t` buffers). | Free the top-level structs **and** the nested buffers on disconnect (host path; reconnect re-allocates). |
| **MM-P5** Sound / DirectMusic | `free(sound*)` freed inside its own scan loop (iterator hazard); `~CMidiMusic` **double-released** `m_pMusicPort` and could skip the performance release. | Release `s->s`, clear `tempsound[]` safely, free `s` once after the loop; single guarded release sequence in the destructor; `soundshutdown()` on exit. |
| **MM-P6** Lighting / visibility | 5 (lighting) and 7 (visibility) sequential `malloc`s with **no rollback** — a mid-sequence failure leaked the earlier ones. | Stage all allocations in temporaries, roll back on any failure, commit (free-old + install-new) only after all succeed. |
| **MM-P7** Shutdown | No documented, ordered teardown; crash path teardown contract undocumented. | Added an ordered shutdown checklist + `_DEBUG` teardown logs in `u6o7.cpp`; documented the deliberate "dump-and-die" contract of `MyUnhandledExceptionFilter` (process death reclaims everything; releasing from a corrupt heap is unsafe). |
| **MM-P8** RAII (long-term) | Raw `malloc`/COM management is error-prone. | Tagged 5 subsystems with `MM-P8.1: RAII candidate`. **Actual RAII migration (MM-P8.2/8.3) is handed off to the modernization program** — see below. |
| **MM-P9.1** Chat history | `inpmess` linked list grows one node per new unique chat line; never freed. | `cleanup_input_message_history()` (crash-safe: re-seeds the sentinel node) wired into client teardown. **Bounded**, not the idle leak. |
| **MM-P9.2** Name tags | One `txtnew()` per genuinely-new player id; `idlstn` was also unbounded vs the `[1024]` arrays. | `cleanup_player_namelist()` on teardown; `if (idlstn < 1023)` overflow guard. **Bounded**, not the idle leak. |
| **MM-P9.3** Portrait reload | Type-43 portrait packets allocated a fresh 56×64 surface per message and `loadportrait()` overwrote the cached 112×128 / 28×32 surfaces **without releasing the old ones** (~37 KB/message). | `loadportrait()` frees the prior derivatives before rebuilding; the type-43 handler **reuses** the cached surface. Real leak — but gated to load-once per index, so still not the continuous idle climb. |
| **MM-P9.5** Per-string text `GetDC` | NVIDIA legacy-ddraw leaks ~6 KB per `IDirectDrawSurface::GetDC`; `txtout()`/`txtouts()` did one **per drawn string** (~343 KB/s, 74% of the idle leak). | Per-surface **cached text DC** acquired once and lazily released before any DDraw `Blt`/`GetDC`. Pixel-identical. `oldtextdc` cmd-line keeps the legacy path for A/B. (Also fixed 33 foreign measurement-`GetDC` sites that regressed text centering.) |
| **MM-P9.6** Per-frame present `GetDC` | The residual ~94–120 KB/s was the **present path** (`refresh()`) doing one DDraw `GetDC` per frame to BitBlt to the window (same NVIDIA per-GetDC leak); `cls()` colour-fill correlated only because it, too, is per-frame. | `refresh()` presents through the **cached** text DC (one DDraw `GetDC` per session, not per frame); `cls()` raw-fills system-memory surfaces by writing `s->o` directly instead of `DDBLT_COLORFILL`. Pixel-identical. |

---

## Why it only showed on modern NVIDIA hardware

The dominant leak (MM-P9.5/9.6) is **not a bug in the game's own allocation
bookkeeping** — every counted pool (surfaces, `txt`, CRT heap, GDI/USER handles,
kernel handles, thread stacks, audio buffers) stayed flat while `commitKB`
climbed. The leak is inside **NVIDIA's emulation of the deprecated DirectDraw 7
`IDirectDrawSurface::GetDC`/`ReleaseDC` path**: each cycle leaks a small fixed
amount of GDI/kernel-heap commit that is invisible to `GetGuiResources` and the
CRT. Intel's emulation does not leak, so the same binary was flat on Iris Xe and
climbing on an RTX 5090. The fix removes the per-frame `GetDC` churn rather than
trying to "free" memory the game never owned.

The structural cure (replace emulated DirectDraw 7 presentation with a modern
Direct3D 11 / DXGI swap chain, eliminating the legacy-emulation surface
entirely) is planned separately as
[`plan-modernPresenter.md`](../../todo/plan-modernPresenter.md) (MPRES-P*).

---

## Diagnostic scaffolding — intentionally still in the source

The investigation added behavior-preserving instrumentation that is **still in
the tree on purpose**:

- the `U6O-DIAG` 5-second heartbeat in `txtout()`
  (`commitKB`/`handles`/`threads`/`surf`/`txt`/`heapKB`/`heapN`/`gdi`/`user`/
  `midiPlay`/`midiLoad`/`sndDup`/`sndLive`/`bltFill`/`bltCopy`/`bltKey`),
- the `g_surf_live` / `g_txt_live` / `g_blt_*` / `g_midi_*` counters,
- the A/B switches `oldtextdc`, `diagpresent`, `diagbltskip`.

These are **not** to be removed as part of this cleanup. Their retirement is
explicitly owned by **MPRES-P5.1** in
[`plan-modernPresenter.md`](../../todo/plan-modernPresenter.md), which keeps the
relevant switches as short-lived escape hatches until the modern presenter lands
and the NVIDIA emulation leak class is structurally cured. Removing them now
would delete the A/B baseline that plan depends on.

---

## What remains (handed off, not blocking "done")

The **leak remediation is complete**. Two threads of follow-up work are tracked
elsewhere and were never part of closing out the memory climb:

- **MM-P8.2 / MM-P8.3 — RAII modernization.** Migrating the 5 tagged subsystems
  (`MM-P8.1: RAII candidate`) from raw `malloc`/COM to RAII / smart pointers is
  behavior-sensitive polish, owned by the modernization program
  (`docs/plans/todo/modernization/` → `MCLI-P*` / `MCOM-P*`) and driven through
  the `cpp-modernizer` agent. The code tags remain in place for that pickup.
- **MPRES-P\* — modern presenter.** Structurally removes the emulated DirectDraw
  present path that caused MM-P9.5/9.6, after which MPRES-P5 retires the
  diagnostic scaffolding above.

---

## Verification

Behavior preservation held throughout: **no** `U6O_VERSION` bump, **no** wire /
`.sav` / `struct player` layout change, and the pixel-affecting fixes
(MM-P9.5/9.6) are engineered to be byte-identical (same `TextOut` calls/order/DC
type; `cls()` raw-fill writes the same `dwFillColor` bytes `DDBLT_COLORFILL`
would). The leak resolution was confirmed interactively: idle `commitKB` is flat
on NVIDIA with the default (fixed) path and still climbs under `oldtextdc`,
proving the attribution.

