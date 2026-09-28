#include "port/audio.h"

#include <SDL3/SDL_error.h>
#include <aurora/lib/logging.hpp>
#include <dolphin/ai.h>
#include <musyx/musyx.h>
#include <musyx/pc.h>
#include <port/settings.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

namespace {

aurora::Module Log("partyboard::audio");

std::recursive_mutex sAudioMutex;

AIDCallback sDmaCallback = nullptr;
s32 sMasterVolume = 100;

bool sDeviceOpened = false;
bool sDeviceStarted = false;

std::thread sSilentThread;
std::atomic<bool> sSilentRunning{false};

struct StreamRead {
    DVDFileInfo *fileInfo;
    void *addr;
    s32 length;
    s32 offset;
    DVDCallback callback;
};

std::mutex sReadMutex;
std::condition_variable sReadCond;
std::deque<StreamRead> sReadQueue;
std::thread sReadThread;
bool sReadStop = false;

void output_callback(s16 *output, size_t frames, u32 rate, u32 channels) {
    (void)rate;

    // 5 ms MusyX cycle
    if (sDmaCallback != nullptr) {
        sDmaCallback();
    }

    if (sMasterVolume < 100) {
        const s32 gain = sMasterVolume * 32768 / 100;
        for (size_t i = 0; i < frames * channels; ++i) {
            output[i] = static_cast<s16>((output[i] * gain) >> 15);
        }
    }
}

void silent_render_loop() {
    using namespace std::chrono;

    const SND_PC_RENDER_INFO info = sndPCGetRenderInfo();
    const u64 cycleFrames = (info.mixRate + 199) / 200;
    std::vector<s16> buffer(cycleFrames * info.channels);
    const auto start = steady_clock::now();
    u64 rendered = 0;

    while (sSilentRunning.load(std::memory_order_relaxed)) {
        const u64 elapsedUs = duration_cast<microseconds>(steady_clock::now() - start).count();
        const u64 due = elapsedUs * info.mixRate / 1000000;
        // don't try to catch up on more than a second
        if (due > rendered + info.mixRate) {
            rendered = due - info.mixRate;
        }
        while (rendered < due) {
            const u64 frames = std::min(cycleFrames, due - rendered);
            sndPCRender(buffer.data(), frames);
            rendered += frames;
        }
        std::this_thread::sleep_for(milliseconds(5));
    }
}

void stream_read_loop() {
    std::unique_lock lock(sReadMutex);
    while (true) {
        sReadCond.wait(lock, [] { return sReadStop || !sReadQueue.empty(); });
        if (sReadStop) {
            break;
        }
        const StreamRead request = sReadQueue.front();
        sReadQueue.pop_front();
        lock.unlock();

        const s32 result = DVDReadPrio(request.fileInfo, request.addr, request.length, request.offset, 2);
        {
            std::lock_guard audioLock(sAudioMutex);
            if (request.callback != nullptr) {
                request.callback(result, request.fileInfo);
            }
        }

        lock.lock();
    }
}

}

extern "C" {

void partyboard_audio_lock(void) { sAudioMutex.lock(); }

void partyboard_audio_unlock(void) { sAudioMutex.unlock(); }

void partyboard_audio_pre_init(void) {
    sMasterVolume = std::clamp(partyboard::getSettings().audio.masterVolume.getValue(), 0, 100);
    sndPCSetSynchronization(partyboard_audio_lock, partyboard_audio_unlock);

    SND_PC_CONFIG obtained{};
    sDeviceOpened = sndPCOpenAudio(nullptr, &obtained);
    if (sDeviceOpened) {
        Log.info("Audio output: {} Hz, {} channels", obtained.mixRate, obtained.channels);
    } else {
        Log.warn("Unable to open an audio device, sound will be muted: {}", SDL_GetError());
    }
}

void partyboard_audio_start(void) {
    sndPCSetOutputCallback(output_callback);

    {
        std::lock_guard lock(sReadMutex);
        sReadStop = false;
    }
    sReadThread = std::thread(stream_read_loop);

    if (sDeviceOpened) {
        sDeviceStarted = sndPCStartAudio();
        if (sDeviceStarted) {
            return;
        }
        Log.warn("Unable to start audio playback, sound will be muted: {}", SDL_GetError());
    }
    sSilentRunning.store(true);
    sSilentThread = std::thread(silent_render_loop);
}

void partyboard_audio_shutdown(void) {
    // stop rendering first
    if (sDeviceOpened) {
        sndPCStopAudio();
        sDeviceOpened = sDeviceStarted = false;
    }
    if (sSilentThread.joinable()) {
        sSilentRunning.store(false);
        sSilentThread.join();
    }
    if (sReadThread.joinable()) {
        {
            std::lock_guard lock(sReadMutex);
            sReadStop = true;
            sReadQueue.clear();
        }
        sReadCond.notify_one();
        sReadThread.join();
    }
}

BOOL partyboard_audio_read_async(DVDFileInfo *fileInfo, void *addr, s32 length, s32 offset, DVDCallback callback) {
    // DVDGetCommandBlockStatus
    fileInfo->cb.state = DVD_STATE_BUSY;
    {
        std::lock_guard lock(sReadMutex);
        sReadQueue.push_back({fileInfo, addr, length, offset, callback});
    }
    sReadCond.notify_one();
    return TRUE;
}

void AIInit(u8 *stack) { (void)stack; }

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
    std::lock_guard lock(sAudioMutex);
    const AIDCallback old = sDmaCallback;
    sDmaCallback = callback;
    return old;
}

void AISetStreamPlayState(u32 state) { (void)state; }
void AISetStreamVolLeft(u8 vol) { (void)vol; }
void AISetStreamVolRight(u8 vol) { (void)vol; }

}
