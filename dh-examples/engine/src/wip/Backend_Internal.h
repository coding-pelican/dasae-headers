#ifndef ENGINE_BACKEND_INTERNAL_INCLUDED
#define ENGINE_BACKEND_INTERNAL_INCLUDED (1)

#include "engine-wip/Backend.h"
#include "engine-wip/KeyButtonStates.h"
#include "engine-wip/KeyCode.h"
#include "engine-wip/MouseButton.h"

struct engine_BackendVT_Internal {
    /* Window Layer */
    Vec2i (*getWindowPos)(const anyptr ctx);
    Vec2u (*getWindowDim)(const anyptr ctx);
    Vec2u (*getWindowRes)(const anyptr ctx);

    Vec2u (*getWindowMinRes)(const anyptr ctx);
    Vec2u (*getWindowMaxRes)(const anyptr ctx);
    Err$void (*setWindowMinRes)(anyptr ctx, Vec2u size) must_check;
    Err$void (*setWindowMaxRes)(anyptr ctx, Vec2u size) must_check;

    bool (*isWindowFocused)(const anyptr ctx);
    bool (*isWindowMinimized)(const anyptr ctx);
    bool (*isWindowMaximized)(const anyptr ctx);

    /* Input Layer */
    u8 (*getKeyboardState)(const anyptr ctx, engine_KeyCode key);
    bool (*isKeyboardState)(const anyptr ctx, engine_KeyCode key, engine_KeyButtonStates state);

    bool (*pressedKeyboard)(const anyptr ctx, engine_KeyCode key);
    bool (*heldKeyboard)(const anyptr ctx, engine_KeyCode key);
    bool (*releasedKeyboard)(const anyptr ctx, engine_KeyCode key);

    u8 (*getMouseState)(const anyptr ctx, engine_MouseButton button);
    bool (*isMouseState)(const anyptr ctx, engine_MouseButton button, engine_KeyButtonStates state);

    bool (*pressedMouse)(const anyptr ctx, engine_MouseButton button);
    bool (*heldMouse)(const anyptr ctx, engine_MouseButton button);
    bool (*releasedMouse)(const anyptr ctx, engine_MouseButton button);

    Vec2i (*getMousePos)(const anyptr ctx);
    Vec2i (*getMousePosDelta)(const anyptr ctx);
    Vec2f (*getMouseWheelScrollDelta)(const anyptr ctx);
};

/* Window Layer */
extern Vec2i engine_Backend_getWindowPos(const engine_Backend self);
extern Vec2u engine_Backend_getWindowDim(const engine_Backend self);
extern Vec2u engine_Backend_getWindowRes(const engine_Backend self);

extern Vec2u    engine_Backend_getWindowMinRes(const engine_Backend self);
extern Vec2u    engine_Backend_getWindowMaxRes(const engine_Backend self);
extern Err$void engine_Backend_setWindowMinRes(engine_Backend self, Vec2u size) must_check;
extern Err$void engine_Backend_setWindowMaxRes(engine_Backend self, Vec2u size) must_check;

extern bool engine_Backend_isWindowFocused(const engine_Backend self);
extern bool engine_Backend_isWindowMinimized(const engine_Backend self);
extern bool engine_Backend_isWindowMaximized(const engine_Backend self);

/* Input Layer */
extern u8   engine_Backend_getKeyboardState(const engine_Backend self, engine_KeyCode key);
extern bool engine_Backend_isKeyboardState(const engine_Backend self, engine_KeyCode key, engine_KeyButtonStates state);

extern bool engine_Backend_pressedKeyboard(const engine_Backend self, engine_KeyCode key);
extern bool engine_Backend_heldKeyboard(const engine_Backend self, engine_KeyCode key);
extern bool engine_Backend_releasedKeyboard(const engine_Backend self, engine_KeyCode key);

extern u8   engine_Backend_getMouseState(const engine_Backend self, engine_MouseButton button);
extern bool engine_Backend_isMouseState(const engine_Backend self, engine_MouseButton button, engine_KeyButtonStates state);

extern bool engine_Backend_pressedMouse(const engine_Backend self, engine_MouseButton button);
extern bool engine_Backend_heldMouse(const engine_Backend self, engine_MouseButton button);
extern bool engine_Backend_releasedMouse(const engine_Backend self, engine_MouseButton button);

extern Vec2i engine_Backend_getMousePos(const engine_Backend self);
extern Vec2i engine_Backend_getMousePoseDelta(const engine_Backend self);
extern Vec2f engine_Backend_getMouseWheelScrollDelta(const engine_Backend self);

#endif /* ENGINE_BACKEND_INTERNAL_INCLUDED */
