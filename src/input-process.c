#include "../include/input-process.h"

int is_key_down(int key) {
    return ((GetAsyncKeyState(key) & 0x8000) != 0);
}
