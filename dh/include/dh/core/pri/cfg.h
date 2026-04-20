/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
 *
 * @brief   Configuration settings for primitive types
 * @details Defines compile-time options and configuration parameters
 *          for primitive types and functions.
 */
#ifndef CORE_PRIM_CFG_INCLUDED
#define CORE_PRIM_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../builtin.h"
#include "../claim/assert_static.h"

#include <stddef.h>
#if !lang_version_in_c23 && UNUSED_CODE
#include <stdbool.h> /* deprecated in C23; `bool`, `true`, `false` are now keywords. */
#include <iso646.h> /* deprecated in C23; `not`, `and`, `or` are now keywords. */
#endif /* !lang_version_in_c23 && UNUSED_CODE */

#include <stdint.h>
#include <limits.h>
#include <float.h>

#if lang_version_in_c23 && UNUSED_CODE
#include <stdbit.h>
#include <stdckdint.h>
#include <stdcountof.h>
#endif /* lang_version_in_c23 && UNUSED_CODE */

#if !lang_version_in_c23 && UNUSED_CODE
#include <stdalign.h> /* deprecated in C23; use `alignof`, `alignas` or `[[alignas]]`. */
/* C11 `stdnoreturn.h` conflicts with Windows when included before `windows.h`.
 * `noreturn` expands to `_Noreturn`; MSVC uses `__declspec(noreturn)`, yielding
 * invalid `__declspec(_Noreturn)`. Known since 2012.
 * Gnulib: https://www.gnu.org/software/gnulib/manual/html_node/stdnoreturn_002eh.html
 * Eric Blake (autoconf 2012-04): https://lists.gnu.org/archive/html/autoconf/2012-04/msg00051.html */
#include <stdnoreturn.h> /* deprecated in C23; use `[[noreturn]]`. */
#endif /* !lang_version_in_c23 && UNUSED_CODE */

#include <stdarg.h>
#if !defined(__STDC_NO_ATOMICS__) && UNUSED_CODE
#include <stdatomic.h>
#endif /* !defined(__STDC_NO_ATOMICS__) && UNUSED_CODE */

/*========== Macros and Declarations ========================================*/

/*--- Zero-sized type ---*/

typedef struct Void {
} Void;

/*--- Boolean ---*/

typedef _Bool bool;
#define bool_(_literal...) (as$(bool)(_literal))
#define true bool_(1)
#define false bool_(0)

/*--- Unsigned integer ---*/

typedef uint8_t u8;
#define u8_(_literal...) (as$(u8)(_literal))
#define u8_bits (8u)
#define u8_bytes (1u)
#define u8_limit u8_limit_max
#define u8_limit_min u8_(0)
#define u8_limit_max u8_(UINT8_MAX)
#define u8_limit_flt_min_bound_excl (-1.0)
#define u8_limit_flt_max_bound_excl (0x1p8)

typedef uint16_t u16;
#define u16_(_literal...) (as$(u16)(_literal))
#define u16_bits (16u)
#define u16_bytes (2u)
#define u16_limit u16_limit_max
#define u16_limit_min u16_(0)
#define u16_limit_max u16_(UINT16_MAX)
#define u16_limit_flt_min_bound_excl (-1.0)
#define u16_limit_flt_max_bound_excl (0x1p16)

typedef uint32_t u32;
#define u32_(_literal...) (as$(u32)(_literal))
#define u32_bits (32u)
#define u32_bytes (4u)
#define u32_limit u32_limit_max
#define u32_limit_min u32_(0)
#define u32_limit_max u32_(UINT32_MAX)
#define u32_limit_flt_min_bound_excl (-1.0)
#define u32_limit_flt_max_bound_excl (0x1p32)

typedef uint64_t u64;
#define u64_(_literal...) (as$(u64)(_literal))
#define u64_bits (64u)
#define u64_bytes (8u)
#define u64_limit u64_limit_max
#define u64_limit_min u64_(0)
#define u64_limit_max u64_(UINT64_MAX)
#define u64_limit_flt_min_bound_excl (-1.0)
#define u64_limit_flt_max_bound_excl (0x1p64)

