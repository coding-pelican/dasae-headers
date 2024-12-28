#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/time.h"

#define using_Ptr$(T)                     \
    typedef rawptr(T) pp_join($, Ptr, T); \
    typedef const rawptr(T) pp_join($, PtrConst, T);

typedef union engine_Vec3f32 {
    f32 xyz[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
} engine_Vec3f32;

typedef struct engine_Transform {
    engine_Vec3f32 position;
    engine_Vec3f32 rotation;
    engine_Vec3f32 scale;
} engine_Transform;

#endif /* ENGINE_COMMON_INCLUDED */
