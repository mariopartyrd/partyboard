// see include/msm/msmpc.h
#define MSM_PC_WRAPPERS
#include "game/msm.h"
#include "msm/msmsys.h"

#define MSM_LOCKED(type, name, params, args) \
    type name##Unlocked params;              \
    type name params                         \
    {                                        \
        type result;                         \
        msmSysIrqDisable();                  \
        result = name##Unlocked args;        \
        msmSysIrqEnable();                   \
        return result;                       \
    }

#define MSM_LOCKED_VOID(name, params, args) \
    void name##Unlocked params;             \
    void name params                        \
    {                                       \
        msmSysIrqDisable();                 \
        name##Unlocked args;                \
        msmSysIrqEnable();                  \
    }

MSM_LOCKED(BOOL, msmSysSetOutputMode, (SND_OUTPUTMODE mode), (mode))
MSM_LOCKED(s32, msmSysSetAux, (s32 auxA, s32 auxB), (auxA, auxB))

MSM_LOCKED(int, msmSePlay, (int seId, MSM_SEPARAM *param), (seId, param))
MSM_LOCKED(s32, msmSeStop, (int seNo, s32 speed), (seNo, speed))
MSM_LOCKED_VOID(msmSeStopAll, (BOOL checkGrp, s32 speed), (checkGrp, speed))
MSM_LOCKED_VOID(msmSePauseAll, (BOOL pause, s32 speed), (pause, speed))
MSM_LOCKED(s32, msmSeSetParam, (int seNo, MSM_SEPARAM *param), (seNo, param))
MSM_LOCKED(s32, msmSeSetListener, (Vec *pos, Vec *heading, float sndDist, float sndSpeed, MSM_SELISTENER *listener),
    (pos, heading, sndDist, sndSpeed, listener))
MSM_LOCKED(s32, msmSeUpdataListener, (Vec *pos, Vec *heading), (pos, heading))
MSM_LOCKED_VOID(msmSeDelListener, (void), ())

MSM_LOCKED(int, msmMusPlay, (int musId, MSM_MUSPARAM *musParam), (musId, musParam))
MSM_LOCKED(s32, msmMusStop, (int musNo, s32 speed), (musNo, speed))
MSM_LOCKED_VOID(msmMusStopAll, (BOOL checkGrp, s32 speed), (checkGrp, speed))
MSM_LOCKED(s32, msmMusPause, (int musNo, BOOL pause, s32 speed), (musNo, pause, speed))
MSM_LOCKED_VOID(msmMusPauseAll, (BOOL pause, s32 speed), (pause, speed))
MSM_LOCKED(s32, msmMusSetParam, (s32 musNo, MSM_MUSPARAM *param), (musNo, param))

MSM_LOCKED(int, msmStreamPlay, (int streamId, MSM_STREAMPARAM *streamParam), (streamId, streamParam))
MSM_LOCKED(s32, msmStreamStop, (int streamNo, s32 speed), (streamNo, speed))
MSM_LOCKED_VOID(msmStreamStopAll, (s32 speed), (speed))
MSM_LOCKED_VOID(msmStreamSetMasterVolume, (s32 vol), (vol))