// typedef __uint128_t u128;
// #define u128_(_literal...) (as$(u128)(_literal))
// #define u128_bits (128u)
// #define u128_bytes (16u)
// #define u128_limit u128_limit_max
// #define u128_limit_min u128_(0)
// #define u128_limit_max u128_(UINT128_MAX)
// #define u128_limit_flt_min_bound_excl (-1.0)
// #define u128_limit_flt_max_bound_excl (0x1p127)

typedef uintptr_t usize;
#define usize_(_literal...) (as$(usize)(_literal))
#define usize_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_bits), \
    pp_else_(u32_bits) \
)
#define usize_bytes pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_bytes), \
    pp_else_(u32_bytes) \
)
#define usize_limit usize_limit_max
#define usize_limit_min pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_min), \
    pp_else_(u32_limit_min) \
)
#define usize_limit_max pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_max), \
    pp_else_(u32_limit_max) \
)
#define usize_limit_flt_min_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_flt_min_bound_excl), \
    pp_else_(u32_limit_flt_min_bound_excl) \
)
#define usize_limit_flt_max_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(u64_limit_flt_max_bound_excl), \
    pp_else_(u32_limit_flt_max_bound_excl) \
)

/*--- Signed integer ---*/

typedef int8_t i8;
#define i8_(_literal...) (as$(i8)(_literal))
#define i8_bits (8u)
#define i8_bytes (1u)
#define i8_limit_min i8_(INT8_MIN)
#define i8_limit_max i8_(INT8_MAX)
#define i8_limit_max_ngtv i8_(INT8_MIN)
#define i8_limit_min_ngtv i8_(-1)
#define i8_limit_min_pstv i8_(+1)
#define i8_limit_max_pstv i8_(INT8_MAX)
#define i8_limit_flt_min_bound_excl (-0x1p7 - 1.0)
#define i8_limit_flt_max_bound_excl (0x1p7)

typedef int16_t i16;
#define i16_(_literal...) (as$(i16)(_literal))
#define i16_bits (16u)
#define i16_bytes (2u)
#define i16_limit_min i16_(INT16_MIN)
#define i16_limit_max i16_(INT16_MAX)
#define i16_limit_max_ngtv i16_(INT16_MIN)
#define i16_limit_min_ngtv i16_(-1)
#define i16_limit_min_pstv i16_(+1)
#define i16_limit_max_pstv i16_(INT16_MAX)
#define i16_limit_flt_min_bound_excl (-0x1p15 - 1.0)
#define i16_limit_flt_max_bound_excl (0x1p15)

typedef int32_t i32;
#define i32_(_literal...) (as$(i32)(_literal))
#define i32_bits (32u)
#define i32_bytes (4u)
#define i32_limit_min i32_(INT32_MIN)
#define i32_limit_max i32_(INT32_MAX)
#define i32_limit_max_ngtv i32_(INT32_MIN)
#define i32_limit_min_ngtv i32_(-1)
#define i32_limit_min_pstv i32_(+1)
#define i32_limit_max_pstv i32_(INT32_MAX)
#define i32_limit_flt_min_bound_excl (-0x1p31 - 1.0)
#define i32_limit_flt_max_bound_excl (0x1p31)

typedef int64_t i64;
#define i64_(_literal...) (as$(i64)(_literal))
#define i64_bits (64u)
#define i64_bytes (8u)
#define i64_limit_min i64_(INT64_MIN)
#define i64_limit_max i64_(INT64_MAX)
#define i64_limit_max_ngtv i64_(INT64_MIN)
#define i64_limit_min_ngtv i64_(-1)
#define i64_limit_min_pstv i64_(+1)
#define i64_limit_max_pstv i64_(INT64_MAX)
#define i64_limit_flt_min_bound_excl (-0x1p63 - 1.0)
#define i64_limit_flt_max_bound_excl (0x1p63)

