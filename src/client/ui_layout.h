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

namespace u6o {
    namespace client {
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
            int offsetX;
            int offsetY;
            int sizeX;
            int sizeY;
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
        UiRect ResolveRect(const UiPlacement &p, int clientW, int clientH);

        // ---------------------------------------------------------------------
        // RW-P3.2: built-in panel inventory.
        //
        // Identifies every UI element with a *static* default position in the
        // legacy 1024x768 layout. Panels that are positioned dynamically at
        // runtime (party inventory windows, spellbook overlays, the input
        // portrait when the chat box pops open) are NOT in this table — those
        // stay off-screen until activated and are placed by the same code that
        // activates them. The table covers the "always-visible" / "comes back
        // to a known spot" panels that need to follow the window edges as it
        // resizes.
        //
        // A `sizeX`/`sizeY` of 0 means "use the panel's surface dimensions" —
        // the panel's `graphic` surface knows its own size and is the source
        // of truth. The table is concerned with position only. ResolveRect()
        // will still produce a valid rect; consumers that need accurate
        // right/bottom edges should override sizeX/sizeY from the surface
        // before calling.
        // ---------------------------------------------------------------------
        enum class UiPanelId : int {
            // Conversation log scroll arrows (`con_frm`).
            ConvoArrows = 0,
            // Conversation log history image overlay (`con_frm_img`).
            ConvoHistory,
            // Party-list quick-stats panel (`qkstf`) — top-right corner block.
            PartyList,
            // Volume control sliders (`volcontrol`) — bottom-right.
            VolumeControl,
            // "View previous status message" up-arrow (`statusmessage_viewprev`)
            // anchored to the bottom-left.
            StatusViewPrev,

            // Sentinel.
            Count
        };

        // RW-P3.4: structured layout table for equipment slots
        struct EquipSlot {
            int x;
            int y;
        };

        enum class EquipSlotId : int {
            Helm = 0,
            WepRight,
            WepLeft,
            Armour,
            Boots,
            RingRight,
            RingLeft,
            Neck,
            Count
        };

        extern EquipSlot kEquipSlotLayout[];

        void RecomputeEquipSlotLayout(float scaleX, float scaleY);

        // Default placement for each UiPanelId. Indexed by the integer value of
        // the enum; size == int(UiPanelId::Count). Defined in ui_layout.cpp.
        extern const UiPlacement kBuiltinPanels[];

        // Convenience accessor with bounds check.
        const UiPlacement &GetBuiltinPanel(UiPanelId id);

        // RW-P3.3: re-apply the kBuiltinPanels[] placements to the live panel
        // globals (`con_frm`, `qkstf`, `volcontrol`, `statusmessage_viewprev`,
        // `con_frm_img`) using the supplied client size. Called once at startup
        // (end of setup_client.inc) and again from the dirtyClientSize handler
        // when the window is resized. Implemented in ui_panels_apply.cpp where
        // the panel globals are visible (via data_client.h).
        //
        // Caller used to be responsible for a `windowResize` feature gate; that
        // gate was removed 2026-05-27 (always-on now) so callers may invoke
        // this unconditionally.
        void RepositionAnchoredPanels(int clientW, int clientH);

        void ValidateUiMetrics();

        extern bool g_volcontrol_visible;

        // RW: per-panel persistence overrides.
        //
        // When the user drags `qkstf` or `volcontrol` to a custom location (or
        // such a custom location was restored from settings.bin at startup),
        // the corresponding `*_user_positioned` flag flips to true and the
        // `*_user_x/y` fields cache the chosen offset. RepositionAnchoredPanels
        // then restores from the cache on every reposition event (initial
        // snap, window resize, volcontrol show/hide toggle) instead of
        // snapping the panel back to the anchored default from kBuiltinPanels.
        // The per-frame mirror in loop_client.cpp uses the cache to populate
        // cltset.qkstf_offset_x/y and cltset.volcontrol_offset_x/y so that the
        // shutdown writer (u6o7.cpp) persists the user's choice; when the flag
        // is false the mirror writes the 32767 "no override" sentinel.
        extern bool g_qkstf_user_positioned;
        extern int g_qkstf_user_x;
        extern int g_qkstf_user_y;

        extern bool g_volcontrol_user_positioned;
        extern int g_volcontrol_user_x;
        extern int g_volcontrol_user_y;

        // StatusViewPrev arrow follows the same user-positioned override
        // pattern as qkstf: a real drag (or a restored settings.bin value)
        // flips the flag so RepositionAnchoredPanels stops snapping it back to
        // the bottom-left anchor and the per-frame mirror persists the chosen
        // offset to cltset.statusprev_offset_x/y.
        extern bool g_statusprev_user_positioned;
        extern int g_statusprev_user_x;
        extern int g_statusprev_user_y;
    }
} // namespace u6o::client

// Unqualified shim so existing C-style call sites in setup_client.inc /
// loop_client.cpp can use the name without the `u6o::client::` prefix.
inline void RepositionAnchoredPanels(int clientW, int clientH) {
    u6o::client::RepositionAnchoredPanels(clientW, clientH);
}

inline void ValidateUiMetrics() {
    u6o::client::ValidateUiMetrics();
}

#endif // UI_LAYOUT_H