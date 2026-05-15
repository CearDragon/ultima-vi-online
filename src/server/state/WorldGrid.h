#pragma once
#include "../data_both.h"
#include "../data_host.h"

namespace u6o::server::state {
    struct WorldGrid {
        unsigned char btu6[1024][1024];
        unsigned char btu60[256][256];
        object* od[1024][2048];
        unsigned char btflags[256];
        unsigned char chunks[1024][8][8];
        unsigned char u6wci2[8][8][16][8][3];
        unsigned short u6wci[128][128];
        unsigned char u60ci2[32][16][3];
        unsigned short u60ci[32][32];
        objentry ol[32768];
        long oln;
        object* oul[65536];
        long ouln;
        object* f_oul[65536];
        long f_ouln;
    };
}