// typedef __int128_t i128;
// #define i128_(_literal...) (as$(i128)(_literal))
// #define i128_bits      (128u)
// #define i128_bytes     (16u)
// #define i128_limit_min i128_(INT128_MIN)
// #define i128_limit_max i128_(INT128_MAX)
// #define i128_limit_max_ngtv i128_(INT128_MIN)
// #define i128_limit_min_ngtv i128_(-1)
// #define i128_limit_min_pstv i128_(+1)
// #define i128_limit_max_pstv i128_(INT128_MAX)
// #define i128_limit_flt_min_bound_excl (-0x1p127 - 1.0)
// #define i128_limit_flt_max_bound_excl (0x1p127)

typedef intptr_t isize;
#define isize_(_literal...) (as$(isize)(_literal))
#define isize_bits pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_bits), \
    pp_else_(i32_bits) \
)
#define isize_bytes pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_bytes), \
    pp_else_(i32_bytes) \
)
#define isize_limit_min pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min), \
    pp_else_(i32_limit_min) \
)
#define isize_limit_max pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max), \
    pp_else_(i32_limit_max) \
)
#define isize_limit_max_ngtv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max_ngtv), \
    pp_else_(i32_limit_max_ngtv) \
)
#define isize_limit_min_ngtv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min_ngtv), \
    pp_else_(i32_limit_min_ngtv) \
)
#define isize_limit_min_pstv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_min_pstv), \
    pp_else_(i32_limit_min_pstv) \
)
#define isize_limit_max_pstv pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_max_pstv), \
    pp_else_(i32_limit_max_pstv) \
)
#define isize_limit_flt_min_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_flt_min_bound_excl), \
    pp_else_(i32_limit_flt_min_bound_excl) \
)
#define isize_limit_flt_max_bound_excl pp_if_(arch_bits_is_64bit)( \
    pp_then_(i64_limit_flt_max_bound_excl), \
    pp_else_(i32_limit_flt_max_bound_excl) \
)

/* floating point */
// typedef _Float16 f16;
// #define f16_(_literal...) (as$(f16)(_literal))
// #define f16_limit_min      f16_(FLT16_MIN)
// #define f16_limit_max      f16_(FLT16_MAX)
// #define f16_limit_max_ngtv f16_(-FLT16_MAX)
// #define f16_limit_min_ngtv f16_(-FLT16_MIN)
// #define f16_limit_min_pstv f16_(+FLT16_MIN)
// #define f16_limit_max_pstv f16_(+FLT16_MAX)
// #define f16_bits           (16u)
// #define f16_bytes          (2u)
// #define f16_eps            f16_(FLT16_EPSILON)
// #define f16_nan            f16_(__builtin_nan(""))
// #define f16_inf            f16_(__builtin_inf())
// #define f16_inf_ngtv       f16_(-f16_inf)
// #define f16_inf_pstv       f16_(+f16_inf)

typedef float f32;
#define f32_(_literal...) (as$(f32)(_literal))
#define f32_bits (32u)
#define f32_bytes (4u)
#define f32_limit_min f32_(FLT_MIN)
#define f32_limit_max f32_(FLT_MAX)
#define f32_limit_max_ngtv f32_(-FLT_MAX)
#define f32_limit_min_ngtv f32_(-FLT_MIN)
#define f32_limit_min_pstv f32_(+FLT_MIN)
#define f32_limit_max_pstv f32_(+FLT_MAX)
#define f32_eps f32_(FLT_EPSILON)
#define f32_nan f32_(__builtin_nan(""))
#define f32_inf f32_(__builtin_inf())
#define f32_inf_ngtv f32_(-f32_inf)
#define f32_inf_pstv f32_(+f32_inf)

typedef double f64;
#define f64_(_literal...) (as$(f64)(_literal))
#define f64_bits (64u)
#define f64_bytes (8u)
#define f64_limit_min f64_(FLT_MIN)
#define f64_limit_max f64_(DBL_MAX)
#define f64_limit_max_ngtv f64_(-DBL_MAX)
#define f64_limit_min_ngtv f64_(-DBL_MIN)
#define f64_limit_min_pstv f64_(+DBL_MIN)
#define f64_limit_max_pstv f64_(+DBL_MAX)
#define f64_eps f64_(DBL_EPSILON)
#define f64_nan f64_(__builtin_nan(""))
#define f64_inf f64_(__builtin_inf())
#define f64_inf_ngtv f64_(-f64_inf)
#define f64_inf_pstv f64_(+f64_inf)

