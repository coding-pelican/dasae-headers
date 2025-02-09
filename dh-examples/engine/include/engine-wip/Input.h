// input.h
#ifndef ENGINE_INPUT_INCLUDED
#define ENGINE_INPUT_INCLUDED (1)

#include "engine-wip/common.h"

// Key code identifiers
typedef enum engine_KeyCode {
    engine_KeyCode_none = 0,

    // Special keys
    engine_KeyCode_backspace = 0x08,
    engine_KeyCode_tab       = 0x09,
    engine_KeyCode_enter     = 0x0D,

    // Modifiers
    engine_KeyCode_shift_left  = 0xA0,
    engine_KeyCode_shift_right = 0xA1,
    engine_KeyCode_shift       = engine_KeyCode_shift_left,
    engine_KeyCode_ctrl_left   = 0xA2,
    engine_KeyCode_ctrl_right  = 0xA3,
    engine_KeyCode_ctrl        = engine_KeyCode_ctrl_left,
    engine_KeyCode_alt_left    = 0xA4,
    engine_KeyCode_alt_right   = 0xA5,
    engine_KeyCode_alt         = engine_KeyCode_alt_left,

    // Special Keys
    engine_KeyCode_esc   = 0x1B,
    engine_KeyCode_space = 0x20,

    // Arrow keys
    engine_KeyCode_arrow_left  = 0x25,
    engine_KeyCode_arrow_up    = 0x26,
    engine_KeyCode_arrow_right = 0x27,
    engine_KeyCode_arrow_down  = 0x28,

    // Number keys
    engine_KeyCode_n1 = 0x31,
    engine_KeyCode_n2 = 0x32,
    engine_KeyCode_n3 = 0x33,
    engine_KeyCode_n4 = 0x34,
    engine_KeyCode_n5 = 0x35,
    engine_KeyCode_n6 = 0x36,
    engine_KeyCode_n7 = 0x37,
    engine_KeyCode_n8 = 0x38,
    engine_KeyCode_n9 = 0x39,
    engine_KeyCode_n0 = 0x30,

    // Alphabetical keys
    engine_KeyCode_a = 0x41,
    engine_KeyCode_b = 0x42,
    engine_KeyCode_c = 0x43,
    engine_KeyCode_d = 0x44,
    engine_KeyCode_e = 0x45,
    engine_KeyCode_f = 0x46,
    engine_KeyCode_g = 0x47,
    engine_KeyCode_h = 0x48,
    engine_KeyCode_i = 0x49,
    engine_KeyCode_j = 0x4A,
    engine_KeyCode_k = 0x4B,
    engine_KeyCode_l = 0x4C,
    engine_KeyCode_m = 0x4D,
    engine_KeyCode_n = 0x4E,
    engine_KeyCode_o = 0x4F,
    engine_KeyCode_p = 0x50,
    engine_KeyCode_q = 0x51,
    engine_KeyCode_r = 0x52,
    engine_KeyCode_s = 0x53,
    engine_KeyCode_t = 0x54,
    engine_KeyCode_u = 0x55,
    engine_KeyCode_v = 0x56,
    engine_KeyCode_w = 0x57,
    engine_KeyCode_x = 0x58,
    engine_KeyCode_y = 0x59,
    engine_KeyCode_z = 0x5A,

    // Numpad keys
    engine_KeyCode_kp_n0      = 0x60,
    engine_KeyCode_kp_n1      = 0x61,
    engine_KeyCode_kp_n2      = 0x62,
    engine_KeyCode_kp_n3      = 0x63,
    engine_KeyCode_kp_n4      = 0x64,
    engine_KeyCode_kp_n5      = 0x65,
    engine_KeyCode_kp_n6      = 0x66,
    engine_KeyCode_kp_n7      = 0x67,
    engine_KeyCode_kp_n8      = 0x68,
    engine_KeyCode_kp_n9      = 0x69,
    engine_KeyCode_kp_mul     = 0x6A,
    engine_KeyCode_kp_add     = 0x6B,
    engine_KeyCode_kp_sub     = 0x6D,
    engine_KeyCode_kp_decimal = 0x6E,
    engine_KeyCode_kp_div     = 0x6F,

    engine_KeyCode_count = 256
} engine_KeyCode;
use_Opt$(engine_KeyCode);

// Key code states
typedef enum engine_KeyStates {
    engine_KeyStates_none     = 0,
    engine_KeyStates_pressed  = 1 << 0, // Key was just pressed this frame
    engine_KeyStates_held     = 1 << 1, // Key is being held down
    engine_KeyStates_released = 1 << 2  // Key was just released this frame
} engine_KeyStates;
use_Opt$(engine_KeyStates);

// Mouse button identifiers
typedef enum engine_MouseButton {
    engine_MouseButton_none = 0,

    engine_MouseButton_left,
    engine_MouseButton_right,
    engine_MouseButton_middle,
    engine_MouseButton_x1,
    engine_MouseButton_x2,

    engine_MouseButton_count
} engine_MouseButton;

// Mouse event types
typedef enum engine_MouseEventType {
    engine_MouseEventType_none = 0,
    engine_MouseEventType_move,
    engine_MouseEventType_button,
    engine_MouseEventType_scroll
} engine_MouseEventType;

