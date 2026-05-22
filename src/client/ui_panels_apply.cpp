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


namespace u6o { namespace client {

namespace {
    // Apply a UiPanelId's resolved rect to a FRAME-like struct. Templated
    // so the same body works for both FRAME* and FRM_IMAGE* (both have
    // offset_x and offset_y fields with identical types).
    template <typename T>
    void apply_to(T* panel, UiPanelId id, int clientW, int clientH) {
        if (!panel) return;
        const UiPlacement& p = GetBuiltinPanel(id);
        const UiRect r = ResolveRect(p, clientW, clientH);
        panel->offset_x = r.left;
        panel->offset_y = r.top;
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

    // RW-P3.4: Recompute equipment slots layout of party member portraits on resize
    RecomputeEquipSlotLayout(uiscaling ? uiscalex : 1.0f, uiscaling ? uiscaley : 1.0f);

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

    apply_to(con_frm,                 UiPanelId::ConvoArrows,    clientW, clientH);
    apply_to(con_frm_img,             UiPanelId::ConvoHistory,   clientW, clientH);
    apply_to(qkstf,                   UiPanelId::PartyList,      clientW, clientH);
    if (g_volcontrol_visible) {
        apply_to(volcontrol,          UiPanelId::VolumeControl,  clientW, clientH);
    } else if (volcontrol) {
        volcontrol->offset_x = clientW + 2048;
        volcontrol->offset_y = clientH + 2048;
    }
    apply_to(statusmessage_viewprev,  UiPanelId::StatusViewPrev, clientW, clientH);

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
            assert(volcontrol->offset_x >= w + 2048);
            assert(volcontrol->offset_y >= h + 2048);
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

}} // namespace u6o::client

