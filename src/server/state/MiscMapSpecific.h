#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct MiscMapSpecific {
        unsigned short orbx[5][5];
        unsigned short orby[5][5];
        object* nuggetsfix;
    };
}
