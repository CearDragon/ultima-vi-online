#pragma once
#include "../mytxt.h"

namespace u6o::server::state {
    struct MiscPlayerCreation {
        txt* tname;
        txt* tusername;
        txt* tuserpassword;
        txt* tnewuserpassword;
        unsigned char tcustomportrait_upload;
        unsigned short tcustomportrait[3584];
        unsigned long tnamecolour;
        unsigned char tmale_female;
        unsigned short tport;
        unsigned short ttype;
        unsigned char tcreatecharacter;
    };
}
