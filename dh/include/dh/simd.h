/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    simd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-08 (date of creation)
 * @updated 2025-11-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  simd
 *
 * @brief   Cross-platform SIMD vector operations
 * @details Provides vector types and operations inspired by Zig's @Vector,
 *          using compiler builtins and platform-specific intrinsics.
 */

#ifndef Vec__included
#define Vec__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/* TODO */

/*========== Includes =======================================================*/

#include "builtin.h"
#include "core.h"

/*========== Architecture-Specific Intrinsics ===============================*/

#if arch_is_x86 || arch_is_x86_64
#include <immintrin.h>
#elif arch_is_arm || arch_is_aarch64
#include <arm_neon.h>
#endif

/*========== Macros and Declarations ========================================*/

/// Vector type construction: Vec$(type, length)
/// Example: Vec$(4, f32) => __attribute__((vector_size(16))) f32
#define $Vec$(_N, _T...) TypeOf(_T __attribute__((vector_size(_N * sizeOf$(_T)))))
#define Vec$(_N, _T...) pp_join3($, Vec, _N, _T)
#define Vec$$(_N, _T...) $Vec$(_N, _T)

/// Type declaration helpers
#define T_use_Vec$(_N, _T) \
    T_impl_Vec$(_N, _T)
#define T_decl_Vec$(_N, _T) \
    typedef $Vec$(_N, _T) Vec$(_N, _T)
#define T_impl_Vec$(_N, _T) \
    typedef $Vec$(_N, _T) Vec$(_N, _T)

/// Get inner length and type of vector
#define Vec_innerN$(_T...) Vec_len$(_T)
#define Vec_InnerT$(_T...) TypeOf((*(as$(_T*)(null)))[0])

/*---------- Vector Initialization ------------------------------------------*/

/// Initialize vector from initial values
#define Vec_init(/*{_initial...}*/...) \
    __VA_ARGS__
#define Vec_init$(/*(_T){_initial...}*/...) __op__Vec_init$(__op__Vec_init$__parse __VA_ARGS__)
#define __op__Vec_init$(...) __op__Vec_init$__emit(__VA_ARGS__)
#define __op__Vec_init$__parse(_T...) _T,
#define __op__Vec_init$__emit(_T, _initial...) \
    lit$((_T)_initial)

/// Initialize vector with all lanes set to same value (splat)
#define Vec_splat$(/*(_T)(_val)*/... /*(_T)*/) __op__Vec_splat$(__op__Vec_splat$__parse __VA_ARGS__)
#define __op__Vec_splat$(...) __op__Vec_splat$__emit(__VA_ARGS__)
#define __op__Vec_splat$__parse(_T...) _T,
#define __op__Vec_splat$__emit(_T, _val...) ({ \
    var_(__ret, _T) = {}; \
    let __scalar = _val; \
    for (usize __i = 0; __i < Vec_len$(_T); ++__i) { \
        __ret[__i] = __scalar; \
    } \
    __ret; \
})

/// Initialize vector length inferred from initial values
#define Vec_from$(/*(_T){_initial...}*/...) __op__Vec_from$(__op__Vec_from$__parse __VA_ARGS__)
#define __op__Vec_from$(...) __op__Vec_from$__emit(__VA_ARGS__)
#define __op__Vec_from$__parse(_T...) _T,
#define __op__Vec_from$__emit(_T, _initial...) \
    lit$((Vec$$(sizeOf$(TypeOf((_T[])_initial)) / sizeOf$(_T), _T)) _initial)

/// Initialize vector from array
#define Vec_fromA$(/*(_T)(_a)*/...) __op__Vec_fromA$(__op__Vec_fromA$__parse __VA_ARGS__)
#define __op__Vec_fromA$(...) __op__Vec_fromA$__emit(__VA_ARGS__)
#define __op__Vec_fromA$__parse(_T...) _T,
#define __op__Vec_fromA$__emit(_T, _a...) \
    (*ptrAlignCast$((_T*)(&copy(_a))))
#define Vec_fromA(_a...) __op__Vec_fromA(_a)
#define __op__Vec_fromA(_a...) \
    Vec_fromA$((Vec$$(A_len$(TypeOf(_a)), A_InnerT$(TypeOf(_a))))(_a))

/// Store vector to array
#define Vec_toA$(/*(_T)(_vec)*/...) __op__Vec_toA$(__op__Vec_toA$__parse __VA_ARGS__)
#define __op__Vec_toA$(...) __op__Vec_toA$__emit(__VA_ARGS__)
#define __op__Vec_toA$__parse(_T...) _T,
#define __op__Vec_toA$__emit(_T, _vec...) \
    (*as$(_T*)(&copy(_vec)))
#define Vec_toA(_vec...) __op__Vec_toA(_vec)
#define __op__Vec_toA(_vec...) \
    Vec_toA$((A$$(Vec_len$(TypeOf(_vec)), Vec_InnerT$(TypeOf(_vec))))(_vec))

/// Concatenate two vectors sequentially
#define Vec_cat$(/*(_T)(_lhs, _rhs)*/... /*(_T)*/) \
    __op__Vec_cat$(__op__Vec_cat$__parseT __VA_ARGS__)
#define __op__Vec_cat$(...) \
    __op__Vec_cat$__emit(__VA_ARGS__)
#define __op__Vec_cat$__parseT(_T...) \
    _T, __op__Vec_cat$__parseParams
#define __op__Vec_cat$__parseParams(_lhs, _rhs...) \
    pp_uniqTok(lhs), pp_uniqTok(rhs), _lhs, _rhs
#define __op__Vec_cat$__emit(_T, __lhs, __rhs, _lhs, _rhs...) ({ \
    var __lhs = &(_lhs); \
    var __rhs = &(_rhs); \
    typedef union { \
        struct { \
            var_(lhs, TypeOf(*__lhs)); \
            var_(rhs, TypeOf(*__rhs)); \
        }; \
        var_(catted, _T); \
    } Catting; \
    lit$((Catting){ .lhs = *__lhs, .rhs = *__rhs }).catted; \
})
#define Vec_cat(_lhs, _rhs...) \
    Vec_cat$((Vec$$(Vec_len(_lhs) + Vec_len(_rhs), TypeOf(_lhs[0])))(_lhs, _rhs))

/// Get lane length of vector
#define Vec_len$(_T... /*(usize)*/) __op__Vec_len$(_T)
#define __op__Vec_len$(_T...) countOf$(_T)
#define Vec_len(_self /*: Vec$(_N, _T)*/... /*(usize)*/) __op__Vec_len(_self)
#define __op__Vec_len(_self...) Vec_len$(TypeOf(_self))

/// Get lane value pointer of vector
#define Vec_at(/*(_self: Vec$(_N, _T))[_idx]*/... /*(P(_T))*/) \
    __op__Vec_at(__op__Vec_at__parse __VA_ARGS__)
#define __op__Vec_at__parse(_self...) pp_uniqTok(self), pp_uniqTok(idx), _self,
#define __op__Vec_at(...) __op__Vec_at__emit(__VA_ARGS__)
#define __op__Vec_at__emit(__self, __idx, _self, _idx...) ({ \
    let_(__self, TypeOf(_self)*) = &_self; \
    typedef TypeOf((*__self)[0]) ScalarType; \
    let_(__idx, usize) = sizeOf$(TypeOf(u8 _idx)); \
    claim_assert_fmt(__idx < Vec_len(*__self), "Index out of bounds: idx({:uz}) >= len({:uz})", __idx, Vec_len(*__self)); \
    as$(ScalarType*)(__self) + __idx; \
})

