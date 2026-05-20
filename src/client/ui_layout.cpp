// ui_layout.cpp — RW-P3.1 implementation of ResolveRect.
//
// Pure math, no globals. Used by the future P3.3 panel-draw rerouting to
// turn a UiPlacement into absolute client-area pixel coordinates given
// the current window size.

#include "ui_layout.h"

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

}} // namespace u6o::client

