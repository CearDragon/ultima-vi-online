#pragma once
#include "../data_both.h"
#include "../define_host.h"
#include "../mytxt.h"

namespace u6o::server::state {
    struct MiscAccountLogin {
        unsigned char save_buffer[::u6o::server::kSaveSlotLast + 1];
        txt* save_username[::u6o::server::kSaveSlotLast + 1];
        txt* save_password[::u6o::server::kSaveSlotLast + 1];
        txt* save_name[::u6o::server::kSaveSlotLast + 1];
        unsigned long save_exp[::u6o::server::kSaveSlotLast + 1];
        unsigned long save_bytes[::u6o::server::kSaveSlotLast + 1];
        unsigned char save_dump;
        unsigned char login_dead_callback;
        unsigned short save_version;
        unsigned char encryptcode[65536];
        unsigned char format_usernames;
    };
}
