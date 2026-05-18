#include "ServerState.h"

namespace u6o::server::state {

    ServerState global_state;

    ServerState& ServerState::Get() {
        return global_state;
    }

}