/*---------- Arithmetic Operations ------------------------------------------*/

/// Binary operations (component-wise)
#define Vec_add(_a, _b) __op__Vec_add(_a, _b)
#define Vec_sub(_a, _b) __op__Vec_sub(_a, _b)
#define Vec_mul(_a, _b) __op__Vec_mul(_a, _b)
#define Vec_div(_a, _b) __op__Vec_div(_a, _b)
#define Vec_mod(_a, _b) __op__Vec_mod(_a, _b)

/// Unary operations
#define Vec_sgn(_a) __op__Vec_sgn(_a)
#define Vec_neg(_a) __op__Vec_neg(_a)
#define Vec_abs(_a) __op__Vec_abs(_a)

/*---------- Bitwise Operations ---------------------------------------------*/

#define Vec_not(_a) __op__Vec_not(_a)
#define Vec_shl(_a, _b) __op__Vec_shl(_a, _b)
#define Vec_shr(_a, _b) __op__Vec_shr(_a, _b)
#define Vec_and(_a, _b) __op__Vec_and(_a, _b)
#define Vec_xor(_a, _b) __op__Vec_xor(_a, _b)
#define Vec_or(_a, _b) __op__Vec_or(_a, _b)

/*---------- Comparison Operations ------------------------------------------*/

/// Returns vector of bool/int (0 or -1 for each lane)
#define Vec_cmp(_a, _b) __op__Vec_cmp(_a, _b)
#define Vec_eq(_a, _b) __op__Vec_eq(_a, _b)
#define Vec_ne(_a, _b) __op__Vec_ne(_a, _b)
#define Vec_lt(_a, _b) __op__Vec_lt(_a, _b)
#define Vec_le(_a, _b) __op__Vec_le(_a, _b)
#define Vec_gt(_a, _b) __op__Vec_gt(_a, _b)
#define Vec_ge(_a, _b) __op__Vec_ge(_a, _b)

/*---------- Min/Max Operations ---------------------------------------------*/

#define Vec_min(_a, _b) __op__Vec_min(_a, _b)
#define Vec_max(_a, _b) __op__Vec_max(_a, _b)
#define Vec_clamp(_val, _min, _max) __op__Vec_clamp(_val, _min, _max)
#define Vec_wrapU(_val, _min, _max) __op__Vec_wrapU(_val, _min, _max)
#define Vec_wrapI(_val, _min, _max) __op__Vec_wrapI(_val, _min, _max)
#define Vec_wrapF(_val, _min, _max) __op__Vec_wrapF(_val, _min, _max)

/*---------- Reduction Operations -------------------------------------------*/

/// Reduce vector to scalar
#define Vec_reduceAdd(_vec) __op__Vec_reduceAdd(_vec)
#define Vec_reduceMul(_vec) __op__Vec_reduceMul(_vec)
#define Vec_reduceMin(_vec) __op__Vec_reduceMin(_vec)
#define Vec_reduceMax(_vec) __op__Vec_reduceMax(_vec)
#define Vec_reduceAnd(_vec) __op__Vec_reduceAnd(_vec)
#define Vec_reduceOr(_vec) __op__Vec_reduceOr(_vec)
#define Vec_reduceXor(_vec) __op__Vec_reduceXor(_vec)

/*---------- Shuffle and Select Operations ----------------------------------*/

/// Shuffle/swizzle vector lanes
#define Vec_shuffle$(_VT, _a, _b, ...) __op__Vec_shuffle$(_VT, _a, _b, __VA_ARGS__)
#define Vec_shuffle(_a, _b, ...) Vec_shuffle$(TypeOfUnqual(_a), _a, _b, __VA_ARGS__)

/// Select lanes based on mask (mask ? b : a)
#define Vec_select(_mask, _a, _b) __op__Vec_select(_mask, _a, _b)

/*---------- Mathematical Functions -----------------------------------------*/

/// Floating-point operations (requires math library or intrinsics)
#define Vec_sqrt(_a) __op__Vec_sqrt(_a)
#define Vec_floor(_a) __op__Vec_floor(_a)
#define Vec_ceil(_a) __op__Vec_ceil(_a)
#define Vec_round(_a) __op__Vec_round(_a)
#define Vec_trunc(_a) __op__Vec_trunc(_a)

/// Fused multiply-add: (a * b) + c
#define Vec_fma(_a, _b, _c) __op__Vec_fma(_a, _b, _c)

/*---------- Type Conversions -----------------------------------------------*/

/// Convert vector element types
#define Vec_cast$(_VT, _vec) __op__Vec_cast$(_VT, _vec)

/*---------- Horizontal Operations ------------------------------------------*/

/// Horizontal add (sum adjacent pairs)
#define Vec_hAdd(_a, _b) __op__Vec_hAdd(_a, _b)

/// Horizontal sub (subtract adjacent pairs)
#define Vec_hSub(_a, _b) __op__Vec_hSub(_a, _b)

/*---------- Mask Operations ------------------------------------------------*/

/// Convert comparison result to bitmask
#define Vec_toBitMask(_vec) __op__Vec_toBitMask(_vec)

/// Check if all lanes are true
#define Vec_allTrue(_vec) __op__Vec_allTrue(_vec)

/// Check if any lane is true
#define Vec_anyTrue(_vec) __op__Vec_anyTrue(_vec)

/*========== Macros and Definitions =========================================*/

/*---------- Initialization -------------------------------------------------*/
#if UNUSED_CODE
#define __op__Vec_splat$(_VT, _val) ({ \
    var_(__result, _VT); \
    let __scalar = (_val); \
    for (usize __i = 0; __i < sizeof(_VT) / sizeof(__scalar); ++__i) { \
        __result[__i] = __scalar; \
    } \
    __result; \
})

#define __op__Vec_fromArray$(_VT, _p_arr) ({ \
    var_(__result, _VT); \
    __builtin_memcpy(&__result, _p_arr, sizeof(_VT)); \
    __result; \
})

#define __op__Vec_from$(_len, _T, ...) ({ \
    typedef Vec$(_len, _T) Vec; \
    let __arr = (_T[]){ __VA_ARGS__ }; \
    Vec_fromArray$(Vec, __arr); \
})

#define __op__Vec_toArray(_p_arr, _vec) ({ \
    __builtin_memcpy(_p_arr, &(__vec), sizeof(__vec)); \
})

/*---------- Lane Access ----------------------------------------------------*/

#define __op__Vec_getLane(_vec, _idx) ((_vec)[_idx])

#define __op__Vec_setLane(_vec, _idx, _val) ({ \
    var_(__result, TypeOfUnqual(_vec)) = __vec; \
    __result[_idx] = (__val); \
    __result; \
})
#endif /* UNUSED_CODE */

/*---------- Arithmetic Operations ------------------------------------------*/

#define __op__Vec_add(_a, _b...) ((_a) + (_b))
#define __op__Vec_sub(_a, _b...) ((_a) - (_b))
#define __op__Vec_mul(_a, _b...) ((_a) * (_b))
#define __op__Vec_div(_a, _b...) ((_a) / (_b))
#define __op__Vec_mod(_a, _b...) ((_a) % (_b))
#define __op__Vec_rem(_a, _b...) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_remainder(vec[__i], _b[__i]); \
    } \
    result; \
})

