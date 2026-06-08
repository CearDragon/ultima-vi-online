# Client UI — Developer Reference

> Audience: developers touching `src/client/**`. This document inventories
> every on-screen UI element, names the C++ globals that back it, points
> to the files where it lives, and explains the shared mechanisms
> (anchored placement, show/hide sentinel, drag, persistence, validation).
>
> Companion documents:
>
> - [`docs/resizable-window-hotspots.md`](../resizable-window-hotspots.md) —
>   catalog of hard-coded `1024×768` literals that interact with UI
>   layout. Cross-referenced throughout this README as "hotspots-§X".
> - [`docs/plans/`](../plans/) — multi-phase refactor plans
>   (`plan-resizableWindow.md`, `plan-dynamicObjectBuffer.md`,
>   `plan-serverRefactor.md`).
>
> Last updated: 2026-05-27.

---

## 1. Two parallel UI systems

The client has **two** independent UI systems that coexist; both
contribute pixels to the back buffer (`ps`) every frame.

### 1.1 The `FRAME*` panel system  (`src/client/frame.{h,cpp}`)

A doubly-linked list of `FRAME` objects, each owning a `surf*` graphic
and (optionally) child widgets (`FRM_IMAGE`, `FRM_TXT`, `FRM_INPUT`,
`FRM_ONOFF`, `FRM_LIST`). Every "movable" window in the game — chat
input, party stats, conversation log, spellbooks, minimap window,
treasure map, volume control, music keyboard, voice chat — is a
`FRAME`. The display loop walks the list once per frame and blits each
panel onto `ps` at its `offset_x/offset_y`.

Key fields in `FRAME` (see `frame.h`):

| Field | Purpose |
|---|---|
| `offset_x`, `offset_y` | Top-left position on the back buffer (`short`). |
| `size_x`, `size_y` | Width/height. 0 means "use `graphic`'s native size". |
| `display` | If FALSE the panel is invisible AND inactive. |
| `active` | If FALSE the panel ignores mouse/keyboard. |
| `move` | If TRUE the panel can be dragged with mouse button 2. |
| `graphic` | Background `surf*`. |
| `mouse_over`, `mouse_pressed`, `mouse_click`, `mouse_x/y` | Input flags consumed each frame. |
| `previous`, `next` | Doubly-linked list pointers; reorder for z-order. |
| `firstobject` | Head of the child-widget list. |

Global drag state lives in `frame.h` (`drg`, `FRAME_drg_begin`,
`FRAME_mbl[2]`, `FRAME_mblf[2]`) and is processed in
`src/client/loop_client.cpp` around line 1373.

### 1.2 The `uipanel*` widget-table system  (`src/client/data_client.h`, `function_client.cpp`)

An **index-based**, fixed-array UI for the static sidebar/HUD. Each
panel has an integer ID (`uipanelsidebar`, `uipanelminimap`,
`uipanelactionbar1`, …) and its widgets are described by parallel
2D/3D arrays keyed by `[panelId][widgetId][stateId]`:

```cpp
extern int uipanelx    [UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanely    [UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
extern int uipanelsizex[UI_PANEL_MAX][UI_PANELWIDGET_MAX][UI_WIDGETSTATE_MAX];
// ...sizey, image indices, etc.
```

The sidebar registers itself in `function_client.cpp::uipanel_*setup()`
(around line 2298). Geometry constants (`uipanelsizex[uipanelsidebar]
[UI_WIDGET_DEF][UI_STATE_DEF] = 260`, etc.) are baked in there.

**Use which?** New player-facing windows should be `FRAME`s — they
support drag, focus, child widgets, and (now) persistent positions.
The `uipanel*` system is for the static sidebar/action-bar layout
that gets re-themed but not user-moved.

---

## 2. UI element inventory

All variables below are `extern` in `src/client/data_client.h` and
defined in `src/common/globals.inc`. Files in parentheses are the
primary draw/update site. **`FRAME*` panels** are sorted by user role.

### 2.1 Always-visible panels (anchored to window edges)

