#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct MiscObjectCreation {
        unsigned long objr_x;
        unsigned long objr_y;
        unsigned long ol_tag;
        unsigned long ol_tag_prev;
        object* newll;
        unsigned long mycount;
    };
}
