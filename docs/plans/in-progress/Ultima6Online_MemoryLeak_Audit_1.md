# Ultima 6 Online — Client Memory Leak Audit (Updated)

Scope: the `client` build target, as defined by `CMakeLists.txt`. The first
archive submitted was missing 9 `.inc` files (`client_bmp.inc`,
`new_getspr.inc`, `setup_client.inc`, `globals.inc`, `secret_a.inc`,
`secret_b.inc`, `setup_both.inc`, `host.inc`, `setup_host.inc`) plus the
CMake project files; a second, complete archive was provided and used to
close out every open question from the first pass. Nothing from the first
archive was missing/removed in the second — it was a strict superset (every
shared `.cpp`/`.h` file was byte-identical between the two).

This was a manual, static read-through of the source (no compiler, debug
heap, or profiler was run), cross-referencing every custom allocator
(`txtnew()`, `newsurf()`, `OBJnew_local()`, `malloc()`, `new`, and the
DirectSound/DirectMusic/GDI+ COM object lifecycles) against its matching
free/release path, and specifically hunting for monotonic counters that index
into fixed-size arrays without ever being reset.

---

## 1. CONFIRMED LEAKS

### 1.1 Chat / console message history (`inpmess_index` linked list)
**File:** `client/loop_client.cpp`, two duplicate call sites: lines ~4376–4378
(normal chat entry) and ~4857–4859 (slash-command path).

```cpp
static inpmess_index* inpmess_index_new;
inpmess_index_new = (inpmess_index*)malloc(sizeof(inpmess_index));
inpmess_index_new->next = inpmess_mostrecent;
txtset(inpmess_index_new->t = txtnew(), inpmess);
inpmess_mostrecent = inpmess_index_new;
```

Every time the player presses Enter on a *unique* line of chat text, a new
`inpmess_index` node (`malloc`) plus a new `txt` string (`txtnew()`) is
prepended to the `inpmess_mostrecent` linked list, which backs the
up/down-arrow chat-history scrollback feature. The list starts with exactly
one seed node, allocated once at startup in `client/setup_client.inc`
(`inpmess_mostrecent = (inpmess_index*)malloc(...)`), then grows by one node
per unique line typed for the rest of the session. I searched the entire
codebase for any code that frees an `inpmess_index` node or trims the list
and found none — there is no cap on length and nothing ever calls `free()`
on these nodes.

**Suggested fix:** cap the list to the last N entries; when trimming, free
both the dropped node and its `->t` text (the `free(txt*)` overload already
exists in `txt.cpp`).

---

### 1.2 "Seen player/NPC name" cache (`idlst_name[]`) — also a buffer overflow risk
**File:** `client/loop_client.cpp` lines ~7946–7951.
**Declared in:** `client/data_client.h` lines 307–312.

```cpp
extern unsigned long idlst[1024];
extern txt   *idlst_name[1024];
extern unsigned char idlst_volume[1024];
extern unsigned long idlst_namecolour[1024];
extern long idlstn;        // = -1 once at startup, never reset thereafter
...
if (z3 == 0) { // add new id
    idlstn++;
    idlst[idlstn] = tplayer->mv_playerid[i];
    idlst_name[idlstn] = txtnew();      // allocated — never freed
    txtset(idlst_name[idlstn], "reading data");
    idlst_namecolour[idlstn] = 0xFFFFFF;
    ...
}
```

I traced every reference to `idlstn` in the codebase. It is set to `-1`
exactly once at startup. The **only other write anywhere** is the
`idlstn++` shown above. It is never decremented or reset during gameplay.
Every distinct player/NPC ID the client ever encounters permanently consumes
one of the 1024 slots and allocates a `txt*` that lives for the rest of the
session.

For comparison, the sibling per-frame trackers in the very same function
(`osn`, `stormcloak_last2`, `stolenitemwarningn`) are all explicitly reset to
`-1` at the top of every frame. `idlstn` conspicuously has no such reset,
which strongly suggests this is an oversight rather than an intentional
permanent cache — the data is clearly a *working set* (existing entries are
actively re-read/updated by ID match elsewhere in the same function), not an
intentional log.

This is more severe than 1.1 because the backing arrays are **fixed at 1024
elements with no bounds check** before the write. Once a session (or one
busy/populated server) has produced 1024 distinct player/NPC IDs, the next
new ID overflows the array — `idlst[idlstn]`, `idlst_name[idlstn]`, and
`idlst_namecolour[idlstn]` all write out of bounds. This is a buffer overflow
/ memory corruption bug riding on top of the leak.

**Suggested fix:** add a bounds check (`if (idlstn >= 1023) { evict-oldest or
reuse-LRU }`) before the increment, and free the `txt*` of any evicted/reused
slot.

---

