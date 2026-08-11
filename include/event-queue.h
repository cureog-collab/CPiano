#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdbool.h>
#include <stdatomic.h>

#define EVENT_QUEUE_CAPACITY 128

typedef enum {
    NOTE_ON,
    NOTE_OFF
} NoteAction;

typedef struct {
    NoteAction action;
    int midiNote;
    float velocity;
} NoteEvent;

typedef struct {
    NoteEvent events[EVENT_QUEUE_CAPACITY];
    atomic_uint front;
    atomic_uint rear;
} EventQueue;

void event_queue_init(EventQueue *queue);
bool event_queue_insert( EventQueue *queue, NoteEvent event);
bool event_queue_pop(EventQueue *queue, NoteEvent *event);

#endif