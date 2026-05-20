# Resizable Window — Hotspot Catalogue

> Generated 2026-05-20 (RW-P0.1). Source of truth for `docs/plan-resizableWindow.md`.
> Each row is a literal that bakes in a fixed 1024×768 / 32×24-tile assumption
> and must be replaced or parameterized before the client can render at
> arbitrary client sizes. **This catalogue covers `src/client/**` and the
> client-relevant parts of `src/common/**` only.** Server code (`src/server/**`)
> is intentionally out of scope here — server-side view-extent work is
> tracked separately in plan phase RW-P5.

> The "Phase" column ties each row to a phase ID in
> `docs/plan-resizableWindow.md`.

> **Scope reduction 2026-05-20 (Option A applied):** the small classic
> 512×384 mode (`hWnd3`), the "N1 enhanced" alternate window (`hWnd4`),
> and the WS_POPUP fullscreen fallback have been removed from the runtime
> path. As a result, several rows below that targeted those modes are now
> moot and have been struck through. The remaining rows are the work
> needed to make the **single remaining** Mode 1 (main classic 1024×768)
> resizable.

## Summary

- 6 categories (A–G).
- ~75 distinct hotspots, concentrated in 8 files.
- Highest-density files (by hotspot count, descending):
  1. `src/client/loop_client.cpp` (and its near-duplicate `loop_client.inc`)
  2. `src/client/function_client.cpp`
  3. `src/client/setup_client.inc`
  4. `src/client/data_client.h` / `src/common/globals.inc` (declarations)
  5. `src/client/myddraw.cpp`
  6. `src/common/define_both.h`
  7. `src/common/u6o7.cpp`
  8. `src/common/data_both.h`

`loop_client.cpp` and `loop_client.inc` contain the same code; `function_client.cpp`
and `function_client.h` ditto for some of the hotspots. When fixing a row
in one file, also fix the duplicate.

---

## A. Back-buffer dimensions (1024 / 768)

The `1024` and `768` values used as **back-buffer pixel dimensions** (not as
world-map widths or other unrelated meanings).

