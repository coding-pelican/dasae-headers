#ifndef ENGINE_KEYBOARD_INCLUDED
#define ENGINE_KEYBOARD_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"

#include "engine/KeyCode.h"
#include "engine/KeyButtonStates.h"

typedef struct engine_Keyboard {
    struct {
        var_(curr_states, Arr$$(engine_KeyCode_count, engine_KeyButtonStates)); /**< Bitfield of current key states */
        var_(prev_states, Arr$$(engine_KeyCode_count, engine_KeyButtonStates)); /**< Bitfield of previous key states */
    } keys;
} engine_Keyboard;
use_Opt$(engine_Keyboard);
use_Err$(engine_Keyboard);

/// Get all states for a key
extern fn_(engine_Keyboard_getState(const engine_Keyboard* self, engine_KeyCode key), engine_KeyButtonStates);
/// Queries input state for a specific state
extern fn_(engine_Keyboard_isState(const engine_Keyboard* self, engine_KeyCode key, engine_KeyButtonStates state), bool);

/// Return true if the key was just pressed this frame
extern fn_(engine_Keyboard_pressed(const engine_Keyboard* self, engine_KeyCode key), bool);
/// Return true if the key is being held down
extern fn_(engine_Keyboard_held(const engine_Keyboard* self, engine_KeyCode key), bool);
/// Return true if the key was just released this frame
extern fn_(engine_Keyboard_released(const engine_Keyboard* self, engine_KeyCode key), bool);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_KEY_INCLUDED */
