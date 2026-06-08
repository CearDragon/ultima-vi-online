// ui_panels_apply.cpp — RW-P3.3 implementation of RepositionAnchoredPanels.
//
// This is the seam between the pure ui_layout module and the live game
// panel globals. ui_layout.{h,cpp} stays free of any game-data
// dependency; this file alone reaches into the FRAME pointers declared
// in data_client.h to write `offset_x`/`offset_y` from a resolved
// UiRect.

#include "stdafx.h"
#include <cassert>
#include "data_client.h" // qkstf, volcontrol, con_frm, con_frm_img, statusmessage_viewprev
#include "ui_layout.h"
#include "viewport.h"


namespace u6o {
    namespace client {
        namespace {
            // Apply a UiPanelId's resolved rect to a FRAME-like struct. Templated
            // so the same body works for both FRAME* and FRM_IMAGE* (both have
            // offset_x and offset_y fields with identical types).
            template<typename T>
            void apply_to(T *panel, UiPanelId id, int clientW, int clientH) {
                if (!panel) return;
                const UiPlacement &p = GetBuiltinPanel(id);
                const UiRect r = ResolveRect(p, clientW, clientH);
                panel->offset_x = r.left;
                panel->offset_y = r.top;
            }

            // RW-P4.11: bring a FRAME panel out of hide-sentinel space and clamp
            // it to the live back-buffer bounds so it is immediately reachable.
            void force_frame_into_view(FRAME *panel, int clientW, int clientH) {
                if (!panel) return;

                int x = panel->offset_x;
                int y = panel->offset_y;
                while (x >= kPanelHideThresholdX) x -= kPanelHideDeltaX;
                while (y >= kPanelHideThresholdY) y -= kPanelHideDeltaY;

                int w = panel->size_x;
                int h = panel->size_y;
                if (panel->graphic && w == 0 && h == 0) {
                    w = panel->graphic->d.dwWidth;
                    h = panel->graphic->d.dwHeight;
                }
                if (w <= 0) w = 1;
                if (h <= 0) h = 1;

                int minX = 8 - w;
                int minY = 8 - h;
                int maxX = clientW - 8;
                int maxY = clientH - 8;

                if (x < minX) x = minX;
                if (x > maxX) x = maxX;
                if (y < minY) y = minY;
                if (y > maxY) y = maxY;

                panel->offset_x = x;
                panel->offset_y = y;
            }

            // FRM_IMAGE lacks size_x/size_y, so infer dimensions from its graphic.
            void force_image_into_view(FRM_IMAGE *panel, int clientW, int clientH) {
                if (!panel) return;

                int x = panel->offset_x;
                int y = panel->offset_y;
                while (x >= kPanelHideThresholdX) x -= kPanelHideDeltaX;
                while (y >= kPanelHideThresholdY) y -= kPanelHideDeltaY;

                int w = 1;
                int h = 1;
                if (panel->graphic) {
                    w = panel->graphic->d.dwWidth;
                    h = panel->graphic->d.dwHeight;
                    if (w <= 0) w = 1;
                    if (h <= 0) h = 1;
                }

                int minX = 8 - w;
                int minY = 8 - h;
                int maxX = clientW - 8;
                int maxY = clientH - 8;

                if (x < minX) x = minX;
                if (x > maxX) x = maxX;
                if (y < minY) y = minY;
                if (y > maxY) y = maxY;

                panel->offset_x = x;
                panel->offset_y = y;
            }
        }