### 1.3 Custom portrait receive handler — one confirmed leak, two related code defects with uncertain real-world frequency
Triggered by network message type **43** ("receive portrait data from
host"), `client/loop_client.cpp` line ~6919 onward, and `loadportrait()` in
`client/function_client.cpp`.

**1.3a — `receiveport` surface — CONFIRMED, unconditional on every message-43 receipt:**
```cpp
static surf *receiveport = NULL;
receiveport = newsurf(56, 64, SURF_SYSMEM16);   // unconditional, no guard
```
Every other "static pointer, allocate once" site in this codebase uses the
guarded `if (x == NULL) x = ...;` lazy-singleton pattern. This one is missing
the guard, so **every single receipt of message 43** — i.e. every distinct
player/NPC custom portrait viewed over a session — allocates a brand-new
56×64 surface and discards the previous pointer without freeing it. This
does not depend on receiving the same portrait twice; it leaks once per
distinct portrait ever viewed, which still scales with session length and
server population.

**1.3b / 1.3c — `portrait_doublesize[i]` / `portrait_halfsize[i]` — confirmed
code defect, but I found a gating mechanism that limits how often it can
actually trigger:**
```cpp
void loadportrait(unsigned short i, surf *s) {
    portrait_loaded[i] = TRUE;        // set here, but never READ as a guard
    portrait[i] = s;
    portrait_doublesize[i] = newsurf(112, 128, SURF_SYSMEM16);  // leak on reload
    portrait_halfsize[i] = newsurf(28, 32, SURF_SYSMEM16);      // leak on reload
}
```
There is no check of `portrait_loaded[i]` before reassigning — confirmed,
this is a genuine code defect. *However*, I found that the three client-side
accessor functions (`getportrait()`, `getportrait_doublesize()`,
`getportrait_halfsize()`) all gate the outgoing network *request* for a
portrait behind a separate `portrait_requested[65536]` flag that is also
never reset — meaning, under normal client-initiated requests, the client
can only ever ask the server for a given portrait index **once per process
run**. That means the obvious trigger path (the client re-requesting a
portrait it already has) cannot actually happen via the client's own logic.
The startup code (`client/setup_client.inc`) explicitly zeroes
`portrait_loaded[]`/`portrait_requested[]`, confirming this gating is
intentional design, not an oversight. The remaining open question — which
I cannot resolve without the server source — is whether the server can
proactively/unprompted resend portrait data for an index the client never
requested (e.g. pushing an update when a nearby player changes their custom
portrait). If it can, 1.3b/1.3c would still leak; if it can't, they're
effectively dead code in practice. I'm flagging both possibilities rather
than asserting one.

**Suggested fix:** guard `receiveport` with the standard
`if (receiveport == NULL)` lazy-init pattern (it's a fixed 56×64 scratch
buffer, doesn't need reallocation at all); in `loadportrait()`, `free()` the
existing `portrait_doublesize[i]` / `portrait_halfsize[i]` whenever
`portrait_loaded[i]` is already `TRUE` before calling `newsurf()` again,
regardless of how often that path actually fires today.

---

## 2. CONFIRMED BUGS — CURRENTLY DORMANT (no active runtime impact found, but worth fixing)

### 2.1 Inverted success/failure check in `loadfile()`
**File:** `common/myfile.cpp`
```cpp
void *loadfile(LPCSTR name) {
    f = open(name);
    if (f->h != HFILE_ERROR) return NULL;  // backwards: this IS the success case
    loadfile_FILESIZE = lof(f);
    v = malloc(loadfile_FILESIZE);
    get(f, v, loadfile_FILESIZE);
    close(f);
    return v;
}
```
`HFILE_ERROR` means the open failed, so `f->h != HFILE_ERROR` means it
**succeeded** — yet the code returns `NULL` (treating success as failure)
*without ever closing `f`*, leaking the `file*` struct and the OS file handle
on every successful call. I searched the whole `client` target's source for
any caller of `loadfile()` and found none — it currently appears to be dead
code in this build, so there is no active runtime leak today, but it should
be fixed before it's ever wired up to something.

---

## 3. RELATED BUG (memory-safety, not a leak) found in the same subsystem

### 3.1 Double-free in the custom `free(sound*)` overload
**File:** `client/sound.cpp`
```cpp
void free(sound *s) {
    ...
    for (i = 0; i < 256; i++) {
        if (tempsound[i] != NULL) {
            if (tempsound[i]->ss == s) {
                tempsound[i]->s->Release();
                free((void *) tempsound[i]);
                tempsound[i] = NULL;
            }
            free((void *) s);     // <-- runs for EVERY occupied slot, not just matches
        }
    }
}
```
The `free((void*)s)` call is indented to look like part of the
`tempsound[i]->ss == s` match, but the brace actually closes *before* it —
it sits inside the outer `if (tempsound[i] != NULL)` only. With more than one
sound concurrently playing (very common), calling this overload on any
`sound*` will call `free()` on the same `s` pointer multiple times — a
double-free / heap-corruption risk. I found no current callers of this
specific overload in the `client` target, so — like 2.1 — it has no
confirmed active impact today, but it's a serious latent bug in the same
allocator family as the leaks above and should be fixed in the same pass.

**Suggested fix:** move the `free((void*)s);` inside the
`if (tempsound[i]->ss == s)` block, and `break`/`return` once the match is
found and freed (a given `sound*` should only need to match one slot).

---

## 4. ONE ITEM FLAGGED WITH LOWER CONFIDENCE (could not fully verify by static reading alone)

### 4.1 Possible missing `Unload()` in DirectMusic segment loading
**File:** `client/dmusic.cpp`

`LoadMidiFromFile()`, `LoadMidiFromResource()`, and `LoadMidiFromMemory()`
all correctly release the previous `IDirectMusicSegment` COM interface
(`SAFE_RELEASE(m_pSegment)`) before loading the next track, but none of them
call `m_pSegment->Unload(m_pPerformance)` (or the performance-level
equivalent) first. Per DirectMusic's documented contract, releasing the
segment's COM interface does not by itself unload the band/instrument data
that `Download()` pushed into the performance's synthesizer — that requires
an explicit `Unload`. This function is called every time the background or
foreground music track changes (confirmed via call sites in
`loop_client.cpp` around lines 12683 and 12712), which happens routinely
during normal play (e.g., moving between regions, combat stingers).

I'm flagging this with explicitly lower confidence than the items above
because I could not execute the DirectX/COM code to directly observe memory
growth — this is based on reading the DirectMusic API contract, not on
runtime evidence. Separately confirmed clean: `u6omidi` (the `CMidiMusic`
object itself) is correctly `delete`d in `common/u6o7.cpp`'s shutdown
sequence, with a well-commented, correctly-ordered teardown.

---

## 5. EVERYTHING CHECKED AND RULED OUT

The following allocation sites and subsystems were traced in full and found
to be correctly balanced (allocate-then-free, or allocate-once-at-startup
and keep for the program's lifetime, which is not a leak):

- **Startup sequence** (`client/setup_client.inc`, `client/client_bmp.inc`,
  `common/setup_both_basic.h`, `common/setup_both.inc`,
  `common/secret_a.inc`/`secret_b.inc`, `common/globals.inc`): all
  `#include`d exactly once into the program's entry point
  (`common/u6o7.cpp`'s `WinMain`-equivalent). This is where `tplay`, the
  seed `inpmess_mostrecent` node, `vf`/`inpf`/`con_frm`/`qkstf`/`volcontrol`/
  `musickeyboard`/`voicechat_frame`/`statusmessage_viewprev`/
  `statusmessage_viewnpc`/`fs`/`minimap_frame`/`tmap_frame`/`party_frame[]`/
  `party_spellbook_frame[]` (all via `FRMnew()`), 243 image loads, the
  lighting/moon tables, and `u6omidi` are all allocated, confirmed one-time,
  not leaks.
- **`newmodeinit()`** (`client/function_client.cpp`): confirmed called
  exactly once, from `setup_client.inc` line ~512, during startup. This
  resolves the open question from the previous pass — the `FRMnew()` calls
  inside it (`party_frame_new[]`, `party_spellbook_frame[]`) are genuine
  one-time allocations, and the "is `party_frame_new[i]->mouse_over` ever
  read before initialization" concern raised previously is moot, since
  `newmodeinit()` runs before the main loop.
- **`client/frame.cpp`'s `FRMnew()` family**: no teardown function exists
  anywhere in the codebase, but every call site is now confirmed to be a
  one-time startup allocation (panels are created once and live for the
  process's lifetime by design). Fully ruled out with high confidence.
- **`getspr()`** (`client/new_getspr.inc`, previously missing from the
  archive and only inferred): now read in full. Zero allocation calls
  anywhere — pure pixel-blit rendering into pre-existing surfaces (`bt32`,
  `ps`, `spr8[]`, `spr84[]`, `sfx8`). Confirmed clean with certainty.
- **`common/house.cpp`'s build membership** (previously an open question):
  `CMakeLists.txt` defines three targets — `client`, `host`, and `both`.
  `house.cpp` (along with `spell_code.cpp` and `use_code.cpp`) is compiled
  into `host` and `both`, but **not** into `client`. This confirms the
  dedicated client binary never calls into `house.cpp`, settling the
  question definitively rather than by inference.
- `client/function_client.cpp` — `recreateBackbuffers()` (window resize):
  correctly frees `ps`/`ps3` via the custom `free(surf*)` overload before
  reallocating.
- `client/viewport.cpp` — `lighting_alloc()` / `visibility_alloc()`:
  idempotent (skip reallocation if size unchanged), correctly frees old
  buffers before allocating new ones.
- `tplayer->sobj[x][y]` per-tile object buffers (`loop_client.cpp`): bounded
  by the fixed 96×72 array; the realloc-on-grow logic frees the old buffer
  before `malloc`-ing a bigger one.
- `OBJnew_local()` call sites: all guarded by `static`/`NULL` checks, so each
  allocates exactly once.
- The vast majority of `txtnew()` call sites across `function_client.cpp`,
  `loop_client.cpp`, `u6o7.cpp`, `function_both.cpp`: guarded lazy-singleton
  pattern (`static txt *x; if (x == NULL) x = txtnew();`), allocate once for
  the program's life.
- `sfx[]` special-effects slot array and its "type==3 portrait message" text
  pointer (`sfx[i3].p`) — traced the full lifecycle including the
  float-wait expiry/clamp logic; `free()` is correctly called on the last
  page of a multi-page message before the slot is marked free for reuse.
- `sound.cpp`'s `tempsound[]` slot reuse and the one-shot `voicein`
  voice-chat buffer — both correctly `Release()` the DirectSound COM buffer
  and `free()` the struct memory.
- `client/splash.cpp` — modern C++ GDI+ splash screen; `delete state.image`
  unconditionally runs at the end regardless of path taken; GDI+
  startup/shutdown are correctly paired.
- `client/data_client.cpp`, `common/data_both.cpp` — pure one-time
  initialization of global state, no recurring allocation.
- `client/ui_layout.cpp`, `client/ui_panels_apply.cpp` — pure layout
  math/state-application code, no allocations at all.
- `common/u6o7.cpp`, `common/function_both.cpp` — all `txtnew()` calls are
  lazy-singleton statics except `SHA2()`'s local `t3`, which is correctly
  freed at the very end of the function with no early-return paths in
  between. The socket send/receive buffer logic (`socketclient_ri`/`_si`)
  follows a correct "free old buffer only if too small, then malloc bigger"
  pattern, bounded by the largest message ever seen per slot.
- `common/Xenkan.cpp` — entirely wrapped in `#ifdef HOST`; does not compile
  into the `client` target at all.
- `common/random/randi.cpp`, `common/random/random.cpp` — pure RNG math, no
  allocation calls.
- `tools/symbolize.cpp` — not referenced anywhere in `CMakeLists.txt`; not
  part of any build target, standalone dev utility, out of scope.
- `client/dmusic.cpp`'s three `LoadMidiFrom*()` functions: all three
  correctly `SAFE_RELEASE(m_pSegment)` the previous COM segment interface
  before loading the next one.
- `client/loop_client.cpp`'s `qkstf->graphic` surface (quick-stats frame):
  correctly calls `free()` before reassigning via `newsurf()`, gated behind a
  dirty flag and a size-change check.
- Monotonic per-frame counters `osn`, `stormcloak_last2`,
  `stolenitemwarningn`, `actionlast`, `panelcount`, `uipanelcount`,
  `uipanelwidgetcount` — all either reset every frame or only incremented
  during one-time UI setup; none accumulate unboundedly during gameplay.
- `customportrait[3584]` — a flat `unsigned short` pixel buffer with no
  embedded heap pointers; not a separate leak risk.

---

## 6. RECOMMENDED NEXT STEPS

1. Fix the three confirmed leaks (1.1, 1.2, 1.3a) — 1.2 should be treated as
   higher priority since it doubles as a buffer-overflow risk on long
   sessions or populated servers. Fix 1.3b/1.3c as well even though their
   real-world trigger frequency is uncertain — they're cheap to fix
   regardless (a single `if (portrait_loaded[i])` check before each
   `newsurf()` call).
2. Fix the double-free bug in `sound.cpp` (3.1) and the inverted check in
   `loadfile()` (2.1) while in the area, even though neither is currently
   triggering at runtime — both are landmines in the same allocator
   subsystems being touched.
3. Investigate the DirectMusic `Unload()` question (4.1) with Microsoft's
   DirectMusic documentation or a memory profiler, since it couldn't be
   fully confirmed by static reading alone.
4. If you have access to the server source (`src/server`), it would resolve
   the one remaining open question: whether message-43 (portrait data) can
   be sent to a client without that client having requested it. That's the
   only thing standing between 1.3b/1.3c being "real but rare" and "dead
   code in practice."
5. Once the above are patched, a runtime pass (Application Verifier, a debug
   heap, or a profiler like Dr. Memory / Visual Studio's Diagnostic Tools)
   against a long play session would be the best way to catch anything this
   manual read-through missed, and to confirm these fixes actually stop
   memory growth in practice.
