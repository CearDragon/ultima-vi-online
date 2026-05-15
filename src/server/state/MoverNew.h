#pragma once
namespace u6o::server::state {
    struct MoverNew {
        unsigned long MOVERNEW_GLOBALFLAGS;
        unsigned char MOVERNEW_ERROR;
        unsigned char mover_blood[1024];
        unsigned short mover_body[1024];
    };
}
