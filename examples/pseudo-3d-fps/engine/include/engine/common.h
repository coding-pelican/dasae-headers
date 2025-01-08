#ifndef ENGINE_COMMON_INCLUDED
#define ENGINE_COMMON_INCLUDED (1)

#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
#include "dh/mem/Allocator.h"
#include "dh/time.h"
#include "dh/math/common.h"
#include "dh/math/vec.h"

typedef math_Vec2f Vec2f;
typedef math_Vec3f Vec3f;
typedef math_Vec4f Vec4f;
typedef math_Vec2d Vec2d;
typedef math_Vec3d Vec3d;
typedef math_Vec4d Vec4d;

typedef math_Vec2i Vec2i;
typedef math_Vec3i Vec3i;
typedef math_Vec4i Vec4i;
typedef math_Vec2l Vec2l;
typedef math_Vec3l Vec3l;
typedef math_Vec4l Vec4l;
typedef math_Vec2z Vec2z;
typedef math_Vec3z Vec3z;
typedef math_Vec4z Vec4z;

typedef math_Vec2u  Vec2u;
typedef math_Vec3u  Vec3u;
typedef math_Vec4u  Vec4u;
typedef math_Vec2ul Vec2ul;
typedef math_Vec3ul Vec3ul;
typedef math_Vec4ul Vec4ul;
typedef math_Vec2uz Vec2uz;
typedef math_Vec3uz Vec3uz;
typedef math_Vec4uz Vec4uz;

/* typedef union Vec2u {
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
) */

#define Vec_as$(TVec, v_self) eval(                    \
    let  _self = v_self;                               \
    TVec _ret  = { .s = { 0 } };                       \
    for (usize i = 0; i < countOf(_self.s); ++i) {     \
        _ret.s[i] = as(TypeOf(_ret.s[i]), _self.s[i]); \
    };                                                 \
    eval_return _ret;                                  \
)

typedef struct engine_Transform {
    Vec3f position;
    Vec3f rotation;
    Vec3f scale;
} engine_Transform;

#endif /* ENGINE_COMMON_INCLUDED */