// typedef long double f80;
// #define f80_(_literal...) (as$(f80)(_literal))
// #define f80_bits           (80u)
// #define f80_bytes          (10u)
// #define f80_limit_min      f80_(LDBL_MIN)
// #define f80_limit_max      f80_(LDBL_MAX)
// #define f80_limit_max_ngtv f80_(-LDBL_MAX)
// #define f80_limit_min_ngtv f80_(-LDBL_MIN)
// #define f80_limit_min_pstv f80_(+LDBL_MIN)
// #define f80_limit_max_pstv f80_(+LDBL_MAX)
// #define f80_eps            f80_(LDBL_EPSILON)
// #define f80_nan            f80_(__builtin_nan(""))
// #define f80_inf            f80_(__builtin_inf())
// #define f80_inf_ngtv       f80_(-f80_inf)
// #define f80_inf_pstv       f80_(+f80_inf)

// typedef __float128 f128;
// #define f128_(_literal...) (as$(f128)(_literal))
// #define f128_bits           (128u)
// #define f128_bytes          (16u)
// #define f128_limit_min      f128_(FLT128_MIN)
// #define f128_limit_max      f128_(FLT128_MAX)
// #define f128_limit_max_ngtv f128_(-FLT128_MAX)
// #define f128_limit_min_ngtv f128_(-FLT128_MIN)
// #define f128_limit_min_pstv f128_(+FLT128_MIN)
// #define f128_limit_max_pstv f128_(+FLT128_MAX)
// #define f128_eps            f128_(FLT128_EPSILON)
// #define f128_nan            f128_(__builtin_nan(""))
// #define f128_inf            f128_(__builtin_inf())
// #define f128_inf_ngtv       f128_(-f128_inf)
// #define f128_inf_pstv       f128_(+f128_inf)

/*--- Type Predicates ---*/

#define isBool$(_T... /*(bool)*/) __val__isBool$(_T)
#define isUInt$(_T... /*(bool)*/) __val__isUInt$(_T)
#define isIInt$(_T... /*(bool)*/) __val__isIInt$(_T)
#define isInt$(_T... /*(bool)*/) __val__isInt$(_T)
#define isFlt$(_T... /*(bool)*/) __val__isFlt$(_T)

/*--- Type Limits ---*/

#define int_bits$(_T... /*(u32)*/) __val__int_bits$(_T)
#define int_bytes$(_T... /*(u32)*/) __val__int_bytes$(_T)

#define uint_limit$(_T... /*(T)*/) __val__uint_limit$(_T)
#define int_limit_min$(_T... /*(T)*/) __val__int_limit_min$(_T)
#define int_limit_max$(_T... /*(T)*/) __val__int_limit_max$(_T)
#define iint_limit_max_ngtv$(_T... /*(T)*/) __val__iint_limit_max_ngtv$(_T)
#define iint_limit_min_ngtv$(_T... /*(T)*/) __val__iint_limit_min_ngtv$(_T)
#define iint_limit_min_pstv$(_T... /*(T)*/) __val__iint_limit_min_pstv$(_T)
#define iint_limit_max_pstv$(_T... /*(T)*/) __val__iint_limit_max_pstv$(_T)
#define int_limit_flt_min_bound_excl$(_T... /*(f64)*/) __val__int_limit_flt_min_bound_excl$(_T)
#define int_limit_flt_max_bound_excl$(_T... /*(f64)*/) __val__int_limit_flt_max_bound_excl$(_T)

#define flt_bits$(_T... /*(u32)*/) __val__flt_bits$(_T)
#define flt_bytes$(_T... /*(u32)*/) __val__flt_bytes$(_T)

