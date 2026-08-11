#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "../include/audio.h"
#include "../include/controller.h"
#include "../include/event-queue.h"

#define SAMPLE_RATE 44100

int main(void)
{
    EventQueue mainQueue;
    Controller mainCtrller;
    AudioPack mainAudio;

    event_queue_init(&mainQueue);
    controller_init(&mainCtrller);

    if (!audio_init(&mainAudio, &mainQueue, 0)) {
        fprintf(stderr, "Failed to initialize audio device!\n");
        return EXIT_FAILURE;
    }

    if (!audio_start(&mainAudio)) {
        fprintf(stderr, "Failed to start audio device!\n");
        audio_destroy(&mainAudio);
        return EXIT_FAILURE;
    }

    printf("Virtual piano started.\n");
    printf(" [ / ]: shift octave\n");
    printf("ESC: quit\n");

    while (controller_poll(&mainCtrller, &mainQueue)) {
        Sleep(1);
    }

    audio_destroy(&mainAudio);
    printf("Piano stopped.\n");
    return EXIT_SUCCESS;
}