These are the panels registered in `enum class UiPanelId` in
`src/client/ui_layout.h` and re-positioned by `RepositionAnchoredPanels()`
every time the window resizes (`loop_client.cpp::OnClientResized`,
~line 1066) and at startup (`setup_client.inc:865`).

| Global | Type | Anchor (default) | Purpose | UiPanelId |
|---|---|---|---|---|
| `con_frm` | `FRAME*` | TopLeft `(0, 92)` | Conversation-log scroll-arrows panel | `ConvoArrows` |
| `con_frm_img` | `FRM_IMAGE*` | TopLeft `(0, -256)` | Conversation history image overlay (parked above viewport until scrolled in) | `ConvoHistory` |
| `qkstf` | `FRAME*` | TopRight `(-128, 112)` | Party-list "quick stuff" panel (HP/MP per member) | `PartyList` |
| `volcontrol` | `FRAME*` | BottomRight `(-128, -112)` | Volume sliders (music, SFX, voice). Toggled by **S** | `VolumeControl` |
| `statusmessage_viewprev` | `FRAME*` | BottomLeft `(0, -48)` | "View previous status message" up-arrow. Drag with button-2 to reposition (persisted); left-click toggles the log staying drawn constantly | `StatusViewPrev` |

The anchored placements live in `kBuiltinPanels[]` in `ui_layout.cpp`.
The seam that writes resolved rects into the live `FRAME*` globals is
`src/client/ui_panels_apply.cpp`.

### 2.2 Dynamically-shown panels (toggle on/off via the hide-sentinel)

These panels live mostly off-screen at `offset_x ≥ kPanelHideThresholdX`
(see §4 below) and are slid on-screen only when the user activates
them.

| Global | Type | Activator | Purpose |
|---|---|---|---|
| `inpf` | `FRAME*` | **Enter** / chat | Text-input box for chat & commands |
| `inpf2` | `FRM_INPUT*` | (child of `inpf`) | The actual text input + cursor |
| `inpft` | `FRM_TXT*` | (child of `inpf`) | Title text of the input box |
| `inpftxt` | `txt*` | — | Backing text buffer for `inpft` |
| `voicechat_frame` | `FRAME*` | Voice-chat key | Push-to-talk indicator + level meter |
| `musickeyboard` | `FRAME*` | Music-keyboard key | On-screen 5-track MIDI keyboard |
| `statusmessage_viewnpc` | `FRAME*` | NPC interaction | "Look at NPC" status overlay |
| `minimap_frame` | `FRAME*` | Minimap toggle | Pop-out minimap window |
| `tmap_frame` | `FRAME*` | Treasure map use | Treasure-map viewer |

### 2.3 Per-party-member panels (8 slots)

| Global | Type | Purpose |
|---|---|---|
| `party_frame[8]` | `FRAME*` | Per-character status + inventory window |
| `party_spellbook_frame[8]` | `FRAME*` | Per-character spellbook |
| `party_frame_new[]` | `FRAME*` | Updated party-frame variant (count in `partyframenewmax`) |
| `pmf` | `FRAME*` | Scratch "party-member frame" pointer used by per-frame loops |
| `update[8]` | `unsigned char` | Set to 1 to mark a party frame as dirty |
| `updatepartyframen1` | `int` | Trigger redraw of `party_frame_new` |

Per-member positioning offsets used by the legacy code path:
`party_frame_offset_x/y[8]`, `party_spellbook_frame_offset_x/y[8]`
(in the `client_settings` struct — see §5).

### 2.4 The viewport / world view

| Global | Type | Purpose |
|---|---|---|
| `fs` | `FRAME*` | Full-screen container frame |
| `vf` | `FRAME*` | Viewport (playfield) frame |
| `ps` | `surf*` | Back buffer that everything composites onto |

World-tile drawing happens in the main render loop in
`loop_client.cpp` (very large; see hotspots-§B for the
performance-sensitive blocks). Visibility/lighting buffers
(`ls`, `ls_moon1..4`, `vis`, `vis_window`, etc.) are declared in
`data_client.h` and allocated in `src/client/viewport.cpp`.

### 2.5 The sidebar (uipanel system)

