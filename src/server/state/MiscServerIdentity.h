#pragma once
#include "../data_both.h"
#include "../define_host.h"
#include "../mytxt.h"

namespace u6o::server::state {
    struct MiscServerIdentity {
        txt* admins[::u6o::server::kAdminsMax];
        txt* motd;
        txt* inbritannia;
        long inbritannia_totalplayers;
        unsigned long U6ONEWID;
        player* playerlist[1024];
        long playerlist_last;
        unsigned char party_ok[8];
    };
}