        void RepositionAnchoredPanels(int clientW, int clientH) {
            if (clientW <= 0 || clientH <= 0) return;

            // Update global resolution trackers so they match the resized window/backbuffer
            resxn1w = clientW;
            resyn1w = clientH;
            // RW-P4.8 (2026-05-22): sidebar is no longer anchored to the right
            // edge of the window — it stays at its legacy 1024-relative
            // position. `resxn1m` is the X coordinate just to the LEFT of the
            // sidebar; with the sidebar pinned to the legacy layout this is the
            // legacy 1024 - sidebarW (matches what setup_client.inc produced
            // before the anchoring system existed).
            resxn1m = kBackbufferLegacyW - (uipanelsidebar >= 0 ? uipanelsizex[uipanelsidebar][0][0] : 260);

            // RW-P3.4 / 2026-05-26: equipment-slot positions are pinned to the
            // paperdoll silhouette that is baked into the fixed-size status8.bmp
            // (256x256). Scaling the slot coordinates by uiscalex/uiscaley
            // shifted the equipped item sprites off the silhouette (they
            // floated above the paperdoll head when uiscaling was on, which
            // matches the maximized-window bug report). Until status8 itself
            // becomes resizable, the slot layout must stay at its native 1.0
            // scale to keep the items aligned with the painted silhouette.
            RecomputeEquipSlotLayout(1.0f, 1.0f);

            // RW-P3.5: Reposition the legacy minimap and its frame offsets
            minimapnewx = 2;
            minimapnewy = clientH - 256 - 2;


            // RW-P4.8 (2026-05-22): right-edge sidebar anchoring removed. The
            // sidebar and every panel that was positioned relative to it
            // (partymember parent/0, in-sidebar minimap, actionbar1/2,
            // optionbar1, actiontalkbar1/2/3) now stay at their setup-time
            // legacy coordinates. The widened world view will render behind
            // them and the FRAME display loop draws panels on top, so the
            // sidebar still occludes the world area it always did at the
            // legacy resolution while extra game tiles appear in the
            // newly-exposed space to the right and below.
            //
            // If we ever want the sidebar back on the right edge of the
            // resized window, reinstate the block previously here that wrote
            // uipanelx[uipanelsidebar][0][0] = clientW - sidebarW, plus the
            // dependent panels keyed off uipanelx[uipanelsidebar].

            apply_to(con_frm, UiPanelId::ConvoArrows, clientW, clientH);
            apply_to(con_frm_img, UiPanelId::ConvoHistory, clientW, clientH);
            // RW: if the user dragged qkstf to a custom spot (this session or a
            // restored prior session), keep it there instead of snapping back
            // to the anchored top-right default. Clamp the *live* offset to
            // the current client area so a shrunken window doesn't strand the
            // panel off-screen, but do NOT write the clamp back into the
            // cache — a maximized window may have saved a coordinate that's
            // perfectly valid for the larger size and would be permanently
            // lost if the user briefly opened at the default size. The cache
            // is only ever mutated by an actual user drag (loop_client.cpp).
            if (qkstf) {
                if (g_qkstf_user_positioned) {
                    int x = g_qkstf_user_x;
                    int y = g_qkstf_user_y;
                    if (x < 0) x = 0;
                    if (x > clientW) x = clientW;
                    if (y < 0) y = 0;
                    if (y > clientH) y = clientH;
                    qkstf->offset_x = x;
                    qkstf->offset_y = y;
                } else {
                    apply_to(qkstf, UiPanelId::PartyList, clientW, clientH);
                }
            }
            if (g_volcontrol_visible) {
                // Same override behavior for volcontrol when it's on screen.
                // Same rationale on the no-write-back-to-cache rule.
                if (volcontrol && g_volcontrol_user_positioned) {
                    int x = g_volcontrol_user_x;
                    int y = g_volcontrol_user_y;
                    if (x < 0) x = 0;
                    if (x > clientW) x = clientW;
                    if (y < 0) y = 0;
                    if (y > clientH) y = clientH;
                    volcontrol->offset_x = x;
                    volcontrol->offset_y = y;
                } else {
                    apply_to(volcontrol, UiPanelId::VolumeControl, clientW, clientH);
                }
            } else if (volcontrol) {
                // RW-P4.9: park volcontrol in the hide-sentinel range so the
                // FRAME display loop's offscreen culling skips it AND the
                // legacy `offset_x >= kPanelHideThresholdX` toggle checks read
                // it as "hidden". A bare `clientW + 2048` could land back
                // inside the visible range at the largest supported window
                // sizes; the sentinel-relative offset stays safely beyond.
                volcontrol->offset_x = kPanelHideDeltaX + clientW;
                volcontrol->offset_y = kPanelHideDeltaY + clientH;
            }
            apply_to(statusmessage_viewprev, UiPanelId::StatusViewPrev, clientW, clientH);

            ValidateUiMetrics();
        }