Sidebar panel IDs assigned by `function_client.cpp`:

| Variable | Purpose |
|---|---|
| `uipanelsidebar` | The main right-edge sidebar container |
| `uipanelminimap` | In-sidebar minimap region |
| `uipanelworldmap`, `uipanelworldmapbar` | World-map view + its top bar |
| `uipanelpartymemberparent` | Container for in-sidebar party portraits |
| `uipanelactionbarparent`, `uipanelactionbar1`, `uipanelactionbar2` | Combat / world action buttons |
| `uipaneloptionbar1` | Option / settings strip |
| `uipanelactiontalkbarparent`, `uipanelactiontalkbar1..3` | Conversation action buttons |

Standalone surfaces drawn by the sidebar:

- `minimaptilesurf`, `minimaptilesurf1`, `minimaptilesurf2`, `minimap_surf_new`
  — minimap tile surfaces (`minimaptype`, `minimaptypemax`,
  `minimapnewx`, `minimapnewy`, `minimapdelta*`, `minimaptile*` for
  state).
- `statusbar_r255/r128/b255/b128/g255/g128` — colored bar segments
  used to draw HP/MP/etc. meters.
- `voicechat_voicebar` — voice-activity bar surface.
- `volcontrol_background`, `volcontrol_surf`, `volcontrol_tab1..3`
  — bitmaps loaded in `client_bmp.inc:410–414` for the volume panel.

### 2.6 Portraits & "look" display

| Global | Purpose |
|---|---|
| `customportrait[3584]` | Pixel buffer for the user's custom portrait |
| `customportrait_upload` | Upload flag |
| `portraitlook_*` | State for "look at" portrait overlay (`_wait`, `_portrait`, `_equip`, `_type[8]`, `_plusbonus[8]`, `_name`, `_namecolour`) |
| `userspellbook` | Index of which spellbook the user has open |

### 2.7 Cursors

`cur1` … `cur9` (`HCURSOR`) for generic states, plus 8-way directional
targeting cursors `cur_u, cur_ru, cur_r, cur_rd, cur_d, cur_ld, cur_l, cur_lu`.

### 2.8 Equipment slots (paperdoll)

Slot layout for items shown on a character paperdoll. Defined in
`src/common/define_both.h` (`helmx/y`, `wep_rightx/y`, `wep_leftx/y`,
`armourx/y`, `bootsx/y`, `ring_rightx/y`, `ring_leftx/y`, `neckx/y`),
modeled as `EquipSlotId` in `src/client/ui_layout.h`, and accessed
through the runtime table:

```cpp
extern EquipSlot kEquipSlotLayout[];           // ui_layout.h
void RecomputeEquipSlotLayout(float sx, float sy); // call to rescale
```

(See hotspots-§E for the migration history.)

### 2.9 Startup / splash

- `src/client/splash.{h,cpp}` — splash screen shown during asset load.
- `intro_timer` — drives the intro fade.

---

## 3. Anchored placement system (`src/client/ui_layout.*`)

Replaces hard-coded `1024×768` panel positions with an
anchor + offset + size description. Resolves to absolute pixels each
time the window resizes.

```cpp
enum class UiAnchor {
    TopLeft, TopRight, BottomLeft, BottomRight,
    TopCenter, BottomCenter, LeftCenter, RightCenter, Center
};

struct UiPlacement { UiAnchor anchor; int offsetX, offsetY, sizeX, sizeY; };
struct UiRect      { int left, top, right, bottom; };

UiRect ResolveRect(const UiPlacement& p, int clientW, int clientH);
```

The placement table is `kBuiltinPanels[]` in `ui_layout.cpp`, indexed
by `UiPanelId`. `RepositionAnchoredPanels(clientW, clientH)` (defined
in `ui_panels_apply.cpp`) reads it, calls `ResolveRect`, and writes
the result into the matching `FRAME*`'s `offset_x/offset_y`. Call
sites:

- `setup_client.inc:865` — initial snap after startup.
- `loop_client.cpp:1066` — every `OnClientResized` notification.
- `loop_client.cpp:3184` — every time the user presses **S** to
  toggle the volume panel.

