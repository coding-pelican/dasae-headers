#include <windows.h>

#include "../include/engine/input.h"

engine_InputState* engine_InputState_global = null;

static void engine_Input_updateFrameInfo(void) {
    let state                    = engine_InputState_global;
    state->frame_info.prev_time  = state->frame_info.curr_time;
    state->frame_info.curr_time  = (f64)GetTickCount64() / 1000.0;
    state->frame_info.delta_time = state->frame_info.curr_time - state->frame_info.prev_time;
}

static void engine_Input_processKey(engine_KeyCode key, bool is_down) {
    let curr_state = &engine_InputState_global->curr_states[key];
    let prev_state = engine_InputState_global->prev_states[key];

    // Clear previous state flags
    *curr_state = 0;

    if (is_down) {
        if (!(prev_state & engine_KeyStates_held)) {
            // Key was just pressed
            *curr_state |= engine_KeyStates_pressed;

            // Create pressed event
            engine_InputEvent event = {
                .key       = key,
                .state     = engine_KeyStates_pressed,
                .timestamp = (f64)GetTickCount64() / 1000.0 // Convert to seconds
            };
            engine_InputEventBuffer_push(event);
        }
        // Key is being held
        *curr_state |= engine_KeyStates_held;

        // Create held event
        engine_InputEvent event = {
            .key       = key,
            .state     = engine_KeyStates_held,
            .timestamp = (f64)GetTickCount64() / 1000.0 // Convert to seconds
        };
        engine_InputEventBuffer_push(event);
    } else if (prev_state & (engine_KeyStates_pressed | engine_KeyStates_held)) {
        // Key was just released
        *curr_state |= engine_KeyStates_released;

        // Create released event
        engine_InputEvent event = {
            .key       = key,
            .state     = engine_KeyStates_released,
            .timestamp = (f64)GetTickCount64() / 1000.0 // Convert to seconds
        };
        engine_InputEventBuffer_push(event);
    }
}

void engine_Input_init(void) {
    if (engine_InputState_global) { return; }

    engine_InputState_global = (engine_InputState*)malloc(sizeof(engine_InputState));
    if (!engine_InputState_global) { return; }

    memset(engine_InputState_global, 0, sizeof(engine_InputState));
}

void engine_Input_update(void) {
    if (!engine_InputState_global) { return; }

    // Update frame timing
    engine_Input_updateFrameInfo();

    // Save previous state
    memcpy(engine_InputState_global->prev_states, engine_InputState_global->curr_states, sizeof(engine_InputState_global->curr_states));

    // Process each key
    for (i32 i = 0; i < engine_KeyCode_count; ++i) {
        SHORT key_state = GetAsyncKeyState(i);
        bool  is_down   = (key_state & 0x8000) != 0;
        engine_Input_processKey(i, is_down);
    }
}

void engine_Input_fini(void) {
    if (!engine_InputState_global) { return; }

    free(engine_InputState_global);
    engine_InputState_global = null;
}

// Event buffer implementation
void engine_InputEventBuffer_push(engine_InputEvent event) {
    let buffer = &engine_InputState_global->event_buffer;
    if (engine_InputEventBuffer_size <= buffer->count) {
        // Buffer is full, remove oldest event
        buffer->tail = (buffer->tail + 1) % engine_InputEventBuffer_size;
        buffer->count--;
    }

    buffer->events[buffer->head] = event;
    buffer->head                 = (buffer->head + 1) % engine_InputEventBuffer_size;
    buffer->count++;
}

Opt$engine_InputEvent engine_InputEventBuffer_pop(void) {
    reserveReturn(Opt$engine_InputEvent);

    let buffer = &engine_InputState_global->event_buffer;
    if (buffer->count == 0) {
        return_none();
    }

    let event    = buffer->events[buffer->tail];
    buffer->tail = (buffer->tail + 1) % engine_InputEventBuffer_size;
    buffer->count--;

    return_some(event);
}

Opt$engine_InputEvent engine_InputEventBuffer_peek(void) {
    reserveReturn(Opt$engine_InputEvent);

    let buffer = &engine_InputState_global->event_buffer;
    if (buffer->count == 0) {
        return_none();
    }

    return_some(buffer->events[buffer->tail]);
}

void engine_InputEventBuffer_clear(void) {
    let buffer    = &engine_InputState_global->event_buffer;
    buffer->head  = 0;
    buffer->tail  = 0;
    buffer->count = 0;
}

// bool engine_Input_anyKeyState(engine_KeyStates state) {
//     let buffer = &engine_InputState_global->event_buffer;

//     // Check most recent events first (from head backwards)
//     i32 idx = (buffer->head - 1 + engine_InputEventBuffer_size) % engine_InputEventBuffer_size;
//     for (i32 i = 0; i < buffer->count; ++i) {
//         if (buffer->events[idx].state & state) {
//             return true;
//         }
//         idx = (idx - 1 + engine_InputEventBuffer_size) % engine_InputEventBuffer_size;
//     }
//     return false;
// }

// i32 engine_Input_countEvents(engine_KeyStates state) {
//     let buffer = &engine_InputState_global->event_buffer;
//     i32 count  = 0;

//     i32 idx = buffer->tail;
//     for (i32 i = 0; i < buffer->count; ++i) {
//         if (buffer->events[idx].state & state) {
//             count++;
//         }
//         idx = (idx + 1) % engine_InputEventBuffer_size;
//     }
//     return count;
// }

// i32 engine_Input_countEventsBetweenFrames(engine_KeyStates state) {
//     let buffer      = &engine_InputState_global->event_buffer;
//     let frame_start = engine_InputState_global->frame_info.prev_time;
//     let frame_end   = engine_InputState_global->frame_info.curr_time;
//     i32 count       = 0;

//     i32 idx = buffer->tail;
//     for (i32 i = 0; i < buffer->count; ++i) {
//         let event_time = buffer->events[idx].timestamp;
//         if ((buffer->events[idx].state & state) && event_time >= frame_start && event_time <= frame_end) {
//             count++;
//         }
//         idx = (idx + 1) % engine_InputEventBuffer_size;
//     }
//     return count;
// }

// bool engine_Input_isEventRecent(engine_KeyCode key, engine_KeyStates state, f64 time_threshold) {
//     let buffer    = &engine_InputState_global->event_buffer;
//     let curr_time = engine_InputState_global->frame_info.curr_time;

//     // Check from most recent events backwards
//     i32 idx = (buffer->head - 1 + engine_InputEventBuffer_size) % engine_InputEventBuffer_size;
//     for (i32 i = 0; i < buffer->count; ++i) {
//         let event = &buffer->events[idx];
//         if (event->key == key && (event->state & state)) {
//             // Check if event occurred within the time threshold
//             if (curr_time - event->timestamp <= time_threshold) {
//                 return true;
//             }
//             // If we found the key but it's too old, we can stop searching
//             break;
//         }
//         idx = (idx - 1 + engine_InputEventBuffer_size) % engine_InputEventBuffer_size;
//     }
//     return false;
// }
