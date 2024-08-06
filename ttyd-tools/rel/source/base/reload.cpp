#include "ttyd/seqdrv.h"
#include "ttyd/fadedrv.h"
#include "ttyd/seq_mapchange.h"
#include "string.h"
#include "ttyd/camdrv.h"
#include "ttyd/mariost.h"
#include "ttyd/pmario_sound.h"
#include "ttyd/mario_cam.h"

// Fades return values
#define FADE_NOT_ACTIVE      -2
#define FADE_DONT_RESOLVE    -1
#define FADE_RESOLVE_SUCCESS 1

int32_t resolveFade(uint32_t index)
{
    // Check if the current fade is active
    ttyd::fadedrv::FadeEntry *FadeEntry = &ttyd::fadedrv::gpFadeWork->entry[index];
    
    bool FadeIsActive = FadeEntry->flags & (1 << 0); // Check if 0 bit is active
    if (!FadeIsActive)
    {
        return FADE_NOT_ACTIVE;
    }
    
    // Check if the selected fade needs to be resolved
    ttyd::fadedrv::FadeType Type = FadeEntry->fadeType;
    
    switch (Type)
    {
        case ttyd::fadedrv::FadeType::kBlackMarioHeadFadeOut:
        {
            Type = ttyd::fadedrv::FadeType::kBlackMarioHeadFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackCircleFadeOut:
        {
            Type = ttyd::fadedrv::FadeType::kBlackCircleFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackCircleFadeOut2:
        {
            Type = ttyd::fadedrv::FadeType::kBlackCircleFadeIn2;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackCircleFadeOut3:
        {
            Type = ttyd::fadedrv::FadeType::kBlackCircleFadeIn3;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackFadeOut:
        {
            Type = ttyd::fadedrv::FadeType::kBlackFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackFadeOut2:
        {
            Type = ttyd::fadedrv::FadeType::kBlackFadeIn2;
            break;
        }
        case ttyd::fadedrv::FadeType::kBlackStatic:
        case ttyd::fadedrv::FadeType::kBlackFadeOut3:
        {
            Type = ttyd::fadedrv::FadeType::kBlackFadeIn3;
            break;
        }
        case ttyd::fadedrv::FadeType::kWhiteFadeOut:
        {
            Type = ttyd::fadedrv::FadeType::kWhiteFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kWhiteFadeOut2:
        {
            Type = ttyd::fadedrv::FadeType::kWhiteFadeIn2;
            break;
        }
        case ttyd::fadedrv::FadeType::kWhiteFadeOut3:
        {
            Type = ttyd::fadedrv::FadeType::kWhiteFadeIn3;
            break;
        }
        case ttyd::fadedrv::FadeType::kCurtainPullDown:
        {
            Type = ttyd::fadedrv::FadeType::kCurtainPullUp;
            break;
        }
        case ttyd::fadedrv::FadeType::kCurtainPullLeft:
        {
            Type = ttyd::fadedrv::FadeType::kCurtainPullRight;
            break;
        }
        case ttyd::fadedrv::FadeType::kCurtainStatic:
        case ttyd::fadedrv::FadeType::kCurtainPullLeft2:
        {
            Type = ttyd::fadedrv::FadeType::kCurtainPullRight2;
            break;
        }
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainClose:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainClosedStatic:
        {
            Type = ttyd::fadedrv::FadeType::kTitleScreenCurtainOpen;
            break;
        }
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainOpen:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainOpenStatic:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainAppearGradual:
        {
            Type = ttyd::fadedrv::FadeType::kTitleScreenCurtainLeaveGradual;
            break;
        }
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainOpen2:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainOpenStatic2:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainAppearGradual2:
        {
            Type = ttyd::fadedrv::FadeType::kTitleScreenCurtainLeaveGradual2;
            break;
        }
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainClosed2:
        case ttyd::fadedrv::FadeType::kTitleScreenCurtainClosedStatic2:
        {
            Type = ttyd::fadedrv::FadeType::kTitleScreenCurtainOpen2;
            break;
        }
        case ttyd::fadedrv::FadeType::kEnterPipeFromUp:
        {
            Type = ttyd::fadedrv::FadeType::kExitPipeFromUp;
            break;
        }
        case ttyd::fadedrv::FadeType::kEnterPipeFromDown:
        {
            Type = ttyd::fadedrv::FadeType::kExitPipeFromDown;
            break;
        }
        case ttyd::fadedrv::FadeType::kEnterPipeFromLeft:
        {
            Type = ttyd::fadedrv::FadeType::kExitPipeFromLeft;
            break;
        }
        case ttyd::fadedrv::FadeType::kEnterPipeFromRight:
        {
            Type = ttyd::fadedrv::FadeType::kExitPipeFromRight;
            break;
        }
        case ttyd::fadedrv::FadeType::kFreezeScreen:
        {
            Type = ttyd::fadedrv::FadeType::kUnfreezeScreenPullTopRight;
            break;
        }
        case ttyd::fadedrv::FadeType::kMarioHeadFadeOut:
        case ttyd::fadedrv::FadeType::kMarioHeadStaticBlinking:
        {
            Type = ttyd::fadedrv::FadeType::kMarioHeadFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kPeachHeadFadeOut:
        case ttyd::fadedrv::FadeType::kPeachHeadStatic:
        {
            Type = ttyd::fadedrv::FadeType::kPeachHeadFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kBowserHeadFadeOut:
        case ttyd::fadedrv::FadeType::kBowserHeadStatic:
        {
            Type = ttyd::fadedrv::FadeType::kBowserHeadFadeIn;
            break;
        }
        case ttyd::fadedrv::FadeType::kCurtainPullLeft3:
        {
            Type = ttyd::fadedrv::FadeType::kCurtainPullRight3;
            break;
        }
        default:
        {
            // The selected fade does not need to be resolved
            return FADE_DONT_RESOLVE;
        }
    }
    
    // Resolve the selected fade
    uint32_t Color = 0x000000FF;
    int32_t Duration = 0;
    
    ttyd::fadedrv::fadeEntry(Type, Duration, reinterpret_cast<uint8_t *>(&Color));
    return FADE_RESOLVE_SUCCESS;
}

bool checkForSpecificSeq(ttyd::seqdrv::SeqIndex wantedSeq)
{
    ttyd::seqdrv::SeqIndex NextSeq = ttyd::seqdrv::seqGetNextSeq();
    if (NextSeq != wantedSeq)
    {
        return false;
    }
    
    ttyd::seqdrv::SeqIndex CurrentSeq = ttyd::seqdrv::seqGetSeq();
    return CurrentSeq == wantedSeq;
}

void setSeqMapChange(const char *map, const char *bero)
{
    ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, (void*)map, (void*)bero);
}

bool checkIfSystemLevelIsSet()
{
    uint32_t SystemLevelFlags = ttyd::mariost::marioStGetSystemLevel();
    int32_t SystemLevel = ttyd::mariost::_mariostSystemLevel;
    
    if ((SystemLevelFlags == 0) && (SystemLevel == 0))
    {
        return false;
    }
    
    return true;
}

struct ReloadRoomStruct
{
    bool ManuallyReloadingRoom;
    bool SystemLevelShouldBeLowered;
    char NewBero[17]; // 16 bytes for NewBero, one byte for NULL
    char NewMap[9]; // 8 bytes for NewMap, one byte for NULL
};

struct ReloadRoomStruct ReloadRoom;

void reloadRoomMain()
{
    ttyd::seqdrv::SeqIndex NextSeq   = ttyd::seqdrv::seqGetNextSeq();
    ttyd::seqdrv::SeqIndex Game      = ttyd::seqdrv::SeqIndex::kGame;
    ttyd::seqdrv::SeqIndex MapChange = ttyd::seqdrv::SeqIndex::kMapChange;
    
    // Only run while a file is loaded
    if ((NextSeq < Game) || (NextSeq > MapChange))
    {
        return;
    }
    
    ttyd::seqdrv::SeqIndex Seq = ttyd::seqdrv::seqGetSeq();
    ttyd::seqdrv::SeqIndex Battle = ttyd::seqdrv::SeqIndex::kBattle;
    
    if (Seq == Battle)
    {
        // Reloading the room with the Seq set to Battle will cause the game to crash, so don't allow it
        return;
    }
    
    // Resolve any necessary fades, as they can sometimes cause issues if not resolved, such as black screens
    // Only resolve the fades if not currently in a screen transition
    if (checkForSpecificSeq(Game))
    {
        uint32_t MaxFadeEntries = 5;
        for (uint32_t i = 0; i < MaxFadeEntries; i++)
        {
            resolveFade(i);
        }
    }
    
    // A separate address for NextBero is needed, as the original value will be cleared during the reloading process
    // The game will crash if NextMap is used directly in seqSetSeq, so a separate address must be used instead
    // NewBero and NewMap need to be global variables
    
    char *tempNewBero = ReloadRoom.NewBero;
    constexpr uint32_t NewBeroSize = sizeof(ReloadRoom.NewBero) - 1;
    strncpy(tempNewBero, ttyd::seq_mapchange::NextBero, NewBeroSize);
    tempNewBero[NewBeroSize] = '\0';
    
    char *tempNewMap = ReloadRoom.NewMap;
    constexpr uint32_t NewMapSize = sizeof(ReloadRoom.NewMap) - 1;
    strncpy(tempNewMap, ttyd::seq_mapchange::NextMap, NewMapSize);
    tempNewMap[NewMapSize] = '\0';
    
    setSeqMapChange(tempNewMap, tempNewBero);
    
    // Set a bool to prevent battles from starting
    // This needs to be done after setSeqMapChange is called
    ReloadRoom.ManuallyReloadingRoom = true;
    
    // Reset the camera - mainly for the black bars at the top and bottom of the screen
    uint32_t CameraPointer = reinterpret_cast<uint32_t>(ttyd::camdrv::camGetPtr(ttyd::dispdrv::CameraId::k2d));
    *reinterpret_cast<uint16_t *>(CameraPointer) &= ~((1 << 8) | (1 << 9)); // Turn off the 8 and 9 bits
    
    if (!checkIfSystemLevelIsSet())
    {
        return;
    }
    
    // Only run the following if the System Level is not 0
    // Re-enable the 3D camera if necessary
    ttyd::camdrv::L_camDispOn(ttyd::dispdrv::CameraId::k3d);
    
    // Enable sound effects, set the default camera id for Mario, and give back control to the player
    ttyd::pmario_sound::psndClearFlag(0x80);
    ttyd::mario_cam::marioSetCamId(ttyd::dispdrv::CameraId::k3d);
    ttyd::mariost::marioStSystemLevel(0);
    ReloadRoom.SystemLevelShouldBeLowered = true;
}