`ValidateUiMetrics()` is called at the end of each reposition and
asserts that the anchored panels stayed within `[0, clientW]` ×
`[0, clientH]` (with documented exceptions for `con_frm_img` and
`statusmessage_viewprev`). Useful for catching layout regressions in
debug builds.

### 3.1 User-positioned overrides (`qkstf`, `volcontrol`, `statusmessage_viewprev`)

If the user drags `qkstf`, `volcontrol`, or `statusmessage_viewprev` out of
its anchored spot, `ui_layout` flips a per-panel flag and caches the offset:

```cpp
extern bool g_qkstf_user_positioned;
extern int  g_qkstf_user_x, g_qkstf_user_y;
extern bool g_volcontrol_user_positioned;
extern int  g_volcontrol_user_x, g_volcontrol_user_y;
extern bool g_statusprev_user_positioned;
extern int  g_statusprev_user_x, g_statusprev_user_y;
```

`RepositionAnchoredPanels()` then restores from the cache (clamped to
the live client area for display only — the cache itself is never
mutated by a clamp) instead of snapping back to the anchored default.
The cache survives the volcontrol show/hide cycle AND survives
sessions that open the window smaller than it was when the position
was saved (e.g. a maximized save followed by a default-size restart).
Toggling **S** off and back on returns the panel to the user's chosen
spot. See "Persistence" in §5.

---

## 4. The "hide sentinel" idiom

Panels with `display = TRUE` are always walked by the draw loop, so
"hiding" a frame is done by parking it **off-screen** at a coordinate
greater than the client width. Two constants in `viewport.h` codify
this:

```cpp
constexpr int kPanelHideDeltaX     = 2048; // added to offset to park
constexpr int kPanelHideDeltaY     = 1536; // (vertical variant)
constexpr int kPanelHideThresholdX = 1024; // "is panel still on-screen?"
constexpr int kPanelHideThresholdY =  768;
```

Idiom in `loop_client.cpp`:

```cpp
// Show:
if (inpf->offset_x >= kPanelHideThresholdX) inpf->offset_x -= kPanelHideDeltaX;
// Hide:
if (inpf->offset_x <  kPanelHideThresholdX) inpf->offset_x += kPanelHideDeltaX;
```

Panels currently using this pattern (see hotspots-§D.2):
`volcontrol`, `inpf`, `voicechat_frame`, `party_spellbook_frame[i]`,
`party_frame[x]`, `musickeyboard`, `statusmessage_viewnpc`,
`minimap_frame`, `tmap_frame`.

> The literal `2048` is structural ("must be off-screen") rather than
> a layout constant; the `1024` / `768` thresholds ARE layout
> constants and key off the legacy back-buffer dimensions.
> Long-term they should become `IsPanelVisible() / ShowPanel() /
> HidePanel()` calls (planned in RW-P3).

---

## 5. Persistence — `settings.bin`

User-mutable UI state is serialized to `dr\settings.bin` via the
`client_settings` POD struct in `src/client/data_client.h`:

```cpp
struct client_settings {
    short party_frame_offset_x[8],          party_frame_offset_y[8];
    short party_spellbook_frame_offset_x[8],party_spellbook_frame_offset_y[8];
    short musickeyboard_offset_x,           musickeyboard_offset_y;
    short inpf_offset_x,                    inpf_offset_y;
    short con_frm_offset_x,                 con_frm_offset_y;
    short volcontrol_offset_x,              volcontrol_offset_y;
    short qkstf_offset_x,                   qkstf_offset_y;
    unsigned char u6ovolume;
    unsigned char u6omidivolume;
    unsigned char u6ovoicevolume;
    unsigned char spellrecall_partymember[8];
    unsigned char spellrecall_i[8];
    short minimap_offset_x, minimap_offset_y;
    short tmap_offset_x,    tmap_offset_y;
    short statusprev_offset_x, statusprev_offset_y; // appended at end
};
extern client_settings cltset;   // live (mirrors current state)
extern client_settings cltset2;  // restored-from-disk snapshot
```

Lifecycle:

