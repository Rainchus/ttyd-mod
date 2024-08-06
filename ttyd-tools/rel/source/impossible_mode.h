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

void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale);
void reloadRoomMain(void);

extern const int32_t swSet_BH = 0x800858FC;
extern const int32_t swSet_EH = 0x80085900;

extern const int32_t start_enemy_hp_BH = 0x80129248;
extern const int32_t start_enemy_hp_EH = 0x8012924C;

extern const int32_t start_enemy_hp_2_BH = 0x8012925C;
extern const int32_t start_enemy_hp_2_EH = 0x80129260;

extern const int32_t start_enemy_atk_BH = 0x800FD890;
extern const int32_t end_enemy_atk_EH = 0x800FD894;

extern const int32_t ItemAdd_BH = 0x800d50CC;
extern const int32_t ItemAdd_EH = 0x800d50D0;

extern const int32_t SetSeq_BH = 0x8002e220;
extern const int32_t SetSeq_EH = 0x8002e224;

typedef double f64;
typedef float f32;
typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef char s8;
typedef unsigned char u8;