| File                                        | Line  | Snippet                                                                                | Meaning                                                | Phase    |
| ------------------------------------------- | ----- | -------------------------------------------------------------------------------------- | ------------------------------------------------------ | -------- |
| ~~`src/client/setup_client.inc`~~           | ~~23~~| ~~`ps=newsurf(1024,768,SURF_SYSMEM16);`~~                                              | ~~Back buffer alloc (legacy branch)~~ — Mode 1 always uses the `resxo/resyo` branch on lines 40-48 | DEAD (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~25~~| ~~`ps2=newsurf(1024/2,768/2,SURF_SYSMEM);`~~                                           | ~~Half-res helper~~                                    | DEAD (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~26~~| ~~`ps3=newsurf(1024,768,SURF_SYSMEM);`~~                                               | ~~Back buffer alloc~~                                  | DEAD (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~27~~| ~~`ps4=newsurf(1024/2,768/2,SURF_SYSMEM16);`~~                                         | ~~Half-res helper~~                                    | DEAD (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~29~~| ~~`ps2=newsurf(1024/2,768/2,SURF_SYSMEM16);`~~                                         | ~~Half-res helper~~                                    | DEAD (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~31~~| ~~`ps5=newsurf(1024,768,SURF_SYSMEM16);`~~                                             | ~~Unconverted minimap surface~~                        | DEAD (Option A) |
| `src/client/setup_client.inc`               | 40-48 | `ps=newsurf(resxo,resyo,…)` etc.                                                       | "New" branch already uses `resxo/resyo` — keep         | RW-P2.2  |
| ~~`src/common/u6o7.cpp`~~                   | ~~484~~ | ~~`if ((desktop_rect.right>1024)&&(desktop_rect.bottom>768)){ … }`~~                 | ~~Desktop big-enough check~~ — replaced by single create | DONE (Option A) |
| ~~`src/client/myddraw.cpp`~~                | ~~378~~ | ~~`if ((desktop_rect.right>1024)&&(desktop_rect.bottom>768)){ … }`~~                 | ~~refresh() desktop check~~                            | DONE (Option A) |
| ~~`src/client/setup_client.inc`~~           | ~~11~~  | ~~`if ((desktop_rect.right<1024)…) MessageBox(…recommended…)`~~                      | ~~User-facing min-resolution warning~~ — should also be deleted | DEAD (Option A) |
| ~~`src/client/function_client.cpp`~~        | ~~1962~~| ~~`// rrr can't change this; it will be broken`~~                                    | ~~Commented block~~                                    | DEAD (Option A) |

`globals.inc` line 30 (`unsigned int resya = 768;`) and line 40
(`unsigned int resxo = resxa;`) define the canonical "old" resolution. These
are intentionally kept — they're the correct way to refer to "the legacy
1024×768 size" once we're done.

## B. Mouse-coord clamps and view-blit clamps tied to 1024×768

Hard limits that assume the visible game area is 1024×768 pixels.

| File                                | Line  | Snippet                                                                          | Meaning                                                      | Phase    |
| ----------------------------------- | ----- | -------------------------------------------------------------------------------- | ------------------------------------------------------------ | -------- |
| `src/client/function_client.cpp`    | 261   | `if ((xoff+x_axis_size)>1024) x4-=xoff+x_axis_size-1024;`                        | LIGHTnew(): X clamp of light blit to back-buffer width       | RW-P2.3  |
| `src/client/function_client.cpp`    | 266   | `if ((yoff+x_axis_size)>768) y4-=yoff+x_axis_size-768;`                          | LIGHTnew(): Y clamp of light blit to back-buffer height      | RW-P2.3  |
| `src/client/function_client.cpp`    | 269   | `asm_copy_vc_destskip=1024-asm_copy_vc_bytesx;`                                  | LIGHTnew(): pitch of `ls[]` lighting buffer                  | RW-P2.3  |
| `src/client/function_client.cpp`    | 271   | `asm_copy_vc_destoffset=(y2<<10)+(unsigned long)&ls+x2;`                         | LIGHTnew(): row stride 1024 (`<<10`) into `ls[]`             | RW-P2.3  |
| `src/client/loop_client.cpp`        | 8070  | `if (x7<0) x7=0; if (y7<0) y7=0; if (x8>1024) x8=1024; if (y8>768) y8=768;`      | Crop rect for some lighting effect                           | RW-P2.3  |
| `src/client/loop_client.cpp`        | 8071  | `ls_off=(y7<<10)+x7; ls_off_add=1024-(x8-x7);`                                   | Same: pitch into `ls[]`                                      | RW-P2.3  |
| `src/client/loop_client.cpp`        | 8076  | `if ((z>>10)!=(ls_off>>10)) z=(ls_off>>10<<10)+1023;`                            | Same: row-boundary math at 1024-stride                       | RW-P2.3  |
| `src/client/loop_client.inc`        | 6466-6472 | (duplicate of loop_client.cpp 8070-8076)                                     | Duplicate                                                    | RW-P2.3  |

## C. Tile-pixel-size shifts (`<<5`, `<<= 5`) used in view math

The renderer encodes 32-pixel tiles via `<<5`. These specifically multiply
**tile counts** by **tile size**, so they're only resize-relevant when the
tile-count side is also being changed in RW-P4. If the tile size itself
stays at 32px (which the plan assumes), most of these stay intact; only the
ones that combine a `<<5` with a hard-coded 1024/768 need attention.

| File                                | Line     | Snippet                                                  | Meaning                                                          | Phase   |
| ----------------------------------- | -------- | -------------------------------------------------------- | ---------------------------------------------------------------- | ------- |
| `src/client/function_client.cpp`    | 253      | `xoff<<=5;`                                              | tile-x → pixel-x in LIGHTnew()                                   | RW-P4.3 |
| `src/client/function_client.cpp`    | 255      | `yoff<<=5;`                                              | tile-y → pixel-y in LIGHTnew()                                   | RW-P4.3 |
| `src/client/function_client.cpp`    | 256      | `x_axis_size<<=5;`                                       | tile-count → pixel-count in LIGHTnew()                           | RW-P4.3 |

(The other `<<5` occurrences in the codebase are RGB565 channel packing —
`(g<<5)`, `(r<<11)` — and are NOT layout-related. Ignore.)

The `<<10` shifts are the lighting-buffer **row pitch** (=1024). Listed in
section B; they all need to become `currentBackbufferW()` instead.

## D. Hard-coded UI panel / widget positions

The biggest cluster of hotspots. Each of these is an absolute pixel
coordinate that assumes the back buffer is 1024×768 (and that the playfield
or sidebar starts at a specific x). They need to migrate to anchored
placements (`UiPlacement` in RW-P3.1).

The same idiom `if (offset_x < 1024) offset_x += 2048;` (and its inverse
`-= 2048`) is used throughout to **toggle a panel on or off screen** by
parking it in the off-screen 1024..2047 column. This relies on the back
buffer being exactly 1024 wide.

### D.1 `setup_client.inc` static positions

| File                          | Line | Snippet                                            | Meaning                                                | Phase   |
| ----------------------------- | ---- | -------------------------------------------------- | ------------------------------------------------------ | ------- |
| `src/client/setup_client.inc` | 435  | `inpf->offset_x=256+2048;`                         | Text-input frame, parked off screen                    | RW-P3.2 |
| `src/client/setup_client.inc` | 444  | `inpf2->offset_x=56+69;`                           | Text-input portrait offset                             | RW-P3.2 |
| `src/client/setup_client.inc` | 455  | `inpft->offset_x=56;`                              | Text-input frame title offset                          | RW-P3.2 |
| `src/client/setup_client.inc` | 462  | `con_frm->offset_x=0;`                             | Conversation log frame, top-left                       | RW-P3.2 |
| `src/client/setup_client.inc` | 469  | `con_frm_img->offset_x=0;`                         | Conversation portrait                                  | RW-P3.2 |
| `src/client/setup_client.inc` | 484  | `party_frame[i]->offset_x=4096;`                   | Party member frame, far-off-screen sentinel            | RW-P3.2 |
| `src/client/setup_client.inc` | 493  | `party_spellbook_frame[i]->offset_x=4096;`         | Spellbook frame, sentinel                              | RW-P3.2 |
| `src/client/setup_client.inc` | 505  | `qkstf->offset_x=1024-128;`                        | Quick stuff frame at right edge of 1024-wide screen    | RW-P3.2 |
| `src/client/setup_client.inc` | 513  | `volcontrol->offset_x=1024-128;`                   | Volume control at right edge                           | RW-P3.2 |
| `src/client/setup_client.inc` | 520  | `musickeyboard->offset_x=176+2048; offset_y=677;`  | Music keyboard parked off screen                       | RW-P3.2 |
| `src/client/setup_client.inc` | 527  | `voicechat_frame->offset_x=314+2048;`              | Voice chat parked off screen                           | RW-P3.2 |
| `src/client/setup_client.inc` | 533  | `statusmessage_viewprev->offset_x=0;`              | Status message previous-list frame                     | RW-P3.2 |
| `src/client/setup_client.inc` | 542  | `statusmessage_viewnpc->offset_x=424+2048;`        | NPC view of status message parked off screen           | RW-P3.2 |
| `src/client/setup_client.inc` | 555  | `minimap_frame->offset_x=4096;`                    | Minimap frame sentinel                                 | RW-P3.2 |
| `src/client/setup_client.inc` | 564  | `tmap_frame->offset_x=4096;`                       | Treasure-map frame sentinel                            | RW-P3.2 |

### D.2 The "1024 / 2048 toggle" idiom in loop_client.cpp / .inc

These are **all** the same pattern (toggle a frame on/off-screen by adding
or subtracting 2048). They all need to become "set anchored visibility"
calls in RW-P3.

| File                           | Lines                                  | Frame                                                  |
| ------------------------------ | -------------------------------------- | ------------------------------------------------------ |
| `src/client/loop_client.cpp`   | 3101-3104                              | `volcontrol`                                           |
| `src/client/loop_client.cpp`   | 3267, 3379, 3734, 3881                 | `inpf` (text input)                                    |
| `src/client/loop_client.cpp`   | 3547, 3608                             | `voicechat_frame`                                      |
| `src/client/loop_client.cpp`   | 5055-5057, 6022                        | `party_spellbook_frame[i]`                             |
| `src/client/loop_client.cpp`   | 6023                                   | `party_frame[x]`                                       |
| `src/client/loop_client.cpp`   | 5457, 5485                             | `musickeyboard`                                        |
| `src/client/loop_client.cpp`   | 6261                                   | `statusmessage_viewnpc`                                |
| `src/client/loop_client.cpp`   | 11206-11220                            | `party_spellbook_frame[i]`, `party_frame[i]` (Y-axis variant: `<768` / `+= 1536`) |
| `src/client/loop_client.inc`   | 8676-8690                              | (duplicates of loop_client.cpp 11206-11220)            |

## E. Equipment-slot offsets (`define_both.h`)

These are pixel offsets for displaying equipped items on the character
panel — currently absolute coordinates inside the 1024×768 back buffer.
They need to become relative offsets inside the equipment-panel rect
(RW-P3.4).

| File                          | Line  | `#define`              | Value | Phase   |
| ----------------------------- | ----- | ---------------------- | ----- | ------- |
| `src/common/define_both.h`    | 46    | `helmx`                | 52    | RW-P3.4 |
| `src/common/define_both.h`    | 47    | `helmy`                | 132   | RW-P3.4 |
| `src/common/define_both.h`    | 48    | `wep_rightx`           | 20    | RW-P3.4 |
| `src/common/define_both.h`    | 49    | `wep_righty`           | 170   | RW-P3.4 |
| `src/common/define_both.h`    | 50    | `wep_leftx`            | 84    | RW-P3.4 |
| `src/common/define_both.h`    | 51    | `wep_lefty`            | 170   | RW-P3.4 |
| `src/common/define_both.h`    | 52    | `armourx`              | 52    | RW-P3.4 |
| `src/common/define_both.h`    | 53    | `armoury`              | 166   | RW-P3.4 |
| `src/common/define_both.h`    | 54    | `bootsx`               | 52    | RW-P3.4 |
| `src/common/define_both.h`    | 55    | `bootsy`               | 216   | RW-P3.4 |
| `src/common/define_both.h`    | 56    | `ring_rightx`          | 22    | RW-P3.4 |
| `src/common/define_both.h`    | 57    | `ring_righty`          | 202   | RW-P3.4 |
| `src/common/define_both.h`    | 58    | `ring_leftx`           | 84    | RW-P3.4 |
| `src/common/define_both.h`    | 59    | `ring_lefty`           | 202   | RW-P3.4 |
| `src/common/define_both.h`    | 60    | `neckx`                | 20    | RW-P3.4 |
| `src/common/define_both.h`    | 61    | `necky`                | 138   | RW-P3.4 |

## F. Lighting / visibility / scene buffer sizes

Fixed-size global arrays sized for 1024×768 pixels or 32×24 tiles.

### F.1 Pixel-sized lighting buffers (1024*768 = 786 432 bytes each)

| File                          | Line  | Declaration                                  | Phase   |
| ----------------------------- | ----- | -------------------------------------------- | ------- |
| `src/common/globals.inc`      | 721   | `unsigned char ls[1024*768];`                | RW-P2.1 |
| `src/common/globals.inc`      | 722   | `unsigned char ls_moon1[1024*768];`          | RW-P2.1 |
| `src/common/globals.inc`      | 723   | `unsigned char ls_moon2[1024*768];`          | RW-P2.1 |
| `src/common/globals.inc`      | 724   | `unsigned char ls_moon3[1024*768];`          | RW-P2.1 |
| `src/common/globals.inc`      | 725   | `unsigned char ls_moon4[1024*768];`          | RW-P2.1 |
| `src/client/data_client.h`    | 269-273 | `extern unsigned char ls[1024*768];` …     | RW-P2.1 |

### F.2 Tile-sized visibility buffers (32×24, 34×26)

| File                          | Line  | Declaration                                          | Meaning                                       | Phase   |
| ----------------------------- | ----- | ---------------------------------------------------- | --------------------------------------------- | ------- |
| `src/client/data_client.h`    | 73    | `unsigned char vis[34+2][26+2];`                     | Visibility (32×24 view + 2-tile padding ring) | RW-P4.2 |
| `src/client/data_client.h`    | 74    | `unsigned char vis_window[34+2][26+2];`              | Windows in view                               | RW-P4.2 |
| `src/client/data_client.h`    | 75    | `unsigned char vis_chair[34+2][26+2];`               | Chairs in view                                | RW-P4.2 |
| `src/client/data_client.h`    | 78    | `unsigned char vis_bed[34+2][26+2];`                 | Beds in view                                  | RW-P4.2 |
| `src/client/data_client.h`    | 79    | `unsigned char vis_slime[34+2][26+2];`               | Slime in view                                 | RW-P4.2 |
| `src/client/data_client.h`    | 76    | `unsigned char vischeck[32][24];`                    | "Object visible here?" 1 byte per tile        | RW-P4.2 |
| `src/client/data_client.h`    | 309   | `unsigned char nonvis[32][24];`                      | Inverse visibility                            | RW-P4.2 |
| `src/client/data_client.h`    | 77    | `unsigned char visalways[256][1024];`                | Bit array, 256-tile-stride; **likely OK**     | TBD     |

### F.3 Other fixed-size client buffers worth a pass

| File                          | Line  | Declaration                                  | Note                                                                | Phase   |
| ----------------------------- | ----- | -------------------------------------------- | ------------------------------------------------------------------- | ------- |
| `src/common/globals.inc`      | 405   | `unsigned char stormcloak[8][480*480];`      | Storm cloak 480×480 pixel field per partymember                     | RW-P4.4 |
| `src/common/globals.inc`      | 1376  | `char ktar_x[768];`                          | Keyboard targeting; 768 = max targets, **NOT layout — keep**        | —       |

## G. Other / boundary cases

| File                                | Line  | Snippet                                                                | Phase   |
| ----------------------------------- | ----- | ---------------------------------------------------------------------- | ------- |
| `src/client/loop_client.cpp`        | 121-127 | `multiplierx = (double) resxo / resxs;` etc.                         | RW-P3.6 |
| `src/client/myddraw.cpp`            | 340-354 | `scalexm = (double)resxo / resxz;` (set on smallwindow swap)         | RW-P3.6 |

---

## Notes

1. **Duplicate sources.** `loop_client.cpp` is a regenerated/refactored copy
   of `loop_client.inc`; line numbers differ but the resize-hostile
   patterns are duplicated 1:1. Same for `function_client.cpp` /
   `function_client.h`. Edit both whenever a hotspot is fixed.

2. **The 2048 idiom is structural, not layout.** Many `+= 2048` /
   `-= 2048` calls in `loop_client.cpp` are showing/hiding panels by
   parking them at column 2048+. The number `2048` itself isn't a layout
   constant — it just needs to be "off screen" — but the `< 1024` test
   that pairs with it **is** a layout constant (it asks "is the panel
   on-screen?"). Replace each pair with explicit
   `IsPanelVisible()` / `ShowPanel()` / `HidePanel()` calls in RW-P3.

3. **`<<10` vs `<<5`.** `<<10` (×1024) is **always** the lighting-buffer
   row pitch and is layout-relevant. `<<5` (×32) is **almost always** the
   tile-size shift and is only a problem when paired with a hardcoded
   1024/768 in the same expression. The remaining `<<5`/`<<11` instances
   in this codebase are RGB565 channel packing — those are NOT layout.

4. **`function_client.cpp` 1399-1418** has several
   `if ((x>=1024)&&(y>=…)…)` checks that look like back-buffer clamps but
   are actually **world-map coordinate** checks (Britannia is 2048×1024
   world tiles; gargoyle lands begin at world-X 1024). These are NOT in
   scope for resize work. Listed here so they don't get accidentally
   "fixed".

5. **The `WS_POPUP` fullscreen-fallback path** (created when desktop is
   ≤ 1024×768) is intentionally non-resizable. It does NOT have `WS_THICKFRAME`
   and the maximize button is irrelevant for it. RW-P1 changes don't apply.

6. **`stormcloak[8][480*480]`** is sized to a per-partymember 480-pixel
   square (the "weather field" around each character). That's somewhat
   independent of view size — RW-P4 may or may not need to widen it
   depending on how stormcloak is rendered. Flagged to revisit during
   that phase.

7. **`visalways[256][1024]`** uses 1024 as a tile-bit stride
   (1 bit per world-x tile, packed 8 per byte = 1024 bytes). Not a
   pixel dimension — leave alone.

