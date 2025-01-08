#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/time.h"

typedef union Vec2u {
    u32 scalars[2];
    struct {
        u32 x;
        u32 y;
    };
} Vec2u;

typedef union Vec3u {
    u32 scalars[3];
    struct {
        u32 x;
        u32 y;
        u32 z;
    };
} Vec3u;

typedef union Vec2i {
    i32 scalars[2];
    struct {
        i32 x;
        i32 y;
    };
} Vec2i;

typedef union Vec3i {
    i32 scalars[3];
    struct {
        i32 x;
        i32 y;
        i32 z;
    };
} Vec3i;

typedef union Vec2f {
    f32 scalars[2];
    struct {
        f32 x;
        f32 y;
    };
} Vec2f;

typedef union Vec3f {
    f32 scalars[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
} Vec3f;

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

#define Vec_mul(v_lhs, v_rhs) eval(                                 \
    var lhs = v_lhs;                                                \
    let rhs = v_rhs;                                                \
    let len = prim_min(countOf(lhs.scalars), countOf(lhs.scalars)); \
    for (usize i = 0; i < len; ++i) {                               \
        lhs.scalars[i] *= rhs.scalars[i];                           \
    };                                                              \
    eval_return lhs;                                                \
)

#define Vec_div(v_lhs, v_rhs) eval(                                 \
    var lhs = v_lhs;                                                \
    let rhs = v_rhs;                                                \
    let len = prim_min(countOf(lhs.scalars), countOf(lhs.scalars)); \
    for (usize i = 0; i < len; ++i) {                               \
        if (rhs.scalars[i] == 0.0f) {                               \
            lhs = makeCleared(TypeOf(lhs));                         \
            break;                                                  \
        }                                                           \
        lhs.scalars[i] /= rhs.scalars[i];                           \
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
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
} engine_Transform;

#endif /* ENGINE_COMMON_INCLUDED */
