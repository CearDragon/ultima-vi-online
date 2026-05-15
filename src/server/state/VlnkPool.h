#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct VlnkPool {
        void* vlnkb_lnk[65536];
        unsigned long vlnkb_off[65536];
        void* vlnkb_lnks[65536];
        long vlnkb_last;
        unsigned long vlnkb_free[65536];
        long vlnkb_free_last;
    };
}
