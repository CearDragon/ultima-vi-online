// ui_layout.cpp — RW-P3.1 implementation of ResolveRect.
//
// Pure math, no globals. Used by the future P3.3 panel-draw rerouting to
// turn a UiPlacement into absolute client-area pixel coordinates given
// the current window size.

#include "ui_layout.h"
#include <stddef.h> // size_t for the static_assert below

namespace u6o { namespace client {

UiRect ResolveRect(const UiPlacement& p, int clientW, int clientH) {
    // Anchor base point in client-area pixels.
    int anchorX = 0;
    int anchorY = 0;
    switch (p.anchor) {
        case UiAnchor::TopLeft:      anchorX = 0;           anchorY = 0;           break;
        case UiAnchor::TopRight:     anchorX = clientW;     anchorY = 0;           break;
        case UiAnchor::BottomLeft:   anchorX = 0;           anchorY = clientH;     break;
        case UiAnchor::BottomRight:  anchorX = clientW;     anchorY = clientH;     break;
        case UiAnchor::TopCenter:    anchorX = clientW / 2; anchorY = 0;           break;
        case UiAnchor::BottomCenter: anchorX = clientW / 2; anchorY = clientH;     break;
        case UiAnchor::LeftCenter:   anchorX = 0;           anchorY = clientH / 2; break;
        case UiAnchor::RightCenter:  anchorX = clientW;     anchorY = clientH / 2; break;
        case UiAnchor::Center:       anchorX = clientW / 2; anchorY = clientH / 2; break;
    }

    // Offset is always measured with +x right, +y down regardless of
    // which anchor is in use. This keeps the table in P3.2 readable —
    // a sidebar with TopRight anchor and offsetX = -256 means "256 px
    // in from the right edge" rather than requiring the table to know
    // the sign convention per anchor.
    UiRect r;
    r.left   = anchorX + p.offsetX;
    r.top    = anchorY + p.offsetY;
    r.right  = r.left + p.sizeX;
    r.bottom = r.top  + p.sizeY;
    return r;
}

// ---------------------------------------------------------------------
// RW-P3.2: built-in panel inventory.
//
// Each entry's offset reproduces the legacy literal arithmetic from
// setup_client.inc when it was written assuming clientW=1024,
// clientH=768. The arithmetic was already partly edge-relative
// (`1024-128`, `768-64-32-16` etc.), which is the strongest signal that
// the original author intended these panels to live at fixed distances
// from the bottom/right edges. Re-expressing them as anchors makes
// that intent first-class.
//
// sizeX/sizeY are 0 here — see header comment. Panels that draw via
// the FRM_* system know their surface dimensions; placements only
// describe position.
// ---------------------------------------------------------------------
const UiPlacement kBuiltinPanels[] = {
    // ConvoArrows: `con_frm->offset_x=0; con_frm->offset_y=96-4;`
    { UiAnchor::TopLeft,     0,    92,  0, 0 },

    // ConvoHistory: `con_frm_img->offset_x=0; con_frm_img->offset_y=-256;`
    // Anchored to TopLeft; offsetY=-256 keeps the history surface above
    // the visible area until the user scrolls into it.
    { UiAnchor::TopLeft,     0,   -256, 0, 0 },

    // PartyList: `qkstf->offset_x=1024-128; qkstf->offset_y=128-16;`
    // = top-right corner with 128 px of right inset and 112 px down.
    { UiAnchor::TopRight,    -128, 112, 0, 0 },

    // VolumeControl: `volcontrol->offset_x=1024-128;
    //                 volcontrol->offset_y=768-64-32-16;`
    // = bottom-right with 128 px of right inset and 112 px (= 64+32+16)
    // up from the bottom edge.
    { UiAnchor::BottomRight, -128, -112, 0, 0 },

    // StatusViewPrev: `statusmessage_viewprev->offset_x=0;
    //                  statusmessage_viewprev->offset_y=768-32-16;`
    // = bottom-left with 48 px (= 32+16) up from the bottom edge.
    { UiAnchor::BottomLeft,  0,    -48, 0, 0 },
};

static_assert(
    sizeof(kBuiltinPanels) / sizeof(kBuiltinPanels[0]) == (size_t)UiPanelId::Count,
    "kBuiltinPanels[] size must match UiPanelId::Count"
);

const UiPlacement& GetBuiltinPanel(UiPanelId id) {
    int i = (int)id;
    if (i < 0 || i >= (int)UiPanelId::Count) {
        static const UiPlacement kFallback = { UiAnchor::TopLeft, 0, 0, 0, 0 };
        return kFallback;
    }
    return kBuiltinPanels[i];
}

}} // namespace u6o::client