#define __op__Vec_sgn(_a...) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = vec[__i] < 0 ? -1 : (0 < vec[__i] ? 1 : 0); \
    } \
    result; \
})

#define __op__Vec_neg(_a) (-(_a))

#if arch_is_x86 || arch_is_x86_64
#define __op__Vec_abs(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32)) { \
        if (sizeof(VecType) == 16) { \
            *((__m128*)&result) = _mm_andnot_ps(_mm_set1_ps(-0.0f), *((__m128*)&(_a))); \
        } else if (sizeof(VecType) == 32) { \
            *((__m256*)&result) = _mm256_andnot_ps(_mm256_set1_ps(-0.0f), *((__m256*)&(_a))); \
        } else { \
            result = (_a) < 0 ? -(_a) : (_a); \
        } \
    } else if (Type_eq$(ScalarType, f64)) { \
        if (sizeof(VecType) == 16) { \
            *((__m128d*)&result) = _mm_andnot_pd(_mm_set1_pd(-0.0), *((__m128d*)&(_a))); \
        } else if (sizeof(VecType) == 32) { \
            *((__m256d*)&result) = _mm256_andnot_pd(_mm256_set1_pd(-0.0), *((__m256d*)&(_a))); \
        } else { \
            result = (_a) < 0 ? -(_a) : (_a); \
        } \
    } else { \
        result = (_a) < 0 ? -(_a) : (_a); \
    } \
    result; \
})
#else
#define __op__Vec_abs(_a) ((_a) < 0 ? -(_a) : (_a))
#endif

/*---------- Bitwise Operations ---------------------------------------------*/

#define __op__Vec_not(_a) (~(_a))
#define __op__Vec_shl(_a, _b) ((_a) << (_b))
#define __op__Vec_shr(_a, _b) ((_a) >> (_b))
#define __op__Vec_and(_a, _b) ((_a) & (_b))
#define __op__Vec_xor(_a, _b) ((_a) ^ (_b))
#define __op__Vec_or(_a, _b) ((_a) | (_b))

/*---------- Comparison Operations ------------------------------------------*/

#define __op__Vec_eq(_a, _b) ((_a) == (_b))
#define __op__Vec_ne(_a, _b) ((_a) != (_b))
#define __op__Vec_lt(_a, _b) ((_a) < (_b))
#define __op__Vec_le(_a, _b) ((_a) <= (_b))
#define __op__Vec_gt(_a, _b) ((_a) > (_b))
#define __op__Vec_ge(_a, _b) ((_a) >= (_b))

/*---------- Min/Max Operations ---------------------------------------------*/

#define __op__Vec_min(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(vec_a[0]); ++__i) { \
        result[__i] = vec_a[__i] < vec_b[__i] ? vec_a[__i] : vec_b[__i]; \
    } \
    result; \
})

#define __op__Vec_max(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(vec_a[0]); ++__i) { \
        result[__i] = vec_a[__i] > vec_b[__i] ? vec_a[__i] : vec_b[__i]; \
    } \
    result; \
})

#define __op__Vec_clamp(_val, _min, _max) \
    Vec_min(Vec_max(_val, _min), _max)

/*---------- Reduction Operations -------------------------------------------*/

#define __op__Vec_reduceAdd(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    var_(__sum, ScalarType) = 0; \
    let __vec = (_vec); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        __sum += __vec[__i]; \
    } \
    __sum; \
})

#define __op__Vec_reduceMul(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    var_(__prod, ScalarType) = 1; \
    let __vec = (_vec); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        __prod *= __vec[__i]; \
    } \
    __prod; \
})

#define __op__Vec_reduceMin(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let __vec = (_vec); \
    var_(__min_val, ScalarType) = __vec[0]; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (__vec[__i] < __min_val) __min_val = __vec[__i]; \
    } \
    __min_val; \
})

#define __op__Vec_reduceMax(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let __vec = (_vec); \
    var_(__max_val, ScalarType) = __vec[0]; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (__vec[__i] > __max_val) __max_val = __vec[__i]; \
    } \
    __max_val; \
})

#define __op__Vec_reduceAnd(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let __vec = (_vec); \
    var_(__result, ScalarType) = __vec[0]; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        __result &= __vec[__i]; \
    } \
    __result; \
})

#define __op__Vec_reduceOr(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let __vec = (_vec); \
    var_(__result, ScalarType) = __vec[0]; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        __result |= __vec[__i]; \
    } \
    __result; \
})

#define __op__Vec_reduceXor(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let __vec = (_vec); \
    var_(__result, ScalarType) = __vec[0]; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        __result ^= __vec[__i]; \
    } \
    __result; \
})

/*---------- Shuffle and Select Operations ----------------------------------*/

#define __op__Vec_shuffle$(_VT, _a, _b, ...) \
    __builtin_shufflevector(_a, _b, __VA_ARGS__)

#define __op__Vec_select(_mask, _a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    var_(__result, VecType); \
    let __m = (_mask); \
    let __vec_a = (_a); \
    let __vec_b = (_b); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(__vec_a[0]); ++__i) { \
        __result[__i] = __m[__i] ? __vec_b[__i] : __vec_a[__i]; \
    } \
    __result; \
})

/*---------- Mathematical Functions -----------------------------------------*/

#if arch_is_x86 || arch_is_x86_64
#define __op__Vec_sqrt(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32)) { \
        if (sizeof(VecType) == 16) { \
            *((__m128*)&result) = _mm_sqrt_ps(*((__m128*)&(_a))); \
        } else if (sizeof(VecType) == 32) { \
            *((__m256*)&result) = _mm256_sqrt_ps(*((__m256*)&(_a))); \
        } else { \
            let vec = (_a); \
            for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
                result[__i] = __builtin_sqrtf(vec[__i]); \
            } \
        } \
    } else if (Type_eq$(ScalarType, f64)) { \
        if (sizeof(VecType) == 16) { \
            *((__m128d*)&result) = _mm_sqrt_pd(*((__m128d*)&(_a))); \
        } else if (sizeof(VecType) == 32) { \
            *((__m256d*)&result) = _mm256_sqrt_pd(*((__m256d*)&(_a))); \
        } else { \
            let vec = (_a); \
            for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
                result[__i] = __builtin_sqrt(vec[__i]); \
            } \
        } \
    } else { \
        let vec = (_a); \
        for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
            result[__i] = __builtin_sqrt(vec[__i]); \
        } \
    } \
    result; \
})
#else
#define __op__Vec_sqrt(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (Type_eq$(ScalarType, f32)) { \
            result[__i] = __builtin_sqrtf(vec[__i]); \
        } else if (Type_eq$(ScalarType, f64)) { \
            result[__i] = __builtin_sqrt(vec[__i]); \
        } \
    } \
    result; \
})
#endif

#if arch_is_x86_64 && arch_has_sse4_1
#define __op__Vec_floor(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = _mm_floor_ps(*((__m128*)&(_a))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        *((__m128d*)&result) = _mm_floor_pd(*((__m128d*)&(_a))); \
    } else { \
        let vec = (_a); \
        for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
            result[__i] = __builtin_floor(vec[__i]); \
        } \
    } \
    result; \
})
#else
#define __op__Vec_floor(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_floor(vec[__i]); \
    } \
    result; \
})
#endif

#define __op__Vec_ceil(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_ceil(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_round(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_round(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_trunc(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_trunc(vec[__i]); \
    } \
    result; \
})

