#include <math.h>
#include <string.h>

#include "../include/synth.h"

// helpers ==================================================
static double midi_to_frequency(int midiNote);
static Voice *find_free_voice(Synth *synth);
static void update_envelope(Synth *synth, Voice *voice);
// helpers ==================================================

void synth_init(Synth *synth, double sampleRate) {
    memset(synth, 0, sizeof(Synth));
    synth->sampleRate = sampleRate;
    synth->atkTime = 0.002;
    synth->decayTime = 0.15;
    synth->sustainLvl = 0.7;
    synth->releaseTime = 0.3;
    synth->gain = 0.1;
}

void synth_note_on(Synth *synth, int midiNote, float velocity) {
    if (midiNote < 0 || midiNote > 127) return;
    if (velocity > 1) velocity = 1;
    else if (velocity < 0) velocity = 0;
    
    Voice *freeVoice = find_free_voice(synth);
    if (freeVoice == NULL) return;
    freeVoice->active = true;
    freeVoice->midiNote = midiNote;
    freeVoice->phase = 0;
    freeVoice->v = velocity;
    freeVoice->envelopeLvl = 0;
    freeVoice->releaseStep = 0;
    freeVoice->envStage = ENVELOP_ATK;
    freeVoice->dphi = (TAU * midi_to_frequency(midiNote)) / (synth->sampleRate); 
}

void synth_note_off(Synth *synth, int midiNote)
{
    if (midiNote < 0 || midiNote > 127) return;

    float releaseTime = synth->releaseTime;
    double sampleRate = synth->sampleRate;

    for (int i = 0; i < POLYPHONY_LIMIT; ++i) {
        Voice *currVoice = &(synth->voices[i]);

        if (!currVoice->active || currVoice->midiNote != midiNote || currVoice->envStage == ENVELOP_RELEASE) continue;

        if (releaseTime <= 0 || currVoice->envelopeLvl <= 0) {
            currVoice->active = false;
            currVoice->envelopeLvl = 0;
            currVoice->releaseStep = 0;
            currVoice->envStage = ENVELOP_OFF;
        }
        else {
            currVoice->envStage = ENVELOP_RELEASE;
            currVoice->releaseStep = currVoice->envelopeLvl / (releaseTime * sampleRate);
        }
    }
}

float synth_render_sample(Synth *synth) {
    float mixedSample = 0;
    for (int i = 0; i < POLYPHONY_LIMIT; ++i) {
        Voice *currVoice = &(synth->voices[i]);
        if (!synth->voices[i].active) continue;
        
        update_envelope(synth, currVoice);
        if (!currVoice->active) continue;

        mixedSample += sin(currVoice->phase) * currVoice->envelopeLvl * currVoice->v;
        currVoice->phase += currVoice->dphi;

        // prevent overflow if holding key too long
        if (currVoice->phase > TAU) currVoice->phase -= TAU;
    }
    mixedSample *= synth->gain;

    if (mixedSample > 1.0f) mixedSample = 1.0f;
    else if (mixedSample < -1.0f) mixedSample = -1.0f;
    
    return mixedSample;
}

// ==========================================================
static double midi_to_frequency(int midiNote) {
    return 440 * pow(2, (midiNote - 69.0) / 12.0);
}

static Voice *find_free_voice(Synth *synth) {
    for (int i = 0; i < POLYPHONY_LIMIT; ++i) {
        if (!synth->voices[i].active) {return &synth->voices[i];}
    }
    return NULL;
}

static void update_envelope(Synth *synth, Voice *voice)
{
    switch (voice->envStage) {
        case ENVELOP_ATK:
            if (synth->atkTime <= 0.0f) {
                voice->envelopeLvl = 1.0f;
                voice->envStage = ENVELOP_DECAY;
                break;
            }

            voice->envelopeLvl +=
                1.0f /
                (synth->atkTime * synth->sampleRate);

            if (voice->envelopeLvl >= 1.0f) {
                voice->envelopeLvl = 1.0f;
                voice->envStage = ENVELOP_DECAY;
            }
            break;

        case ENVELOP_DECAY:
            if (synth->decayTime <= 0.0f) {
                voice->envelopeLvl = synth->sustainLvl;
                voice->envStage = ENVELOP_SUSTAIN;
                break;
            }

            voice->envelopeLvl -=
                (1.0f - synth->sustainLvl) /
                (synth->decayTime * synth->sampleRate);

            if (voice->envelopeLvl <= synth->sustainLvl) {
                voice->envelopeLvl = synth->sustainLvl;
                voice->envStage = ENVELOP_SUSTAIN;
            }
            break;

        case ENVELOP_SUSTAIN:
            voice->envelopeLvl = synth->sustainLvl;
            break;

        case ENVELOP_RELEASE:
            voice->envelopeLvl -= voice->releaseStep;

            if (voice->envelopeLvl <= 0.0f) {
                voice->envelopeLvl = 0.0f;
                voice->releaseStep = 0.0f;
                voice->envStage = ENVELOP_OFF;
                voice->active = false;
            }
            break;

        case ENVELOP_OFF:
            voice->envelopeLvl = 0.0f;
            voice->active = false;
            break;
    }
}