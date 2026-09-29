#ifndef PORT_AUDIO_H_
#define PORT_AUDIO_H_

#include <dolphin/dvd.h>
#include <dolphin/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void partyboard_audio_lock(void);
void partyboard_audio_unlock(void);
void partyboard_audio_pre_init(void);
void partyboard_audio_start(void);
void partyboard_audio_shutdown(void);

typedef struct PartyboardAudioOutput {
    s16 *samples;
    size_t frames;
    u32 rate;
    u32 channels;
} PartyboardAudioOutput;

// The buffer is available only while the registered AI callback is running.
BOOL partyboard_audio_output_get(PartyboardAudioOutput *output);
BOOL partyboard_audio_read_async(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, DVDCallback callback);

#ifdef __cplusplus
}
#endif

#endif
