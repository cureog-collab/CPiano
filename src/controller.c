#include <stdio.h>
#include <string.h>

#include "../include/controller.h"
#include "../include/input-process.h"

static const int PIANO_KEYS[KEYS_COUNT] = {
    // lower octave
    'Z', 'S', 'X', 'D', 'C', 'V',
    'G', 'B', 'H', 'N', 'J', 'M',

    // higher octave
    'Q', '2', 'W', '3', 'E', 'R',
    '5', 'T', '6', 'Y', '7', 'U'
};

void controller_init(Controller *ctrller) {
    memset(ctrller, 0, sizeof(Controller));
    for (int i = 0; i < KEYS_COUNT; ++i) ctrller->activeMidi[i] = -1;
}

bool controller_poll(Controller *ctrller, EventQueue *queue) {
    if (is_key_down(ESC_KEY)) return false;

    bool octaveDown = is_key_down(OCTAVE_DOWN);
    bool octaveUp = is_key_down(OCTAVE_UP);

    if (octaveDown && !ctrller->octaveDownWasDown && ctrller->octaveShift > MIN_OCTAVE_SHIFT) {
        ctrller->octaveShift--;
        printf("Current octave shift: %i\n", ctrller->octaveShift);
    }
    if (octaveUp && !ctrller->octaveUpWasDown && ctrller->octaveShift < MAX_OCTAVE_SHIFT) {
        ctrller->octaveShift++;
        printf("Current octave shift: %i\n", ctrller->octaveShift);
    }

    ctrller->octaveDownWasDown = octaveDown;
    ctrller->octaveUpWasDown = octaveUp;

    for (int i = 0; i < KEYS_COUNT; ++i) {
        bool isDown = is_key_down(PIANO_KEYS[i]);

        if (isDown && !ctrller->keyWasDown[i]) {
            int midiNote = FIRST_MIDI_NOTE + i + ctrller->octaveShift * 12;
            NoteEvent event = {
                .action = NOTE_ON,
                .midiNote = midiNote,
                .velocity = 1.0
            };

            if (event_queue_insert(queue, event)) {
                ctrller->keyWasDown[i] = true;
                ctrller->activeMidi[i] = midiNote;
            }
        }
        else if (!isDown && ctrller->keyWasDown[i]) {
            NoteEvent event = {
                .action = NOTE_OFF,
                .midiNote = ctrller->activeMidi[i],
                .velocity = 0.0f
            };

            if (event_queue_insert(queue, event)) {
                ctrller->keyWasDown[i] = false;
                ctrller->activeMidi[i] = -1;
            }
        }
    }
    return true;
}

