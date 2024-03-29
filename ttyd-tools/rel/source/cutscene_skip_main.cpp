#include "cutscene_skip_main.h"

#define FALSE 0
#define TRUE 1

#define DEBUG TRUE

#define MINIMUM 0
#define MEDIUM 1
#define HEAVY 2

int skipLevel = HEAVY;


char DisplayBuffer[256];

void setNextMap(const char *map)
{
    char tempMap[9];
    strncpy(tempMap, map, sizeof(tempMap) - 1);
    tempMap[sizeof(tempMap) - 1] = '\0';
    
    strcpy(ttyd::seq_mapchange::NextMap, tempMap);
    strncpy(ttyd::seq_mapchange::NextArea, tempMap, 3);
}

void setNextBero(const char *bero)
{
    char tempBero[17];
    strncpy(tempBero, bero, sizeof(tempBero) - 1);
    tempBero[sizeof(tempBero) - 1] = '\0';
    
    strcpy(ttyd::seq_mapchange::NextBero, tempBero);
}

bool checkButtonComboEveryFrame(uint32_t combo)
{
    uint32_t ButtonInput = ttyd::system::keyGetButton(0);
    return (ButtonInput & combo) == combo;
}

void giveStarByIndex(int starIndex) {
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();

    if (starIndex == 0) { //is
        if (!(pouch->star_powers_obtained & 1)) {
            ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::MAGICAL_MAP);
            pouch->star_powers_obtained |= 1;
            pouch->max_sp += 100;
            pouch->current_sp = pouch->max_sp; //fully restore sp
        }
        return;
    }

    if (!(pouch->star_powers_obtained & (1 << starIndex))) {
        ttyd::mario_pouch::pouchGetItem(113 + starIndex); //give player the star in their inventory
        pouch->star_powers_obtained |= (1 << starIndex);
        //increment star power
        pouch->max_sp += 100;
        pouch->current_sp = pouch->max_sp; //fully restore sp
    }
}


namespace mod {

int32_t GSWFCopy;
int32_t keyItemReceived;
int32_t keyItemRemovedCopy;

uint32_t setGSWFHook(uint32_t arg0) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();

    GSWFCopy = arg0;
    //flag set when koop's dad comes out of hooktail
    if (GSWFCopy == 1500) {
        if (mario_st->gsw0 == 54) {
            setNextBero("e_bero_1");
            setNextMap("tik_01");
            ttyd::swdrv::swSet(1493); //plane curse chest open
            mario_st->gsw0 = 63; //after peach's email from intermission
            giveStarByIndex(1); //give diamond star
            reloadRoomMain();         
        }
    }
    return arg0;
}

uint32_t ItemAddHook(uint32_t arg0) {
    keyItemReceived = arg0;
    return arg0;
}

uint32_t ItemRemoveHook(uint32_t arg0) {
    keyItemRemovedCopy = arg0;
    return arg0;
}

void seqSetHook(uint32_t seqValue) {
    //seqSetHook
    return;
}

uint32_t RemovePouchItemHook(uint32_t removedItemID) {
    auto* mario_st = ttyd::mariost::g_MarioSt;

    //if just gave black key to plane curse chest
    if (mario_st->gsw0 == 13) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "tik_19")) {
            ttyd::mario_pouch::pouchGetItem(4); //plane curse ability
            setNextBero("n_bero_5");
            setNextMap("tik_04");
            ttyd::swdrv::swSet(1352); //plane curse chest open
            mario_st->gsw0 = 15;
            reloadRoomMain();
        }
    }

    //if just gave black key to paper curse chest
    if (removedItemID == 34) {
        if (mario_st->gsw0 == 44) {
            if (!strcmp(ttyd::seq_mapchange::NextMap, "gon_06")) {
                ttyd::mario_pouch::pouchGetItem(2); //paper curse ability
                setNextBero("e_bero");
                setNextMap("gon_05");
                ttyd::swdrv::swSet(1493); //plane curse chest open
                mario_st->gsw0 = 45;
                reloadRoomMain();
            }
        }
    }

    //if just gave flurrie her necklace
    if (removedItemID == 63) {
        if (mario_st->gsw0 == 86) {
            if (!strcmp(ttyd::seq_mapchange::NextMap, "win_04")) {
                mario_st->gsw0 = 87;
                ttyd::mario_party::partyJoin(5);
                setNextBero("n_bero");
                setNextMap("win_03");
                reloadRoomMain();
            }
        }
    }

    //if just unlocked the red cage with the red key in ch2
    if (removedItemID == 16) {
        if (mario_st->gsw0 == 93) {
            if (!strcmp(ttyd::seq_mapchange::NextMap, "mri_03")) {
                mario_st->gsw0 = 94;
                setNextBero("dokan1");
                setNextMap("mri_20");
                reloadRoomMain();
            }
        }
    }

    return removedItemID;  
}