#define flt_limit_min$(_T... /*(T)*/) __val__flt_limit_min$(_T)
#define flt_limit_max$(_T... /*(T)*/) __val__flt_limit_max$(_T)
#define flt_limit_max_ngtv$(_T... /*(T)*/) __val__flt_limit_max_ngtv$(_T)
#define flt_limit_min_ngtv$(_T... /*(T)*/) __val__flt_limit_min_ngtv$(_T)
#define flt_limit_min_pstv$(_T... /*(T)*/) __val__flt_limit_min_pstv$(_T)
#define flt_limit_max_pstv$(_T... /*(T)*/) __val__flt_limit_max_pstv$(_T)

#define flt_eps$(_T... /*(T)*/) __val__flt_eps$(_T)
#define flt_nan$(_T... /*(T)*/) __val__flt_nan$(_T)
#define flt_inf$(_T... /*(T)*/) __val__flt_inf$(_T)
#define flt_inf_ngtv$(_T... /*(T)*/) __val__flt_inf_ngtv$(_T)
#define flt_inf_pstv$(_T... /*(T)*/) __val__flt_inf_pstv$(_T)

/*--- Pointer ---*/

typedef TypeOf(const void*) P_const$raw;
typedef TypeOf(void*) P$raw;

#define $P_const$(_T...) TypeOf(const _T*)
#define $P$(_T...) TypeOf(_T*)

#define isNull(_p /*: P(_T)*/... /*(bool)*/) ____isNull(_p)
#define isNonnull(_p /*: P(_T)*/... /*(bool)*/) ____isNonnull(_p)
#define ensureNonnull(_p /*: P(_T)*/... /*(P(_T))*/) __step__ensureNonnull(_p)

#define ref(_v /*: _T*/... /*(P(_T))*/) ____ref(_v)
#define deref(_p /*: P(_T)*/... /*(_T)*/) ____deref(_p)

#define ptrToInt(_p /*: PtrType*/... /*(usize)*/) ____ptrToInt(_p)
#define intToPtr$(/*(_P_T: PtrType)(_val: usize)*/... /*(_P_T)*/) __step__intToPtr$(__VA_ARGS__)

#define ptrCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) __step__ptrCast$(__VA_ARGS__)
#define ptrAlignCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) __step__ptrAlignCast$(__VA_ARGS__)
#define ptrQualCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) ____ptrQualCast$(__VA_ARGS__)
#define ptrAlignQualCast$(/*(_P_T: PtrType)(_val: PtrType)*/... /*(_P_T)*/) ____ptrAlignQualCast$(__VA_ARGS__)

#define mutCast(_p /*: P(_T)*/... /*(P_const(_T))*/) ____mutCast(_p)
#define constCast(_p /*: P_const(_T)*/... /*(P(_T))*/) ____constCast(_p)

/*========== Macros and Definitions =========================================*/

#define __val__isBool$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((bool)(true)), \
        T_default_(false) \
    ) pp_end \
)
#define __val__isUInt$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(true)), \
        T_case$((u16)(true)), \
        T_case$((u32)(true)), \
        T_case$((u64)(true)), \
        T_default_(false) \
    ) pp_end \
)
#define __val__isIInt$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((i8)(true)), \
        T_case$((i16)(true)), \
        T_case$((i32)(true)), \
        T_case$((i64)(true)), \
        T_default_(false) \
    ) pp_end \
)
#define __val__isInt$(_T...) (isUInt$(_T) || isIInt$(_T))
#define __val__isFlt$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(true)), \
        T_case$((f64)(true)), \
        T_default_(false) \
    ) pp_end \
)

#define __val__int_bits$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_bits)), \
        T_case$((u16)(u16_bits)), \
        T_case$((u32)(u32_bits)), \
        T_case$((u64)(u64_bits)), \
        T_case$((i8)(i8_bits)), \
        T_case$((i16)(i16_bits)), \
        T_case$((i32)(i32_bits)), \
        T_case$((i64)(i64_bits)) \
    ) pp_end \
)
#define __val__int_bytes$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_bytes)), \
        T_case$((u16)(u16_bytes)), \
        T_case$((u32)(u32_bytes)), \
        T_case$((u64)(u64_bytes)), \
        T_case$((i8)(i8_bytes)), \
        T_case$((i16)(i16_bytes)), \
        T_case$((i32)(i32_bytes)), \
        T_case$((i64)(i64_bytes)) \
    ) pp_end \
)

