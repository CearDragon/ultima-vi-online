#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct MiscDoorsLevers {
        long doorclose_last;
        float doorclose_wait[1024];
        object* doorclose_obj[1024];
        object* doorclose_obj2[1024];
        unsigned short doorclose_oldtype[1024];
        unsigned short doorclose_oldtype2[1024];
        long leverchange_last;
        float leverchange_wait[1024];
        object* leverchange_obj[1024];
        unsigned short leverchange_oldtype[1024];
        object* portcullis[256][16];
        object* lever[256][16];
        object* efield[256][16];
        object* eswitch[256][16];
    };
}
