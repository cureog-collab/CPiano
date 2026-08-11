#ifndef SYNTH_H
#define SYNTH_H

#include <stdbool.h>
#include <stdint.h>

#define POLYPHONY_LIMIT 32
#define TAU 6.28318530717958647692

typedef enum {
    ENVELOP_OFF,
    ENVELOP_ATK,
    ENVELOP_DECAY,
    ENVELOP_SUSTAIN,
    ENVELOP_RELEASE
} EnvelopeStage;

typedef struct {
    EnvelopeStage envStage;
    double phase;
    double dphi;
    float v;
    float envelopeLvl;
    float releaseStep;
    int midiNote;
    bool active;
} Voice;

typedef struct {
    Voice voices[POLYPHONY_LIMIT];
    double sampleRate;
    float atkTime;
    float decayTime;
    float sustainLvl;
    float releaseTime;
    float gain;
} Synth;

void synth_init(Synth *synth, double sampleRate);
void synth_note_on(Synth *synth, int midiNote, float velocity);
void synth_note_off(Synth *synth, int midiNote);
float synth_render_sample(Synth *synth);

#endif // SYNTH_H