#define __val__uint_limit$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_limit)), \
        T_case$((u16)(u16_limit)), \
        T_case$((u32)(u32_limit)), \
        T_case$((u64)(u64_limit)) \
    ) pp_end \
)
#define __val__int_limit_min$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_limit_min)), \
        T_case$((u16)(u16_limit_min)), \
        T_case$((u32)(u32_limit_min)), \
        T_case$((u64)(u64_limit_min)), \
        T_case$((i8)(i8_limit_min)), \
        T_case$((i16)(i16_limit_min)), \
        T_case$((i32)(i32_limit_min)), \
        T_case$((i64)(i64_limit_min)) \
    ) pp_end \
)
#define __val__int_limit_max$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_limit_max)), \
        T_case$((u16)(u16_limit_max)), \
        T_case$((u32)(u32_limit_max)), \
        T_case$((u64)(u64_limit_max)), \
        T_case$((i8)(i8_limit_max)), \
        T_case$((i16)(i16_limit_max)), \
        T_case$((i32)(i32_limit_max)), \
        T_case$((i64)(i64_limit_max)) \
    ) pp_end \
)
#define __val__iint_limit_max_ngtv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((i8)(i8_limit_max_ngtv)), \
        T_case$((i16)(i16_limit_max_ngtv)), \
        T_case$((i32)(i32_limit_max_ngtv)), \
        T_case$((i64)(i64_limit_max_ngtv)) \
    ) pp_end \
)
#define __val__iint_limit_min_ngtv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((i8)(i8_limit_min_ngtv)), \
        T_case$((i16)(i16_limit_min_ngtv)), \
        T_case$((i32)(i32_limit_min_ngtv)), \
        T_case$((i64)(i64_limit_min_ngtv)) \
    ) pp_end \
)
#define __val__iint_limit_min_pstv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((i8)(i8_limit_min_pstv)), \
        T_case$((i16)(i16_limit_min_pstv)), \
        T_case$((i32)(i32_limit_min_pstv)), \
        T_case$((i64)(i64_limit_min_pstv)) \
    ) pp_end \
)
#define __val__iint_limit_max_pstv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((i8)(i8_limit_max_pstv)), \
        T_case$((i16)(i16_limit_max_pstv)), \
        T_case$((i32)(i32_limit_max_pstv)), \
        T_case$((i64)(i64_limit_max_pstv)) \
    ) pp_end \
)
#define __val__int_limit_flt_min_bound_excl$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_limit_flt_min_bound_excl)), \
        T_case$((u16)(u16_limit_flt_min_bound_excl)), \
        T_case$((u32)(u32_limit_flt_min_bound_excl)), \
        T_case$((u64)(u64_limit_flt_min_bound_excl)), \
        T_case$((i8)(i8_limit_flt_min_bound_excl)), \
        T_case$((i16)(i16_limit_flt_min_bound_excl)), \
        T_case$((i32)(i32_limit_flt_min_bound_excl)), \
        T_case$((i64)(i64_limit_flt_min_bound_excl)) \
    ) pp_end \
)
#define __val__int_limit_flt_max_bound_excl$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((u8)(u8_limit_flt_max_bound_excl)), \
        T_case$((u16)(u16_limit_flt_max_bound_excl)), \
        T_case$((u32)(u32_limit_flt_max_bound_excl)), \
        T_case$((u64)(u64_limit_flt_max_bound_excl)), \
        T_case$((i8)(i8_limit_flt_max_bound_excl)), \
        T_case$((i16)(i16_limit_flt_max_bound_excl)), \
        T_case$((i32)(i32_limit_flt_max_bound_excl)), \
        T_case$((i64)(i64_limit_flt_max_bound_excl)) \
    ) pp_end \
)

#define __val__flt_bits$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_bits)), \
        T_case$((f64)(f64_bits)) \
    ) pp_end \
)
#define __val__flt_bytes$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_bytes)), \
        T_case$((f64)(f64_bytes)) \
    ) pp_end \
)

