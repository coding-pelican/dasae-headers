#ifndef ENGINE_INPUT_INCLUDED
#define ENGINE_INPUT_INCLUDED (1)

#include "common.h"

typedef enum engine_KeyCode {
    engine_KeyCode_None = 0,

    // Special keys
    engine_KeyCode_Escape    = 0x1B,
    engine_KeyCode_Space     = 0x20,
    engine_KeyCode_Enter     = 0x0D,
    engine_KeyCode_Tab       = 0x09,
    engine_KeyCode_Backspace = 0x08,

    // Common movement keys
    engine_KeyCode_W = 0x57,
    engine_KeyCode_A = 0x41,
    engine_KeyCode_S = 0x53,
    engine_KeyCode_D = 0x44,

    // Arrow keys
    engine_KeyCode_Left  = 0x25,
    engine_KeyCode_Up    = 0x26,
    engine_KeyCode_Right = 0x27,
    engine_KeyCode_Down  = 0x28,

    // Modifiers
    engine_KeyCode_Shift   = 0x10,
    engine_KeyCode_Control = 0x11,
    engine_KeyCode_Alt     = 0x12,

    engine_KeyCode_Count = 256
} engine_KeyCode;

typedef struct engine_InputState {
    bool keys[engine_KeyCode_Count];
    bool prev_keys[engine_KeyCode_Count];
} engine_InputState;
extern engine_InputState* engine_InputState_global;

extern void engine_Input_init(void);
extern void engine_Input_update(void);
extern void engine_Input_fini(void);

force_inline bool engine_Key_pressed(engine_KeyCode key) {
    debug_assert_nonnull(engine_InputState_global);
    return engine_InputState_global->keys[key];
}

force_inline bool engine_Key_justPressed(engine_KeyCode key) {
    debug_assert_nonnull(engine_InputState_global);
    return engine_InputState_global->keys[key] && !engine_InputState_global->prev_keys[key];
}

force_inline bool engine_Key_justReleased(engine_KeyCode key) {
    debug_assert_nonnull(engine_InputState_global);
    return !engine_InputState_global->keys[key] && engine_InputState_global->prev_keys[key];
}

#endif /* ENGINE_INPUT_INCLUDED */
