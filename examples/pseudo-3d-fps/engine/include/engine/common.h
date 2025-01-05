#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/time.h"

typedef union engine_Vec2u32 {
    u32 scalars[2];
    struct {
        u32 x;
        u32 y;
    };
} engine_Vec2u32;

typedef union engine_Vec3u32 {
    u32 scalars[3];
    struct {
        u32 x;
        u32 y;
        u32 z;
    };
} engine_Vec3u32;

typedef union engine_Vec2i32 {
    i32 scalars[2];
    struct {
        i32 x;
        i32 y;
    };
} engine_Vec2i32;

typedef union engine_Vec3i32 {
    i32 scalars[3];
    struct {
        i32 x;
        i32 y;
        i32 z;
    };
} engine_Vec3i32;

typedef union engine_Vec2f32 {
    f32 scalars[2];
    struct {
        f32 x;
        f32 y;
    };
} engine_Vec2f32;

typedef union engine_Vec3f32 {
    f32 scalars[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
} engine_Vec3f32;

#define Vec_add(v_lhs, v_rhs) eval(                                 \
    var lhs = v_lhs;                                                \
    let rhs = v_rhs;                                                \
    let len = prim_min(countOf(lhs.scalars), countOf(lhs.scalars)); \
    for (usize i = 0; i < len; ++i) {                               \
        lhs.scalars[i] += rhs.scalars[i];                           \
    };                                                              \
    eval_return lhs;                                                \
)

#define Vec_sub(v_lhs, v_rhs) eval(                                 \
    var lhs = v_lhs;                                                \
    let rhs = v_rhs;                                                \
    let len = prim_min(countOf(lhs.scalars), countOf(lhs.scalars)); \
    for (usize i = 0; i < len; ++i) {                               \
        lhs.scalars[i] -= rhs.scalars[i];                           \
    };                                                              \
    eval_return lhs;                                                \
)

#define Vec_scale(v_lhs, s_rhs) eval( \
    var lhs = v_lhs;                  \
    let rhs = s_rhs;                  \
    let len = countOf(lhs.scalars);   \
    for (usize i = 0; i < len; ++i) { \
        lhs.scalars[i] *= rhs;        \
    };                                \
    eval_return lhs;                  \
)

#define Vec_as(T, v_self) eval(                                   \
    let self = v_self;                                            \
    let len  = countOf(self.scalars);                             \
    T   ret  = { .scalars = { 0 } };                              \
    for (usize i = 0; i < len; ++i) {                             \
        ret.scalars[i] = (TypeOf(ret.scalars[i]))self.scalars[i]; \
    };                                                            \
    eval_return ret;                                              \
)

#define PI (3.14159265358979323846)

typedef struct engine_Transform {
    engine_Vec3f32 position;
    engine_Vec3f32 rotation;
    engine_Vec3f32 scale;
} engine_Transform;

#endif /* ENGINE_COMMON_INCLUDED */
