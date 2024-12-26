#include "../include/engine/input.h"

#include <windows.h>

engine_InputState* engine_InputState_global = null;

void engine_Input_init(void) {
    if (engine_InputState_global) { return; }

    engine_InputState_global = (engine_InputState*)malloc(sizeof(engine_InputState));
    if (!engine_InputState_global) { return; }

    memset(engine_InputState_global, 0, sizeof(engine_InputState));
}

void engine_Input_update(void) {
    if (!engine_InputState_global) { return; }

    // Save previous state
    memcpy(engine_InputState_global->prev_keys, engine_InputState_global->keys, sizeof(engine_InputState_global->keys));

    // Update current state
    for (i32 i = 0; i < engine_KeyCode_Count; ++i) {
        engine_InputState_global->keys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
}

void engine_Input_shutdown(void) {
    if (engine_InputState_global) {
        free(engine_InputState_global);
        engine_InputState_global = null;
    }
}
