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
    fn_(((*getWindowPos)(const P$raw ctx))(m_V2i32));
    fn_(((*getWindowDim)(const P$raw ctx))(m_V2u32));
    fn_(((*getWindowRes)(const P$raw ctx))(m_V2u32));

    fn_(((*getWindowMinRes)(const P$raw ctx))(m_V2u32));
    fn_(((*getWindowMaxRes)(const P$raw ctx))(m_V2u32));
    fn_(((*setWindowMinRes)(P$raw ctx, m_V2u32 size))(E$void)) $must_check;
    fn_(((*setWindowMaxRes)(P$raw ctx, m_V2u32 size))(E$void)) $must_check;

    fn_(((*isWindowFocused)(const P$raw ctx))(bool));
    fn_(((*isWindowMinimized)(const P$raw ctx))(bool));
    fn_(((*isWindowMaximized)(const P$raw ctx))(bool));

    /* Input Layer */
    fn_(((*getKeyboardState)(const P$raw ctx, engine_KeyCode key))(u8));
    fn_(((*isKeyboardState)(const P$raw ctx, engine_KeyCode key, engine_KeyButtonStates state))(bool));

    fn_(((*pressedKeyboard)(const P$raw ctx, engine_KeyCode key))(bool));
    fn_(((*heldKeyboard)(const P$raw ctx, engine_KeyCode key))(bool));
    fn_(((*releasedKeyboard)(const P$raw ctx, engine_KeyCode key))(bool));

    fn_(((*getMouseState)(const P$raw ctx, engine_MouseButton button))(u8));
    fn_(((*isMouseState)(const P$raw ctx, engine_MouseButton button, engine_KeyButtonStates state))(bool));

    fn_(((*pressedMouse)(const P$raw ctx, engine_MouseButton button))(bool));
    fn_(((*heldMouse)(const P$raw ctx, engine_MouseButton button))(bool));
    fn_(((*releasedMouse)(const P$raw ctx, engine_MouseButton button))(bool));

    fn_(((*getMousePos)(const P$raw ctx))(m_V2i32));
    fn_(((*getMousePosDelta)(const P$raw ctx))(m_V2i32));
    fn_(((*getMouseWheelScrollDelta)(const P$raw ctx))(m_V2f32));
};

/* Window Layer */
extern fn_((engine_Backend_getWindowPos(const engine_Backend self))(m_V2i32));
extern fn_((engine_Backend_getWindowDim(const engine_Backend self))(m_V2u32));
extern fn_((engine_Backend_getWindowRes(const engine_Backend self))(m_V2u32));

extern fn_((engine_Backend_getWindowMinRes(const engine_Backend self))(m_V2u32));
extern fn_((engine_Backend_getWindowMaxRes(const engine_Backend self))(m_V2u32));
extern fn_((engine_Backend_setWindowMinRes(engine_Backend self, m_V2u32 size))(E$void) $must_check);
extern fn_((engine_Backend_setWindowMaxRes(engine_Backend self, m_V2u32 size))(E$void) $must_check);

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

extern fn_((engine_Backend_getMousePos(const engine_Backend self))(m_V2i32));
extern fn_((engine_Backend_getMousePoseDelta(const engine_Backend self))(m_V2i32));
extern fn_((engine_Backend_getMouseWheelScrollDelta(const engine_Backend self))(m_V2f32));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_BACKEND_INTERNAL_INCLUDED */
