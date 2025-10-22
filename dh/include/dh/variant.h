/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    variant.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2025-06-15 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Variant implementation with pattern matching
 * @details Provides macros and utilities for creating and working with tagged unions (discriminated unions),
 *          including pattern matching syntax, payload extraction, and type-safe union construction.
 *          Supports compile-time type checking and exhaustive pattern matching.
 */

#ifndef VARIANT_INCLUDED
#define VARIANT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Declarations ========================================*/

#undef match_
#undef comp_syn__match_
#undef pattern_
#undef comp_syn__pattern_
#undef fallback_
#undef comp_syn__fallback_

#define enum_of$(/*(_Alias)(_value)*/...) \
    pp_expand(pp_defer(block_inline__enum_of$)(comp_param__enum_of$ _value))
#define comp_param__enum_of$(_value...)   _value, pp_expand
#define block_inline__enum_of$(_value...) as$((_Alias)(_value))

#define variant_(_alias, ...) \
    comp_gen__variant_(_alias, __VA_ARGS__)

#define variant_of(_tag, _payload...) \
    comp_op__variant_of(_tag, _payload)
#define variant_of$(T_variant, _tag, _payload...) \
    comp_op__variant_of$(T_variant, _tag, _payload)
#define variant_asg$(T_Variant, var_addr_variant, val_variant...) \
    comp_op__variant_asg$(pp_uniqTok(var_addr_variant), T_Variant, var_addr_variant, val_variant)
#define variant_asg(var_addr_variant, val_variant...) \
    comp_op__variant_asg(var_addr_variant, val_variant)
#define variant_as(_addr_variant, _tag...) \
    comp_op__variant_as(pp_uniqTok(addr_variant), _addr_variant, _tag)
#define variant_extract(_var_variant, _tag...) \
    comp_op__variant_extract(pp_uniqTok(var_variant), _var_variant, _tag)

#define match_(_Variant, _Body...) \
    comp_syn__match_(_Variant, _Body)

#define pattern_(_Tag, _Capture_w_Body...) \
    pp_overload(__pattern, _Capture_w_Body)(_Tag, _Capture_w_Body)
#define __pattern_1(_Tag, _Body...) \
    comp_syn__pattern_1(_Tag, _Body)
#define __pattern_2(_Tag, _Capture, _Body...) \
    comp_syn__pattern_2(_Tag, _Capture, _Body)

#define fallback_(_Body...) comp_syn__fallback_(_Body)

/*========== Macros and Definitions =========================================*/

#define comp_gen__variant_(_alias, ...) \
    struct pp_expand _alias { \
        enum { \
            comp_gen__variant__enumTags(_alias, __VA_ARGS__) \
        } tag; \
        union { \
            comp_gen__variant__unionTypes(_alias, __VA_ARGS__) \
        } payload; \
    }

#define comp_gen__variant__enumTags(T_variant, ...) \
    pp_foreach (comp_gen__variant__enumTag, T_variant, __VA_ARGS__)
#define comp_gen__variant__enumTag(T_variant, _Pair) \
    pp_Tuple_get1st _Pair,

#define comp_gen__variant__unionTypes(T_variant, ...) \
    pp_foreach (comp_gen__variant__unionType, T_variant, __VA_ARGS__)
#define comp_gen__variant__unionType(T_variant, _Pair) \
    pp_Tuple_get1stRest _Pair pp_join($, tagged, pp_Tuple_get1st _Pair)[1];

#define comp_op__variant_of(_tag, _payload...) \
    { \
        .tag     = _tag, \
        .payload = { .pp_join($, tagged, _tag) = { [0] = _payload } }, \
    }
#define comp_op__variant_of$(T_variant, _tag, _payload...) \
    ((T_variant)variant_of(_tag, _payload))
#define comp_op__variant_asg$(__addr_variant, T_Variant, var_addr_variant, val_variant...) eval({ \
    let __addr_variant = var_addr_variant; \
    debug_assert_nonnull(__addr_variant); \
    *__addr_variant = *(T_Variant[1]){ [0] = val_variant }; \
    eval_return __addr_variant; \
})
#define comp_op__variant_asg(var_addr_variant, val_variant...) \
    variant_asg$(TypeOf(*var_addr_variant), var_addr_variant, val_variant)
#define comp_op__variant_as(__addr_variant, _addr_variant, _tag...) eval({ \
    let __addr_variant = _addr_variant; \
    debug_assert(__addr_variant->tag == _tag); \
    eval_return __addr_variant->payload.pp_join($, tagged, _tag); \
})
#define comp_op__variant_extract(__var_variant, _var_variant, _tag...) eval({ \
    let __var_variant = _var_variant; \
    debug_assert(__var_variant.tag == _tag); \
    eval_return __var_variant.payload.pp_join($, tagged, _tag)[0]; \
})

// #define match_(_Variant) \
//     with_(let __variant = (_Variant)) switch (__variant.tag)
#define comp_syn__match_(_Variant, _Body...) eval({ \
    let __variant = _Variant; \
    switch (__variant.tag) \
        _Body \
})

#define comp_syn__pattern_1(_Tag, _Body...) \
    case _Tag: \
        _Body
#define comp_syn__pattern_2(_Tag, _Capture, _Body...) \
    case _Tag: { \
        let pp_Tuple_unwrap _Capture = __variant.payload.pp_join($, tagged, _Tag); \
        _Body; \
    }

#define comp_syn__fallback_(_Body...) \
    default: \
        _Body

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* VARIANT_INCLUDED */
