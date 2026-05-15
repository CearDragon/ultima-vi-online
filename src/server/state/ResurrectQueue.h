#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct ResurrectQueue {
        object* resu[65536];
        object* resu_body[65536];
        unsigned short resu_body_type[65536];
        float resu_wait[65536];
        player* resu_player[65536];
        unsigned char resu_partymember[65536];
        unsigned short resu_x[65536];
        unsigned short resu_y[65536];
        long nresu;
    };
}
