#pragma once

#include "AutoSetupState.h"
#include "EconomyTables.h"
#include "HouseSaves.h"
#include "KeybTarget.h"
#include "MiscAccountLogin.h"
#include "MiscDoorsLevers.h"
#include "MiscMapSpecific.h"
#include "MiscObjectCreation.h"
#include "MiscPlayerCreation.h"
#include "MiscServerIdentity.h"
#include "MiscSpellsReagents.h"
#include "MoongateState.h"
#include "MoverNew.h"
#include "NpcRegistry.h"
#include "ObjectPool.h"
#include "RespawnQueue.h"
#include "ResurrectQueue.h"
#include "ReviveLoopExit.h"
#include "ScheduleTables.h"
#include "SfxBuffer.h"
#include "VlnkPool.h"
#include "WindState.h"
#include "WizardEyeState.h"
#include "WorldGrid.h"

namespace u6o::server::state {

    struct ServerState {
        static ServerState& Get();
        AutoSetupState autoSetup;
        EconomyTables economyTables;
        HouseSaves houseSaves;
        KeybTarget keybTarget;
        MiscAccountLogin accountLogin;
        MiscDoorsLevers doorsLevers;
        MiscMapSpecific mapSpecific;
        MiscObjectCreation objectCreation;
        MiscPlayerCreation playerCreation;
        MiscServerIdentity serverIdentity;
        MiscSpellsReagents spellsReagents;
        MoongateState moongates;
        MoverNew moverSystem;
        NpcRegistry npcRegistry;
        ObjectPool objectPool;
        RespawnQueue respawnQueue;
        ResurrectQueue resurrectQueue;
        ReviveLoopExit reviveLoop;
        ScheduleTables scheduleTables;
        SfxBuffer sfxBuffer;
        VlnkPool vlnkPool;
        WindState windState;
        WizardEyeState wizardEyeState;
        WorldGrid worldGrid;
    };

} // namespace u6o::server::state
