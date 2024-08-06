#include "impossible_mode.h"

#define FALSE 0
#define TRUE 1

#define DEBUG TRUE

char DisplayBuffer[256];

namespace mod {

extern "C" {
s32 HP_Multiplier = 150;
s32 Atk_Multiplier = 300;
}

void drawModMain(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    char *tempDisplayBuffer = DisplayBuffer;
    f32 xPos = -230;
    f32 yPos = -185;

    if (mario_st->bInBattle == TRUE) {
        sprintf(tempDisplayBuffer,"Hp:  %.1fx", HP_Multiplier/100.0f);
        DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);

        yPos -= 15.0f;

        sprintf(tempDisplayBuffer,"Atk: %.1fx", Atk_Multiplier/100.0f);
        DrawText(tempDisplayBuffer, xPos, yPos, 255, true, ~0U, 0.75f, /* alignment = center */ 4);
    }
}

// Assembly patch functions.
extern "C" {
    void start_enemy_hp();
    void end_enemy_hp();

    void start_enemy_hp_2();
    void end_enemy_hp_2();

    void start_enemy_atk();
    void end_enemy_atk();
}

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
    mod::patch::writePatch(reinterpret_cast<void*>(0x802EE018),0x0A090800U); //remove normal guard window
    

    //modify enemy hp as they spawn in patch 1
    mod::patch::writeBranchPair(
        reinterpret_cast<void*>(start_enemy_hp_BH),
        reinterpret_cast<void*>(start_enemy_hp_EH),
        reinterpret_cast<void*>(start_enemy_hp),
        reinterpret_cast<void*>(end_enemy_hp));

    //related to modify enemy hp...what does this do?
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

MOD_UPDATE_FUNCTION() {
}

}