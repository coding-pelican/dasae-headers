#ifndef ENGINE_MOUSE_INCLUDED
#define ENGINE_MOUSE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"

#include "engine/MouseButton.h"
#include "engine/KeyButtonStates.h"

typedef struct engine_Mouse {
    struct {
        var_(curr_states, A$$(engine_MouseButton_count, engine_KeyButtonStates)); /**< Bitfield of current button states */
        var_(prev_states, A$$(engine_MouseButton_count, engine_KeyButtonStates)); /**< Bitfield of previous button states */
    } buttons;
    struct {
        var_(prev_pos, Vec2i);
        var_(curr_pos, Vec2i);
    } cursor;
    struct {
        var_(prev_scroll_amount, Vec2f);
        var_(curr_scroll_amount, Vec2f);
    } wheel;
} engine_Mouse;
use_O$(engine_Mouse);
use_E$(engine_Mouse);

/// Get all states for a button
extern fn_((engine_Mouse_getState(const engine_Mouse* self, engine_MouseButton button))(engine_KeyButtonStates));
/// Queries input state for a specific state
extern fn_((engine_Mouse_isState(const engine_Mouse* self, engine_MouseButton button, engine_KeyButtonStates state))(bool));

/// Return true if the button was just pressed this frame
extern fn_((engine_Mouse_pressed(const engine_Mouse* self, engine_MouseButton button))(bool));
/// Return true if the button is being held down
extern fn_((engine_Mouse_held(const engine_Mouse* self, engine_MouseButton button))(bool));
/// Return true if the button was just released this frame
extern fn_((engine_Mouse_released(const engine_Mouse* self, engine_MouseButton button))(bool));

/// Returns position: on display (ex: (10,10) pixels))
extern fn_((engine_Mouse_getPos(const engine_Mouse* self))(Vec2i));
/// Returns position delta: relative to last frame (ex: (-5,-5) pixels)
extern fn_((engine_Mouse_getPosDelta(const engine_Mouse* self))(Vec2i));
/// Returns wheel scroll amount delta: relative to last frame (ex: (0,10) units)
extern fn_((engine_Mouse_getWheelScrollDelta(const engine_Mouse* self))(Vec2f));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_MOUSE_INCLUDED */
