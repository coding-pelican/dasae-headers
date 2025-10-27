#ifndef ENGINE_MOUSE_BUTTON_INCLUDED
#define ENGINE_MOUSE_BUTTON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"

/// Mouse button identifiers
enum {
    engine_MouseButton_none = -1,

    /* Standard mouse buttons */
    engine_MouseButton_left   = 0,
    engine_MouseButton_right  = 1,
    engine_MouseButton_middle = 2,

    /* Extended mouse buttons */
    engine_MouseButton_x1 = 3,
    engine_MouseButton_x2 = 4,

    engine_MouseButton_count = 5
};
typedef i8 engine_MouseButton;
use_P$(engine_MouseButton);
use_S$(engine_MouseButton);
use_O$(engine_MouseButton);
use_E$(engine_MouseButton);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_MOUSE_BUTTON_INCLUDED */
