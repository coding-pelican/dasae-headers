#ifndef ENGINE_KEY_INCLUDED
#define ENGINE_KEY_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/KeyCode.h"
#include "engine-wip/KeyButtonStates.h"

typedef struct engine_Keyboard {
    struct {
        engine_KeyButtonStates curr_states[engine_KeyCode_count]; /**< Bitfield of current key states */
        engine_KeyButtonStates prev_states[engine_KeyCode_count]; /**< Bitfield of previous key states */
    } keys;
} engine_Keyboard;
use_Opt$(engine_Keyboard);
use_Err$(engine_Keyboard);

/// Get all states for a key
extern u8   engine_Keyboard_getState(engine_KeyCode key);
/// Queries input state for a specific state
extern bool engine_Keyboard_isState(engine_KeyCode key, engine_KeyButtonStates state);

/// Return true if the key was just pressed this frame
extern bool engine_Keyboard_pressed(engine_KeyCode key);
/// Return true if the key is being held down
extern bool engine_Keyboard_held(engine_KeyCode key);
/// Return true if the key was just released this frame
extern bool engine_Keyboard_released(engine_KeyCode key);

#endif /* ENGINE_KEY_INCLUDED */
