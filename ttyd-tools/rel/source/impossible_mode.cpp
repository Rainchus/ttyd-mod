#include "impossible_mode.h"
#include "ttyd/mario_pouch.h"

#define FALSE 0
#define TRUE 1

typedef enum BattleUnitType {
	kNullUnitKind, //0x0
	kUnitGoomba, //0x1
	kUnitParagoomba, //0x2
	kUnitSpikyGoomba, //0x3
	kUnitSpinia, //0x4
	kUnitSpania, //0x5
	kUnitLordCrumpPrologue, //0x6
	kUnitGus, //0x7
	kUnitBlooper, //0x8
	kUnitBlooperLeftTentacle, //0x9
	kUnitBlooperRightTentacle, //0xA
	kUnitKoopatrol, //0xB
	kUnitMagikoopa, //0xC
	kUnitMagikoopaClone, //0xD
	kUnitKoopaTroopa, //0xE
	kUnitParatroopa, //0xF
	kUnitFuzzy, //0x10
	kUnitDullBones, //0x11
	kUnitBaldCleft, //0x12
	kUnitBristle, //0x13
	kUnitGoldFuzzy, //0x14
	kUnitFuzzyHorde, //0x15
	kUnitRedBones, //0x16
	kUnitHooktail, //0x17
	kUnitDarkPuff, //0x18
	kUnitPalePiranha, //0x19
	kUnitCleft, //0x1A
	kUnitPider, //0x1B
	kUnitXNaut, //0x1C
	kUnitYux, //0x1D
	kUnitMiniYux, //0x1E
	kUnitBeldamChapter2, //0x1F
	kUnitMarilynChapter2, //0x20
	kUnitVivianChapter2, //0x21
	kUnitMagnus, //0x22
	kUnitXFist, //0x23
	kUnitGoombaGlitzville, //0x24
	kUnitKpKoopa, //0x25
	kUnitKpParatroopa, //0x26
	kUnitPokey, //0x27
	kUnitLakitu, //0x28
	kUnitSpiny, //0x29
	kUnitHyperBaldCleft, //0x2A
	kUnitBobOmb, //0x2B
	kUnitBandit, //0x2C
	kUnitBigBandit, //0x2D
	kUnitRedSpikyBuzzy, //0x2E
	kUnitShadyKoopa, //0x2F
	kUnitShadyParatroopa, //0x30
	kUnitRedMagikoopa, //0x31
	kUnitRedMagikoopaClone, //0x32
	kUnitWhiteMagikoopa, //0x33
	kUnitWhiteMagikoopaClone, //0x34
	kUnitGreenMagikoopa, //0x35
	kUnitGreenMagikoopaClone, //0x36
	kUnitDarkCraw, //0x37
	kUnitHammerBro, //0x38
	kUnitBoomerangBro, //0x39
	kUnitFireBro, //0x3A
	kUnitRedChomp, //0x3B
	kUnitDarkKoopatrol, //0x3C
	kUnitIronCleftRed, //0x3D
	kUnitIronCleftGreen, //0x3E
	kUnitBowserChapter3, //0x3F
	kUnitRawkHawk, //0x40
	kUnitMachoGrubba, //0x41
	kUnitHyperGoomba, //0x42
	kUnitHyperParagoomba, //0x43
	kUnitHyperSpikyGoomba, //0x44
	kUnitCrazeeDayzee, //0x45
	kUnitAmazyDayzee, //0x46
	kUnitHyperCleft, //0x47
	kUnitBuzzyBeetle, //0x48
	kUnitSpikeTop, //0x49
	kUnitSwooper, //0x4A
	kUnitBoo, //0x4B
	kUnitAtomicBoo, //0x4C
	kUnitDooplissChapter4Fight1, //0x4D
	kUnitDooplissChapter4Invincible, //0x4E
	kUnitDooplissChapter4Fight2, //0x4F
	kUnitGoombellaChapter4, //0x50
	kUnitKoopsChapter4, //0x51
	kUnitYoshiChapter4, //0x52
	kUnitFlurrieChapter4, //0x53
	kUnitEmber, //0x54
	kUnitLavaBubble, //0x55
	kUnitGreenFuzzy, //0x56
	kUnitFlowerFuzzy, //0x57
	kUnitPutridPiranha, //0x58
	kUnitParabuzzy, //0x59
	kUnitBillBlaster, //0x5A
	kUnitBulletBill, //0x5B
	kUnitBulkyBobOmb, //0x5C
	kUnitCortez, //0x5D
	kUnitCortezBonePile, //0x5E
	kUnitCortezSword, //0x5F
	kUnitCortezHook, //0x60
	kUnitCortezRapier, //0x61
	kUnitCortezSaber, //0x62
	kUnitLordCrumpChapter5, //0x63
	kUnitXNautsCrumpFormation1, //0x64
	kUnitXNautsCrumpFormation2, //0x65
	kUnitXNautsCrumpFormation3, //0x66
	kUnitRuffPuff, //0x67
	kUnitPoisonPokey, //0x68
	kUnitSpikyParabuzzy, //0x69
	kUnitDarkBoo, //0x6A
	kUnitSmorg, //0x6B
	kUnitSmorgMiasmaTentacleA, //0x6C
	kUnitSmorgMiasmaTentacleB, //0x6D
	kUnitSmorgMiasmaTentacleC, //0x6E
	kUnitSmorgMiasmaClaw, //0x6F
	kUnitIcePuff, //0x70
	kUnitFrostPiranha, //0x71
	kUnitMoonCleft, //0x72
	kUnitZYux, //0x73
	kUnitMiniZYux, //0x74
	kUnitXYux, //0x75
	kUnitMIniXYux, //0x76
	kUnitXNautPhD, //0x77
	kUnitEliteXNaut, //0x78
	kUnitMagnus20, //0x79
	kUnitXPunch, //0x7A
	kUnitSwoopula, //0x7B
	kUnitPhantomEmber, //0x7C
	kUnitBombshellBillBlaster, //0x7D
	kUnitBombshellBill, //0x7E
	kUnitChainChomp, //0x7F
	kUnitDarkWizzerd, //0x80
	kUnitDarkWizzerdClone, //0x81
	kUnitDryBones, //0x82
	kUnitDarkBones, //0x83
	kUnitGloomtail, //0x84
	kUnitBeldamChapter8, //0x85
	kUnitMarilynChapter8, //0x86
	kUnitDooplissChapter8, //0x87
	kUnitDooplissChapter8Mario, //0x88
	kUnitDooplissChapter8Goombella, //0x89
	kUnitDooplissChapter8Koops, //0x8A
	kUnitDooplissChapter8Yoshi, //0x8B
	kUnitDooplissChapter8Flurrie, //0x8C
	kUnitDooplissChapter8Vivian, //0x8D
	kUnitDooplissChapter8Bobbery, //0x8E
	kUnitDooplissChapter8MsMowz, //0x8F
	kUnitBowserChapter8, //0x90
	kUnitKammyKoopa, //0x91
	kUnitGrodus, //0x92
	kUnitGrodusX, //0x93
	kUnitShadowQueenPeach, //0x94
	kUnitShadowQueenInvincible, //0x95
	kUnitShadowQueenDemon, //0x96
	kUnitShadowQueenLeftRightHand, //0x97
	kUnitShadowQueenDeadHands, //0x98
	kUnitGloomba, //0x99
	kUnitParagloomba, //0x9A
	kUnitSpikyGloomba, //0x9B
	kUnitDarkKoopa, //0x9C
	kUnitDarkParatroopa, //0x9D
	kUnitBadgeBandit, //0x9E
	kUnitDarkLakitu, //0x9F
	kUnitSkyBlueSpiny, //0xA0
	kUnitWizzerd, //0xA1
	kUnitPiranhaPlant, //0xA2
	kUnitSpunia, //0xA3
	kUnitArantula, //0xA4
	kUnitDarkBristle, //0xA5
	kUnitPoisonPuff, //0xA6
	kUnitSwampire, //0xA7
	kUnitBobUlk, //0xA8
	kUnitEliteWizzerd, //0xA9
	kUnitEliteWizzerdClone, //0xAA
	kUnitBonetail, //0xAB
	kUnitUnusedRedBuzzy, //0xAC
	kUnitUnusedRedParabuzzy, //0xAD
	kUnitUnusedRedSpikyParabuzzy, //0xAE
	kUnitUnusedHyperBobOmb, //0xAF
	kUnitUnusedUltraBobOmb, //0xB0
	kUnitTutorialGoombella, //0xB1
	kUnitTutorialFranklyB2, //0xB2
	kUnitTutorialFranklyB3, //0xB3
	kUnitTutorialFranklyB4, //0xB4
	kUnitEpilogueDooplissMario, //0xB5
	kUnitEpilogueFlurrie, //0xB6
	kUnitEpilogueBoo, //0xB7
	kUnitEpilogueAtomicBoo, //0xB8
	kUnitEpilogueMaleToad, //0xB9
	kUnitEpilogueFemaleToad, //0xBA
	kUnitUnusedTest, //0xBB
	kUnitUnusedCrump2, //0xBC
	kUnitUnusedBeldam2, //0xBD
	kUnitUnusedMarilyn2, //0xBE
	kUnitUnusedVivian2, //0xBF
	kUnitUnusedBeldam3, //0xC0
	kUnitUnusedMarilyn3, //0xC1
	kUnitUnusedMechaKuri, //0xC2
	kUnitUnusedMechaKame, //0xC3
	kUnitUnusedOkorl, //0xC4
	kUnitUnusedYowarl, //0xC5
	kUnitUnusedTuyonarl, //0xC6
	kUnitUnusedWanawana, //0xC7
	kUnitUnusedMinaraiKamec, //0xC8
	kUnitUnusedShyGuy, //0xC9
	kUnitUnusedGrooveGuy, //0xCA
	kUnitUnusedPyroGuy, //0xCB
	kUnitUnusedSpyGuy, //0xCC
	kUnitUnusedAntiGuy, //0xCD
	kUnitUnusedBzzap, //0xCE
	kUnitUnusedMiniBzzap, //0xCF
	kUnitUnusedUfo, //0xD0
	kUnitUnusedPennington, //0xD1
	kUnitUnusedFighter, //0xD2
	kUnitUnusedZessT, //0xD3
	kUnitUnusedMaster, //0xD4
	kUnitUnusedReporter, //0xD5
	kUnitUnusedHotdogMaster, //0xD6
	kUnitUnusedFlavio, //0xD7
	kUnitUnusedTree, //0xD8
	kUnitUnusedSwitch, //0xD9
	kUnitUnusedTestnpc, //0xDA
	kUnitBombSquadBomb, //0xDB
	kUnitSystem, //0xDC
	kUnitPrologueGoombella, //0xDD
	UNIT_MARIO, //0xDE
	kUnitShellShield, //0xDF
	kUnitGoombella, //0xE0
#define TYPE_PARTNER_MIN kUnitGoombella
	kUnitKoops, //0xE1
	kUnitYoshi, //0xE2
	kUnitFlurrie, //0xE3
	kUnitVivian, //0xE4
	kUnitBobbery, //0xE5
	kUnitMsMowz, //0xE6
#define TYPE_PARTNER_MAX kUnitMsMowz+1
	kUnitMax //0xE7
} BattleUnitType;