void drawModMain(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    char *tempDisplayBuffer = DisplayBuffer;
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    f32 yPos = -105;

    sprintf(tempDisplayBuffer,"Added Item ID: %d", keyItemReceived);
    DrawText(tempDisplayBuffer, -180, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    yPos -= 15.0f;
    sprintf(tempDisplayBuffer,"Removed Item ID: %d", keyItemRemovedCopy);
    DrawText(tempDisplayBuffer, -180, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    yPos -= 15.0f;
    sprintf(tempDisplayBuffer,"Flag: %08d", GSWFCopy);
    DrawText(tempDisplayBuffer, -190, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    yPos -= 15.0f;
    sprintf(tempDisplayBuffer,"Map: %s", ttyd::seq_mapchange::NextMap);
    DrawText(tempDisplayBuffer, -190, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    yPos -= 15.0f;
    sprintf(tempDisplayBuffer,"Seq: %d", mario_st->gsw0);
    DrawText(tempDisplayBuffer, -190, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    yPos -= 15.0f;
    sprintf(tempDisplayBuffer,"Bero: %s", ttyd::seq_mapchange::NextBero);
    DrawText(tempDisplayBuffer, -190, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
}

// Assembly patch functions.
extern "C" {
    // map_change_patches.s
    void startGSWF();
    void endGSWF();
    void startRemovePouch();
    void endRemovePouch();
    void startAddPouch();
    void endAddPouch();
    void startSeqSet();
    void endSeqSet();

    //for running custom code right after the sequence value has been updated
    void set_seq_hook(uint32_t seqValue) { return mod::seqSetHook(seqValue); }
    //for running custom code upon an item being removed (used for black chest)
    int32_t remove_pouch_item_hook(uint32_t arg0) { return mod::RemovePouchItemHook(arg0); }
    //for tracking the last flag set
    int32_t set_GSWF_Hook(uint32_t arg0) { return mod::setGSWFHook(arg0); }
    //for tracking the last item added to the pouch
    int32_t key_item_func_add(uint32_t arg0) { return mod::ItemAddHook(arg0); }
    //for tracking the last item removed from the pouch
    int32_t key_item_func_remove(uint32_t arg0) { return mod::ItemRemoveHook(arg0); }
}

MOD_INIT_FUNCTION() {
    mod::infinite_pit::core::ApplyFixedPatches();
    mod::infinite_pit::options::ApplyFixedPatches();

    //track last global flag set
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(swSet_BH),
        reinterpret_cast<void*>(swSet_EH),
        reinterpret_cast<void*>(startGSWF),
        reinterpret_cast<void*>(endGSWF));

    //track last pouch removes
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(ItemRemove_BH),
        reinterpret_cast<void*>(ItemRemove_EH),
        reinterpret_cast<void*>(startRemovePouch),
        reinterpret_cast<void*>(endRemovePouch));

    //track last pouch adds
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(ItemAdd_BH),
        reinterpret_cast<void*>(ItemAdd_EH),
        reinterpret_cast<void*>(startAddPouch),
        reinterpret_cast<void*>(endAddPouch));

    //set sequence hook
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(SetSeq_BH),
        reinterpret_cast<void*>(SetSeq_EH),
        reinterpret_cast<void*>(startSeqSet),
        reinterpret_cast<void*>(endSeqSet));

    //auto mash text with Y holding
    mod::patch::writePatch(reinterpret_cast<void*>(0x80080FCC),0x4BF84061U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80080FF0),0x4BF8403DU);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80084268),0x4BF80DC5U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x8000502C),0x3C60803DU);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005030),0xA063A398U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005034),0x70630800U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005038),0x2C030800U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x8000503C),0x38600200U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005040),0x4D820020U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005044),0x38600000U);
    mod::patch::writePatch(reinterpret_cast<void*>(0x80005048),0x48000F38U);

}