1. **Startup load** — `setup_client.inc:775` reads `settings.bin` into
   `cltset2`, then applies non-sentinel fields to the live panels.
   The sentinel value `32767` means "no saved override, use default".
2. **Per-frame mirror** — `loop_client.cpp:11700–11750` walks every
   tracked panel and writes its live `offset_x/y` into the matching
   `cltset.*` field. For `qkstf` / `volcontrol` the mirror reads from
   the `g_*_user_*` cache instead of `pmf->offset_x`. This is critical:
   the live offset is auto-clamped each frame to `backbufferW()`, so
   if the window is smaller than the saved position (a far-right
   coord saved while maximized, reopened at the default size) the
   live offset gets shrunk. Reading from the cache keeps the saved
   value intact through the small-window session, so the next
   maximize restores the panel to its original spot. If the flag is
   unset the mirror writes `32767` instead.
3. **Cache mutation rule** — the `g_*_user_x/y` cache is updated
   **only** by an actual user drag (`loop_client.cpp:~1376`). Neither
   `RepositionAnchoredPanels()` nor the per-frame clamp ever writes
   to it. Auto-clamps adjust the **live** `offset_x/y` for the
   current frame; the cache (and therefore `cltset` and
   `settings.bin`) remembers the user's intended position regardless
   of the current window size.
4. **Shutdown save** — `src/common/u6o7.cpp:316` writes `cltset`
   back to `settings.bin` (only if `clientsettingsvalid` is `TRUE`).

> **Window placement** (size + maximized state) is persisted
> separately via `WINDOW_*` entries in `settings.txt`, written in
> `WndProc`'s `WM_DESTROY` handler (see comment at `u6o7.cpp:320`).

### 5.1 Adding a new persistent panel position

1. Add `short mypanel_offset_x, mypanel_offset_y;` to
   `client_settings`.
2. Initialize to `32767` in the "no settings file" branch
   (`setup_client.inc:792–803`).
3. Apply on load (`setup_client.inc:785–789` block):
   ```cpp
   if (cltset2.mypanel_offset_x != 32767) {
       mypanel->offset_x = cltset2.mypanel_offset_x;
       mypanel->offset_y = cltset2.mypanel_offset_y;
   }
   ```
4. Mirror to `cltset` in the per-frame loop in `loop_client.cpp`
   (~line 11737–11749). Either always mirror (legacy panels do) or
   use the flag-and-cache pattern that `qkstf`/`volcontrol` use if
   the panel toggles off-screen.

> The on-disk layout of `client_settings` is positional, not tagged.
> **Adding a field invalidates existing `settings.bin` files** unless
> you add to the end AND `get(tfh, &cltset2, sizeof(client_settings))`
> at load time is replaced with a versioned reader. Today the load
> code just reads `sizeof(client_settings)` bytes blindly. Treat the
> struct layout as a file format.

---

## 6. Drag handling

Mouse-button-2 drag of any `FRAME` whose `move == TRUE`:
`loop_client.cpp:1373–1395` updates `drg->offset_x/y` by the cursor
delta. Per-frame clamp at `loop_client.cpp:11722–11732` snaps each
panel back inside `[8 - sizeX, backbufferW() - 8]` ×
`[8 - sizeY, backbufferH() - 8]` whenever it's "supposed to be
on-screen" (i.e., not parked at the hide sentinel).

For `qkstf` and `volcontrol`, the drag block also sets
`g_qkstf_user_positioned` / `g_volcontrol_user_positioned` so the
panel stops being re-anchored on resize and starts being persisted to
`settings.bin`. See §3.1.

---

## 7. How to add a new panel

A minimal recipe:

1. **Allocate the FRAME** in `setup_client.inc`:
   ```cpp
   mypanel = FRMnew();
   mypanel->graphic     = loadimage(".\\dr\\mypanel.bmp", SURF_SYSMEM16);
   mypanel->display     = TRUE;
   mypanel->active      = TRUE;
   mypanel->move        = TRUE;          // if user-draggable
   mypanel->sound_move  = snd_move;      // optional
   mypanel->offset_x    = /* default x */;
   mypanel->offset_y    = /* default y */;
   ```
