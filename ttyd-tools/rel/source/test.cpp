#include <ttyd/system.h>
#include "mod.h"
#include "patches_core.h"
#include "patch.h"
#include "ttyd/mariost.h"
#include "ttyd/seq_mapchange.h"
#include <stdio.h>
#include "common_ui.h"
#include "string.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/party.h"
#include "ttyd/mario_party.h"
#include "ttyd/evtmgr.h"
#include "ttyd/item_data.h"
#include "ttyd/mario.h"

void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale);
// namespace mod::infinite_pit {

// // Applies all patches that only need to be applied once at initialization.
// inline void ApplyAllFixedPatches() {
//     core::ApplyFixedPatches();
// }

// }

// extern "C" {
//     void ApplyAllFixedPatches() { mod::infinite_pit::core::ApplyFixedPatches(); }
//     //void onMapUnload() { mod::infinite_pit::core::OnMapUnloaded(); }
// }


typedef unsigned char u8;
namespace mod {

MOD_INIT_FUNCTION() {
    mod::infinite_pit::core::ApplyFixedPatches();
}

//gsw0 = story progression
// u8* test = (u8*)0x80312F5C;
// *test = 4;

char DisplayBuffer[256];

void drawModMain(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    uint32_t color = 0x151515E0;
    char *tempDisplayBuffer = DisplayBuffer;

    sprintf(tempDisplayBuffer,"Map: %s", ttyd::seq_mapchange::NextMap);
    DrawText(tempDisplayBuffer, -190, -150, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Seq: %d", mario_st->gsw0);
    DrawText(tempDisplayBuffer, -190, -165, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Bero: %s", ttyd::seq_mapchange::NextBero);
    DrawText(tempDisplayBuffer, -190, -180, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
}

MOD_UPDATE_FUNCTION() {

    // uint32_t color = 0x151515E0;
    // float scale   = 0.6f;
    // char *tempDisplayBuffer = DisplayBuffer;

    // sprintf(tempDisplayBuffer,"Map: %s", ttyd::seq_mapchange::NextMap);
    
    // drawText(tempDisplayBuffer, 0, 0, color, scale);
}
}

void setNextMap(const char *map)
{
    char tempMap[9];
    strncpy(tempMap, map, sizeof(tempMap) - 1);
    tempMap[sizeof(tempMap) - 1] = '\0';
    
    strcpy(ttyd::seq_mapchange::NextMap, tempMap);
    strncpy(ttyd::seq_mapchange::NextArea, tempMap, 3);
}

// void spawnPartnerOrFollower(ttyd::party::PartyMembers id)
// {
//     ttyd::party::PartySlotId PartySlotId;
//     uint32_t partySlotId_u32 = static_cast<uint32_t>(PartySlotId);
//     uint32_t id_u32 = static_cast<uint32_t>(id);

//     // Make sure the partner/follower is valid
//     if ((id <= ttyd::party::PartyMembers::kNone) || 
//         (id >= ttyd::party::PartyMembers::kMsMowzFollower))
//     {
//         return;
//     }
    
    
//     // If spawning a partner, make sure they are enabled when marioPartyEntry is called
//     if ((id >= ttyd::party::PartyMembers::kGoombella) && 
//         (id <= ttyd::party::PartyMembers::kMsMowz))
//     {
//         uint32_t PouchPtr = reinterpret_cast<uint32_t>(ttyd::mario_pouch::pouchGetPtr());
        
//         bool *PartnerEnabled = reinterpret_cast<bool *>(
//             PouchPtr + (static_cast<uint32_t>(id) * 0xE) + 0x1);
        
//         // Preserve the current value of the enabled bool
//         bool CurrentPartnerEnabled = *PartnerEnabled;
        
//         // Make sure the selected partner is enabled
//         *PartnerEnabled = true;
        
//         // Spawn the partner
//         partySlotId_u32 = ttyd::mario_party::marioPartyEntry(id_u32);
        
//         // Restore the value of the enabled bool
//         *PartnerEnabled = CurrentPartnerEnabled;
//     }
//     else
//     {
//         // Spawn the follower
//         partySlotId_u32 = ttyd::mario_party::marioPartyEntry(id_u32);
//     }

//     PartySlotId = static_cast<ttyd::party::PartySlotId>(partySlotId_u32);
    
//     // Make sure the partner/follower spawned properly
//     if (PartySlotId <= ttyd::party::PartySlotId::kNone)
//     {
//         return;
//     }
    
//     // Make sure the partner/follower is moving
//     ttyd::party::partyRun(ttyd::party::partyGetPtr(PartySlotId));
// }


enum Options_v2 {
 OPT_PARTNER_RANK            = 0x1'0a'2'0'003,
};

void InitializePartyMember (int partnerIndex) {
    const int16_t starting_hp = 10;
    auto& party_data = ttyd::mario_pouch::pouchGetPtr()->party_data[partnerIndex];
    party_data.base_max_hp = starting_hp;
    party_data.max_hp = starting_hp;
    party_data.current_hp = starting_hp;
    party_data.hp_level = 0;
    party_data.attack_level = 0;
    party_data.tech_level = 0;
}

void reloadRoomMain(void);

void skipPeachIntermissions(void) {
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();

    auto* mario_st = ttyd::mariost::g_MarioSt;
    const char* area = ttyd::seq_mapchange::NextArea;

    if (mario_st->gsw0 == 0) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "aaa_00")) {
            mario_st->gsw0 = 6;
            player->prevPartyId[0] = 1;
            player->prevPartyId[1] = 0;
            ttyd::mario_party::partyJoin(1);
            strncpy(ttyd::seq_mapchange::NextBero, "s_bero", 7);
            setNextMap("gor_01");
            reloadRoomMain();
            return;
        }
    }

}