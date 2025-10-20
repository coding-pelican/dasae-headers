#ifndef ENGINE_BACKEND_INTERNAL_INCLUDED
#define ENGINE_BACKEND_INTERNAL_INCLUDED (1)
#include "engine/Backend.h"
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/KeyButtonStates.h"
#include "engine/KeyCode.h"
#include "engine/MouseButton.h"

struct engine_Backend_VT_Internal {
    /* Window Layer */
    fn_(((*getWindowPos)(const anyptr ctx))(Vec2i));
    fn_(((*getWindowDim)(const anyptr ctx))(Vec2u));
    fn_(((*getWindowRes)(const anyptr ctx))(Vec2u));

    fn_(((*getWindowMinRes)(const anyptr ctx))(Vec2u));
    fn_(((*getWindowMaxRes)(const anyptr ctx))(Vec2u));
    fn_(((*setWindowMinRes)(anyptr ctx, Vec2u size))(Err$void)) $must_check;
    fn_(((*setWindowMaxRes)(anyptr ctx, Vec2u size))(Err$void)) $must_check;

    fn_(((*isWindowFocused)(const anyptr ctx))(bool));
    fn_(((*isWindowMinimized)(const anyptr ctx))(bool));
    fn_(((*isWindowMaximized)(const anyptr ctx))(bool));

    /* Input Layer */
    fn_(((*getKeyboardState)(const anyptr ctx, engine_KeyCode key))(u8));
    fn_(((*isKeyboardState)(const anyptr ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool));

    fn_(((*pressedKeyboard)(const anyptr ctx, engine_KeyCode key))(bool));
    fn_(((*heldKeyboard)(const anyptr ctx, engine_KeyCode key))(bool));
    fn_(((*releasedKeyboard)(const anyptr ctx, engine_KeyCode key))(bool));

    fn_(((*getMouseState)(const anyptr ctx, engine_MouseButton button))(u8));
    fn_(((*isMouseState)(const anyptr ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool));

    fn_(((*pressedMouse)(const anyptr ctx, engine_MouseButton button))(bool));
    fn_(((*heldMouse)(const anyptr ctx, engine_MouseButton button))(bool));
    fn_(((*releasedMouse)(const anyptr ctx, engine_MouseButton button))(bool));

    fn_(((*getMousePos)(const anyptr ctx))(Vec2i));
    fn_(((*getMousePosDelta)(const anyptr ctx))(Vec2i));
    fn_(((*getMouseWheelScrollDelta)(const anyptr ctx))(Vec2f));
};

/* Window Layer */
extern fn_((engine_Backend_getWindowPos(const engine_Backend self))(Vec2i));
extern fn_((engine_Backend_getWindowDim(const engine_Backend self))(Vec2u));
extern fn_((engine_Backend_getWindowRes(const engine_Backend self))(Vec2u));

extern fn_((engine_Backend_getWindowMinRes(const engine_Backend self))(Vec2u));
extern fn_((engine_Backend_getWindowMaxRes(const engine_Backend self))(Vec2u));
extern fn_((engine_Backend_setWindowMinRes(engine_Backend self, Vec2u size))(Err$void) $must_check);
extern fn_((engine_Backend_setWindowMaxRes(engine_Backend self, Vec2u size))(Err$void) $must_check);

extern fn_((engine_Backend_isWindowFocused(const engine_Backend self))(bool));
extern fn_((engine_Backend_isWindowMinimized(const engine_Backend self))(bool));
extern fn_((engine_Backend_isWindowMaximized(const engine_Backend self))(bool));

/* Input Layer */
extern fn_((engine_Backend_getKeyboardState(const engine_Backend self, engine_KeyCode key))(u8));
extern fn_((engine_Backend_isKeyboardState(const engine_Backend self, engine_KeyCode key, engine_KeyButtonStates state))(bool));

extern fn_((engine_Backend_pressedKeyboard(const engine_Backend self, engine_KeyCode key))(bool));
extern fn_((engine_Backend_heldKeyboard(const engine_Backend self, engine_KeyCode key))(bool));
extern fn_((engine_Backend_releasedKeyboard(const engine_Backend self, engine_KeyCode key))(bool));

extern fn_((engine_Backend_getMouseState(const engine_Backend self, engine_MouseButton button))(u8));
extern fn_((engine_Backend_isMouseState(const engine_Backend self, engine_MouseButton button, engine_KeyButtonStates state))(bool));

extern fn_((engine_Backend_pressedMouse(const engine_Backend self, engine_MouseButton button))(bool));
extern fn_((engine_Backend_heldMouse(const engine_Backend self, engine_MouseButton button))(bool));
extern fn_((engine_Backend_releasedMouse(const engine_Backend self, engine_MouseButton button))(bool));

extern fn_((engine_Backend_getMousePos(const engine_Backend self))(Vec2i));
extern fn_((engine_Backend_getMousePoseDelta(const engine_Backend self))(Vec2i));
extern fn_((engine_Backend_getMouseWheelScrollDelta(const engine_Backend self))(Vec2f));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_BACKEND_INTERNAL_INCLUDED */