MOD_UPDATE_FUNCTION() {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();

    //after beating ch2 xnauts
    if (mario_st->gsw0 == 89) {
        mario_st->gsw0 = 90;
        setNextBero("dokan1");
        setNextMap("mri_02");
        reloadRoomMain();     
    } 
}

}

void setInitialFlags(void) {
    ttyd::swdrv::swSet(38); //read email
    ttyd::swdrv::swSet(233); //remove initial save block text
    ttyd::swdrv::swSet(234); //remove initial heart block text
    ttyd::swdrv::swSet(0); //remove shop explanation text
    ttyd::swdrv::swSet(1335); //stairs before plane curse
    ttyd::swdrv::swSet(1353); //initial plane curse text
    ttyd::swdrv::swSet(1492); //initial paper curse text
    //ttyd::swdrv::swSet(1369); //skip goombella's text about not equipping power smash
    ttyd::swdrv::swSet(1325); //spoke to dazzle for the first time
    ttyd::swdrv::swSet(1805); //goombella explaining her field ability in petal meadows
    //ttyd::swdrv::swSet(43); //removes blue switches from spawning that open hooktail's castle pipe

    //ttyd::swdrv::swSet(1337); //remove black key from ground for plane curse
    //ttyd::swdrv::swSet(1352); //plane curse chest open
}
            
void giveItemsToDebug(void) {
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::ZESS_DYNAMITE);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::JELLY_ULTRA);
    ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::JELLY_ULTRA);
}

int skipInitialCutscenesDebug(void) {
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    auto* mario_st = ttyd::mariost::g_MarioSt;
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();
    //if new file
    if (mario_st->gsw0 == 0) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "aaa_00")) {
            giveItemsToDebug();
            ttyd::swdrv::swSet(1775); //remove moon stone from the ground
            ttyd::swdrv::swSet(1774); //remove sun stone from the ground
            //ttyd::swdrv::swSet(1804); //open pipe to hooktail's castle
            //ttyd::swdrv::swSet(1796); //completed gold fuzzy fight?
            mario_st->gsw0 = 66; //saw punio in the sewer pre ch2
            player->prevPartyId[0] = 1;
            player->prevPartyId[1] = 0;
            ttyd::mario_party::partyJoin(1);
            ttyd::mario_party::partyJoin(2);
            ttyd::mario_pouch::pouchGetItem(33); //plane curse key
            ttyd::mario_pouch::pouchGetItem(4); //plane curse ability
            ttyd::mario_pouch::pouchGetItem(2); //paper curse ability
            giveStarByIndex(0); //give map
            giveStarByIndex(1); //give diamond star
            giveStarByIndex(2); //give emerald star
            setInitialFlags();
            setNextBero("dokan");
            setNextMap("tik_04");
            reloadRoomMain();
            return 1; //did skip cutscenes
        }
    }
    return 0;
}

int skipInitialCutscenesRelease(void) {
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();
    auto* mario_st = ttyd::mariost::g_MarioSt;
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();

    //if new file
    if (mario_st->gsw0 == 0) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "aaa_00")) {
            mario_st->gsw0 = 11; //after frankly asks if you know how action commands work
            player->prevPartyId[0] = 1;
            player->prevPartyId[1] = 0;
            ttyd::mario_party::partyJoin(1);
            setInitialFlags();
            setNextMap("gor_01");
            setNextBero("e_bero");
            pouch->star_points = 9; // 9 star points you would normally get from crump
            ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::MAGICAL_MAP);
            reloadRoomMain();
            return 1;
        }
    }   
    return 0; 
}