        void ValidateUiMetrics() {
            int w = backbufferW();
            int h = backbufferH();

            // 1. Backbuffer bounds validation
            assert(w >= kBackbufferLegacyW && w <= kBackbufferMaxW);
            assert(h >= kBackbufferLegacyH && h <= kBackbufferMaxH);

            // 2. Builtin panels bounds validation (No Out-Of-Bounds Drift)
            if (con_frm) {
                assert(con_frm->offset_x >= 0 && con_frm->offset_x <= w);
                assert(con_frm->offset_y >= 0 && con_frm->offset_y <= h);
            }

            if (con_frm_img) {
                assert(con_frm_img->offset_x >= 0 && con_frm_img->offset_x <= w);
                // ConvoHistory image is offset at -256 to hide it initially, which is a known valid off-screen state
                assert(con_frm_img->offset_y >= -256 && con_frm_img->offset_y <= h);
            }

            if (qkstf) {
                assert(qkstf->offset_x >= 0 && qkstf->offset_x <= w);
                assert(qkstf->offset_y >= 0 && qkstf->offset_y <= h);
            }

            if (volcontrol) {
                if (g_volcontrol_visible) {
                    assert(volcontrol->offset_x >= 0 && volcontrol->offset_x <= w);
                    assert(volcontrol->offset_y >= 0 && volcontrol->offset_y <= h);
                } else {
                    assert(volcontrol->offset_x >= w + kPanelHideDeltaX);
                    assert(volcontrol->offset_y >= h + kPanelHideDeltaY);
                }
            }

            if (statusmessage_viewprev) {
                assert(statusmessage_viewprev->offset_x >= 0 && statusmessage_viewprev->offset_x <= w);
                assert(statusmessage_viewprev->offset_y >= -100 && statusmessage_viewprev->offset_y <= h);
            }

            // 3. Minimap Synchronization
            assert(minimapnewx == 2);
            assert(minimapnewy == h - 256 - 2);


            // RW-P4.8: the in-sidebar uipanel minimap is no longer slaved to a
            // right-edge sidebar nor to clientH. It keeps whatever position
            // setup_client.inc gave it. No assertions remain here for the
            // in-sidebar minimap; the always-on world-overlay minimap is
            // covered by `minimapnewx`/`minimapnewy` above.
        }

        void ResetUiPanelsIntoView(int clientW, int clientH) {
            if (clientW <= 0) clientW = backbufferW();
            if (clientH <= 0) clientH = backbufferH();
            if (clientW <= 0 || clientH <= 0) return;

            // RW-P4.11: explicit recovery path should surface volcontrol.
            g_volcontrol_visible = true;

            // Re-establish anchored defaults first, then force all requested
            // recoverable panels into the visible area.
            RepositionAnchoredPanels(clientW, clientH);

            for (int i = 0; i < 8; ++i) {
                force_frame_into_view(party_frame[i], clientW, clientH);
                force_frame_into_view(party_spellbook_frame[i], clientW, clientH);
            }
            force_frame_into_view(tmap_frame, clientW, clientH);
            force_frame_into_view(minimap_frame, clientW, clientH);
            force_frame_into_view(con_frm, clientW, clientH);
            force_image_into_view(con_frm_img, clientW, clientH);
            force_frame_into_view(qkstf, clientW, clientH);
            force_frame_into_view(volcontrol, clientW, clientH);
            force_frame_into_view(statusmessage_viewprev, clientW, clientH);
        }
    }
} // namespace u6o::client

