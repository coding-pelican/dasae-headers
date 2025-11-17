#define auto __comp_syn__auto
#define var  __comp_syn__var
#define let  __comp_syn__let

#define var_(_ident, _T...) __comp_syn__var_(_ident, _T)
#define let_(_ident, _T...) __comp_syn__let_(_ident, _T)

#define var_const __comp_syn__var_const
#define let_const __comp_syn__let_const

#define var_const_(_ident, _T...) __comp_syn__var_const_(_ident, _T)
#define let_const_(_ident, _T...) __comp_syn__let_const_(_ident, _T)

#if defined(__cplusplus)
/* In C++, auto is a standard keyword since C++11 */
#if (__cplusplus >= 201103L)
/* C++11 or later */
#undef auto
#define __comp_syn__var auto
#define __comp_syn__let auto const
#else
/* Pre-C++11 */
#define __comp_syn__auto __no_support__auto
#define __comp_syn__var  __no_support__var
#define __comp_syn__let  __no_support__let
#warning "C++11 or later is required for auto keyword support"
#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L)
/* C18 later supports the auto keyword */
#undef auto
#define __comp_syn__var auto
#define __comp_syn__let auto
#elif defined(__GNUC__) || defined(__GNUG__)
/* GCC or Clang - use __auto_type extension available in C99 and later */
#define __comp_syn__auto __auto_type
#define __comp_syn__var  __auto_type
#define __comp_syn__let  __auto_type const
#else
/* Compiler does not support auto type deduction */
#define __comp_syn__auto __no_support__auto
#define __comp_syn__var  __no_support__var
#define __comp_syn__let  __no_support__let
#warning "No support for auto type deduction in this compiler"
#endif

#define __comp_syn__var_(_ident, _T...) _T _ident
#define __comp_syn__let_(_ident, _T...) _T const _ident

#define __comp_syn__var_const const var*
#define __comp_syn__let_const let* const

#define __comp_syn__var_const_(_ident, _T...) const _T _ident
#define __comp_syn__let_const_(_ident, _T...) const _T const _ident

#define claim_assert_static(_Expr)           __step__static_assert(_Expr)
#define claim_assert_static_msg(_Expr, _msg) __step__static_assert_msg(_Expr, _msg)

#define static_assert(_Expr)           __step__static_assert(_Expr)
#define static_assert_msg(_Expr, _msg) __step__static_assert_msg(_Expr, _msg)

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define __step__static_assert(_Expr)           static_assert((_Expr), "Failed assertion")
#define __step__static_assert_msg(_Expr, _msg) static_assert((_Expr), "Failed assertion: " _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define __step__static_assert(_Expr)           _Static_assert((_Expr), "Failed assertion")
#define __step__static_assert_msg(_Expr, _msg) _Static_assert((_Expr), "Failed assertion: " _msg)
#else
/* Older versions - emulate static assert */
#include "pp.h"
#ifdef __COUNTER__
#define __step__static_assert(_Expr) \
    typedef int pp_join(__, __step__static_assert, __COUNTER__)[(_Expr) ? 1 : -1]
#define __step__static_assert_msg(_Expr, _msg) \
    typedef int pp_join(__, __step__static_assert_msg, __COUNTER__)[(_Expr) ? 1 : -1]
#else
#define __step__static_assert(_Expr) \
    typedef int pp_join(__, __step__static_assert, __LINE__)[(_Expr) ? 1 : -1]
#define __step__static_assert_msg(_Expr, _msg) \
    typedef int pp_join(__, __step__static_assert_msg, __LINE__)[(_Expr) ? 1 : -1]
#endif
#endif

#include <stdbool.h>
#include <stdint.h>

/// Unsigned integer

typedef uint8_t u8;
#define u8_limit     u8_limit_max
#define u8_limit_min (0)
#define u8_limit_max (UINT8_MAX)

typedef uint16_t u16;
#define u16_limit     u16_limit_max
#define u16_limit_min (0)
#define u16_limit_max (UINT16_MAX)

typedef uint32_t u32;
#define u32_limit     u32_limit_max
#define u32_limit_min (0)
#define u32_limit_max (UINT32_MAX)

typedef uint64_t u64;
#define u64_limit     u64_limit_max
#define u64_limit_min (0)
#define u64_limit_max (UINT64_MAX)

// typedef __uint128_t u128;
// #define u128_limit u128_limit_max
// #define u128_limit_min (0)
// #define u128_limit_max (UINT128_MAX)

