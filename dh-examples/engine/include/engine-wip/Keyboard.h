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
extern engine_KeyButtonStates engine_Keyboard_getState(const engine_Keyboard* self, engine_KeyCode key);
/// Queries input state for a specific state
extern bool                   engine_Keyboard_isState(const engine_Keyboard* self, engine_KeyCode key, engine_KeyButtonStates state);

/// Return true if the key was just pressed this frame
extern bool engine_Keyboard_pressed(const engine_Keyboard* self, engine_KeyCode key);
/// Return true if the key is being held down
extern bool engine_Keyboard_held(const engine_Keyboard* self, engine_KeyCode key);
/// Return true if the key was just released this frame
extern bool engine_Keyboard_released(const engine_Keyboard* self, engine_KeyCode key);

#endif /* ENGINE_KEY_INCLUDED */
