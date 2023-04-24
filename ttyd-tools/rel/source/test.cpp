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
#include "battle_patches.h"
#include "ttyd/swdrv.h"

enum PAD_INPUTS
{
    PAD_DPAD_LEFT  = 0x0001,
    PAD_DPAD_RIGHT = 0x0002,
    PAD_DPAD_DOWN  = 0x0004,
    PAD_DPAD_UP    = 0x0008,
    PAD_Z          = 0x0010,
    PAD_R          = 0x0020,
    PAD_L          = 0x0040,
    // unused      = 0x0080,
    PAD_A          = 0x0100,
    PAD_B          = 0x0200,
    PAD_X          = 0x0400,
    PAD_Y          = 0x0800,
    PAD_START      = 0x1000,
};

char DisplayBuffer[256];
void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale);

void unkMashFunc1(int);
void unkMashFunc2(int);
void unkMashFunc3(int);
void reloadRoomMain(void);

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

uint32_t autoMashText(uint32_t controllerPort)
{
    if (!checkButtonComboEveryFrame(PAD_Y))
    {
        // Return the intended value
        return ttyd::system::keyGetButtonTrg(controllerPort);
    }
    
    // Return the value for B to make sure the text is being mashed through as fast as possible
    return PAD_B;
}

// 800d4c38:pouchRemoveItem
// 800d4ed4:pouchCheckItem
// 800d50cc:pouchGetItem

typedef unsigned char u8;
namespace mod {

void (*tempTestPtr)(int arg0) = nullptr;
extern const int32_t swSet_BH = 0x800858FC;
extern const int32_t swSet_EH = 0x80085900;

extern const int32_t ItemRemove_BH = 0x800D4C38;
extern const int32_t ItemRemove_EH = 0x800D4C3C;

extern const int32_t ItemAdd_BH = 0x800d50CC;
extern const int32_t ItemAdd_EH = 0x800d50D0;

extern const int32_t SetSeq_BH = 0x8002e220;
extern const int32_t SetSeq_EH = 0x8002e224;

int32_t GSWFCopy;
int32_t keyItemReceived;
int32_t keyItemRemovedCopy;

uint32_t setGSWFHook(uint32_t arg0) {
    GSWFCopy = arg0;
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

uint32_t CheckIfShouldExit(uint32_t arg0) {
    auto* mario_st = ttyd::mariost::g_MarioSt;

    //if just gave black key to plane curse chest
    if (mario_st->gsw0 == 13) { //update to be correct (should be 13)
        if (!strcmp(ttyd::seq_mapchange::NextMap, "tik_19")) {
            ttyd::mario_pouch::pouchGetItem(4); //plane curse ability
            setNextBero("n_bero_5");
            setNextMap("tik_04");
            ttyd::swdrv::swSet(1352); //plane curse chest open
            mario_st->gsw0 = 15;
            reloadRoomMain();
        }
    }
    return arg0;  
}

void drawModMain(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    char *tempDisplayBuffer = DisplayBuffer;
    //struct PouchData* pouch = *ttyd:mario_pouch:pouchGetPtr();

    sprintf(tempDisplayBuffer,"Added Item ID: %d", keyItemReceived);
    DrawText(tempDisplayBuffer, -180, -105, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Removed Item ID: %d", keyItemRemovedCopy);
    DrawText(tempDisplayBuffer, -180, -120, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Flag: %d", GSWFCopy);
    DrawText(tempDisplayBuffer, -190, -135, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Map: %s", ttyd::seq_mapchange::NextMap);
    DrawText(tempDisplayBuffer, -190, -150, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Seq: %d", mario_st->gsw0);
    DrawText(tempDisplayBuffer, -190, -165, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

    sprintf(tempDisplayBuffer,"Bero: %s", ttyd::seq_mapchange::NextBero);
    DrawText(tempDisplayBuffer, -190, -180, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
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

    void set_seq_hook(uint32_t seqValue) { return mod::seqSetHook(seqValue); }
    int32_t check_if_should_exit(uint32_t arg0) { return mod::CheckIfShouldExit(arg0); }
    int32_t set_GSWF_Hook(uint32_t arg0) { return mod::setGSWFHook(arg0); }
    int32_t key_item_func_add(uint32_t arg0) { return mod::ItemAddHook(arg0); }
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

}



MOD_UPDATE_FUNCTION() {

}

}

void setInitialFlags(void) {
    ttyd::swdrv::swSet(233); //remove initial save block text
    ttyd::swdrv::swSet(0); //remove shop explanation text
    ttyd::swdrv::swSet(1335); //stairs before plane curse
    ttyd::swdrv::swSet(1353); //initial plane curse text
    ttyd::swdrv::swSet(1369); //skip goombella's text about not equipping power smash

    //ttyd::swdrv::swSet(1337); //black key for plane curse flag
    //pouchGetItem give black key
    //ttyd::swdrv::swSet(1352); //plane curse chest open
}

void doInitialSetup(void) {
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    auto* mario_st = ttyd::mariost::g_MarioSt;

    mario_st->gsw0 = 11; //after frankly asks if you know how action commands work
    player->prevPartyId[0] = 1;
    player->prevPartyId[1] = 0;
    ttyd::mario_party::partyJoin(1);
    //ttyd::mario_pouch::pouchGetItem(33); //plane curse key
    setInitialFlags();
    setNextMap("gor_01");
    reloadRoomMain();
}

#define DEBUG

void skipPeachIntermissions(void) {
    ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
    auto* mario_st = ttyd::mariost::g_MarioSt;

    #ifndef DEBUG
    //if opening scene
    if (mario_st->gsw0 == 0) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "aaa_00")) {
            doInitialSetup();
            return;
        }
    }
    #else
    if (mario_st->gsw0 == 0) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "aaa_00")) {
            mario_st->gsw0 = 13; //after frankly asks if you know how action commands work
            player->prevPartyId[0] = 1;
            player->prevPartyId[1] = 0;
            ttyd::mario_party::partyJoin(1);
            ttyd::mario_pouch::pouchGetItem(33); //plane curse key
            setInitialFlags();
            setNextMap("tik_19");
            reloadRoomMain();
            return;
        }
    }
    #endif

    //if entering thousand year door room for first time
    if (mario_st->gsw0 == 15) {
        if (!strcmp(ttyd::seq_mapchange::NextMap, "tik_05")) {
            mario_st->gsw0 = 18;
            setInitialFlags();
            setNextMap("gor_02");
            reloadRoomMain();
            return;
        }
    }
}