#define DEBUG TRUE

char DisplayBuffer[256];

namespace mod {

extern "C" {
s32 HP_Multiplier = 150;
s32 Atk_Multiplier = 100;
s8 Def_Extra = 0;

void OSReport(const char *fmt, ...);

void TestOSReport(void) {
    OSReport("Testing\n");
}

}



void drawModMain(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    char *tempDisplayBuffer = DisplayBuffer;
    f32 xPos = -265;
    f32 yPos = -170;

    if (mario_st->bInBattle == TRUE) {
        //check if player is attempting to change modifier
        
        sprintf(tempDisplayBuffer,"Hp:\t%.1fx", HP_Multiplier/100.0f);
        DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 0);

        yPos -= 15.0f;

        sprintf(tempDisplayBuffer,"Atk:\t%.1fx", Atk_Multiplier/100.0f);
        DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 0);

        yPos -= 15.0f;

        if (Def_Extra <= 0) {
            sprintf(tempDisplayBuffer,"Def:\t-%d", Def_Extra);
            DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 0);
        } else {
            sprintf(tempDisplayBuffer,"Def:\t+%d", Def_Extra);
            DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 0);
        }
    }
}

// Assembly patch functions.
extern "C" {
    void start_enemy_hp();
    void end_enemy_hp();

    // void start_enemy_hp_2();
    // void end_enemy_hp_2();

    void start_enemy_atk();
    void end_enemy_atk();
}