#if arch_is_x86_64 && arch_has_fma
#define __op__Vec_fma(_a, _b, _c) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = _mm_fmadd_ps(*((__m128*)&(_a)), *((__m128*)&(_b)), *((__m128*)&(_c))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        *((__m128d*)&result) = _mm_fmadd_pd(*((__m128d*)&(_a)), *((__m128d*)&(_b)), *((__m128d*)&(_c))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        *((__m256*)&result) = _mm256_fmadd_ps(*((__m256*)&(_a)), *((__m256*)&(_b)), *((__m256*)&(_c))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32) { \
        *((__m256d*)&result) = _mm256_fmadd_pd(*((__m256d*)&(_a)), *((__m256d*)&(_b)), *((__m256d*)&(_c))); \
    } else { \
        result = (_a) * (_b) + (_c); \
    } \
    result; \
})
#else
#define __op__Vec_fma(_a, _b, _c) ((_a) * (_b) + (_c))
#endif

/*---------- Type Conversions -----------------------------------------------*/

#define __op__Vec_cast$(_VT, _vec) ({ \
    typedef _VT TargetVec; \
    typedef TypeOfUnqual(_vec) SourceVec; \
    typedef TypeOfUnqual((*(_VT*)0)[0]) TargetScalar; \
    typedef TypeOfUnqual((_vec)[0]) SourceScalar; \
    var_(result, TargetVec); \
    let vec = (_vec); \
    for (usize __i = 0; __i < sizeof(SourceVec) / sizeof(SourceScalar); ++__i) { \
        result[__i] = (TargetScalar)vec[__i]; \
    } \
    result; \
})

/*---------- Horizontal Operations ------------------------------------------*/

#if arch_is_x86 || arch_is_x86_64
#if arch_has_ssse3
#define __op__Vec_hAdd(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = _mm_hadd_ps(*((__m128*)&(_a)), *((__m128*)&(_b))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        *((__m128d*)&result) = _mm_hadd_pd(*((__m128d*)&(_a)), *((__m128d*)&(_b))); \
    } else { \
        let vec_a = (_a); \
        let vec_b = (_b); \
        usize len = sizeof(VecType) / sizeof(ScalarType); \
        for (usize __i = 0; __i < len / 2; ++__i) { \
            result[__i] = vec_a[2 * __i] + vec_a[2 * __i + 1]; \
            result[__i + len / 2] = vec_b[2 * __i] + vec_b[2 * __i + 1]; \
        } \
    } \
    result; \
})
#else
#define __op__Vec_hAdd(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        result[__i] = vec_a[2 * __i] + vec_a[2 * __i + 1]; \
        result[__i + len / 2] = vec_b[2 * __i] + vec_b[2 * __i + 1]; \
    } \
    result; \
})
#endif
#else
#define __op__Vec_hAdd(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        result[__i] = vec_a[2 * __i] + vec_a[2 * __i + 1]; \
        result[__i + len / 2] = vec_b[2 * __i] + vec_b[2 * __i + 1]; \
    } \
    result; \
})
#endif

#define __op__Vec_hSub(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        result[__i] = vec_a[2 * __i] - vec_a[2 * __i + 1]; \
        result[__i + len / 2] = vec_b[2 * __i] - vec_b[2 * __i + 1]; \
    } \
    result; \
})

/*---------- Mask Operations ------------------------------------------------*/

#define __op__Vec_toBitMask(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let vec = (_vec); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    var_(mask, usize) = 0; \
    for (usize __i = 0; __i < len; ++__i) { \
        if (vec[__i]) mask |= (1ULL << __i); \
    } \
    mask; \
})

#define __op__Vec_allTrue(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let vec = (_vec); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    var_(all, bool) = true; \
    for (usize __i = 0; __i < len; ++__i) { \
        if (!vec[__i]) { \
            all = false; \
            break; \
        } \
    } \
    all; \
})

#define __op__Vec_anyTrue(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let vec = (_vec); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    var_(any, bool) = false; \
    for (usize __i = 0; __i < len; ++__i) { \
        if (vec[__i]) { \
            any = true; \
            break; \
        } \
    } \
    any; \
})

/*========== Convenience Type Definitions ===================================*/

/* Common vector types */
T_use_Vec$(1, f32); /* Vec$1$f32  - 1x float32  */
T_use_Vec$(2, f32); /* Vec$2$f32  - 2x float32  */
T_use_Vec$(4, f32); /* Vec$4$f32  - 4x float32  */
T_use_Vec$(8, f32); /* Vec$8$f32  - 8x float32  */
T_use_Vec$(16, f32); /* Vec$16$f32 - 16x float32 */

T_use_Vec$(1, f64); /* Vec$1$f64  - 1x float64  */
T_use_Vec$(2, f64); /* Vec$2$f64  - 2x float64  */
T_use_Vec$(4, f64); /* Vec$4$f64  - 4x float64  */
T_use_Vec$(8, f64); /* Vec$8$f64  - 8x float64  */

T_use_Vec$(1, i32); /* Vec$1$i32  - 1x int32    */
T_use_Vec$(2, i32); /* Vec$2$i32  - 2x int32    */
T_use_Vec$(4, i32); /* Vec$4$i32  - 4x int32    */
T_use_Vec$(8, i32); /* Vec$8$i32  - 8x int32    */
T_use_Vec$(16, i32); /* Vec$16$i32 - 16x int32   */

T_use_Vec$(1, i64); /* Vec$1$i64  - 1x int64    */
T_use_Vec$(2, i64); /* Vec$2$i64  - 2x int64    */
T_use_Vec$(4, i64); /* Vec$4$i64  - 4x int64    */
T_use_Vec$(8, i64); /* Vec$8$i64  - 8x int64    */

T_use_Vec$(1, u32); /* Vec$1$u32  - 1x uint32   */
T_use_Vec$(2, u32); /* Vec$2$u32  - 2x uint32   */
T_use_Vec$(4, u32); /* Vec$4$u32  - 4x uint32   */
T_use_Vec$(8, u32); /* Vec$8$u32  - 8x uint32   */
T_use_Vec$(16, u32); /* Vec$16$u32 - 16x uint32  */

T_use_Vec$(1, u64); /* Vec$1$u64  - 1x uint64   */
T_use_Vec$(2, u64); /* Vec$2$u64  - 2x uint64   */
T_use_Vec$(4, u64); /* Vec$4$u64  - 4x uint64   */
T_use_Vec$(8, u64); /* Vec$8$u64  - 8x uint64   */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Vec__included */





/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Vec_opt.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-08 (date of creation)
 * @updated 2025-11-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  simd
 *
 * @brief   Optimized SIMD operations with platform-specific intrinsics
 * @details High-performance vector operations optimized for x86/x64 (SSE/AVX)
 *          and ARM (NEON), with fallback to generic implementations.
 */

#ifndef Vec_opt__included
#define Vec_opt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Platform-Specific Optimized Operations =========================*/

/*---------- Optimized Load/Store -------------------------------------------*/

/// Aligned load (requires 16/32-byte alignment)
#define Vec_loadAligned(_p_arr) __op__Vec_loadAligned(_p_arr)

/// Unaligned load
#define Vec_loadUnaligned(_p_arr) __op__Vec_loadUnaligned(_p_arr)

/// Aligned store (requires 16/32-byte alignment)
#define Vec_storeAligned(_p_arr, _vec) __op__Vec_storeAligned(_p_arr, _vec)

