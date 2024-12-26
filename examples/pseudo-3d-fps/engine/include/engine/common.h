#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core/prim.h"
#include "dh/core/pp.h"
#include "dh/claim.h"
#include "dh/debug.h"
#include "dh/extensions.h"
#include "dh/Str.h"

typedef union engine_Vec3f32 {
    f32 xyz[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
} engine_Vec3f32;
using_ExtTypes(engine_Vec3f32);

typedef struct engine_Transform {
    engine_Vec3f32 position;
    engine_Vec3f32 rotation;
    engine_Vec3f32 scale;
} engine_Transform;
using_ExtTypes(engine_Transform);

#endif /* ENGINE_COMMON_INCLUDED */
