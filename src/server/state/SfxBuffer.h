#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct SfxBuffer {
        unsigned char sfbuffersend;
        float sfbufferwait;
        unsigned long sfi[256][256];
        sfxtype sf[65536];
        unsigned long sf_playerid[65536];
        long sfn;
        unsigned long sfx_playerid[256];
    };
}
