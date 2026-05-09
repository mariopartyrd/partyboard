#include <cstdint>
#include <cstring>

extern "C" {
#include "game/dvd.h"
}
#include "port/dolassets.h"

struct IncludeEntry {
    uint32_t address;
    uint32_t size;
};

static constexpr uint32_t DOL_SHIFT = 0x80003000u;
static constexpr const char *DOL_PATH = "sys/main.dol";

static constexpr IncludeEntry s_includes[DOL_INCLUDE_COUNT] = {
    /* INCLUDE_ANK8X8_4B     */ { 0x8011FE00, 0x2000 },
    /* INCLUDE_ASCII8X8_1BPP */ { 0x8012DCD7, 0x0800 },
    /* INCLUDE_COVEROPEN_EN  */ { 0x80132208, 0x1384 },
    /* INCLUDE_FATALERROR_EN */ { 0x8013358C, 0x1384 },
    /* INCLUDE_HILITEDATA    */ { 0x8012C360, 0x0480 },
    /* INCLUDE_HILITEDATA2   */ { 0x8012C7E0, 0x0480 },
    /* INCLUDE_HILITEDATA3   */ { 0x8012CC60, 0x0480 },
    /* INCLUDE_HILITEDATA4   */ { 0x8012D0E0, 0x0480 },
    /* INCLUDE_LOADING_EN    */ { 0x80134910, 0x1384 },
    /* INCLUDE_NODISC_EN     */ { 0x80135C94, 0x1384 },
    /* INCLUDE_REFMAPDATA0   */ { 0x801225A0, 0x1240 },
    /* INCLUDE_REFMAPDATA1   */ { 0x801237E0, 0x1100 },
    /* INCLUDE_REFMAPDATA2   */ { 0x801248E0, 0x2080 },
    /* INCLUDE_REFMAPDATA3   */ { 0x80126960, 0x2080 },
    /* INCLUDE_REFMAPDATA4   */ { 0x801289E0, 0x2080 },
    /* INCLUDE_RETRYERROR_EN */ { 0x80137018, 0x1384 },
    /* INCLUDE_TOONMAPDATA   */ { 0x8012AA60, 0x0880 },
    /* INCLUDE_TOONMAPDATA2  */ { 0x8012B2E0, 0x1080 },
    /* INCLUDE_WRONGDISC_EN  */ { 0x8013839C, 0x1384 },
};

static u8 *dolPtr;

extern "C" {

void InitializeDol()
{
    s32 dolSize;
    const u8 *dol = DVDGetDOLLocation(&dolSize);
    if (!dol)
        return;
    dolPtr = new u8[dolSize];
    memcpy(dolPtr, dol, dolSize);
}

void *GetDolIncludeData(int index)
{
    if (index < 0 || index >= DOL_INCLUDE_COUNT)
        return nullptr;

    const auto &entry = s_includes[index];
    return &dolPtr[entry.address - DOL_SHIFT];
}

};