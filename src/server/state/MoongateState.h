#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct MoongateState {
        unsigned char showmoongates;
        unsigned short moongatex[8];
        unsigned short moongatey[8];
        object* moongate[8][2];
        unsigned char moonphase;
    };
}
