#pragma once
namespace u6o::server::state {
    struct MiscSpellsReagents {
        unsigned char cast_spell;
        unsigned char staff_cast_spell;
        long CASTSPELL_SPELLTYPE;
        unsigned long treagent[8];
        unsigned long tspell[256];
        bool spellattcrt;
        unsigned char itemused;
    };
}
