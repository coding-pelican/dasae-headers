#include <windows.h>

#include "engine/input.h"

static engine_InputState engine_Input_s_instance = cleared();

static void engine_Input_updateFrameInfo(void);
static void engine_Input_processKey(engine_KeyCode key, bool is_down);
static void engine_Input_processMouse(void);

// Core input system functions
void engine_Input_init(void) {
    memset(&engine_Input_s_instance, 0, sizeof(engine_InputState));
}

void engine_Input_fini(void) {
    memset(&engine_Input_s_instance, 0, sizeof(engine_InputState));
}

void engine_Input_update(void) {
    let input = &engine_Input_s_instance;

    // Update frame timing
    engine_Input_updateFrameInfo();

    // Save previous state
    memcpy(input->prev_states, input->curr_states, sizeof(input->curr_states));

    // Process each key
    for (engine_KeyCode key = engine_KeyCode_none + 1; key < engine_KeyCode_count; ++key) {
        SHORT key_state = GetAsyncKeyState(key);
        bool  is_down   = (key_state & 0x8000) != 0;
        engine_Input_processKey(key, is_down);
    }

    // Process mouse input
    engine_Input_processMouse();
}

// Event buffer management
void engine_InputEventBuffer_push(engine_InputEvent event) {
    let input  = &engine_Input_s_instance;
    let buffer = &input->event_buffer;
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

    let input  = &engine_Input_s_instance;
    let buffer = &input->event_buffer;
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

    let input  = &engine_Input_s_instance;
    let buffer = &input->event_buffer;
    if (buffer->count == 0) {
        return_none();
    }

    return_some(buffer->events[buffer->tail]);
}

void engine_InputEventBuffer_clear(void) {
    let input     = &engine_Input_s_instance;
    let buffer    = &input->event_buffer;
    buffer->head  = 0;
    buffer->tail  = 0;
    buffer->count = 0;
}

engine_InputState* engine_Input_instance(void) {
    return &engine_Input_s_instance;
}

// Input event processing
static void engine_Input_updateFrameInfo(void) {
    let input                    = &engine_Input_s_instance;
    input->frame_info.prev_time  = input->frame_info.curr_time;
    input->frame_info.curr_time  = (f64)GetTickCount64() / 1000.0;
    input->frame_info.delta_time = input->frame_info.curr_time - input->frame_info.prev_time;
}

static void engine_Input_processKey(engine_KeyCode key, bool is_down) {
    let input      = &engine_Input_s_instance;
    let curr_state = &input->curr_states[key];
    let prev_state = input->prev_states[key];

    // Clear previous state flags
    *curr_state = engine_KeyStates_none;

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
    let input = &engine_Input_s_instance;

    // Save previous mouse position
    input->mouse.prev_x = input->mouse.x;
    input->mouse.prev_y = input->mouse.y;

    // Get current mouse position
    POINT cursor_pos = cleared();
    GetCursorPos(&cursor_pos);

    // Convert to client coordinates if we have a window
    HWND foreground = GetForegroundWindow();
    if (foreground) {
        ScreenToClient(foreground, &cursor_pos);
    }

    // Update mouse position
    input->mouse.x = cursor_pos.x;
    input->mouse.y = cursor_pos.y;

    // Create mouse move event if position changed
    if (input->mouse.x != input->mouse.prev_x || input->mouse.y != input->mouse.prev_y) {
        engine_MouseEvent event = {
            .type = engine_MouseEventType_motion,
            .move = {
                .x = input->mouse.x,
                .y = input->mouse.y },
            .timestamp = (f64)GetTickCount64() / 1000.0
        };
        engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
    }

    // Save previous button states
    memcpy(input->mouse.prev_button_states, input->mouse.curr_button_states, sizeof(input->mouse.curr_button_states));

    // Process mouse buttons
    static const i32 button_vks[] = {
        0,           // None
        VK_LBUTTON,  // Left
        VK_RBUTTON,  // Right
        VK_MBUTTON,  // Middle
        VK_XBUTTON1, // X1
        VK_XBUTTON2, // X2
    };

    for (engine_MouseButton button = engine_MouseButton_none + 1; button < engine_MouseButton_count; ++button) {
        SHORT button_state = GetAsyncKeyState(button_vks[button]);
        bool  is_down      = (button_state & 0x8000) != 0;
        let   curr_state   = &input->mouse.curr_button_states[button];
        let   prev_state   = input->mouse.prev_button_states[button];

        *curr_state = 0;

        if (is_down) {
            if (!(prev_state & engine_KeyStates_held)) {
                *curr_state |= engine_KeyStates_pressed;

                engine_MouseEvent event = {
                    .type   = engine_MouseEventType_button,
                    .button = {
                        .key   = button,
                        .state = engine_KeyStates_pressed },
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
                    .key   = button,
                    .state = engine_KeyStates_released },
                .timestamp = (f64)GetTickCount64() / 1000.0
            };
            engine_InputEventBuffer_push(*(engine_InputEvent*)&event);
        }
    }

    // Process mouse wheel
    // Note: This would ideally be handled in WM_MOUSEWHEEL message processing
    // but for now I'll use a simpler approach
    input->mouse.scroll_delta = 0; // Reset scroll delta each frame
}
