#pragma once
#ifndef simd__included
#define simd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl/simd.h"

/*========== Target Utilities ===============================================*/

#define simd_suggestLen$(_$T...) \
    (simd_width_bits > 0 ? as$(usize)(simd_width_bits / (sizeOf$(_$T) * arch_bits_per_byte)) : as$(usize)(1))

/*========== Lane Sequence Operations =======================================*/

#define simd_shiftElemsLeft(_$v, _$amount, _$in...) __op__simd_shiftElemsLeft(pp_uniqTok(v), pp_uniqTok(amount), pp_uniqTok(ret), pp_uniqTok(i), _$v, _$amount, _$in)
#define simd_shiftElemsRight(_$v, _$amount, _$in...) __op__simd_shiftElemsRight(pp_uniqTok(v), pp_uniqTok(amount), pp_uniqTok(ret), pp_uniqTok(i), _$v, _$amount, _$in)
#define simd_rotateElemsLeft(_$v, _$amount...) __op__simd_rotateElemsLeft(pp_uniqTok(v), pp_uniqTok(amount), pp_uniqTok(ret), pp_uniqTok(i), _$v, _$amount)
#define simd_rotateElemsRight(_$v, _$amount...) __op__simd_rotateElemsRight(pp_uniqTok(v), pp_uniqTok(amount), pp_uniqTok(ret), pp_uniqTok(i), _$v, _$amount)
#define simd_reverseOrder(_$v...) __op__simd_reverseOrder(pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _$v)

/*========== Lane Search Operations =========================================*/

#define simd_bool_findFirst(_$v...) __op__simd_bool_findFirst(pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _$v)
#define simd_bool_findLast(_$v...) __op__simd_bool_findLast(pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _$v)
#define simd_bool_count(_$v...) __op__simd_bool_count(pp_uniqTok(v), pp_uniqTok(ret), pp_uniqTok(i), _$v)

#define simd_findFirst(_$v, _$value...) __op__simd_findFirst(pp_uniqTok(v), pp_uniqTok(value), _$v, _$value)
#define simd_findLast(_$v, _$value...) __op__simd_findLast(pp_uniqTok(v), pp_uniqTok(value), _$v, _$value)
#define simd_count(_$v, _$value...) __op__simd_count(pp_uniqTok(v), pp_uniqTok(value), _$v, _$value)

/*========== Memory Operations ==============================================*/

#define simd_gather$(/*(_$T: simd_V(n, T))(_$base: P(T), _$indices: simd_V(n, IntType))*/... /*(_$T)*/) __op__simd_gather$(__VA_ARGS__)
#define simd_scatter(_$base, _$indices, _$values...) __op__simd_scatter(pp_uniqTok(base), pp_uniqTok(indices), pp_uniqTok(values), pp_uniqTok(i), _$base, _$indices, _$values)

#define simd_loadMask(_$base, _$mask, _$default...) __op__simd_loadMask(pp_uniqTok(base), pp_uniqTok(mask), pp_uniqTok(ret), pp_uniqTok(i), _$base, _$mask, _$default)
#define simd_storeMask(_$base, _$mask, _$values...) __op__simd_storeMask(pp_uniqTok(base), pp_uniqTok(mask), pp_uniqTok(values), pp_uniqTok(i), _$base, _$mask, _$values)

/*========== Macros and Definitions =========================================*/

#define __op__simd_shiftElemsLeft(__v, __amount, __ret, __i, _$v, _$amount, _$in...) ({ \
    let __v = _$v; \
    let_(__amount, usize) = _$amount; \
    var_(__ret, TypeOfUnqual(__v)) = simd_V_splat(__v, _$in); \
    claim_assert(__amount <= simd_V_len(__v)); \
    loop_inline_(for) (usize __i = 0; __i + __amount < simd_V_len(__v); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__v)[__i + __amount]); \
    } \
    __ret; \
})
#define __op__simd_shiftElemsRight(__v, __amount, __ret, __i, _$v, _$amount, _$in...) ({ \
    let __v = _$v; \
    let_(__amount, usize) = _$amount; \
    var_(__ret, TypeOfUnqual(__v)) = simd_V_splat(__v, _$in); \
    claim_assert(__amount <= simd_V_len(__v)); \
    loop_inline_(for) (usize __i = __amount; __i < simd_V_len(__v); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__v)[__i - __amount]); \
    } \
    __ret; \
})
#define __op__simd_rotateElemsLeft(__v, __amount, __ret, __i, _$v, _$amount...) ({ \
    let __v = _$v; \
    let_(__amount, usize) = (_$amount) % simd_V_len(__v); \
    var_(__ret, TypeOfUnqual(__v)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__v); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__v)[(__i + __amount) % simd_V_len(__v)]); \
    } \
    __ret; \
})
#define __op__simd_rotateElemsRight(__v, __amount, __ret, __i, _$v, _$amount...) ({ \
    let __v = _$v; \
    let_(__amount, usize) = (_$amount) % simd_V_len(__v); \
    var_(__ret, TypeOfUnqual(__v)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__v); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__v)[(__i + simd_V_len(__v) - __amount) % simd_V_len(__v)]); \
    } \
    __ret; \
})
#define __op__simd_reverseOrder(__v, __ret, __i, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, TypeOfUnqual(__v)) = simd_V_zero(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__v); ++__i) { \
        *simd_V_at((__ret)[__i]) = *simd_V_at((__v)[simd_V_len(__v) - 1 - __i]); \
    } \
    __ret; \
})

