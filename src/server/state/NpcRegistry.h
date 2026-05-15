#pragma once
#include "../data_both.h"
#include "../data_host.h"

namespace u6o::server::state {
    struct NpcRegistry {
        unsigned long npci[256];
        unsigned char* npcinf;
        npcbin_i* npcbin;
        unsigned short npcobj[256];
        crtenum_struct crtenum[1073];
        object* crtenum_pathok_castok[1024];
        short crtenum_pathok_castok_i;
        object* crtenum_pathok[1024];
        short crtenum_pathok_i;
        object* crtenum_castok[1024];
        short crtenum_castok_i;
    };
}
