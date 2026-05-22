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
    resxn1m = clientW - (uipanelsidebar >= 0 ? uipanelsizex[uipanelsidebar][0][0] : 260);

    // RW-P3.4: Recompute equipment slots layout of party member portraits on resize
    RecomputeEquipSlotLayout(uiscaling ? uiscalex : 1.0f, uiscaling ? uiscaley : 1.0f);

    // RW-P3.5: Reposition the legacy minimap and its frame offsets
    minimapnewx = 2;
    minimapnewy = clientH - 256 - 2;



    // Reposition the sidebar and its nested panels/widgets
    if (uipanelsidebar >= 0 && uipanelsidebar < uipanelcount) {
        uipanelsizey[uipanelsidebar][0][0] = clientH;
        uipanelx[uipanelsidebar][0][0] = clientW - uipanelsizex[uipanelsidebar][0][0];
        uipanely[uipanelsidebar][0][0] = 0;
    }

    if (uipanelpartymemberparent >= 0 && uipanelpartymemberparent < uipanelcount) {
        uipanelx[uipanelpartymemberparent][0][0] = uipanelx[uipanelsidebar][0][0];
    }

    if (uipanelpartymember0 >= 0 && uipanelpartymember0 < uipanelcount) {
        uipanelx[uipanelpartymember0][0][0] = uipanelx[uipanelsidebar][0][0] + 2;
        uipanely[uipanelpartymember0][0][0] = uipanely[uipanelsidebar][0][0] + 2;
    }

    if (uipanelminimap >= 0 && uipanelminimap < uipanelcount) {
        uipanelx[uipanelminimap][0][0] = uipanelx[uipanelsidebar][0][0] + 2;
        uipanely[uipanelminimap][0][0] = clientH - 258;
    }

    if (uipanelactionbar1 >= 0 && uipanelactionbar1 < uipanelcount) {
        uipanelx[uipanelactionbar1][0][0] = uipanelx[uipanelsidebar][0][0] + 2;
        uipanely[uipanelactionbar1][0][0] = uipanely[uipanelsidebar][0][0] + 260;

        for (int i = 1; i < uipanelwidgetcount[uipanelactionbar1]; i++) {
            uipanelx[uipanelactionbar1][i][0] = uipanelx[uipanelactionbar1][0][0] + (uipanelsizex[uipanelactionbar1][i][0] * (i - 1));
            uipanely[uipanelactionbar1][i][0] = uipanely[uipanelactionbar1][0][0];
        }
    }

    if (uipanelactionbar2 >= 0 && uipanelactionbar2 < uipanelcount) {
        uipanelx[uipanelactionbar2][0][0] = uipanelx[uipanelactionbar1][0][0];
        uipanely[uipanelactionbar2][0][0] = uipanely[uipanelactionbar1][0][0] + uipanelsizey[uipanelactionbar1][0][0] + 1;

        for (int i = 1; i < uipanelwidgetcount[uipanelactionbar2]; i++) {
            uipanelx[uipanelactionbar2][i][0] = uipanelx[uipanelactionbar2][0][0] + (uipanelsizex[uipanelactionbar2][i][0] * (i - 1));
            uipanely[uipanelactionbar2][i][0] = uipanely[uipanelactionbar2][0][0];
        }
    }

    if (uipaneloptionbar1 >= 0 && uipaneloptionbar1 < uipanelcount) {
        uipanelx[uipaneloptionbar1][0][0] = uipanelx[uipanelactionbar2][0][0];
        uipanely[uipaneloptionbar1][0][0] = uipanely[uipanelactionbar2][0][0] + uipanelsizey[uipanelactionbar2][0][0] + 1;

        for (int i = 1; i < uipanelwidgetcount[uipaneloptionbar1]; i++) {
            uipanelx[uipaneloptionbar1][i][0] = uipanelx[uipaneloptionbar1][0][0] + (uipanelsizex[uipaneloptionbar1][i][0] * (i - 1));
            uipanely[uipaneloptionbar1][i][0] = uipanely[uipaneloptionbar1][0][0];
        }
    }

    if (uipanelactiontalkbar1 >= 0 && uipanelactiontalkbar1 < uipanelcount) {
        uipanelx[uipanelactiontalkbar1][0][0] = uipanelx[uipanelsidebar][0][0] + 2;
        uipanely[uipanelactiontalkbar1][0][0] = uipanely[uipanelsidebar][0][0] + 260 + 52 + 52;

        int i2 = 0;
        for (int i = 1; i < uipanelwidgetcount[uipanelactiontalkbar1]; i++) {
            uipanelx[uipanelactiontalkbar1][i][0] = uipanelx[uipanelactiontalkbar1][0][0] + (uipanelsizex[uipanelactiontalkbar1][i][0] * i2);
            if (i % 2 == 0) {
                i2++;
                uipanely[uipanelactiontalkbar1][i][0] = uipanely[uipanelactiontalkbar1][0][0] + 26;
            } else {
                uipanely[uipanelactiontalkbar1][i][0] = uipanely[uipanelactiontalkbar1][0][0];
            }
        }
    }

    if (uipanelactiontalkbar2 >= 0 && uipanelactiontalkbar2 < uipanelcount) {
        uipanelx[uipanelactiontalkbar2][0][0] = uipanelx[uipanelactiontalkbar1][0][0];
        uipanely[uipanelactiontalkbar2][0][0] = uipanely[uipanelactiontalkbar1][0][0] + uipanelsizey[uipanelactiontalkbar1][0][0] + 1;

        int i2 = 0;
        for (int i = 1; i < uipanelwidgetcount[uipanelactiontalkbar2]; i++) {
            uipanelx[uipanelactiontalkbar2][i][0] = uipanelx[uipanelactiontalkbar2][0][0] + (uipanelsizex[uipanelactiontalkbar2][i][0] * i2);
            if (i % 2 == 0) {
                i2++;
                uipanely[uipanelactiontalkbar2][i][0] = uipanely[uipanelactiontalkbar2][0][0] + 26;
            } else {
                uipanely[uipanelactiontalkbar2][i][0] = uipanely[uipanelactiontalkbar2][0][0];
            }
        }
    }

    if (uipanelactiontalkbar3 >= 0 && uipanelactiontalkbar3 < uipanelcount) {
        uipanelx[uipanelactiontalkbar3][0][0] = uipanelx[uipanelactiontalkbar2][0][0];
        uipanely[uipanelactiontalkbar3][0][0] = uipanely[uipanelactiontalkbar2][0][0] + uipanelsizey[uipanelactiontalkbar2][0][0] + 1;

        int i2 = 0;
        for (int i = 1; i < uipanelwidgetcount[uipanelactiontalkbar3]; i++) {
            uipanelx[uipanelactiontalkbar3][i][0] = uipanelx[uipanelactiontalkbar3][0][0] + (uipanelsizex[uipanelactiontalkbar3][i][0] * i2);
            if (i % 2 == 0) {
                i2++;
                uipanely[uipanelactiontalkbar3][i][0] = uipanely[uipanelactiontalkbar3][0][0] + 26;
            } else {
                uipanely[uipanelactiontalkbar3][i][0] = uipanely[uipanelactiontalkbar3][0][0];
            }
        }
    }

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


    if (uipanelminimap >= 0 && uipanelminimap < uipanelcount) {
        int expected_sidebar_x = w - (uipanelsidebar >= 0 ? uipanelsizex[uipanelsidebar][0][0] : 260);
        assert(uipanelx[uipanelminimap][0][0] == expected_sidebar_x + 2);
        assert(uipanely[uipanelminimap][0][0] == h - 258);
    }
}

}} // namespace u6o::client

