#pragma once
#include "../data_both.h"

namespace u6o::server::state {
    struct WizardEyeState {
        unsigned char wizardeyesi;
        unsigned short wizardeyesx[256];
        unsigned short wizardeyesy[256];
        unsigned char wizardeyesi2;
        unsigned char wizardeyesadded;
        object* wizardeyesobj;
    };
}