/// Unaligned store
#define Vec_storeUnaligned(_p_arr, _vec) __op__Vec_storeUnaligned(_p_arr, _vec)

/// Stream store (non-temporal, bypasses cache)
#define Vec_storeStream(_p_arr, _vec) __op__Vec_storeStream(_p_arr, _vec)

/*---------- Optimized Arithmetic -------------------------------------------*/

/// Reciprocal (1/x) - fast approximation
#define Vec_rcp(_a) __op__Vec_rcp(_a)

/// Reciprocal square root (1/sqrt(x)) - fast approximation
#define Vec_rsqrt(_a) __op__Vec_rsqrt(_a)

/*---------- Optimized Comparisons ------------------------------------------*/

/// Move mask from vector comparison to integer
#define Vec_moveMask(_vec) __op__Vec_moveMask(_vec)

/*---------- Cache Control --------------------------------------------------*/

/// Prefetch data into cache
#define Vec_prefetch(_p_addr, _locality) __op__Vec_prefetch(_p_addr, _locality)

/*---------- Special Operations ---------------------------------------------*/

/// Dot product (optimized for SSE4.1+)
#define Vec_dot$(_VT, _a, _b) __op__Vec_dot$(_VT, _a, _b)
#define Vec_dot(_a, _b) Vec_dot$(TypeOfUnqual(_a), _a, _b)

/// Cross product (3D vectors)
#define Vec_cross(_a, _b) __op__Vec_cross(_a, _b)

/// Matrix-vector multiply (4x4 matrix * 4D vector)
#define Vec_matMul4x4(_mat, _vec) __op__Vec_matMul4x4(_mat, _vec)

/*========== Optimized Implementations ======================================*/

/*---------- Load/Store -----------------------------------------------------*/

#if arch_is_x86 || arch_is_x86_64

#define __op__Vec_loadAligned(_p_arr) ({ \
    typedef TypeOfUnqual(*(_p_arr)) ScalarType; \
    typedef TypeOfUnqual((_p_arr)) PtrType; \
    var_(result, TypeOf(*(PtrType)0)); \
    if (Type_eq$(ScalarType, f32) && sizeof(result) == 16) { \
        *((__m128*)&result) = _mm_load_ps((f32 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(result) == 16) { \
        *((__m128d*)&result) = _mm_load_pd((f64 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(result) == 32) { \
        *((__m256*)&result) = _mm256_load_ps((f32 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(result) == 32) { \
        *((__m256d*)&result) = _mm256_load_pd((f64 const*)(_p_arr)); \
    } else { \
        __builtin_memcpy(&result, _p_arr, sizeof(result)); \
    } \
    result; \
})

#define __op__Vec_loadUnaligned(_p_arr) ({ \
    typedef TypeOfUnqual(*(_p_arr)) ScalarType; \
    typedef TypeOfUnqual((_p_arr)) PtrType; \
    var_(result, TypeOf(*(PtrType)0)); \
    if (Type_eq$(ScalarType, f32) && sizeof(result) == 16) { \
        *((__m128*)&result) = _mm_loadu_ps((f32 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(result) == 16) { \
        *((__m128d*)&result) = _mm_loadu_pd((f64 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(result) == 32) { \
        *((__m256*)&result) = _mm256_loadu_ps((f32 const*)(_p_arr)); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(result) == 32) { \
        *((__m256d*)&result) = _mm256_loadu_pd((f64 const*)(_p_arr)); \
    } else { \
        __builtin_memcpy(&result, _p_arr, sizeof(result)); \
    } \
    result; \
})

#define __op__Vec_storeAligned(_p_arr, _vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        _mm_store_ps((f32*)(_p_arr), *((__m128*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        _mm_store_pd((f64*)(_p_arr), *((__m128d*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        _mm256_store_ps((f32*)(_p_arr), *((__m256*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32) { \
        _mm256_store_pd((f64*)(_p_arr), *((__m256d*)&(_vec))); \
    } else { \
        __builtin_memcpy(_p_arr, &(_vec), sizeof(_vec)); \
    } \
})

#define __op__Vec_storeUnaligned(_p_arr, _vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        _mm_storeu_ps((f32*)(_p_arr), *((__m128*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        _mm_storeu_pd((f64*)(_p_arr), *((__m128d*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        _mm256_storeu_ps((f32*)(_p_arr), *((__m256*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32) { \
        _mm256_storeu_pd((f64*)(_p_arr), *((__m256d*)&(_vec))); \
    } else { \
        __builtin_memcpy(_p_arr, &(_vec), sizeof(_vec)); \
    } \
})

#define __op__Vec_storeStream(_p_arr, _vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        _mm_stream_ps((f32*)(_p_arr), *((__m128*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        _mm_stream_pd((f64*)(_p_arr), *((__m128d*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        _mm256_stream_ps((f32*)(_p_arr), *((__m256*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32) { \
        _mm256_stream_pd((f64*)(_p_arr), *((__m256d*)&(_vec))); \
    } else { \
        __builtin_memcpy(_p_arr, &(_vec), sizeof(_vec)); \
    } \
})

#elif arch_is_arm || arch_is_aarch64

#define __op__Vec_loadAligned(_p_arr) \
    Vec_fromArray$(TypeOfUnqual(*(_p_arr)), _p_arr)

#define __op__Vec_loadUnaligned(_p_arr) \
    Vec_fromArray$(TypeOfUnqual(*(_p_arr)), _p_arr)

#define __op__Vec_storeAligned(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#define __op__Vec_storeUnaligned(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#define __op__Vec_storeStream(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#else

#define __op__Vec_loadAligned(_p_arr) \
    Vec_fromArray$(TypeOfUnqual(*(_p_arr)), _p_arr)

#define __op__Vec_loadUnaligned(_p_arr) \
    Vec_fromArray$(TypeOfUnqual(*(_p_arr)), _p_arr)

#define __op__Vec_storeAligned(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#define __op__Vec_storeUnaligned(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#define __op__Vec_storeStream(_p_arr, _vec) \
    Vec_toArray(_p_arr, _vec)

#endif

/*---------- Reciprocal and RSQRT -------------------------------------------*/

#if arch_is_x86 || arch_is_x86_64

#define __op__Vec_rcp(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = _mm_rcp_ps(*((__m128*)&(_a))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        *((__m256*)&result) = _mm256_rcp_ps(*((__m256*)&(_a))); \
    } else { \
        result = Vec_div(Vec_splat$(VecType, 1.0), _a); \
    } \
    result; \
})

#define __op__Vec_rsqrt(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = _mm_rsqrt_ps(*((__m128*)&(_a))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        *((__m256*)&result) = _mm256_rsqrt_ps(*((__m256*)&(_a))); \
    } else { \
        result = Vec_div(Vec_splat$(VecType, 1.0), Vec_sqrt(_a)); \
    } \
    result; \
})

#elif arch_is_aarch64

#define __op__Vec_rcp(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = vrecpeq_f32(*((__m128*)&(_a))); \
    } else { \
        result = Vec_div(Vec_splat$(VecType, 1.0), _a); \
    } \
    result; \
})

#define __op__Vec_rsqrt(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        *((__m128*)&result) = vrsqrteq_f32(*((__m128*)&(_a))); \
    } else { \
        result = Vec_div(Vec_splat$(VecType, 1.0), Vec_sqrt(_a)); \
    } \
    result; \
})

