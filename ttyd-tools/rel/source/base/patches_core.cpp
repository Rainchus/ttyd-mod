#include <cstdint>
#include <cstring>
#include "patches_core.h"
#include "ttyd/seqdrv.h"
#include "gc/OSLink.h"
#include "ttyd/swdrv.h"
#include "ttyd/seq_mapchange.h"
#include "ttyd/mariost.h"
#include "ttyd/filemgr.h"
#include "ttyd/system.h"
#include "ttyd/mario_pouch.h"
#include "ttyd/item_data.h"
#include "ttyd/memory.h"
#include "patch.h"
#include "mod.h"

extern const int32_t g_seq_mapChangeMain_OnMapUnload_BH = 0x80007e0c;
extern const int32_t g_seq_mapChangeMain_MapLoad_BH = 0x80007ef0;
extern const int32_t g_seq_mapChangeMain_MapLoad_EH = 0x80008148;

// Assembly patch functions.
extern "C" {
    // map_change_patches.s
    void StartMapLoad();
    void BranchBackMapLoad();
    void StartOnMapUnload();
    void BranchBackOnMapUnload();
    
    int32_t mapLoad() { return mod::infinite_pit::core::LoadMap(); }
    void onMapUnload() { mod::infinite_pit::core::OnMapUnloaded(); }
}

namespace mod::infinite_pit {
namespace {

using ::gc::OSLink::OSModuleInfo;
using ::ttyd::mario_pouch::PouchData;
using ::ttyd::seqdrv::SeqIndex;
using ::ttyd::system::getMarioStDvdRoot;

namespace ItemType = ::ttyd::item_data::ItemType;

}

namespace core {
    
void OnFileLoad(bool new_file = true) {

}

void ApplyFixedPatches() {

}

int32_t LoadMap() {
    auto* mario_st = ttyd::mariost::g_MarioSt;
    const char* area = ttyd::seq_mapchange::NextArea;

    if (!strcmp(ttyd::seq_mapchange::NextMap, "title")) {
        strcpy(mario_st->unk_14c, mario_st->currentMapName);
        strcpy(mario_st->currentAreaName, "");
        strcpy(mario_st->currentMapName, "");
        ttyd::seqdrv::seqSetSeq(
            ttyd::seqdrv::SeqIndex::kTitle, nullptr, nullptr);
        return 1;
    }

    if (!strcmp(area, "tou")) {
        if (ttyd::seqdrv::seqGetSeq() == ttyd::seqdrv::SeqIndex::kTitle) {
            area = "tou2";
        } else if (!strcmp(ttyd::seq_mapchange::NextMap, "tou_03")) {
            area = "tou2";
        }
    }
    
    // Start loading the relocatable module associated with the current area.
    if (ttyd::filemgr::fileAsyncf(
        nullptr, nullptr, "%s/rel/%s.rel", getMarioStDvdRoot(), area)) {
        auto* file = ttyd::filemgr::fileAllocf(
            nullptr, "%s/rel/%s.rel", getMarioStDvdRoot(), area);
        if (file) {
            if (!strncmp(area, "tst", 3) || !strncmp(area, "jon", 3)) {
                auto* module_info = reinterpret_cast<OSModuleInfo*>(
                    ttyd::memory::_mapAlloc(
                        ttyd::memory::mapalloc_base_ptr,
                        reinterpret_cast<int32_t>(file->mpFileData[1])));
                mario_st->pRelFileBase = module_info;
            } else {
                mario_st->pRelFileBase = mario_st->pMapAlloc;
            }
            memcpy(
                mario_st->pRelFileBase, *file->mpFileData,
                reinterpret_cast<int32_t>(file->mpFileData[1]));
            ttyd::filemgr::fileFree(file);
        }
        if (mario_st->pRelFileBase != nullptr) {
            memset(&ttyd::seq_mapchange::rel_bss, 0, 0x3c4);
            gc::OSLink::OSLink(
                mario_st->pRelFileBase, &ttyd::seq_mapchange::rel_bss);
        }
        ttyd::seq_mapchange::_load(
            mario_st->currentMapName, ttyd::seq_mapchange::NextMap,
            ttyd::seq_mapchange::NextBero);
        reinterpret_cast<void(*)(void)>(mario_st->pRelFileBase->prolog)();
        return 2;
    }
    return 1;
}

void OnMapUnloaded() {
    // Normal unloading logic follows...
}

}  // namespace core
}  // namespace mod::infinite_pit