#ifndef MSMPC_H
#define MSMPC_H

#if defined(TARGET_PC) && !defined(MSM_PC_WRAPPERS)
#define msmSysSetOutputMode msmSysSetOutputModeUnlocked
#define msmSysSetAux msmSysSetAuxUnlocked

#define msmSePlay msmSePlayUnlocked
#define msmSeStop msmSeStopUnlocked
#define msmSeStopAll msmSeStopAllUnlocked
#define msmSePauseAll msmSePauseAllUnlocked
#define msmSeSetParam msmSeSetParamUnlocked
#define msmSeSetListener msmSeSetListenerUnlocked
#define msmSeUpdataListener msmSeUpdataListenerUnlocked
#define msmSeDelListener msmSeDelListenerUnlocked

#define msmMusPlay msmMusPlayUnlocked
#define msmMusStop msmMusStopUnlocked
#define msmMusStopAll msmMusStopAllUnlocked
#define msmMusPause msmMusPauseUnlocked
#define msmMusPauseAll msmMusPauseAllUnlocked
#define msmMusSetParam msmMusSetParamUnlocked

#define msmStreamPlay msmStreamPlayUnlocked
#define msmStreamStop msmStreamStopUnlocked
#define msmStreamStopAll msmStreamStopAllUnlocked
#define msmStreamSetMasterVolume msmStreamSetMasterVolumeUnlocked
#endif

#endif
