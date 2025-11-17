#ifndef ENGINE_BACKEND_INCLUDED
#define ENGINE_BACKEND_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "engine/common.h"

typedef struct engine_Backend_VT {
    fn_(((*processEvents)(P$raw ctx))(void));
    fn_(((*presentBuffer)(P$raw ctx))(void));
} engine_Backend_VT;
typedef struct engine_Backend_VT_Internal engine_Backend_VT_Internal;

typedef struct engine_Backend {
    P$raw ptr;
    const engine_Backend_VT* vt;
    const engine_Backend_VT_Internal* vt_internal;
} engine_Backend;
T_use$((engine_Backend)(O, E));

extern fn_((engine_Backend_processEvents(engine_Backend self))(void));
extern fn_((engine_Backend_presentBuffer(engine_Backend self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ENGINE_BACKEND_INCLUDED */
