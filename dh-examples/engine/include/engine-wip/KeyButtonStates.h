#ifndef ENGINE_KEY_BUTTON_STATES_INCLUDED
#define ENGINE_KEY_BUTTON_STATES_INCLUDED (1)

#include "engine-wip/common.h"

/// Key states
enum {
    engine_KeyButtonStates_none     = 0,
    engine_KeyButtonStates_pressed  = 1 << 0, /**< Key/Button was just pressed this frame */
    engine_KeyButtonStates_held     = 1 << 1, /**< Key/Button is being held down */
    engine_KeyButtonStates_released = 1 << 2  /**< Key/Button was just released this frame */
};
typedef u8 engine_KeyButtonStates;
use_Ptr$(engine_KeyButtonStates);
use_Sli$(engine_KeyButtonStates);
use_Opt$(engine_KeyButtonStates);
use_Err$(engine_KeyButtonStates);

#endif /* ENGINE_KEY_BUTTON_STATES_INCLUDED */
