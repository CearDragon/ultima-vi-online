#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct ScheduleTables {
        schedule_i schedule[256][32];
        schedule_i schedule2[1024][32];
        long newschedule2;
    };
}
