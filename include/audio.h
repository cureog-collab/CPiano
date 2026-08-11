#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

#include "../vendor/miniaudio.h"
#include "synth.h"
#include "event-queue.h"

typedef struct {
    EventQueue *eventQueue;
    ma_device device;
    Synth synth;
    bool initialized;
} AudioPack;

bool audio_init(AudioPack *audio, EventQueue *queue, unsigned int inputSampleRate);
bool audio_start(AudioPack *audio);
void audio_destroy(AudioPack *audio);

#endif // AUDIO_H