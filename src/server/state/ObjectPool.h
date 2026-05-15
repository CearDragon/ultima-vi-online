#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct ObjectPool {
        object objb[524228];
        long objb_last;
        unsigned long objb_free[524228];
        long objb_free_last;
    };
}