typedef uintptr_t usize;
#define usize_limit     usize_limit_max
#define usize_limit_min (0)
#define usize_limit_max (UINTPTR_MAX)

/// Signed integer

typedef int8_t i8;
#define i8_limit_min      (INT8_MIN)
#define i8_limit_max      (INT8_MAX)
#define i8_limit_max_ngtv (-INT8_MAX)
#define i8_limit_min_ngtv (-INT8_MIN)
#define i8_limit_min_pstv (+INT8_MIN)
#define i8_limit_max_pstv (+INT8_MAX)

typedef int16_t i16;
#define i16_limit_min      (INT16_MIN)
#define i16_limit_max      (INT16_MAX)
#define i16_limit_max_ngtv (-INT16_MAX)
#define i16_limit_min_ngtv (-INT16_MIN)
#define i16_limit_min_pstv (+INT16_MIN)
#define i16_limit_max_pstv (+INT16_MAX)

typedef int32_t i32;
#define i32_limit_min      (INT32_MIN)
#define i32_limit_max      (INT32_MAX)
#define i32_limit_max_ngtv (-INT32_MAX)
#define i32_limit_min_ngtv (-INT32_MIN)
#define i32_limit_min_pstv (+INT32_MIN)
#define i32_limit_max_pstv (+INT32_MAX)

typedef int64_t i64;
#define i64_limit_min      (INT64_MIN)
#define i64_limit_max      (INT64_MAX)
#define i64_limit_max_ngtv (-INT64_MAX)
#define i64_limit_min_ngtv (-INT64_MIN)
#define i64_limit_min_pstv (+INT64_MIN)
#define i64_limit_max_pstv (+INT64_MAX)

// typedef __int128_t i128;
// #define i128_limit_min (INT128_MIN)
// #define i128_limit_max (INT128_MAX)

typedef intptr_t isize;
#define isize_limit_min      (INTPTR_MIN)
#define isize_limit_max      (INTPTR_MAX)
#define isize_limit_max_ngtv (-INTPTR_MAX)
#define isize_limit_min_ngtv (-INTPTR_MIN)
#define isize_limit_min_pstv (+INTPTR_MIN)
#define isize_limit_max_pstv (+INTPTR_MAX)

typedef float f32;
#define f32_limit_min      (FLT_MIN)
#define f32_limit_max      (FLT_MAX)
#define f32_limit_max_ngtv (-FLT_MAX)
#define f32_limit_min_ngtv (-FLT_MIN)
#define f32_limit_min_pstv (+FLT_MIN)
#define f32_limit_max_pstv (+FLT_MAX)
#define f32_eps            (FLT_EPSILON)
#define f32_nan            (NAN)
#define f32_inf            (INFINITY)
#define f32_inf_ngtv       (-INFINITY)
#define f32_inf_pstv       (+INFINITY)

typedef double f64;
#define f64_limit_min      (FLT_MIN)
#define f64_limit_max      (DBL_MAX)
#define f64_limit_max_ngtv (-DBL_MAX)
#define f64_limit_min_ngtv (-DBL_MIN)
#define f64_limit_min_pstv (+DBL_MIN)
#define f64_limit_max_pstv (+DBL_MAX)
#define f64_eps            (DBL_EPSILON)
#define f64_nan            (NAN)
#define f64_inf            (INFINITY)
#define f64_inf_ngtv       (-INFINITY)
#define f64_inf_pstv       (+INFINITY)

/* Type Information */
#define TypeOf(_Expr...) \
    /** \
     * @brief Get type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Type of the expression \
     */ \
    __typeof__(_Expr)

#define TypeOfUnqual(_Expr...) \
    /** \
     * @brief Get unqualified type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Unqualified type of the expression \
     */ \
    __typeof_unqual(_Expr)

#define isSameType$(T_lhs, T_rhs...) \
    /** \
     * @brief Compare two types for equality \
     * \
     * @param T_lhs First type to compare \
     * @param T_rhs Second type to compare \
     * @return bool True if types are the same \
     */ \
    __builtin_types_compatible_p(T_lhs, T_rhs)

#define as$
#define sizeOf$(_T...)  as$(usize)(sizeof(_T))
#define alignOf$(_T...) as$(usize)(__alignof(_T))
#define countOf$(_T...) (sizeOf$(_T) / sizeOf$(TypeOf((*as$(_T*)(0))[0])))
#define $maybe_unused   __attribute__((unused))

