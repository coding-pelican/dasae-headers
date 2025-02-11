#ifndef ENGINE_BACKEND_INTERNAL_INCLUDED
#define ENGINE_BACKEND_INTERNAL_INCLUDED (1)

#include "engine-wip/Backend.h"
#include "engine-wip/KeyCode.h"
#include "engine-wip/MouseButton.h"

struct engine_BackendVT_Internal {
    /* Window Layer */
    Vec2u (*getWindowPos)(const anyptr ctx);
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
    bool (*pressedKey)(const anyptr ctx, engine_KeyCode code);
    bool (*heldKey)(const anyptr ctx, engine_KeyCode code);
    bool (*releasedKey)(const anyptr ctx, engine_KeyCode code);

    bool (*pressedMouse)(const anyptr ctx, engine_MouseButton button);
    bool (*heldMouse)(const anyptr ctx, engine_MouseButton button);
    bool (*releasedMouse)(const anyptr ctx, engine_MouseButton button);

    Vec2i (*getMousePos)(const anyptr ctx);
    Vec2i (*getMouseDelta)(const anyptr ctx);
    Vec2f (*getMouseScrollDelta)(const anyptr ctx);
};

/* Window Layer */
extern Vec2u engine_Backend_getWindowPos(const engine_Backend self);
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
extern bool engine_Backend_pressedKey(const engine_Backend self, engine_KeyCode code);
extern bool engine_Backend_heldKey(const engine_Backend self, engine_KeyCode code);
extern bool engine_Backend_releasedKey(const engine_Backend self, engine_KeyCode code);

extern bool engine_Backend_pressedMouse(const engine_Backend self, engine_MouseButton button);
extern bool engine_Backend_heldMouse(const engine_Backend self, engine_MouseButton button);
extern bool engine_Backend_releasedMouse(const engine_Backend self, engine_MouseButton button);

extern Vec2i engine_Backend_getMousePos(const engine_Backend self);
extern Vec2i engine_Backend_getMouseDelta(const engine_Backend self);
extern Vec2f engine_Backend_getMouseScrollDelta(const engine_Backend self);

#endif /* ENGINE_BACKEND_INTERNAL_INCLUDED */
