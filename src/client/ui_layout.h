#ifndef UI_LAYOUT_H
#define UI_LAYOUT_H

// RW-P3.1: anchored UI placement types.
//
// Background: every UI panel (sidebar, minimap, status bar, action bar,
// party-member panels, spellbook overlays, message log, etc.) currently
// stores absolute pixel coordinates that assume a 1024x768 client area.
// When the window grows, panels stay welded to the top-left corner and
// the extra space becomes black border. The plan
// (docs/plan-resizableWindow.md, phase RW-P3) is to express each panel
// as an anchor + offset + size, then resolve those to absolute pixels
// every frame from the current client dimensions.
//
// This header introduces the two types the rest of P3 will consume.
// Phase RW-P3.2 builds a static table of UiPlacements that reproduce
// the legacy 1024x768 positions exactly. Phase RW-P3.3 reroutes every
// panel's draw call through ResolveRect(...). Until then this module
// is unused — adding it has no behavioral effect.

namespace u6o { namespace client {

// Where on the client area a UiPlacement is anchored. The anchor names
// the corner / edge-midpoint / center that the placement's offset is
// measured from.
enum class UiAnchor {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    TopCenter,
    BottomCenter,
    LeftCenter,
    RightCenter,
    Center
};

// A panel's placement description. (offsetX, offsetY) is the offset of
// the panel's top-left corner from the anchor point, in client-area
// pixels. Positive offsetX moves right, positive offsetY moves down,
// regardless of which anchor is in use. Sizes are in pixels and are
// invariant under resize (panels keep their native pixel dimensions;
// only their position moves with the window edges).
struct UiPlacement {
    UiAnchor anchor;
    int      offsetX;
    int      offsetY;
    int      sizeX;
    int      sizeY;
};

// Resolved rectangle in absolute client-area pixels. left/top is the
// top-left corner; right/bottom are exclusive (left+sizeX, top+sizeY).
struct UiRect {
    int left;
    int top;
    int right;
    int bottom;
};

// Resolve a placement against a given client size. Pure function: no
// global state read or written. Out-of-bounds offsets are honored as-is
// (the caller is responsible for clamping or scrolling); this lets a
// placement intentionally hide a panel by anchoring it off-screen.
UiRect ResolveRect(const UiPlacement& p, int clientW, int clientH);

}} // namespace u6o::client

#endif // UI_LAYOUT_H