// Mouse state structure
typedef struct engine_MouseState {
    u8  button_curr_states[engine_MouseButton_count]; // Current button states
    u8  button_prev_states[engine_MouseButton_count]; // Previous button states
    i32 x;                                            // Current X position
    i32 y;                                            // Current Y position
    i32 prev_x;                                       // Previous X position
    i32 prev_y;                                       // Previous Y position
    i32 scroll_delta;                                 // Scroll wheel delta
    f64 last_scroll_timestamp;                        // Time of the last scroll event
    i32 accumulated_scroll_delta;                     // Accumulated delta since last speed calculation
    f64 scroll_speed;                                 // Calculated scroll speed
} engine_MouseState;

// Input event structure
typedef struct engine_InputEvent {
    engine_KeyCode   key;
    engine_KeyStates state;
    f64              timestamp; // Time when the event occurred
} engine_InputEvent;
use_Opt$(engine_InputEvent);

// Mouse event structure
typedef struct engine_MouseEvent {
    engine_MouseEventType type;
    union {
        Vec2i move;
        struct {
            engine_MouseButton key;
            engine_KeyStates   state;
        } button;
        struct {
            i32 delta;
        } scroll;
    };
    f64 timestamp;
} engine_MouseEvent;
use_Opt$(engine_MouseEvent);

// Circular buffer for input event stack
#define engine_InputEventBuffer_size (64)
// Buffer for input events between updates
typedef struct engine_InputEventBuffer {
    engine_InputEvent events[engine_InputEventBuffer_size];
    i32               head;
    i32               tail;
    i32               count;
} engine_InputEventBuffer;

// Frame timing information
typedef struct engine_InputFrameInfo {
    f64 curr_time;  // Current frame timestamp
    f64 prev_time;  // Previous frame timestamp
    f64 delta_time; // Time between frames
} engine_InputFrameInfo;

// Input state structure
typedef struct engine_InputState {
    engine_InputEventBuffer event_buffer;
    u8                      curr_states[engine_KeyCode_count]; // Bitfield of current key states
    u8                      prev_states[engine_KeyCode_count]; // Bitfield of previous key states
    engine_MouseState       mouse;
    engine_InputFrameInfo   frame_info;
} engine_InputState;

// Core input system functions
extern void engine_Input_init(void);
extern void engine_Input_fini(void);
extern void engine_Input_update(void);

// Event buffer management
extern void                  engine_InputEventBuffer_push(engine_InputEvent event);
extern Opt$engine_InputEvent engine_InputEventBuffer_pop();
extern Opt$engine_InputEvent engine_InputEventBuffer_peek();
extern void                  engine_InputEventBuffer_clear(void);

// Get input system singleton instance
extern engine_InputState* engine_Input_instance(void);

// Input state queries
force_inline bool engine_Key_isState(engine_KeyCode key, engine_KeyStates state) {
    debug_assert_true(engine_KeyCode_none <= key);
    debug_assert_true(key < engine_KeyCode_count);
    debug_assert_true(engine_KeyStates_none <= state);
    let input = engine_Input_instance();
    return (input->curr_states[key] & state) != engine_KeyStates_none;
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
    debug_assert_true(engine_KeyCode_none <= key);
    debug_assert_true(key < engine_KeyCode_count);
    let input = engine_Input_instance();
    return input->curr_states[key];
}

// Mouse state queries
force_inline bool engine_Mouse_isState(engine_MouseButton button, engine_KeyStates state) {
    debug_assert_true(engine_MouseButton_none <= button);
    debug_assert_true(button < engine_MouseButton_count);
    debug_assert_true(engine_KeyStates_none <= state);
    let input = engine_Input_instance();
    return (input->mouse.button_curr_states[button] & state) != engine_KeyStates_none;
}

force_inline bool engine_Mouse_pressed(engine_MouseButton button) {
    return engine_Mouse_isState(button, engine_KeyStates_pressed);
}

force_inline bool engine_Mouse_held(engine_MouseButton button) {
    return engine_Mouse_isState(button, engine_KeyStates_held);
}

force_inline bool engine_Mouse_released(engine_MouseButton button) {
    return engine_Mouse_isState(button, engine_KeyStates_released);
}

force_inline Vec2i engine_Mouse_getPosition(void) {
    let input = engine_Input_instance();
    return (Vec2i){
        .s = {
            input->mouse.x,
            input->mouse.y,
        }
    };
}

force_inline Vec2i engine_Mouse_getDelta(void) {
    let input = engine_Input_instance();
    return (Vec2i){
        .s = {
            input->mouse.x - input->mouse.prev_x,
            input->mouse.y - input->mouse.prev_y,
        }
    };
}

force_inline f64 engine_Mouse_getScrollSpeed(void) {
    let input = engine_Input_instance();
    return input->mouse.scroll_speed;
}

force_inline i32 engine_Mouse_getScrollDelta(void) {
    let input = engine_Input_instance();
    return input->mouse.scroll_delta;
}

force_inline f64 engine_Mouse_getScrollDeltaAccumulated(void) {
    let input = engine_Input_instance();
    return input->mouse.accumulated_scroll_delta;
}


#endif /* ENGINE_INPUT_INCLUDED */