2. **Declare** `extern FRAME* mypanel;` in `src/client/data_client.h`
   and **define** `FRAME* mypanel;` in `src/common/globals.inc`.
3. **Drive interaction** in `loop_client.cpp`: read `mypanel->mouse_click`,
   `mouse_over`, etc., per frame.
4. **(If always-visible)** add to `UiPanelId` in `ui_layout.h`, add a
   placement to `kBuiltinPanels[]` in `ui_layout.cpp`, add an
   `apply_to(mypanel, UiPanelId::MyPanel, w, h)` call in
   `RepositionAnchoredPanels()`, and extend `ValidateUiMetrics()`.
5. **(If toggleable)** wire `kPanelHideDeltaX/Y` show/hide in the key
   handler.
6. **(If persistent)** follow the §5.1 recipe.

---

## 8. File map (at a glance)

| File | What lives there |
|---|---|
| `src/client/frame.h` / `frame.cpp` | `FRAME` / `FRM_*` types, list management, focus, click/hover state |
| `src/client/data_client.h` | All `extern` UI globals + `client_settings` POD |
| `src/common/globals.inc` | Definitions of those globals |
| `src/client/setup_client.inc` | Asset load, panel construction, `settings.bin` load |
| `src/client/loop_client.cpp` | Main loop: drag, input, per-frame clamp, cltset mirror, drawing |
| `src/client/ui_layout.h` / `ui_layout.cpp` | `UiPanelId`, `UiPlacement`, `ResolveRect`, `kBuiltinPanels[]`, `g_*_user_positioned` flags, equipment-slot table |
| `src/client/ui_panels_apply.cpp` | `RepositionAnchoredPanels`, `ValidateUiMetrics` (the seam that touches FRAME globals) |
| `src/client/client_bmp.inc` | Bitmap loads for panel backgrounds |
| `src/client/viewport.{h,cpp}` | Back-buffer / lighting allocations, `kPanelHide*` constants, `backbufferW/H` accessors |
| `src/client/myddraw.{h,cpp}` | DirectDraw surface wrappers (`surf`), scaling math |
| `src/client/function_client.cpp` | `uipanel_*setup`, sidebar layout, scaled-UI math |
| `src/client/splash.{h,cpp}` | Splash screen |
| `src/common/define_both.h` | Pixel offsets for the equipment paperdoll |
| `src/common/u6o7.cpp` | `WinMain` / message pump, shutdown writer for `settings.bin` |
| `docs/resizable-window-hotspots.md` | Catalog of fixed-size layout literals (the "what to fix when widening the back buffer" list) |

---

## 9. Common gotchas

- **`loop_client.inc` was deleted.** It used to be a dead mirror of the
  canonical loop; today only `loop_client.cpp` exists and is compiled
  (via `#include` from `u6o7.cpp:462`). Edit `loop_client.cpp`. See
  hotspots-§B.
- **`offset_x` is `short`.** Anything you assign to it from an `int`
  triggers a narrowing warning; that's the existing house style for
  this codebase. Don't widen panels past 32 767 px.
- **The draw loop walks even hidden panels.** "Hidden" means
  `offset_x ≥ kPanelHideThresholdX`, not `display = FALSE`. Setting
  `display = FALSE` ALSO deactivates input — usually not what you
  want for a toggleable window.
- **`cltset` is mirrored every frame.** Anything you write to a panel
  position is in `cltset` within one frame and gets persisted on the
  next clean shutdown. Crash-exit will lose the change.
- **Anchored panels are re-snapped on every resize.** If you want a
  panel to remember a user-supplied position, you need the
  `g_*_user_positioned` flag pattern (see §3.1) or skip the anchored
  list entirely.
- **The two UI systems don't share coordinates.** A `FRAME` is at
  back-buffer pixels (`ps`-relative); a `uipanel` widget is at
  similar pixel coordinates but its position is recomputed by
  `function_client.cpp::uipanel_*setup` whenever the sidebar
  resizes, NOT by `RepositionAnchoredPanels`.

