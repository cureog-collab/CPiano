#include "../include/event-queue.h"

void event_queue_init(EventQueue *queue) {
    atomic_init(&(queue->front), 0u);
    atomic_init(&(queue->rear), 0u);
}

bool event_queue_insert(EventQueue *queue, NoteEvent event) {
    unsigned int qFront = atomic_load(&(queue->front));
    unsigned int qRear = atomic_load(&(queue->rear));
    unsigned int next = qFront + 1u;
    if (next >= EVENT_QUEUE_CAPACITY) next %= EVENT_QUEUE_CAPACITY;

    if (next == qRear) return false;

    queue->events[qFront] = event;
    atomic_store(&(queue->front), next);
    return true;
}

bool event_queue_pop(EventQueue *queue, NoteEvent *event) {
    unsigned int qFront = atomic_load(&(queue->front));
    unsigned int qRear = atomic_load(&(queue->rear));

    if (qFront == qRear) return false;

    *event = queue->events[qRear];
    unsigned int next = qRear + 1u;
    if (next >= EVENT_QUEUE_CAPACITY) next %= EVENT_QUEUE_CAPACITY;

    atomic_store(&(queue->rear), next);
    return true;
}