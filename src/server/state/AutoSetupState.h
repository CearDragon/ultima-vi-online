#pragma once
namespace u6o::server::state {
    struct AutoSetupState {
        unsigned short autosetup;
        bool autosetup_next;
        unsigned short autosetup_counter;
        bool autospell;
    };
}
