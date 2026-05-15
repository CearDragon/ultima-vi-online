#pragma once
namespace u6o::server::state {
    struct EconomyTables {
        unsigned char economy_setup[1024][4];
        long economy_limit[1024][4];
        long economy_value[1024][4];
        long economy_change[1024][4];
    };
}
