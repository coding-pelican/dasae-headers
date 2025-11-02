#ifndef MATH_VEC_TYPES_INCLUDED
#define MATH_VEC_TYPES_INCLUDED (1)

#include "dh/prl.h"

#define using_m_V$(T) \
    using_m_V2$(T); \
    using_m_V3$(T); \
    using_m_V4$(T)

#define using_m_V2$(T) \
    typedef union pp_join($, m_V2, T) pp_join($, m_V2, T); \
    union pp_join($, m_V2, T) { \
        T s[2]; \
        struct { \
            T x, y; \
        }; \
    }

#define using_m_V3$(T) \
    typedef union pp_join($, m_V3, T) pp_join($, m_V3, T); \
    union pp_join($, m_V3, T) { \
        T s[3]; \
        struct { \
            T x, y, z; \
        }; \
    }

#define using_m_V4$(T) \
    typedef union pp_join($, m_V4, T) pp_join($, m_V4, T); \
    union pp_join($, m_V4, T) { \
        T s[4]; \
        struct { \
            T x, y, z, w; \
        }; \
    }

using_m_V$(u8);
using_m_V$(u16);
using_m_V$(u32);
using_m_V$(u64);
using_m_V$(usize);

using_m_V$(i8);
using_m_V$(i16);
using_m_V$(i32);
using_m_V$(i64);
using_m_V$(isize);

// using_m_V$(f16);
using_m_V$(f32);
using_m_V$(f64);

#undef using_m_V$
#undef using_m_V2$
#undef using_m_V3$
#undef using_m_V4$

typedef m_V2$u8 m_V2u8;
typedef m_V2$u16 m_V2u16;
typedef m_V2$u32 m_V2u32;
typedef m_V2$u64 m_V2u64;
typedef m_V2$usize m_V2usize;
typedef m_V3$u8 m_V3u8;
typedef m_V3$u16 m_V3u16;
typedef m_V3$u32 m_V3u32;
typedef m_V3$u64 m_V3u64;
typedef m_V3$usize m_V3usize;
typedef m_V4$u8 m_V4u8;
typedef m_V4$u16 m_V4u16;
typedef m_V4$u32 m_V4u32;
typedef m_V4$u64 m_V4u64;
typedef m_V4$usize m_V4usize;

typedef m_V2u8 m_V2ub;
typedef m_V2u16 m_V2us;
typedef m_V2u32 m_V2u;
typedef m_V2u64 m_V2ul;
typedef m_V2usize m_V2uz;
typedef m_V3u8 m_V3ub;
typedef m_V3u16 m_V3us;
typedef m_V3u32 m_V3u;
typedef m_V3u64 m_V3ul;
typedef m_V3usize m_V3uz;
typedef m_V4u8 m_V4ub;
typedef m_V4u16 m_V4us;
typedef m_V4u32 m_V4u;
typedef m_V4u64 m_V4ul;
typedef m_V4usize m_V4uz;

typedef m_V2$i8 m_V2i8;
typedef m_V2$i16 m_V2i16;
typedef m_V2$i32 m_V2i32;
typedef m_V2$i64 m_V2i64;
typedef m_V2$isize m_V2isize;
typedef m_V3$i8 m_V3i8;
typedef m_V3$i16 m_V3i16;
typedef m_V3$i32 m_V3i32;
typedef m_V3$i64 m_V3i64;
typedef m_V3$isize m_V3isize;
typedef m_V4$i8 m_V4i8;
typedef m_V4$i16 m_V4i16;
typedef m_V4$i32 m_V4i32;
typedef m_V4$i64 m_V4i64;
typedef m_V4$isize m_V4isize;

typedef m_V2i8 m_V2b;
typedef m_V2i16 m_V2s;
typedef m_V2i32 m_V2i;
typedef m_V2i64 m_V2l;
typedef m_V2isize m_V2z;
typedef m_V3i8 m_V3b;
typedef m_V3i16 m_V3s;
typedef m_V3i32 m_V3i;
typedef m_V3i64 m_V3l;
typedef m_V3isize m_V3z;
typedef m_V4i8 m_V4b;
typedef m_V4i16 m_V4s;
typedef m_V4i32 m_V4i;
typedef m_V4i64 m_V4l;
typedef m_V4isize m_V4z;

// typedef m_V2$f16 m_V2f16;
typedef m_V2$f32 m_V2f32;
typedef m_V2$f64 m_V2f64;
// typedef m_V3$f16 m_V3f16;
typedef m_V3$f32 m_V3f32;
typedef m_V3$f64 m_V3f64;
// typedef m_V4$f16 m_V4f16;
typedef m_V4$f32 m_V4f32;
typedef m_V4$f64 m_V4f64;

// typedef m_V2f16 m_V2h;
typedef m_V2f32 m_V2f;
typedef m_V2f64 m_V2d;
// typedef m_V3f16 m_V3h;
typedef m_V3f32 m_V3f;
typedef m_V3f64 m_V3d;
// typedef m_V4f16 m_V4h;
typedef m_V4f32 m_V4f;
typedef m_V4f64 m_V4d;

#define m_V_as$(TDestVec, src_vec...)      FUNC_m_V_as$(TDestVec, src_vec)
#define FUNC_m_V_as$(TDestVec, src_vec...) blk({ \
    TDestVec _dest = cleared(); \
    let _src = (src_vec); \
    const usize _len = countOf(_dest.s); \
    claim_assert_static(_len == countOf(_src.s)); \
    for (usize _i = 0; _i < _len; ++_i) { \
        _dest.s[_i] = as$((TypeOf(_dest.s[_i]))(_src.s[_i])); \
    } \
    blk_return _dest; \
})

/* #define m_V2_Match(T) TypeOf(_Generic( \
    (T)0,                                   \
    u8: ((m_V2ub){ 0 }),               \
    u16: ((m_V2us){ 0 }),              \
    u32: ((m_V2u){ 0 }),               \
    u64: ((m_V2ul){ 0 }),              \
    i8: ((m_V2b){ 0 }),                \
    i16: ((m_V2s){ 0 }),               \
    i32: ((m_V2i){ 0 }),               \
    i64: ((m_V2l){ 0 }),               \
    f32: ((m_V2f){ 0 }),               \
    f64: ((m_V2d){ 0 })                \
))
#define m_V3_Match(T) TypeOf(_Generic( \
    (T){ 0 },                               \
    u8: ((m_V3ub){ 0 }),               \
    u16: ((m_V3us){ 0 }),              \
    u32: ((m_V3u){ 0 }),               \
    u64: ((m_V3ul){ 0 }),              \
    i8: ((m_V3b){ 0 }),                \
    i16: ((m_V3s){ 0 }),               \
    i32: ((m_V3i){ 0 }),               \
    i64: ((m_V3l){ 0 }),               \
    f32: ((m_V3f){ 0 }),               \
    f64: ((m_V3d{ 0 }))                \
))
#define m_V4_Match(T) TypeOf(_Generic( \
    (T){ 0 },                               \
    u8: ((m_V4ub){ 0 }),               \
    u16: ((m_V4us){ 0 }),              \
    u32: ((m_V4u){ 0 }),               \
    u64: ((m_V4ul){ 0 }),              \
    i8: ((m_V4b){ 0 }),                \
    i16: ((m_V4s){ 0 }),               \
    i32: ((m_V4i){ 0 }),               \
    i64: ((m_V4l){ 0 }),               \
    f32: ((m_V4f){ 0 }),               \
    f64: ((m_V4d){ 0 })                \
)) */

#endif /* MATH_VEC_TYPES_INCLUDED */
