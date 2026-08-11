#include <string.h>

#include "../include/audio.h"

static void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frameCount);

bool audio_init(AudioPack *audio, EventQueue *queue, unsigned int inputSampleRate) {
    memset(audio, 0, sizeof(AudioPack));
    audio->eventQueue = queue;

    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate = inputSampleRate;
    config.dataCallback = data_callback;
    config.pUserData = audio;
    config.performanceProfile = ma_performance_profile_low_latency;
    config.periodSizeInFrames = 128;
    config.periods = 3;

    if (ma_device_init(NULL, &config, &audio->device) != MA_SUCCESS) return false;

    synth_init(&(audio->synth), (double)audio->device.sampleRate);
    audio->initialized = true;
    return true;
}

bool audio_start(AudioPack *audio) {
    if (!audio->initialized) return false;
    return ma_device_start(&audio->device) == MA_SUCCESS;
}

void audio_destroy(AudioPack *audio) {
    if (!audio->initialized) return;
    ma_device_uninit(&(audio->device));
    audio->initialized = false;
}

static void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frameCount) {
    AudioPack *audio = (AudioPack *)device->pUserData;
    NoteEvent event;
    while (event_queue_pop(audio->eventQueue, &event)) {
        switch (event.action) {
            case NOTE_ON:
                synth_note_on(&(audio->synth), event.midiNote, event.velocity);
                break;

            case NOTE_OFF:
                synth_note_off(&(audio->synth), event.midiNote);
                break;
        }
    }

    float *samples = (float *)output;
    for (ma_uint32 frame = 0; frame < frameCount; ++frame) {
        float sample = synth_render_sample(&(audio->synth));

        samples[frame * 2] = sample;
        samples[frame * 2 + 1] = sample;
    }
}
