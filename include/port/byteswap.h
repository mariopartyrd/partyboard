#ifndef PORT_BYTESWAP_H_
#define PORT_BYTESWAP_H_
#include <game/gamework_data.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "game/animdata.h"
#include "game/hsfformat.h"
#include "ext_math.h"

typedef struct AnimData32b {
    s16 bankNum;
    s16 patNum;
    s16 bmpNum;
    s16 useNum;

    u32 bank;
    u32 pat;
    u32 bmp;
} AnimData32b;

typedef struct AnimBankData32b {
    s16 timeNum;
    s16 unk;
    u32 frame;
} AnimBankData32b;

typedef struct AnimPatData32b {
    s16 layerNum;
    s16 centerX;
    s16 centerY;
    s16 sizeX;
    s16 sizeY;
    u32 layer;
} AnimPatData32b;

typedef struct AnimBmpData32b {
    u8 pixSize;
    u8 dataFmt;
    s16 palNum;
    s16 sizeX;
    s16 sizeY;
    u32 dataSize;
    u32 palData;
    u32 data;
} AnimBmpData32b;

typedef struct HsfCluster32b {
    u32 name[2];
    u32 targetName;
    u32 part;
    float index;
    float weight[32];
    u8 adjusted;
    u8 unk95;
    u16 type;
    u32 vertexCnt;
    u32 vertex;
} HsfCluster32b;

typedef struct HsfAttribute32b {
    u32 name;
    u32 unk04;
    u8 unk8[4];
    float kColor;
    u8 unk10[4];
    float nbtTpLvl;
    u8 unk18[8];
    float unk20;
    u8 unk24[4];
    float unk28;
    float unk2C;
    float unk30;
    float unk34;
    u8 unk38[44];
    u32 wrapS;
    u32 wrapT;
    u8 unk6C[12];
    u32 maxLod;
    u32 flag;
    u32 bitmap;
} HsfAttribute32b;

typedef struct HsfMaterial32b {
    u32 name;
    u8 unk4[4];
    u16 pass;
    u8 vtxMode;
    u8 litColor[3];
    u8 color[3];
    u8 shadowColor[3];
    float hiliteScale;
    float unk18;
    float invAlpha;
    float unk20[2];
    float refAlpha;
    float unk2C;
    u32 flags;
    u32 attrNum;
    u32 attr;
} HsfMaterial32b;

typedef struct HsfMapAttr32b {
    float minX;
    float minZ;
    float maxX;
    float maxZ;
    u32 data;
    u32 dataLen;
} HsfMapAttr32b;

typedef struct HsfBuffer32b {
    u32 name;
    s32 count;
    u32 data;
} HsfBuffer32b;

typedef struct HsfPalette32b {
    u32 name;
    s32 unk;
    u32 palSize;
    u32 data;
} HsfPalette32b;

typedef struct HsfBitmap32b {
    u32 name;
    u32 maxLod;
    u8 dataFmt;
    u8 pixSize;
    s16 sizeX;
    s16 sizeY;
    s16 palSize;
    GXColor tint;
    u32 palData;
    u32 unk;
    u32 data;
} HsfBitmap32b;

typedef struct HsfPart32b {
    u32 name;
    u32 num;
    u32 vertex;
} HsfPart32b;

typedef struct HsfSkeleton32b {
    u32 name;
    HSFTRANSFORM transform;
} HsfSkeleton32b;

typedef struct HsfShape32b {
    u32 name;
    union {
        u16 num16[2];
        u32 vertexCnt;
    };
    u32 vertex;
} HsfShape32b;

typedef struct HsfCenvDual32b {
    u32 target1;
    u32 target2;
    u32 weightNum;
    u32 weight;
} HsfCenvDual32b;

typedef struct HsfCenvMulti32b {
    u32 weightNum;
    u16 pos;
    u16 posNum;
    u16 normal;
    u16 normalNum;
    u32 weight;
} HsfCenvMulti32b;

typedef struct HsfCenv32b {
    u32 name;
    u32 singleData;
    u32 dualData;
    u32 multiData;
    u32 singleCount;
    u32 dualCount;
    u32 multiCount;
    u32 vtxCount;
    u32 copyCount;
} HsfCenv32b;

typedef struct HsfObjectData32b {
    u32 parent;
    u32 childrenCount;
    u32 children;
    HSFTRANSFORM base;
    HSFTRANSFORM curr;
    union {
        struct {
            HuVecF min;
            HuVecF max;
            float baseMorph;
            float morphWeight[32];
            u32 unkF0;
        } mesh;
        u32 replica;
    };

    u32 face;
    u32 vertex;
    u32 normal;
    u32 color;
    u32 st;
    u32 material;
    u32 attribute;
    u8 writeNum;
    u8 unk121;
    u8 shapeType;
    u8 matPass;
    u32 shapeNum;
    u32 shape;
    u32 clusterNum;
    u32 cluster;
    u32 cenvNum;
    u32 cenv;
    u32 vtxtop;
    u32 normtop;
} HsfObjectData32b;

typedef struct HsfObject32b {
    u32 name;
    u32 type;
    u32 constData;
    u32 flags;
    union {
        HsfObjectData32b data;
        HSFCAMERA camera;
        HSFLIGHT light;
    };
} HsfObject32b;

typedef struct HsfTrack32b {
    u8 type;
    u8 start;
    union {
        u16 target;
        s16 target_s16;
    };
    union {
        s32 clusterWeight;
        struct {
            union {
                s16 attrIdx;
                u16 param_u16;
            };
            union {
                u16 channel;
                s16 channel_s16;
            };
        };
    };
    u16 curveType;
    u16 numKeyframes;
    union {
        float value;
        u32 data;
    };
} HsfTrack32b;