// void TattleAllEnemies(void) {
//     //063DAFBC
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFBC),0xF8000000U);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFC0),0xFFFFFFFEU);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFC4),0xFFFFFFFFU);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFC8),0xFFFFC3FFU);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFCC),0xFFFFFFFFU);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFD0),0xFFFFFFFFU);
//     mod::patch::writePatch(reinterpret_cast<void*>(0x803DAFD4),0x0F80003FU);
// }

MOD_INIT_FUNCTION() {
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

    //mod::patch::writePatch(reinterpret_cast<void*>(0x802EE018),0x0A090800U); //remove normal guard window
    
    //mod::TattleAllEnemies(); //why does this not work

    //modify enemy hp as they spawn in patch 1
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(start_enemy_hp_BH),
        reinterpret_cast<void*>(start_enemy_hp_EH),
        reinterpret_cast<void*>(start_enemy_hp),
        reinterpret_cast<void*>(end_enemy_hp));

    // //related to modify enemy hp...what does this do?
    // mod::patch::writeBranchPair(
    //     reinterpret_cast<void*>(start_enemy_hp_2_BH),
    //     reinterpret_cast<void*>(start_enemy_hp_2_EH),
    //     reinterpret_cast<void*>(start_enemy_hp_2),
    //     reinterpret_cast<void*>(end_enemy_hp_2));

    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(start_enemy_atk_BH),
        reinterpret_cast<void*>(end_enemy_atk_EH),
        reinterpret_cast<void*>(start_enemy_atk),
        reinterpret_cast<void*>(end_enemy_atk));

}

// void GiveAndEquipPeekaboo(void) {
//     #define kItemPeekaboo 0x135
//     s32 peekabooInBadgesResult = ttyd::mario_pouch::pouchCheckItem(kItemPeekaboo);

//     if (peekabooInBadgesResult == FALSE) {
//         ttyd::mario_pouch::pouchGetItem(kItemPeekaboo);
//     }

//     s32 peekabooEquippedResult = ttyd::mario_pouch::pouchEquipCheckBadge(kItemPeekaboo); //peekaboo result
//     if (peekabooEquippedResult == FALSE) {
//         ttyd::mario_pouch::pouchEquipBadgeID(kItemPeekaboo);
//     }
// }

MOD_UPDATE_FUNCTION() {
    //mod::GiveAndEquipPeekaboo();
}

}