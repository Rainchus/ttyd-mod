#include <ttyd/system.h>
#include "mod.h"
#include "patches_core.h"
#include "patch.h"
#include "ttyd/mariost.h"
#include "ttyd/seq_mapchange.h"
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

MOD_UPDATE_FUNCTION() {

}
}

void skipPeachIntermissions(void) {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    const char* area = ttyd::seq_mapchange::NextArea;

}