#ifndef ENGINE_BACKEND_INCLUDED
#define ENGINE_BACKEND_INCLUDED (1)

#include "engine-wip/common.h"

typedef struct engine_BackendVT_Internal engine_BackendVT_Internal;
typedef struct engine_BackendVT {
    void (*processEvents)(anyptr ctx);
    void (*presentBuffer)(anyptr ctx);
} engine_BackendVT;

typedef struct engine_Backend {
    anyptr                           ptr;
    const engine_BackendVT*          vt;
    const engine_BackendVT_Internal* vt_internal;
} engine_Backend;
use_Opt$(engine_Backend);
use_Err$(engine_Backend);

extern void engine_Backend_processEvents(engine_Backend self);
extern void engine_Backend_presentBuffer(engine_Backend self);

#endif /* ENGINE_BACKEND_INCLUDED */
