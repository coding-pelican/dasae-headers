#ifndef math_vec_types__included
#define math_vec_types__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Definitions =========================================*/

#define using_m_V$(T) \
    using_m_V2$(T); \
    using_m_V3$(T); \
    using_m_V4$(T)

#define using_m_V2$(T) \
    typedef union pp_join($, m_V2, T) pp_join($, m_V2, T); \
    union pp_join($, m_V2, T) { \
        var_(s, A$$(2, T)); \
        struct { \
            T s0, s1; \
        }; \
        struct { \
            T x, y; \
        }; \
    }

#define using_m_V3$(T) \
    typedef union pp_join($, m_V3, T) pp_join($, m_V3, T); \
    union pp_join($, m_V3, T) { \
        var_(s, A$$(3, T)); \
        struct { \
            T s0, s1, s2; \
        }; \
        struct { \
            T x, y, z; \
        }; \
    }

#define using_m_V4$(T) \
    typedef union pp_join($, m_V4, T) pp_join($, m_V4, T); \
    union pp_join($, m_V4, T) { \
        var_(s, A$$(4, T)); \
        struct { \
            T s0, s1, s2, s3; \
        }; \
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* math_vec_types__included */