// clang-format off
/* if-else as expression block */
#define $local_label       __label__
#define expr_(T_Break...) ({ \
    $local_label __step_break, __step_unscope; \
    let __reserved_break = as$(T_Break*)((u8[_Generic(T_Break, \
        void: 0, \
        default: sizeOf$(T_Break) \
    )]){}); \
    $maybe_unused typedef TypeOf(*__reserved_break) BreakType; \
    $maybe_unused bool __has_broken = false; /* for integration with `eval_` */ \
    if (false) { __step_break: goto __step_unscope; } \
    /* do */
#define $unscoped_expr \
    /* while(false) */; \
__step_unscope: \
    _Generic(BreakType, \
        void: ({}), \
        default: reservedBreak() \
    ); \
})
#define reservedBreak()  (__reserved_break[0])
#define $break_(_Expr...) ({ \
    __builtin_memcpy( \
        as$(u8*)(__reserved_break), \
        as$(u8*)((TypeOf (*__reserved_break)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_break) \
    ); \
    __has_broken = true; \
    goto __step_break; \
})
// clang-format on

#define debug_assert(_Expr) \
    comp_inline__debug_assert(_Expr, #_Expr)
#if debug_comp_enabled
#define comp_inline__debug_assert(_Expr, _ExprStr) \
    $ignore_void((!!(_Expr)) || (debug_assert_fail(_ExprStr, __func__, __FILE__, __LINE__), 0))
$inline_always
$static fn_((debug_assert_fail(const char* expr, const char* func, const char* file, i32 line))(void)) {
    let_ignore = expr;
    let_ignore = func;
    let_ignore = file;
    let_ignore = line;
    __builtin_trap();
}
#else
#define comp_inline__debug_assert(_Expr, _ExprStr) ((void)(0))
#endif /* debug_comp_enabled */

#define claim_unreachable \
    (debug_assert(false), __builtin_unreachable())

#define genericIf$(_Expr, _T, _True, _False...) _Generic(TypeOfUnqual(_Expr), _T: _True, default: _False)
#define genericExprOrDefaultEvaledIfVoid$(_Expr, _T...) \
    genericIf$(_Expr, void, ({ $ignore_void(_Expr); make$(TypeOf(_T)); }), (_Expr))
#define genericExprOrDefaultIfNotT$(_Expr, _T...) \
    genericIf$(_Expr, _T, (_Expr), make$(_T))

// #define defaultEvaledIfVoid$(_T, _Expr...) Generic_match$( \
//     TypeOfUnqual(_Expr), \
//     Generic_pattern$(void)({ \
//         $ignore_void(_Expr); \
//         make$(TypeOf(_T)); \
//     }), \
//     Generic_fallback_(_Expr) \
// )

#define isUInt(_T...) _Generic( \
    _T, \
    u8: true, \
    u16: true, \
    u32: true, \
    u64: true, \
    default: false \
)
#define isIInt(_T...) _Generic( \
    _T, \
    i8: true, \
    i16: true, \
    i32: true, \
    i64: true, \
    default: false \
)
#undef isFlt
#define isFlt(_T...) _Generic( \
    _T, \
    f32: true, \
    f64: true, \
    default: false \
)
#undef isInt
#define isInt(_T) (isUInt(_T) || isIInt(_T))

#if UNUSED_CODE
#define math_rem(_lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    __lhs % __rhs; \
})
#endif /* UNUSED_CODE */

#define castSelfOrUInt$(_Expr...) _Generic( \
    TypeOfUnqual(_Expr), \
    u8: _Expr, \
    u16: _Expr, \
    u32: _Expr, \
    u64: _Expr, \
    default: (as$(usize)(_Expr)) \
)
#define castSelfOrIInt$(_Expr...) _Generic( \
    TypeOfUnqual(_Expr), \
    i8: _Expr, \
    i16: _Expr, \
    i32: _Expr, \
    i64: _Expr, \
    default: (as$(isize)(_Expr)) \
)
#define castSelfOrInt$(_Expr...) _Generic( \
    TypeOfUnqual(_Expr), \
    u8: _Expr, \
    u16: _Expr, \
    u32: _Expr, \
    u64: _Expr, \
    i8: _Expr, \
    i16: _Expr, \
    i32: _Expr, \
    i64: _Expr, \
    default: (as$(isize)(_Expr)) \
)
#define castSelfOrFlt$(_Expr...) _Generic( \
    TypeOfUnqual(_Expr), \
    f32: _Expr, \
    f64: _Expr, \
    default: (as$(f64)(_Expr)) \
)
#define castSelfOrT$() /* TODO */

