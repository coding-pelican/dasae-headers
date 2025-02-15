#ifndef ENGINE_MOUSE_INCLUDED
#define ENGINE_MOUSE_INCLUDED (1)

#include "engine-wip/common.h"
#include "engine-wip/MouseButton.h"
#include "engine-wip/KeyButtonStates.h"

typedef struct engine_Mouse {
    struct {
        engine_KeyButtonStates curr_states[engine_MouseButton_count]; /**< Bitfield of current button states */
        engine_KeyButtonStates prev_states[engine_MouseButton_count]; /**< Bitfield of previous button states */
    } buttons;
    struct {
        Vec2i prev_pos;
        Vec2i curr_pos;
    } cursor;
    struct {
        Vec2f prev_scroll_amount;
        Vec2f curr_scroll_amount;
    } wheel;
} engine_Mouse;
use_Opt$(engine_Mouse);
use_Err$(engine_Mouse);

/// Get all states for a button
extern engine_KeyButtonStates engine_Mouse_getState(const engine_Mouse* self, engine_MouseButton button);
/// Queries input state for a specific state
extern bool                   engine_Mouse_isState(const engine_Mouse* self, engine_MouseButton button, engine_KeyButtonStates state);

/// Return true if the button was just pressed this frame
extern bool engine_Mouse_pressed(const engine_Mouse* self, engine_MouseButton button);
/// Return true if the button is being held down
extern bool engine_Mouse_held(const engine_Mouse* self, engine_MouseButton button);
/// Return true if the button was just released this frame
extern bool engine_Mouse_released(const engine_Mouse* self, engine_MouseButton button);

/// Returns position: on display (ex: (10,10) pixels))
extern Vec2i engine_Mouse_getPos(const engine_Mouse* self);
/// Returns position delta: relative to last frame (ex: (-5,-5) pixels)
extern Vec2i engine_Mouse_getPosDelta(const engine_Mouse* self);
/// Returns wheel scroll amount delta: relative to last frame (ex: (0,10) units)
extern Vec2f engine_Mouse_getWheelScrollDelta(const engine_Mouse* self);

#endif /* ENGINE_MOUSE_INCLUDED */
