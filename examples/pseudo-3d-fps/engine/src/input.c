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

static void engine_Input_processMouse(void) {
    let state = engine_InputState_global;

    // Save previous mouse position
    state->mouse.prev_x = state->mouse.x;
    state->mouse.prev_y = state->mouse.y;

    // Get current mouse position
    POINT cursor_pos = cleared();
    GetCursorPos(&cursor_pos);

    // Convert to client coordinates if we have a window
    HWND foreground = GetForegroundWindow();
    if (foreground) {
        ScreenToClient(foreground, &cursor_pos);
    }

    // Update mouse position
    state->mouse.x = cursor_pos.x;
    state->mouse.y = cursor_pos.y;

    // Create mouse move event if position changed
    if (state->mouse.x != state->mouse.prev_x || state->mouse.y != state->mouse.prev_y) {
        engine_MouseEvent event = {
            .type = engine_MouseEventType_move,
            .move = {
                .x = state->mouse.x,
                .y = state->mouse.y },
            .timestamp = (f64)GetTickCount64() / 1000.0
        };
        engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
    }

    // Save previous button states
    memcpy(state->mouse.button_prev_states, state->mouse.button_curr_states, sizeof(state->mouse.button_curr_states));

    // Process mouse buttons
    static const i32 button_vks[] = {
        VK_LBUTTON, // Left
        VK_RBUTTON, // Right
        VK_MBUTTON, // Middle
    };

    for (i32 i = 1; i < engine_MouseButton_count; ++i) {
        bool is_down    = (GetAsyncKeyState(button_vks[i - 1]) & 0x8000) != 0;
        let  curr_state = &state->mouse.button_curr_states[i];
        let  prev_state = state->mouse.button_prev_states[i];

        *curr_state = 0;

        if (is_down) {
            if (!(prev_state & engine_KeyStates_held)) {
                *curr_state |= engine_KeyStates_pressed;

                engine_MouseEvent event = {
                    .type   = engine_MouseEventType_button,
                    .button = {
                        .button = i,
                        .state  = engine_KeyStates_pressed },
                    .timestamp = (f64)GetTickCount64() / 1000.0
                };
                engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
            }
            *curr_state |= engine_KeyStates_held;
        } else if (prev_state & (engine_KeyStates_pressed | engine_KeyStates_held)) {
            *curr_state |= engine_KeyStates_released;

            engine_MouseEvent event = {
                .type   = engine_MouseEventType_button,
                .button = {
                    .button = i,
                    .state  = engine_KeyStates_released },
                .timestamp = (f64)GetTickCount64() / 1000.0
            };
            engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
        }
    }

    // Process mouse wheel
    // Note: This would ideally be handled in WM_MOUSEWHEEL message processing
    // but for now I'll use a simpler approach
    state->mouse.scroll_delta = 0; // Reset scroll delta each frame
}

void engine_Input_init(void) {
    if (engine_InputState_global) { return; }

    engine_InputState_global = (engine_InputState*)malloc(sizeof(engine_InputState));
    if (!engine_InputState_global) { return; }

    memset(engine_InputState_global, 0, sizeof(engine_InputState));
}

void engine_Input_fini(void) {
    if (!engine_InputState_global) { return; }

    free(engine_InputState_global);
    engine_InputState_global = null;
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

    // Process mouse input
    engine_Input_processMouse();
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