typedef struct HsfMotion32b {
    u32 name;
    s32 numTracks;
    u32 track;
    float maxTime;
} HsfMotion32b;

typedef struct HsfBitmapKey32b {
    float time;
    u32 data;
} HsfBitmapKey32b;

typedef struct HsfFace32b {
    s16 type;
    s16 mat;
    union {
        struct {
            s16 indices[3][4];
            u32 count;
            u32 data;
        } strip;
        s16 indices[4][4];
    };
    Vec nbt;
} HsfFace32b;

typedef struct HsfMatrix32b {
    u32 base_idx;
    u32 count;
    u32 data;
} HsfMatrix32b;

void byteswap_clear_visited_ptrs();
void byteswap_u16(u16 *src);
void byteswap_s16(s16 *src);
void byteswap_u32(u32 *src);
void byteswap_s32(s32 *src);
void byteswap_float(float *src);
void byteswap_vec(Vec *src);
void byteswap_vec2f(Vec2f *src);
void byteswap_hsfvec3f(HuVecF *src);
void byteswap_hsfvec2f(HuVec2f *src);

void byteswap_animdata(void *src, AnimData *dest);
void byteswap_animbankdata(AnimBankData32b *src, AnimBankData *dest);
void byteswap_animpatdata(AnimPatData32b *src, AnimPatData *dest);
void byteswap_animbmpdata(AnimBmpData32b *src, AnimBmpData *dest);
void byteswap_animframedata(AnimFrameData *src);
void byteswap_animlayerdata(AnimLayerData *src);

void byteswap_hsfheader(HSFHEADER *src);
void byteswap_hsfcluster(HsfCluster32b *src, HsfCluster *dest);
void byteswap_hsfattribute(HsfAttribute32b *src, HSFATTRIBUTE *dest);
void byteswap_hsfmaterial(HsfMaterial32b *src, HSFMATERIAL *dest);
void byteswap_hsfscene(HSFSCENE *src);
void byteswap_hsfbuffer(HsfBuffer32b *src, HSFBUFFER *dest);
void byteswap_hsfmatrix(HsfMatrix32b *src, HSFMATRIX *dest);
void byteswap_hsfpalette(HsfPalette32b *src, HSFPALETTE *dest);
void byteswap_hsfpart(HsfPart32b *src, HsfPart *dest);
void byteswap_hsfbitmap(HsfBitmap32b *src, HSFBITMAP *dest);
void byteswap_hsfmapattr(HsfMapAttr32b *src, HSFMAPATTR *dest);
void byteswap_hsfskeleton(HsfSkeleton32b *src, HSFSKELETON *dest);
void byteswap_hsfshape(HsfShape32b *src, HsfShape *dest);
void byteswap_hsfcenv_single(HSFCENVSINGLE *src);
void byteswap_hsfcenv(HsfCenv32b *src, HSFCENV *dest);
void byteswap_hsfobject(HsfObject32b *src, HSFOBJECT *dest);
void byteswap_hsfbitmapkey(HsfBitmapKey32b *src, HSFBITMAPKEY *dest);
void byteswap_hsftrack(HsfTrack32b *src, HSFTRACK *dest);
void byteswap_hsfmotion(HsfMotion32b *src, HSFMOTION *dest);
void byteswap_hsfface(HsfFace32b *src, HSFFACE *dest);
void byteswap_hsfcluster(HsfCluster32b *src, HsfCluster *dest);
void byteswap_hsfattribute(HsfAttribute32b *src, HSFATTRIBUTE *dest);
void byteswap_hsfmaterial(HsfMaterial32b *src, HSFMATERIAL *dest);
void byteswap_hsfscene(HSFSCENE *src);
void byteswap_hsfbuffer(HsfBuffer32b *src, HSFBUFFER *dest);
void byteswap_hsfpalette(HsfPalette32b *src, HSFPALETTE *dest);
void byteswap_hsfpart(HsfPart32b *src, HsfPart *dest);
void byteswap_hsfbitmap(HsfBitmap32b *src, HSFBITMAP *dest);
void byteswap_hsfmapattr(HsfMapAttr32b *src, HSFMAPATTR *dest);
void byteswap_hsfskeleton(HsfSkeleton32b *src, HSFSKELETON *dest);
void byteswap_hsfshape(HsfShape32b *src, HsfShape *dest);
void byteswap_hsfcenv_single(HSFCENVSINGLE *src);
void byteswap_hsfcenv_dual_weight(HSFCENVDUALWEIGHT *src);
void byteswap_hsfcenv_dual(HsfCenvDual32b *src, HSFCENVDUAL *dest);
void byteswap_hsfcenv_multi_weight(HSFCENVMULTIWEIGHT *src);
void byteswap_hsfcenv_multi(HsfCenvMulti32b *src, HsfCenvMulti *dest);
void byteswap_hsfcenv(HsfCenv32b *src, HSFCENV *dest);
void byteswap_hsfobject(HsfObject32b *src, HSFOBJECT *dest);
void byteswap_hsfbitmapkey(HsfBitmapKey32b *src, HSFBITMAPKEY *dest);
void byteswap_hsftrack(HsfTrack32b *src, HSFTRACK *dest);
void byteswap_hsfmotion(HsfMotion32b *src, HSFMOTION *dest);
void byteswap_hsfface(HsfFace32b *src, HSFFACE *dest);

void byteswap_gamestat(GameStat *src);
void byteswap_systemstate(SystemState *src);
void byteswap_playerstate(PlayerState *src);

#ifdef __cplusplus
}
#endif

#endif
