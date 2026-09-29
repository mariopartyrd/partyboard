#ifndef _GAME_THPSIMPLE_H
#define _GAME_THPSIMPLE_H

#include "dolphin.h"

// TODO move these to aurora
#ifdef TARGET_PC

typedef struct THPAudioRecordHeader {
    u32 offsetNextChannel;
    u32 sampleSize;
    s16 lCoef[8][2];
    s16 rCoef[8][2];
    s16 lYn1;
    s16 lYn2;
    s16 rYn1;
    s16 rYn2;
} THPAudioRecordHeader;

// Size 0x30
typedef struct THPHeader {
    char mMagic[4];             // _00
    u32 mVersion;               // _04
    u32 mBufferSize;            // _08
    u32 mAudioMaxSamples;       // _0C
    f32 mFrameRate;             // _10
    u32 mNumFrames;             // _14
    u32 mFirstFrameSize;        // _18
    u32 mMovieDataSize;         // _1C
    u32 mCompInfoDataOffsets;   // _20
    u32 mOffsetDataOffsets;     // _24
    u32 mMovieDataOffsets;      // _28
    u32 mFinalFrameDataOffsets; // _2C
} THPHeader;

// Size 0x14
typedef struct THPFrameCompInfo {
    u32 mNumComponents; // _00
    u8 mFrameComp[16];  // _04
} THPFrameCompInfo;

#else
#include "dolphin/thp/THPFile.h"
#include "dolphin/thp/THPInfo.h"
#endif

// THPReadBuffer?
typedef struct {
    /* 0x00 */ s32 *unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
} UnkThpStruct01; // Size 0xC

typedef struct {
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
    /* 0x08 */ void *unk08;
    /* 0x0C */ s32 unk0C;
} UnkThpStruct02; // Size 0x10

typedef struct {
    /* 0x00 */ void *unk00;
    /* 0x04 */ void *unk04;
    /* 0x08 */ u32 unk08;
} UnkThpStruct03; // Size 0xC

typedef struct {
    /* 0x00 */ u32 unk00;
    /* 0x04 */ u32 unk04;
} UnkThpStruct04; // Size 8

typedef struct {
    /* 0x000 */ DVDFileInfo unk00;
    /* 0x03C */ THPHeader unk3C;
    /* 0x06C */ THPFrameCompInfo unk6C;
    /* 0x080 */ UnkThpStruct04 unk80;
    /* 0x088 */ u8 unk88[0xC];
    /* 0x094 */ void *unk94;
    /* 0x098 */ s32 unk98;
    /* 0x09C */ u8 unk9C;
    /* 0x09D */ u8 unk9D;
    /* 0x09E */ u8 unk9E;
    /* 0x09F */ u8 unk9F;
    /* 0x0A0 */ s32 unkA0;
    /* 0x0A4 */ s32 unkA4;
    /* 0x0A8 */ u32 unkA8;
    /* 0x0AC */ s32 unkAC;
    /* 0x0B0 */ s32 unkB0;
    /* 0x0B4 */ s32 unkB4;
    /* 0x0B8 */ s32 unkB8;
    /* 0x0BC */ float unkBC;
    /* 0x0C0 */ float unkC0;
    /* 0x0C4 */ float unkC4;
    /* 0x0C8 */ s32 unkC8;
    /* 0x0CC */ UnkThpStruct01 unkCC[10];
    /* 0x144 */ UnkThpStruct02 unk144[2];
    /* 0x164 */ UnkThpStruct03 unk164[4];
    /* 0x194 */ s32 unk194;
    /* 0x198 */ s32 unk198;
    /* 0x19C */ u8 unk19C;
    /* 0x19D */ char unk19D[1];
    /* 0x19E */ s16 unk19E;
} UnkThpStruct10; // Size 0x1A0

s32 THPSimpleInit(s32 arg0);
void THPSimpleQuit(void);
s32 THPSimpleOpen(char *arg0);
s32 THPSimpleClose(void);
s32 THPSimpleCalcNeedMemory(void);
s32 THPSimpleSetBuffer(void *arg0);
s32 THPSimplePreLoad(s32 arg0);
void THPSimpleAudioStart(void);
void THPSimpleAudioStop(void);
s32 THPSimpleLoadStop(void);
s32 THPSimpleDecode(void);
s32 THPSimpleDrawCurrentFrame(GXRenderModeObj *arg0, GXColor *arg1, Mtx arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 THPSimpleGetVideoInfo(UnkThpStruct04 *arg0);
s32 THPSimpleGetTotalFrame(void);
s32 THPSimpleSetVolume(s32 left, s32 right);

extern UnkThpStruct10 SimpleControl;

#endif
