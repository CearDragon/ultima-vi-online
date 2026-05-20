// ui_panels_apply.cpp — RW-P3.3 implementation of RepositionAnchoredPanels.
//
// This is the seam between the pure ui_layout module and the live game
// panel globals. ui_layout.{h,cpp} stays free of any game-data
// dependency; this file alone reaches into the FRAME pointers declared
// in data_client.h to write `offset_x`/`offset_y` from a resolved
// UiRect.

#include "stdafx.h"
#include "data_client.h" // qkstf, volcontrol, con_frm, con_frm_img, statusmessage_viewprev
#include "ui_layout.h"

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
    apply_to(con_frm,                 UiPanelId::ConvoArrows,    clientW, clientH);
    apply_to(con_frm_img,             UiPanelId::ConvoHistory,   clientW, clientH);
    apply_to(qkstf,                   UiPanelId::PartyList,      clientW, clientH);
    apply_to(volcontrol,              UiPanelId::VolumeControl,  clientW, clientH);
    apply_to(statusmessage_viewprev,  UiPanelId::StatusViewPrev, clientW, clientH);
}

}} // namespace u6o::client