#define __op__simd_bool_findFirst(__v, __ret, __i, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, O$usize) = none(); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__v); ++__i) { \
        if (*simd_V_at((__v)[__i]) != 0) { \
            __ret = l$((O$usize)some(__i)); \
            break; \
        } \
    } \
    __ret; \
})
#define __op__simd_bool_findLast(__v, __ret, __i, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, O$usize) = none(); \
    loop_inline_(for) (usize __i = simd_V_len(__v); __i > 0; --__i) { \
        if (*simd_V_at((__v)[__i - 1]) != 0) { \
            __ret = l$((O$usize)some(__i - 1)); \
            break; \
        } \
    } \
    __ret; \
})
#define __op__simd_bool_count(__v, __ret, __i, _$v...) ({ \
    let __v = _$v; \
    var_(__ret, usize) = 0; \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__v); ++__i) { \
        __ret += as$(usize)(*simd_V_at((__v)[__i]) != 0); \
    } \
    __ret; \
})

#define __op__simd_findFirst(__v, __value, _$v, _$value...) ({ \
    let __v = _$v; \
    let __value = _$value; \
    simd_bool_findFirst(simd_V_eq(__v, simd_V_splat(__v, __value))); \
})
#define __op__simd_findLast(__v, __value, _$v, _$value...) ({ \
    let __v = _$v; \
    let __value = _$value; \
    simd_bool_findLast(simd_V_eq(__v, simd_V_splat(__v, __value))); \
})
#define __op__simd_count(__v, __value, _$v, _$value...) ({ \
    let __v = _$v; \
    let __value = _$value; \
    simd_bool_count(simd_V_eq(__v, simd_V_splat(__v, __value))); \
})

#define __op__simd_gather$(...) \
    __op__simd_gather$__step(__op__simd_gather$__parseT __VA_ARGS__)
#define __op__simd_gather$__parseT(_$T...) \
    _$T, __op__simd_gather$__parseArgs
#define __op__simd_gather$__parseArgs(_$base, _$indices...) \
    pp_uniqTok(base), pp_uniqTok(indices), pp_uniqTok(ret), pp_uniqTok(i), _$base, _$indices
#define __op__simd_gather$__step(...) \
    __op__simd_gather$__emit(__VA_ARGS__)
#define __op__simd_gather$__emit(_$T, __base, __indices, __ret, __i, _$base, _$indices...) ({ \
    let __base = _$base; \
    let __indices = _$indices; \
    var_(__ret, _$T) = simd_V_zero(); \
    claim_assert(simd_V_len(__ret) == simd_V_len(__indices)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        *simd_V_at((__ret)[__i]) = __base[*simd_V_at((__indices)[__i])]; \
    } \
    __ret; \
})
#define __op__simd_scatter(__base, __indices, __values, __i, _$base, _$indices, _$values...) ({ \
    let __base = _$base; \
    let __indices = _$indices; \
    let __values = _$values; \
    claim_assert(simd_V_len(__indices) == simd_V_len(__values)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__values); ++__i) { \
        __base[*simd_V_at((__indices)[__i])] = *simd_V_at((__values)[__i]); \
    } \
})

#define __op__simd_loadMask(__base, __mask, __ret, __i, _$base, _$mask, _$default...) ({ \
    let __base = _$base; \
    let __mask = _$mask; \
    var __ret = _$default; \
    claim_assert(simd_V_len(__mask) == simd_V_len(__ret)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__ret); ++__i) { \
        if (*simd_V_at((__mask)[__i]) != 0) { \
            *simd_V_at((__ret)[__i]) = __base[__i]; \
        } \
    } \
    __ret; \
})
#define __op__simd_storeMask(__base, __mask, __values, __i, _$base, _$mask, _$values...) ({ \
    let __base = _$base; \
    let __mask = _$mask; \
    let __values = _$values; \
    claim_assert(simd_V_len(__mask) == simd_V_len(__values)); \
    loop_inline_(for) (usize __i = 0; __i < simd_V_len(__values); ++__i) { \
        if (*simd_V_at((__mask)[__i]) != 0) { \
            __base[__i] = *simd_V_at((__values)[__i]); \
        } \
    } \
})

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* simd__included */