#define __val__flt_limit_min$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_min)), \
        T_case$((f64)(f64_limit_min)) \
    ) pp_end \
)
#define __val__flt_limit_max$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_max)), \
        T_case$((f64)(f64_limit_max)) \
    ) pp_end \
)
#define __val__flt_limit_max_ngtv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_max_ngtv)), \
        T_case$((f64)(f64_limit_max_ngtv)) \
    ) pp_end \
)
#define __val__flt_limit_min_ngtv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_min_ngtv)), \
        T_case$((f64)(f64_limit_min_ngtv)) \
    ) pp_end \
)
#define __val__flt_limit_min_pstv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_min_pstv)), \
        T_case$((f64)(f64_limit_min_pstv)) \
    ) pp_end \
)
#define __val__flt_limit_max_pstv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_limit_max_pstv)), \
        T_case$((f64)(f64_limit_max_pstv)) \
    ) pp_end \
)

#define __val__flt_eps$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_eps)), \
        T_case$((f64)(f64_eps)) \
    ) pp_end \
)
#define __val__flt_nan$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_nan)), \
        T_case$((f64)(f64_nan)) \
    ) pp_end \
)
#define __val__flt_inf$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_inf)), \
        T_case$((f64)(f64_inf)) \
    ) pp_end \
)
#define __val__flt_inf_ngtv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_inf_ngtv)), \
        T_case$((f64)(f64_inf_ngtv)) \
    ) pp_end \
)
#define __val__flt_inf_pstv$(_T...) pp_expand( \
    T_switch$ pp_begin(_T)( \
        T_case$((f32)(f32_inf_pstv)), \
        T_case$((f64)(f64_inf_pstv)) \
    ) pp_end \
)

#define ____isNull(_p...) (as$(bool)((_p) == null))
#define ____isNonnull(_p...) (as$(bool)((_p) != null))

#define __step__ensureNonnull(_p...) ____ensureNonnull(pp_uniqTok(p), _p)
#define ____ensureNonnull(__p, _p...) local_({ \
    let_(__p, TypeOf(_p)) = _p; \
    local_return_((claim_assert_nonnull(__p), __p)); \
})

#define ____ref(_v...) (&(_v))
#define ____deref(_p...) (*ensureNonnull(_p))

#define ____ptrToInt(_p...) /* NOLINT(performance-no-int-to-ptr) */ ((usize)(_p))

#define __step__intToPtr$(...) __step__intToPtr$__emit(__step__intToPtr$__parse __VA_ARGS__)
#define __step__intToPtr$__parse(_P_T...) _P_T,
#define __step__intToPtr$__emit(...) ____intToPtr$(__VA_ARGS__)
#define ____intToPtr$(_P_T, _val...) /* NOLINT(performance-no-int-to-ptr) */ ((_P_T)(_val))

#define __step__ptrCast$(...) __step__ptrCast$__emit(__step__ptrCast$__parse __VA_ARGS__)
#define __step__ptrCast$__parse(_P_T...) _P_T,
#define __step__ptrCast$__emit(...) ____ptrCast$(__VA_ARGS__)
#define ____ptrCast$(_P_T, _val...) (as$(_P_T)(_val))

#define __step__ptrAlignCast$(...) __step__ptrAlignCast$__emit(__step__ptrAlignCast$__parse __VA_ARGS__)
#define __step__ptrAlignCast$__parse(_P_T...) _P_T,
#define __step__ptrAlignCast$__emit(...) ____ptrAlignCast$(__VA_ARGS__)
#define ____ptrAlignCast$(_P_T, _val...) $supress_cast_align( \
    ptrCast$((_P_T)(alignCast((alignOf$(DerefType$(_P_T)))(_val)))) \
)

#define ____ptrQualCast$(...) $supress_cast_qual(ptrCast$(__VA_ARGS__))
#define ____ptrAlignQualCast$(...) $supress_cast_align(ptrAlignCast$(__VA_ARGS__))

#define ____mutCast(_p...) ptrCast$((const TypeOfUnqual(*_p)*)(_p))
#define ____constCast(_p...) ptrQualCast$((TypeOfUnqual(*_p)*)(_p))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_CFG_INCLUDED */
