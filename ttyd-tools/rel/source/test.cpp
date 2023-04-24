#include <ttyd/system.h>
#include "mod.h"
#include "patches_core.h"

namespace mod::infinite_pit {

// Applies all patches that only need to be applied once at initialization.
inline void ApplyAllFixedPatches() {
    core::ApplyFixedPatches();
}

}

extern "C" {
    void ApplyAllFixedPatches() { mod::infinite_pit::ApplyAllFixedPatches(); }
}


typedef unsigned char u8;
namespace mod {

MOD_INIT_FUNCTION() {
    //ApplyAllFixedPatches();
}

MOD_UPDATE_FUNCTION() {

}

}