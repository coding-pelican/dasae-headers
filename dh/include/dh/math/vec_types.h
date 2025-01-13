#ifndef MATH_VEC_TYPES_INCLUDED
#define MATH_VEC_TYPES_INCLUDED (1)

#include "dh/core/pp.h"
#include "dh/core/prim.h"

#define using_math_Vec$(T) \
    using_math_Vec2$(T);   \
    using_math_Vec3$(T);   \
    using_math_Vec4$(T)

#define using_math_Vec2$(T)                                          \
    typedef union pp_join($, math_Vec2, T) pp_join($, math_Vec2, T); \
    union pp_join($, math_Vec2, T) {                                 \
        T s[2];                                                      \
        struct {                                                     \
            T x, y;                                                  \
        };                                                           \
    }

#define using_math_Vec3$(T)                                          \
    typedef union pp_join($, math_Vec3, T) pp_join($, math_Vec3, T); \
    union pp_join($, math_Vec3, T) {                                 \
        T s[3];                                                      \
        struct {                                                     \
            T x, y, z;                                               \
        };                                                           \
    }

#define using_math_Vec4$(T)                                          \
    typedef union pp_join($, math_Vec4, T) pp_join($, math_Vec4, T); \
    union pp_join($, math_Vec4, T) {                                 \
        T s[4];                                                      \
        struct {                                                     \
            T x, y, z, w;                                            \
        };                                                           \
    }

using_math_Vec$(u8);
using_math_Vec$(u16);
using_math_Vec$(u32);
using_math_Vec$(u64);
using_math_Vec$(usize);

using_math_Vec$(i8);
using_math_Vec$(i16);
using_math_Vec$(i32);
using_math_Vec$(i64);
using_math_Vec$(isize);

// using_math_Vec$(f16);
using_math_Vec$(f32);
using_math_Vec$(f64);

#undef using_math_Vec$
#undef using_math_Vec2$
#undef using_math_Vec3$
#undef using_math_Vec4$

typedef math_Vec2$u8    math_Vec2ub;
typedef math_Vec2$u16   math_Vec2us;
typedef math_Vec2$u32   math_Vec2u;
typedef math_Vec2$u64   math_Vec2ul;
typedef math_Vec2$usize math_Vec2uz;
typedef math_Vec3$u8    math_Vec3ub;
typedef math_Vec3$u16   math_Vec3us;
typedef math_Vec3$u32   math_Vec3u;
typedef math_Vec3$u64   math_Vec3ul;
typedef math_Vec3$usize math_Vec3uz;
typedef math_Vec4$u8    math_Vec4ub;
typedef math_Vec4$u16   math_Vec4us;
typedef math_Vec4$u32   math_Vec4u;
typedef math_Vec4$u64   math_Vec4ul;
typedef math_Vec4$usize math_Vec4uz;

typedef math_Vec2$i8    math_Vec2b;
typedef math_Vec2$i16   math_Vec2s;
typedef math_Vec2$i32   math_Vec2i;
typedef math_Vec2$i64   math_Vec2l;
typedef math_Vec2$isize math_Vec2z;
typedef math_Vec3$i8    math_Vec3b;
typedef math_Vec3$i16   math_Vec3s;
typedef math_Vec3$i32   math_Vec3i;
typedef math_Vec3$i64   math_Vec3l;
typedef math_Vec3$isize math_Vec3z;
typedef math_Vec4$i8    math_Vec4b;
typedef math_Vec4$i16   math_Vec4s;
typedef math_Vec4$i32   math_Vec4i;
typedef math_Vec4$i64   math_Vec4l;
typedef math_Vec4$isize math_Vec4z;

// typedef math_Vec2$f16 math_Vec2h;
typedef math_Vec2$f32 math_Vec2f;
typedef math_Vec2$f64 math_Vec2d;
// typedef math_Vec3$f16 math_Vec3h;
typedef math_Vec3$f32 math_Vec3f;
typedef math_Vec3$f64 math_Vec3d;
// typedef math_Vec4$f16 math_Vec4h;
typedef math_Vec4$f32 math_Vec4f;
typedef math_Vec4$f64 math_Vec4d;

#define math_Vec_as$(TDestVec, src_vec...)      FUNC_math_Vec_as$(TDestVec, src_vec)
#define FUNC_math_Vec_as$(TDestVec, src_vec...) eval(      \
    TDestVec    _dest = cleared();                         \
    let         _src  = (src_vec);                         \
    const usize _len  = countOf(_dest.s);                  \
    claim_assert_static(_len == countOf(_src.s));          \
    for (usize _i = 0; _i < _len; ++_i) {                  \
        _dest.s[_i] = as(TypeOf(_dest.s[_i]), _src.s[_i]); \
    };                                                     \
    eval_return _dest;                                     \
)

/* #define math_Vec2_Match(T) TypeOf(_Generic( \
    (T)0,                                   \
    u8: ((math_Vec2ub){ 0 }),               \
    u16: ((math_Vec2us){ 0 }),              \
    u32: ((math_Vec2u){ 0 }),               \
    u64: ((math_Vec2ul){ 0 }),              \
    i8: ((math_Vec2b){ 0 }),                \
    i16: ((math_Vec2s){ 0 }),               \
    i32: ((math_Vec2i){ 0 }),               \
    i64: ((math_Vec2l){ 0 }),               \
    f32: ((math_Vec2f){ 0 }),               \
    f64: ((math_Vec2d){ 0 })                \
))
#define math_Vec3_Match(T) TypeOf(_Generic( \
    (T){ 0 },                               \
    u8: ((math_Vec3ub){ 0 }),               \
    u16: ((math_Vec3us){ 0 }),              \
    u32: ((math_Vec3u){ 0 }),               \
    u64: ((math_Vec3ul){ 0 }),              \
    i8: ((math_Vec3b){ 0 }),                \
    i16: ((math_Vec3s){ 0 }),               \
    i32: ((math_Vec3i){ 0 }),               \
    i64: ((math_Vec3l){ 0 }),               \
    f32: ((math_Vec3f){ 0 }),               \
    f64: ((math_Vec3d{ 0 }))                \
))
#define math_Vec4_Match(T) TypeOf(_Generic( \
    (T){ 0 },                               \
    u8: ((math_Vec4ub){ 0 }),               \
    u16: ((math_Vec4us){ 0 }),              \
    u32: ((math_Vec4u){ 0 }),               \
    u64: ((math_Vec4ul){ 0 }),              \
    i8: ((math_Vec4b){ 0 }),                \
    i16: ((math_Vec4s){ 0 }),               \
    i32: ((math_Vec4i){ 0 }),               \
    i64: ((math_Vec4l){ 0 }),               \
    f32: ((math_Vec4f){ 0 }),               \
    f64: ((math_Vec4d){ 0 })                \
)) */

#endif /* MATH_VEC_TYPES_INCLUDED */
