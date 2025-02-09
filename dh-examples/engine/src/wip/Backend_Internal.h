#ifndef ENGINE_BACKEND_INTERNAL_INCLUDED
#define ENGINE_BACKEND_INTERNAL_INCLUDED (1)

#include "engine-wip/Backend.h"

struct engine_BackendVT_Internal {
    Vec2u (*getWindowSize)(anyptr ctx);

    Vec2u (*getWindowMinSize)(anyptr ctx);
    Vec2u (*getWindowMaxSize)(anyptr ctx);
    Err$void (*setWindowMinSize)(anyptr ctx, Vec2u size) must_check;
    Err$void (*setWindowMaxSize)(anyptr ctx, Vec2u size) must_check;

    bool (*isWindowFocused)(anyptr ctx);
    bool (*isWindowMinimized)(anyptr ctx);
    bool (*isWindowMaximized)(anyptr ctx);
};

extern Vec2u engine_Backend_getWindowSize(const engine_Backend self);

extern Vec2u    engine_Backend_getWindowMinSize(const engine_Backend self);
extern Vec2u    engine_Backend_getWindowMaxSize(const engine_Backend self);
extern Err$void engine_Backend_setWindowMinSize(engine_Backend self, Vec2u size) must_check;
extern Err$void engine_Backend_setWindowMaxSize(engine_Backend self, Vec2u size) must_check;

extern bool engine_Backend_isWindowFocused(const engine_Backend self);
extern bool engine_Backend_isWindowMinimized(const engine_Backend self);
extern bool engine_Backend_isWindowMaximized(const engine_Backend self);

#endif /* ENGINE_BACKEND_INTERNAL_INCLUDED */