#define math_rem(_lhs, _rhs...) ({ \
    let __lhs = _lhs; \
    let __rhs = _rhs; \
    typedef TypeOf(__lhs) BreakType; \
    claim_assert_static(isSameType$(BreakType, TypeOf(__rhs))); \
    expr_(BreakType) { \
        if (isInt(__lhs) && isInt(__rhs)) { \
            $break_(castSelfOrInt$(__lhs) % castSelfOrInt$(__rhs)); \
        } else if (isFlt(__lhs) && isFlt(__rhs)) { \
            $break_(__builtin_remainder(castSelfOrFlt$(__lhs), castSelfOrFlt$(__rhs))); \
        } else { \
            claim_unreachable; \
        } \
    } \
    $unscoped_expr; \
})

extern u8 getRhs$u8();
extern i8 getRhs$i8();
extern u32 getRhs$u32();
extern i32 getRhs$i32();
extern f32 getRhs$f32();
extern u64 getRhs$u64();
extern i64 getRhs$i64();
extern f64 getRhs$f64();
extern void io_stream_println(const char* fmt, ...);

void testGeneric8bits(void) {
    u8 l_u8 = 10;
    u8 r_u8 = getRhs$u8();
    let rem_u8 = math_rem(l_u8, r_u8);
    io_stream_println("{:uhh}", rem_u8);

    i8 l_i8 = 10;
    i8 r_i8 = getRhs$i8();
    let rem_i8 = math_rem(l_i8, r_i8);
    io_stream_println("{:ihh}", rem_i8);
}

void testGeneric32bits(void) {
    u32 l_u32 = 10;
    u32 r_u32 = getRhs$u32();
    let rem_u32 = math_rem(l_u32, r_u32);
    io_stream_println("{:u}", rem_u32);

    i32 l_i32 = 10;
    i32 r_i32 = getRhs$i32();
    let rem_i32 = math_rem(l_i32, r_i32);
    io_stream_println("{:i}", rem_i32);

    f32 l_f32 = 10.0f;
    f32 r_f32 = getRhs$f32();
    let rem_f32 = math_rem(l_f32, r_f32);
    io_stream_println("{:f}", rem_f32);
}

void testGeneric64bits(void) {
    u64 l_u64 = 10;
    u64 r_u64 = getRhs$u64();
    let rem_u64 = math_rem(l_u64, r_u64);
    io_stream_println("{:u}", rem_u64);

    i64 l_i64 = 10;
    i64 r_i64 = getRhs$i64();
    let rem_i64 = math_rem(l_i64, r_i64);
    io_stream_println("{:i}", rem_i64);

    f64 l_f64 = 10.0;
    f64 r_f64 = getRhs$f64();
    let rem_f64 = math_rem(l_f64, r_f64);
    io_stream_println("{:f}", rem_f64);
}

void testPure8bits(void) {
    u8 l_u8 = 10;
    u8 r_u8 = getRhs$u8();
    let rem_u8 = l_u8 % r_u8;
    io_stream_println("{:uhh}", rem_u8);

    i8 l_i8 = 10;
    i8 r_i8 = getRhs$i8();
    let rem_i8 = l_i8 % r_i8;
    io_stream_println("{:ihh}", rem_i8);
}

void testPure32bits(void) {
    u32 l_u32 = 10;
    u32 r_u32 = getRhs$u32();
    let rem_u32 = l_u32 % r_u32;
    io_stream_println("{:u}", rem_u32);

    i32 l_i32 = 10;
    i32 r_i32 = getRhs$i32();
    let rem_i32 = l_i32 % r_i32;
    io_stream_println("{:i}", rem_i32);

    f32 l_f32 = 10.0f;
    f32 r_f32 = getRhs$f32();
    let rem_f32 = __builtin_remainder(l_f32, r_f32);
    io_stream_println("{:f}", rem_f32);
}

void testPure64bits(void) {
    u64 l_u64 = 10;
    u64 r_u64 = getRhs$u64();
    let rem_u64 = l_u64 % r_u64;
    io_stream_println("{:u}", rem_u64);

    i64 l_i64 = 10;
    i64 r_i64 = getRhs$i64();
    let rem_i64 = l_i64 % r_i64;
    io_stream_println("{:i}", rem_i64);

    f64 l_f64 = 10.0;
    f64 r_f64 = getRhs$f64();
    let rem_f64 = __builtin_remainder(l_f64, r_f64);
    io_stream_println("{:f}", rem_f64);
}
