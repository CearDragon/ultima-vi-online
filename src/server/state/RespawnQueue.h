#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct RespawnQueue {
        object* respawn[1200];
        unsigned short respawn_delay[1200];
        long respawn_last;
    };
}
