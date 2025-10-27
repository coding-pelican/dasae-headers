#ifndef ENGINE_KEY_BUTTON_STATES_INCLUDED
#define ENGINE_KEY_BUTTON_STATES_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"

/// Key states
enum {
    engine_KeyButtonStates_none     = 0,
    engine_KeyButtonStates_pressed  = 1 << 0, /**< Key/Button was just pressed this frame */
    engine_KeyButtonStates_held     = 1 << 1, /**< Key/Button is being held down */
    engine_KeyButtonStates_released = 1 << 2  /**< Key/Button was just released this frame */
};
typedef u8 engine_KeyButtonStates;
use_P$(engine_KeyButtonStates);
use_S$(engine_KeyButtonStates);
use_O$(engine_KeyButtonStates);
use_E$(engine_KeyButtonStates);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_KEY_BUTTON_STATES_INCLUDED */