void skipCutscenesMain(void) {
    ttyd::mario_pouch::PouchData* pouch = ttyd::mario_pouch::pouchGetPtr();
    // ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    auto* mario_st = ttyd::mariost::g_MarioSt;
    #if DEBUG == TRUE
        if (skipInitialCutscenesDebug()) {return;}
    #else
        if (skipInitialCutscenesRelease()) {return;}
    #endif

    //if entering thousand year door room for first time
    if (mario_st->gsw0 == 15) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "tik_05")) {
            mario_st->gsw0 = 20;
            ttyd::mario_pouch::pouchGetItem(ttyd::item_data::ItemType::POWER_SMASH);
            pouch->star_powers_obtained |= 1;
            pouch->current_sp = 100;
            pouch->max_sp = 100;
            setNextBero("dokan_2");
            setNextMap("tik_01");
            reloadRoomMain();
            return;
        }
    }

    //entering petal meadows for the first time
    if (mario_st->gsw0 == 21) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "sys_01")) {
            mario_st->gsw0 = 28; //after koops talks to you after opening the gate in petalburg
            setNextBero("dokan_2");
            setNextMap("hei_00");
            reloadRoomMain();
            return;
        }
    }

    //entering shwonk fortress for quiz show
    if (mario_st->gsw0 == 30) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "hei_07")) {
            mario_st->gsw0 = 31; //after quiz is completed
            return;
        }
    }

    if (skipLevel == HEAVY) {
        if (mario_st->gsw0 == 32) {
            if (!strcmp(ttyd::seq_mapchange::NextMap, "hei_06")) {
                ttyd::mario_party::partyJoin(2);
                setNextBero("e_bero");
                setNextMap("hei_00");
                reloadRoomMain();
                return;
            }
        }
    }

    //if entering hooktail castle map, skip intro
    if (mario_st->gsw0 == 34) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "gon_01")) {
            mario_st->gsw0 = 37;
            setNextMap("gon_01");
            reloadRoomMain();
            return;
        }
    }

    //if entered red bones room, skip koops and his "dad" cutscene
    if (mario_st->gsw0 == 37) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "gon_03")) {
            mario_st->gsw0 = 38;
            setNextMap("gon_03");
            setNextBero("w_bero");
            reloadRoomMain();
            return;
        }
    }

    //just unlocked door using key gotten from paper mode ability
    if (mario_st->gsw0 == 47) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "gon_04")) {
            mario_st->gsw0 = 49;
            ttyd::swdrv::swSet(1483); //hit hooktail castle blue switch 1
            ttyd::swdrv::swSet(1484); //hit hooktail castle blue switch 2
            setNextMap("gon_04");
            setNextBero("e_bero_3");
            reloadRoomMain();
        }
    }

    //entered room with punio in with pipe to ch2
    if (mario_st->gsw0 == 66) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "tik_03")) {
            ttyd::swdrv::swSet(1334); //path to ch2 pipe opened by punio
            mario_st->gsw0 = 68;
            setNextMap("tik_03");
            setNextBero("s_bero_1");
            reloadRoomMain();
            return;
        }
    }

    //entered room with punio in with pipe to ch2
    if (mario_st->gsw0 == 70) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "sys_01")) {
            mario_st->gsw0 = 71;
            setNextMap("win_06");
            setNextBero("dokan1");
            reloadRoomMain();
            return;
        }
    }

    //entered room with shadow sirens in ch2
    if (mario_st->gsw0 == 71) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "win_00")) {
            if (skipLevel == HEAVY) {
                mario_st->gsw0 = 84;
                setNextMap("win_00");
                setNextBero("w_bero");
                reloadRoomMain();
                return;
            } else {
                mario_st->gsw0 = 72;
                setNextMap("win_00");
                setNextBero("w_bero");
                reloadRoomMain();
                return;
            }
        }
    }

    //entered room with entrance to great tree
    if (mario_st->gsw0 == 73) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "mri_00")) {
            mario_st->gsw0 = 74;
            setNextMap("mri_00");
            setNextBero("w_bero");
            reloadRoomMain();
            return;
        }
    }

    //entered room with entrance to great tree
    if (mario_st->gsw0 == 87) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "mri_00")) {
            mario_st->gsw0 = 88;
            setNextMap("mri_00");
            setNextBero("w_bero");
            reloadRoomMain();
            return;
        }
    }

    //entered room with xnaut and ms mowz cutscene
    if (mario_st->gsw0 == 90) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "mri_04")) {
            mario_st->gsw0 = 92;
            setNextMap("mri_03");
            setNextBero("dokan");
            reloadRoomMain();
            return;
        }
    }

    //entered room with xnaut and ms mowz cutscene
    if (mario_st->gsw0 == 98) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "mri_03")) {
            mario_st->gsw0 = 99;
            setNextMap("mri_08");
            setNextBero("e_bero");
            reloadRoomMain();
            return;
        }
    }
}