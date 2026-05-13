// Credits: TwilitRealm

#ifndef PORT_UI_H_
#define PORT_UI_H_

#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <SDL3/SDL_events.h>

#ifdef __cplusplus
extern "C" {
#endif

bool ui_initialize(void);
bool ui_is_prelaunch_open(void);
void ui_shutdown(void);
void ui_handle_sdl_event(const SDL_Event *event);
void ui_update(void);
void ui_push_toast(const char *type, const char *title, const char *content, uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif
