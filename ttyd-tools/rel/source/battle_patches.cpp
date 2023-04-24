#include "ttyd/battle_database_common.h"
#include "ttyd/evtmgr.h"
#include "ttyd/evtmgr_cmd.h"
#include "ttyd/item_data.h"
#include "patch.h"
#include <string.h>
#include "ttyd/battle_event_cmd.h"
#include "ttyd/battle.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/battle_stage_object.h"

namespace mod::infinite_pit {

namespace {

using ::ttyd::battle_database_common::BattleUnitKind;
using ::ttyd::battle_database_common::BattleWeapon;
using ::ttyd::evtmgr::EvtEntry;
using ::ttyd::evtmgr_cmd::evtGetValue;
using ::ttyd::evtmgr_cmd::evtSetValue;
using ::ttyd::item_data::itemDataTable;

namespace ItemType = ::ttyd::item_data::ItemType;

}

extern int32_t (*g_btlevtcmd_WeaponAftereffect_trampoline)(EvtEntry*, bool);

namespace options {

void ApplyFixedPatches() {
    return;
    // Change the chances of weapon-induced stage effects based on options.
    // has problems linking atm, location of g_btlevtcmd_WeaponAftereffect_trampoline not defined
    g_btlevtcmd_WeaponAftereffect_trampoline = patch::hookFunction(
        ttyd::battle_event_cmd::btlevtcmd_WeaponAftereffect,
        [](EvtEntry* evt, bool isFirstCall) {
            // Make sure the stage jet type is initialized, if possible.
            auto* battleWork = ttyd::battle::g_BattleWork;
            if (ttyd::mario_pouch::pouchGetPtr()->rank > 0) {
                ttyd::battle_stage_object::_nozzle_type_init();
            }
            
            int8_t stage_hazard_chances[12];
            // Store the original stage hazard chances.
            auto& weapon = *reinterpret_cast<BattleWeapon*>(
                evtGetValue(evt, evt->currentCommandArguments[0]));
            memcpy(stage_hazard_chances, &weapon.bg_a1_a2_fall_weight, 12);
            
            //disable fog
            if (battleWork->stage_hazard_work.current_stage_jet_type
                <= 0) {
                weapon.nozzle_turn_chance = 0;
                weapon.nozzle_fire_chance = 0;
            }
        
            
            // Call the function that induces stage effects.
            g_btlevtcmd_WeaponAftereffect_trampoline(evt, isFirstCall);
            
            // Copy back original values.
            memcpy(&weapon.bg_a1_a2_fall_weight, stage_hazard_chances, 12);
            
            return 2;
        });
}

}
}