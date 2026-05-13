#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

extern bool PartyBoard_IsRunning;
extern bool PartyBoard_IsShuttingDown;
extern bool PartyBoard_IsGameLaunched;
extern bool PartyBoard_RestartRequested;

#if defined(__ANDROID__) || (defined(TARGET_OS_IOS) && TARGET_OS_IOS) || \
  (defined(TARGET_OS_TV) && TARGET_OS_TV)
#define SUPPORTS_PROCESS_RESTART FALSE
#else
#define SUPPORTS_PROCESS_RESTART TRUE
#endif

void PartyBoard_RequestRestart(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" std::filesystem::path PartyBoard_ConfigPath;
#endif