#else

#define __op__Vec_rcp(_a) \
    Vec_div(Vec_splat$(TypeOfUnqual(_a), 1.0), _a)

#define __op__Vec_rsqrt(_a) \
    Vec_div(Vec_splat$(TypeOfUnqual(_a), 1.0), Vec_sqrt(_a))

#endif

/*---------- Move Mask ------------------------------------------------------*/

#if arch_is_x86 || arch_is_x86_64

#define __op__Vec_moveMask(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    var_(mask, i32); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        mask = _mm_movemask_ps(*((__m128*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 16) { \
        mask = _mm_movemask_pd(*((__m128d*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32) { \
        mask = _mm256_movemask_ps(*((__m256*)&(_vec))); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32) { \
        mask = _mm256_movemask_pd(*((__m256d*)&(_vec))); \
    } else { \
        mask = Vec_toBitMask(_vec); \
    } \
    mask; \
})

#else

#define __op__Vec_moveMask(_vec) Vec_toBitMask(_vec)

#endif

/*---------- Prefetch -------------------------------------------------------*/

#define __op__Vec_prefetch(_p_addr, _locality) \
    __builtin_prefetch(_p_addr, 0, _locality)

/*---------- Dot Product ----------------------------------------------------*/

#if arch_is_x86_64 && arch_has_sse4_1

#define __op__Vec_dot$(_VT, _a, _b) ({ \
    typedef _VT VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, ScalarType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        __m128 temp = _mm_dp_ps(*((__m128*)&(_a)), *((__m128*)&(_b)), 0xF1); \
        result = _mm_cvtss_f32(temp); \
    } else { \
        let prod = Vec_mul(_a, _b); \
        result = Vec_reduceAdd(prod); \
    } \
    result; \
})

#else

#define __op__Vec_dot$(_VT, _a, _b) ({ \
    let prod = Vec_mul(_a, _b); \
    Vec_reduceAdd(prod); \
})

#endif

/*---------- Cross Product --------------------------------------------------*/

#define __op__Vec_cross(_a, _b) ({ \
    typedef TypeOf(_a) VecType; \
    claim_assert_static(Vec_innerN$(VecType) == 4); \
    /* cross = (a.yzx * b.zxy) - (a.zxy * b.yzx) */ \
    let a_yzx = Vec_shuffle(_a, _a, 1, 2, 0, 3); \
    let a_zxy = Vec_shuffle(_a, _a, 2, 0, 1, 3); \
    let b_yzx = Vec_shuffle(_b, _b, 1, 2, 0, 3); \
    let b_zxy = Vec_shuffle(_b, _b, 2, 0, 1, 3); \
    Vec_sub(Vec_mul(a_yzx, b_zxy), Vec_mul(a_zxy, b_yzx)); \
})

/*---------- Matrix-Vector Multiply -----------------------------------------*/

#if arch_is_x86 || arch_is_x86_64

#define __op__Vec_matMul4x4(_mat, _vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 16) { \
        __m128 v = *((__m128*)&(_vec)); \
        __m128 r0 = _mm_mul_ps(*((__m128*)&(_mat)[0]), _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0))); \
        __m128 r1 = _mm_mul_ps(*((__m128*)&(_mat)[1]), _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1))); \
        __m128 r2 = _mm_mul_ps(*((__m128*)&(_mat)[2]), _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2))); \
        __m128 r3 = _mm_mul_ps(*((__m128*)&(_mat)[3]), _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3))); \
        *((__m128*)&result) = _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)); \
    } else { \
        for (usize __i = 0; __i < 4; ++__i) { \
            result[__i] = 0; \
            for (usize j = 0; j < 4; ++j) { \
                result[__i] += (_mat)[j][__i] * (_vec)[j]; \
            } \
        } \
    } \
    result; \
})

#else

#define __op__Vec_matMul4x4(_mat, _vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    var_(result, VecType); \
    for (usize __i = 0; __i < 4; ++__i) { \
        result[__i] = 0; \
        for (usize j = 0; j < 4; ++j) { \
            result[__i] += (_mat)[j][__i] * (_vec)[j]; \
        } \
    } \
    result; \
})

#endif

/*========== High-Performance Patterns ======================================*/

/// Alignment attribute for SIMD vectors
#if arch_is_x86 || arch_is_x86_64
#define Vec_aligned16 __attribute__((aligned(16)))
#define Vec_aligned32 __attribute__((aligned(32)))
#define Vec_aligned64 __attribute__((aligned(64)))
#elif arch_is_arm || arch_is_aarch64
#define Vec_aligned16 __attribute__((aligned(16)))
#define Vec_aligned32 __attribute__((aligned(32)))
#define Vec_aligned64 __attribute__((aligned(64)))
#else
#define Vec_aligned16
#define Vec_aligned32
#define Vec_aligned64
#endif

/// Prefetch localities
typedef enum Vec_PrefetchLocality {
    Vec_PrefetchLocality_none = 0, ///< No temporal locality (stream)
    Vec_PrefetchLocality_low = 1, ///< Low temporal locality (L3 cache)
    Vec_PrefetchLocality_mid = 2, ///< Medium temporal locality (L2 cache)
    Vec_PrefetchLocality_high = 3, ///< High temporal locality (L1 cache)
} Vec_PrefetchLocality;

/*========== Utility Macros =================================================*/

/// Check if pointer is aligned to N bytes
#define Vec_isAligned(_ptr, _align) \
    (((usize)(_ptr) & ((_align) - 1)) == 0)

/// Align value up to next multiple of alignment
#define Vec_alignUp(_val, _align) \
    (((_val) + (_align) - 1) & ~((_align) - 1))

/// Align value down to previous multiple of alignment
#define Vec_alignDown(_val, _align) \
    ((_val) & ~((_align) - 1))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Vec_opt__included */


/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Vec_advanced.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-08 (date of creation)
 * @updated 2025-11-08 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  simd
 *
 * @brief   Advanced SIMD operations for complex use cases
 * @details Gather/scatter, masked operations, bit manipulation,
 *          and specialized patterns for high-performance computing.
 */

#ifndef Vec_advanced__included
#define Vec_advanced__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

// #include "simd.h"
// #include "Vec_opt.h"

/*========== Advanced Memory Operations =====================================*/

/*---------- Gather/Scatter Operations --------------------------------------*/

/// Gather: Load non-contiguous elements using indices
/// result[__i] = base[indices[__i]]
#define Vec_gather$(_VT, _p_base, _indices) __op__Vec_gather$(_VT, _p_base, _indices)
#define Vec_gather(_p_base, _indices) Vec_gather$(TypeOfUnqual(*_p_base), _p_base, _indices)

/// Scatter: Store to non-contiguous locations using indices
/// base[indices[__i]] = values[__i]
#define Vec_scatter(_p_base, _indices, _values) __op__Vec_scatter(_p_base, _indices, _values)

/// Masked gather: Gather only where mask is true
#define Vec_gatherMask$(_VT, _p_base, _indices, _mask, _default) \
    __op__Vec_gatherMask$(_VT, _p_base, _indices, _mask, _default)

/// Masked scatter: Scatter only where mask is true
#define Vec_scatterMask(_p_base, _indices, _values, _mask) \
    __op__Vec_scatterMask(_p_base, _indices, _values, _mask)

/*---------- Masked Operations ----------------------------------------------*/

