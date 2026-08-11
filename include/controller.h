#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include "event-queue.h"

#define KEYS_COUNT 24
#define ESC_KEY 0x1B
#define OCTAVE_DOWN 0xDB
#define OCTAVE_UP 0xDD
#define MIN_OCTAVE_SHIFT (-5)
#define MAX_OCTAVE_SHIFT 5
#define FIRST_MIDI_NOTE 48

typedef struct {
    int activeMidi[KEYS_COUNT];
    int octaveShift;
    bool keyWasDown[KEYS_COUNT];
    bool octaveDownWasDown;
    bool octaveUpWasDown;
} Controller;

void controller_init(Controller *ctrller);
bool controller_poll(Controller *ctrller, EventQueue *queue);

#endif // CONTROLLER_H