#ifndef _SRC_DOLASSETS_H_
#define _SRC_DOLASSETS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DOL_INCLUDE_ANK8X8_4B       0
#define DOL_INCLUDE_ASCII8X8_1BPP   1
#define DOL_INCLUDE_COVEROPEN_EN    2
#define DOL_INCLUDE_FATALERROR_EN   3
#define DOL_INCLUDE_HILITEDATA      4
#define DOL_INCLUDE_HILITEDATA2     5
#define DOL_INCLUDE_HILITEDATA3     6
#define DOL_INCLUDE_HILITEDATA4     7
#define DOL_INCLUDE_LOADING_EN      8
#define DOL_INCLUDE_NODISC_EN       9
#define DOL_INCLUDE_REFMAPDATA0     10
#define DOL_INCLUDE_REFMAPDATA1     11
#define DOL_INCLUDE_REFMAPDATA2     12
#define DOL_INCLUDE_REFMAPDATA3     13
#define DOL_INCLUDE_REFMAPDATA4     14
#define DOL_INCLUDE_RETRYERROR_EN   15
#define DOL_INCLUDE_TOONMAPDATA     16
#define DOL_INCLUDE_TOONMAPDATA2    17
#define DOL_INCLUDE_WRONGDISC_EN    18
#define DOL_INCLUDE_COUNT           19

#define ank8x8_4b       GetDolIncludeData(DOL_INCLUDE_ANK8X8_4B)
#define Ascii8x8_1bpp   GetDolIncludeData(DOL_INCLUDE_ASCII8X8_1BPP)
#define coveropen_en    GetDolIncludeData(DOL_INCLUDE_COVEROPEN_EN)
#define fatalerror_en   GetDolIncludeData(DOL_INCLUDE_FATALERROR_EN)
#define hiliteData      GetDolIncludeData(DOL_INCLUDE_HILITEDATA)
#define hiliteData2     GetDolIncludeData(DOL_INCLUDE_HILITEDATA2)
#define hiliteData3     GetDolIncludeData(DOL_INCLUDE_HILITEDATA3)
#define hiliteData4     GetDolIncludeData(DOL_INCLUDE_HILITEDATA4)
#define loading_en      GetDolIncludeData(DOL_INCLUDE_LOADING_EN)
#define nodisc_en       GetDolIncludeData(DOL_INCLUDE_NODISC_EN)
#define refMapData0     GetDolIncludeData(DOL_INCLUDE_REFMAPDATA0)
#define refMapData1     GetDolIncludeData(DOL_INCLUDE_REFMAPDATA1)
#define refMapData2     GetDolIncludeData(DOL_INCLUDE_REFMAPDATA2)
#define refMapData3     GetDolIncludeData(DOL_INCLUDE_REFMAPDATA3)
#define refMapData4     GetDolIncludeData(DOL_INCLUDE_REFMAPDATA4)
#define retryerror_en   GetDolIncludeData(DOL_INCLUDE_RETRYERROR_EN)
#define toonMapData     GetDolIncludeData(DOL_INCLUDE_TOONMAPDATA)
#define toonMapData2    GetDolIncludeData(DOL_INCLUDE_TOONMAPDATA2)
#define wrongdisc_en    GetDolIncludeData(DOL_INCLUDE_WRONGDISC_EN)

void InitializeDol();
void* GetDolIncludeData(int index);

#ifdef __cplusplus
}
#endif

#endif