/// Masked load: Load with default value where mask is false
#define Vec_loadMask(_p_arr, _mask, _default) \
    __op__Vec_loadMask(_p_arr, _mask, _default)

/// Masked store: Store only where mask is true
#define Vec_storeMask(_p_arr, _values, _mask) \
    __op__Vec_storeMask(_p_arr, _values, _mask)

/// Masked arithmetic
#define Vec_addMask(_a, _b, _mask) __op__Vec_addMask(_a, _b, _mask)
#define Vec_subMask(_a, _b, _mask) __op__Vec_subMask(_a, _b, _mask)
#define Vec_mulMask(_a, _b, _mask) __op__Vec_mulMask(_a, _b, _mask)
#define Vec_divMask(_a, _b, _mask) __op__Vec_divMask(_a, _b, _mask)

/*========== Bit Manipulation ===============================================*/

/// Count leading zeros
#define Vec_clz(_a) __op__Vec_clz(_a)

/// Count trailing zeros
#define Vec_ctz(_a) __op__Vec_ctz(_a)

/// Population count (number of set bits)
#define Vec_popcnt(_a) __op__Vec_popcnt(_a)

/// Reverse bits
#define Vec_bitReverse(_a) __op__Vec_bitReverse(_a)

/// Byte swap (change endianness)
#define Vec_byteSwap(_a) __op__Vec_byteSwap(_a)

/*========== Advanced Arithmetic ============================================*/

/// Saturating arithmetic (clamps to min/max instead of wrapping)
#define Vec_addSat(_a, _b) __op__Vec_addSat(_a, _b)
#define Vec_subSat(_a, _b) __op__Vec_subSat(_a, _b)

/// Average (rounded): (a + b + 1) >> 1
#define Vec_avgRound(_a, _b) __op__Vec_avgRound(_a, _b)

/// Multiply and keep high half of result
#define Vec_mulHi(_a, _b) __op__Vec_mulHi(_a, _b)

/// Multiply different size operands and widen result
#define Vec_mulWiden$(_VT, _a, _b) __op__Vec_mulWiden$(_VT, _a, _b)

/*========== Special Reductions =============================================*/

/// Horizontal minimum/maximum with index
#define Vec_reduceMinIdx(_vec) __op__Vec_reduceMinIdx(_vec)
#define Vec_reduceMaxIdx(_vec) __op__Vec_reduceMaxIdx(_vec)

/// Sum of absolute differences
#define Vec_sad(_a, _b) __op__Vec_sad(_a, _b)

/*========== Interleave and Deinterleave ====================================*/

/// Interleave low halves: [a0 b0 a1 b1 ...]
#define Vec_interleaveLo(_a, _b) __op__Vec_interleaveLo(_a, _b)

/// Interleave high halves: [a2 b2 a3 b3 ...]
#define Vec_interleaveHi(_a, _b) __op__Vec_interleaveHi(_a, _b)

/// Deinterleave (unzip): extract even and odd lanes
#define Vec_deinterleave(_vec, _p_even, _p_odd) \
    __op__Vec_deinterleave(_vec, _p_even, _p_odd)

/*========== Pack and Unpack ================================================*/

/// Pack two vectors into one (with saturation)
#define Vec_pack$(_VT, _a, _b) __op__Vec_pack$(_VT, _a, _b)

/// Unpack low half to wider type
#define Vec_unpackLo$(_VT, _a) __op__Vec_unpackLo$(_VT, _a)

/// Unpack high half to wider type
#define Vec_unpackHi$(_VT, _a) __op__Vec_unpackHi$(_VT, _a)

/*========== Conditional Operations =========================================*/

/// Conditional increment: where(mask) { a += 1 }
#define Vec_condIncr(_a, _mask) __op__Vec_condIncr(_a, _mask)

/// Conditional decrement: where(mask) { a -= 1 }
#define Vec_condDecr(_a, _mask) __op__Vec_condDecr(_a, _mask)

/// Conditional negate: where(mask) { a = -a }
#define Vec_condNeg(_a, _mask) __op__Vec_condNeg(_a, _mask)

/*========== Special Math Functions =========================================*/

/// Exponential (base e)
#define Vec_exp(_a) __op__Vec_exp(_a)

/// Natural logarithm
#define Vec_log(_a) __op__Vec_log(_a)

/// Power (a^b)
#define Vec_pow(_a, _b) __op__Vec_pow(_a, _b)

/// Sine
#define Vec_sin(_a) __op__Vec_sin(_a)

/// Cosine
#define Vec_cos(_a) __op__Vec_cos(_a)

/// Simultaneous sin and cos
#define Vec_sinCos(_a, _p_sin, _p_cos) __op__Vec_sinCos(_a, _p_sin, _p_cos)

/*========== Implementations ================================================*/

/*---------- Gather/Scatter -------------------------------------------------*/

#if arch_is_x86_64 && arch_has_avx2

#define __op__Vec_gather$(_VT, _p_base, _indices) ({ \
    typedef _VT VecType; \
    typedef TypeOfUnqual((_p_base)[0]) ScalarType; \
    typedef TypeOfUnqual(_indices) IndexVec; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, f32) && sizeof(VecType) == 32 && sizeof(IndexVec) == 32) { \
        *((__m256*)&result) = _mm256_i32gather_ps((f32 const*)(_p_base), *((__m256i*)&(_indices)), sizeof(f32)); \
    } else if (Type_eq$(ScalarType, f64) && sizeof(VecType) == 32 && sizeof(IndexVec) == 16) { \
        *((__m256d*)&result) = _mm256_i32gather_pd((f64 const*)(_p_base), *((__m128i*)&(_indices)), sizeof(f64)); \
    } else { \
        let idx = (_indices); \
        for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
            result[__i] = (_p_base)[idx[__i]]; \
        } \
    } \
    result; \
})

#else

#define __op__Vec_gather$(_VT, _p_base, _indices) ({ \
    typedef _VT VecType; \
    typedef TypeOfUnqual((_p_base)[0]) ScalarType; \
    var_(result, VecType); \
    let idx = (_indices); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = (_p_base)[idx[__i]]; \
    } \
    result; \
})

#endif

#define __op__Vec_scatter(_p_base, _indices, _values) ({ \
    typedef TypeOfUnqual(_values) VecType; \
    typedef TypeOfUnqual((_p_base)[0]) ScalarType; \
    let idx = (_indices); \
    let val = (_values); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        (_p_base)[idx[__i]] = val[__i]; \
    } \
})

#define __op__Vec_gatherMask$(_VT, _p_base, _indices, _mask, _default) ({ \
    typedef _VT VecType; \
    typedef TypeOfUnqual((_p_base)[0]) ScalarType; \
    var_(result, VecType) = _default; \
    let idx = (_indices); \
    let m = (_mask); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (m[__i]) result[__i] = (_p_base)[idx[__i]]; \
    } \
    result; \
})

#define __op__Vec_scatterMask(_p_base, _indices, _values, _mask) ({ \
    typedef TypeOfUnqual(_values) VecType; \
    typedef TypeOfUnqual((_p_base)[0]) ScalarType; \
    let idx = (_indices); \
    let val = (_values); \
    let m = (_mask); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (m[__i]) (_p_base)[idx[__i]] = val[__i]; \
    } \
})

/*---------- Masked Operations ----------------------------------------------*/

