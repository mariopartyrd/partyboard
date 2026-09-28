#ifndef PORT_AUDIO_H_
#define PORT_AUDIO_H_

#include <dolphin/dvd.h>
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void partyboard_audio_lock(void);
void partyboard_audio_unlock(void);
void partyboard_audio_pre_init(void);
void partyboard_audio_start(void);
void partyboard_audio_shutdown(void);
BOOL partyboard_audio_read_async(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, DVDCallback callback);

#ifdef __cplusplus
}
#endif

#endif
