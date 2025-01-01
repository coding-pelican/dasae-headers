// input.h
#ifndef ENGINE_INPUT_INCLUDED
#define ENGINE_INPUT_INCLUDED (1)

#include "common.h"

// Key states for more granular input handling
typedef enum engine_KeyStates {
    engine_KeyStates_none     = 0,
    engine_KeyStates_pressed  = 1 << 0, // Key was just pressed this frame
    engine_KeyStates_held     = 1 << 1, // Key is being held down
    engine_KeyStates_released = 1 << 2, // Key was just released this frame
} engine_KeyStates;
using_Opt$(engine_KeyStates);

typedef enum engine_KeyCode {
    engine_KeyCode_none = 0,

    // Special keys
    engine_KeyCode_Backspace = 0x08,
    engine_KeyCode_Tab       = 0x09,
    engine_KeyCode_Enter     = 0x0D,

    // Modifiers
    engine_KeyCode_Shift = 0x10,
    engine_KeyCode_Ctrl  = 0x11,
    engine_KeyCode_Alt   = 0x12,

    // Special Keys
    engine_KeyCode_Esc   = 0x1B,
    engine_KeyCode_Space = 0x20,

    // Arrow keys
    engine_KeyCode_ArrowLt = 0x25,
    engine_KeyCode_ArrowUp = 0x26,
    engine_KeyCode_ArrowRt = 0x27,
    engine_KeyCode_ArrowDn = 0x28,

    // Number keys
    engine_KeyCode_N1 = 0x31,
    engine_KeyCode_N2 = 0x32,
    engine_KeyCode_N3 = 0x33,
    engine_KeyCode_N4 = 0x34,
    engine_KeyCode_N5 = 0x35,
    engine_KeyCode_N6 = 0x36,
    engine_KeyCode_N7 = 0x37,
    engine_KeyCode_N8 = 0x38,
    engine_KeyCode_N9 = 0x39,
    engine_KeyCode_N0 = 0x30,

    // Alphabetical keys
    engine_KeyCode_A = 0x41,
    engine_KeyCode_B = 0x42,
    engine_KeyCode_C = 0x43,
    engine_KeyCode_D = 0x44,
    engine_KeyCode_E = 0x45,
    engine_KeyCode_F = 0x46,
    engine_KeyCode_G = 0x47,
    engine_KeyCode_H = 0x48,
    engine_KeyCode_I = 0x49,
    engine_KeyCode_J = 0x4A,
    engine_KeyCode_K = 0x4B,
    engine_KeyCode_L = 0x4C,
    engine_KeyCode_M = 0x4D,
    engine_KeyCode_N = 0x4E,
    engine_KeyCode_O = 0x4F,
    engine_KeyCode_P = 0x50,
    engine_KeyCode_Q = 0x51,
    engine_KeyCode_R = 0x52,
    engine_KeyCode_S = 0x53,
    engine_KeyCode_T = 0x54,
    engine_KeyCode_U = 0x55,
    engine_KeyCode_V = 0x56,
    engine_KeyCode_W = 0x57,
    engine_KeyCode_X = 0x58,
    engine_KeyCode_Y = 0x59,
    engine_KeyCode_Z = 0x5A,

    engine_KeyCode_count = 256
} engine_KeyCode;
using_Opt$(engine_KeyCode);

// Frame timing information
typedef struct engine_InputFrameInfo {
    f64 curr_time;  // Current frame timestamp
    f64 prev_time;  // Previous frame timestamp
    f64 delta_time; // Time between frames
} engine_InputFrameInfo;

// Input event structure
typedef struct engine_InputEvent {
    engine_KeyCode   key;
    engine_KeyStates state;
    f64              timestamp; // Time when the event occurred
} engine_InputEvent;
using_Opt$(engine_InputEvent);

// Circular buffer for input event stack
#define engine_InputEventBuffer_size (64)
// Buffer for input events between updates
typedef struct engine_InputEventBuffer {
    engine_InputEvent events[engine_InputEventBuffer_size];
    i32               head;
    i32               tail;
    i32               count;
} engine_InputEventBuffer;

// Add to engine_InputState struct
typedef struct engine_InputState {
    u8                      curr_states[engine_KeyCode_count]; // Bitfield of current key states
    u8                      prev_states[engine_KeyCode_count]; // Bitfield of previous key states
    engine_InputEventBuffer event_buffer;
    engine_InputFrameInfo   frame_info;
} engine_InputState;

extern engine_InputState* engine_InputState_global;

// Core input system functions
extern void engine_Input_init(void);
extern void engine_Input_update(void);
extern void engine_Input_fini(void);

// Event buffer management
extern void                  engine_InputEventBuffer_push(engine_InputEvent event);
extern Opt$engine_InputEvent engine_InputEventBuffer_pop();
extern Opt$engine_InputEvent engine_InputEventBuffer_peek();
extern void                  engine_InputEventBuffer_clear(void);

// Event buffer queries
// extern bool engine_Input_anyKeyState(engine_KeyStates state);
// extern i32  engine_Input_countEvents(engine_KeyStates state);
// extern i32  engine_Input_countEventsBetweenFrames(engine_KeyStates state);
// extern bool engine_Input_isEventRecent(engine_KeyCode key, engine_KeyStates state, f64 time_threshold);

// Input state queries
force_inline bool engine_Key_isState(engine_KeyCode key, engine_KeyStates state) {
    debug_assert_nonnull(engine_InputState_global);
    debug_assert_true(key < engine_KeyCode_count);
    return (engine_InputState_global->curr_states[key] & state) != engine_KeyStates_none;
}

force_inline bool engine_Key_pressed(engine_KeyCode key) {
    return engine_Key_isState(key, engine_KeyStates_pressed);
}

force_inline bool engine_Key_held(engine_KeyCode key) {
    return engine_Key_isState(key, engine_KeyStates_held);
}

force_inline bool engine_Key_released(engine_KeyCode key) {
    return engine_Key_isState(key, engine_KeyStates_released);
}

// Get all current states for a key
force_inline u8 engine_Key_getState(engine_KeyCode key) {
    debug_assert_nonnull(engine_InputState_global);
    debug_assert_true(key < engine_KeyCode_count);
    return engine_InputState_global->curr_states[key];
}

#endif /* ENGINE_INPUT_INCLUDED */