#define __op__Vec_loadMask(_p_arr, _mask, _default) ({ \
    typedef TypeOfUnqual(_default) VecType; \
    var_(result, VecType) = _default; \
    let m = (_mask); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(result[0]); ++__i) { \
        if (m[__i]) result[__i] = (_p_arr)[__i]; \
    } \
    result; \
})

#define __op__Vec_storeMask(_p_arr, _values, _mask) ({ \
    typedef TypeOfUnqual(_values) VecType; \
    let val = (_values); \
    let m = (_mask); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(val[0]); ++__i) { \
        if (m[__i]) (_p_arr)[__i] = val[__i]; \
    } \
})

#define __op__Vec_addMask(_a, _b, _mask) \
    Vec_select(_mask, _a, Vec_add(_a, _b))

#define __op__Vec_subMask(_a, _b, _mask) \
    Vec_select(_mask, _a, Vec_sub(_a, _b))

#define __op__Vec_mulMask(_a, _b, _mask) \
    Vec_select(_mask, _a, Vec_mul(_a, _b))

#define __op__Vec_divMask(_a, _b, _mask) \
    Vec_select(_mask, _a, Vec_div(_a, _b))

/*---------- Bit Manipulation -----------------------------------------------*/

#define __op__Vec_clz(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_clz(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_ctz(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_ctz(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_popcnt(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_popcount(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_bitReverse(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        var_(x, ScalarType) = vec[__i]; \
        var_(rev, ScalarType) = 0; \
        for (usize j = 0; j < sizeof(ScalarType) * 8; ++j) { \
            rev = (rev << 1) | (x & 1); \
            x >>= 1; \
        } \
        result[__i] = rev; \
    } \
    result; \
})

#define __op__Vec_byteSwap(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (sizeof(ScalarType) == 2) { \
            result[__i] = __builtin_bswap16(vec[__i]); \
        } else if (sizeof(ScalarType) == 4) { \
            result[__i] = __builtin_bswap32(vec[__i]); \
        } else if (sizeof(ScalarType) == 8) { \
            result[__i] = __builtin_bswap64(vec[__i]); \
        } \
    } \
    result; \
})

/*---------- Advanced Arithmetic --------------------------------------------*/

#if arch_is_x86 || arch_is_x86_64

#define __op__Vec_addSat(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, i8) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_adds_epi8(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, i16) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_adds_epi16(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, u8) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_adds_epu8(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, u16) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_adds_epu16(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else { \
        result = Vec_add(_a, _b); /* Fallback */ \
    } \
    result; \
})

#define __op__Vec_subSat(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    if (Type_eq$(ScalarType, i8) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_subs_epi8(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, i16) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_subs_epi16(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, u8) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_subs_epu8(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else if (Type_eq$(ScalarType, u16) && sizeof(VecType) == 16) { \
        *((__m128i*)&result) = _mm_subs_epu16(*((__m128i*)&(_a)), *((__m128i*)&(_b))); \
    } else { \
        result = Vec_sub(_a, _b); /* Fallback */ \
    } \
    result; \
})

#else

#define __op__Vec_addSat(_a, _b) Vec_add(_a, _b)
#define __op__Vec_subSat(_a, _b) Vec_sub(_a, _b)

#endif

#define __op__Vec_avgRound(_a, _b) \
    Vec_shr(Vec_add(Vec_add(_a, _b), Vec_splat$(TypeOfUnqual(_a), 1)), Vec_splat$(TypeOfUnqual(_a), 1))

#define __op__Vec_mulHi(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = ((i64)vec_a[__i] * (i64)vec_b[__i]) >> (sizeof(ScalarType) * 8); \
    } \
    result; \
})

/*---------- Special Reductions ---------------------------------------------*/

#define __op__Vec_reduceMinIdx(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let vec = (_vec); \
    var_(min_val, ScalarType) = vec[0]; \
    var_(min_idx, usize) = 0; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (vec[__i] < min_val) { \
            min_val = vec[__i]; \
            min_idx = __i; \
        } \
    } \
    (struct { ScalarType val; usize idx; }){ min_val, min_idx }; \
})

#define __op__Vec_reduceMaxIdx(_vec) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let vec = (_vec); \
    var_(max_val, ScalarType) = vec[0]; \
    var_(max_idx, usize) = 0; \
    for (usize __i = 1; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        if (vec[__i] > max_val) { \
            max_val = vec[__i]; \
            max_idx = __i; \
        } \
    } \
    (struct { ScalarType val; usize idx; }){ max_val, max_idx }; \
})

#define __op__Vec_sad(_a, _b) ({ \
    let diff = Vec_abs(Vec_sub(_a, _b)); \
    Vec_reduceAdd(diff); \
})

/*---------- Interleave/Deinterleave ----------------------------------------*/

#define __op__Vec_interleaveLo(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        result[2 * __i] = vec_a[__i]; \
        result[2 * __i + 1] = vec_b[__i]; \
    } \
    result; \
})

#define __op__Vec_interleaveHi(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        result[2 * __i] = vec_a[len / 2 + __i]; \
        result[2 * __i + 1] = vec_b[len / 2 + __i]; \
    } \
    result; \
})

#define __op__Vec_deinterleave(_vec, _p_even, _p_odd) ({ \
    typedef TypeOfUnqual(_vec) VecType; \
    typedef TypeOfUnqual((_vec)[0]) ScalarType; \
    let v = (_vec); \
    var_(even, VecType); \
    var_(odd, VecType); \
    usize len = sizeof(VecType) / sizeof(ScalarType); \
    for (usize __i = 0; __i < len / 2; ++__i) { \
        even[__i] = v[2 * __i]; \
        odd[__i] = v[2 * __i + 1]; \
    } \
    *(_p_even) = even; \
    *(_p_odd) = odd; \
})

/*---------- Conditional Operations -----------------------------------------*/

#define __op__Vec_condIncr(_a, _mask) ({ \
    let one = Vec_splat$(TypeOfUnqual(_a), 1); \
    Vec_select(_mask, _a, Vec_add(_a, one)); \
})

#define __op__Vec_condDecr(_a, _mask) ({ \
    let one = Vec_splat$(TypeOfUnqual(_a), 1); \
    Vec_select(_mask, _a, Vec_sub(_a, one)); \
})

#define __op__Vec_condNeg(_a, _mask) \
    Vec_select(_mask, _a, Vec_neg(_a))

/*---------- Special Math Functions -----------------------------------------*/

/* Note: These are scalar fallbacks. For production use, consider
 * SVML (Intel), libmvec (glibc), or SLEEF libraries for vectorized math.
 */

#define __op__Vec_exp(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_exp(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_log(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_log(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_pow(_a, _b) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec_a = (_a); \
    let vec_b = (_b); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_pow(vec_a[__i], vec_b[__i]); \
    } \
    result; \
})

#define __op__Vec_sin(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_sin(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_cos(_a) ({ \
    typedef TypeOfUnqual(_a) VecType; \
    typedef TypeOfUnqual((_a)[0]) ScalarType; \
    var_(result, VecType); \
    let vec = (_a); \
    for (usize __i = 0; __i < sizeof(VecType) / sizeof(ScalarType); ++__i) { \
        result[__i] = __builtin_cos(vec[__i]); \
    } \
    result; \
})

#define __op__Vec_sinCos(_a, _p_sin, _p_cos) ({ \
    *(_p_sin) = Vec_sin(_a); \
    *(_p_cos) = Vec_cos(_a); \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Vec_advanced__included */
