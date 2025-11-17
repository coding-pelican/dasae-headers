/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    variant.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2025-06-15 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
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

#include "raw.h"

/*========== Macros and Declarations ========================================*/

#define enum_of$(/*(_Alias)(_value)*/...) \
    pp_expand(pp_defer(block_inline__enum_of$)(comp_param__enum_of$ _value))
#define comp_param__enum_of$(_value...)   _value, pp_expand
#define block_inline__enum_of$(_value...) as$(_Alias)(_value)

#define $fits ,
#define variant_(/*(_Alias $fits _UnderlyingType)((_Enum)(_Type),...)*/...) \
    pp_overload(__variant_, __VA_ARGS__)(__VA_ARGS__)
#define __variant__1(...) \
    __gen__variant_(__gen__variant___parseAlias __VA_ARGS__)
#define __gen__variant_(...)                                       __gen__variant___emit(__VA_ARGS__)
#define __gen__variant___parseAlias(...)                           pp_overload(__gen__variant___parseAlias, __VA_ARGS__)(__VA_ARGS__)
#define __gen__variant___parseAlias_0()                            , , __gen__variant___parsePairEnumTypeList
#define __gen__variant___parseAlias_1(_Alias...)                   _Alias, , __gen__variant___parsePairEnumTypeList
#define __gen__variant___parseAlias_2(_Alias, _UnderlyingType...)  _Alias, : _UnderlyingType, __gen__variant___parsePairEnumTypeList
#define __gen__variant___parsePairEnumTypeList(_Pair_Enum_Type...) _Pair_Enum_Type
#define __gen__variant___emit(_Alias, _EnumUnderlying, _Pair_Enum_Type...) \
    struct _Alias { \
        enum _EnumUnderlying { \
            __gen__variant___emitEnumTags(_Pair_Enum_Type) \
        } tag; \
        union { \
            __gen__variant___emitUnionPayloads(_Pair_Enum_Type) \
        } payload; \
    }
#define __gen__variant___emitEnumTags(_Pair_Enum_Type...) \
    pp_foreach (__gen__variant___emitEnumTag, ~, _Pair_Enum_Type)
#define __gen__variant___emitEnumTag(_$ignored, _Pair_Enum_Type) \
    pp_Tuple_get1st _Pair_Enum_Type,
#define __gen__variant___emitUnionPayloads(_Pair_Enum_Type...) \
    pp_foreach (__gen__variant___emitUnionPayload, ~, _Pair_Enum_Type)
#define __gen__variant___emitUnionPayload(_$ignored, _Pair_Enum_Type) \
    var_(pp_join($, tag, pp_Tuple_get1st _Pair_Enum_Type) $like_ref, pp_Tuple_get2nd _Pair_Enum_Type);
#define __variant__2(...)                  __gen__variant_raw(__VA_ARGS__)
#define __gen__variant_raw(_Alias, _$T...) struct _Alias

#define union_of(/*(_Enum){ _payload... }*/...) \
    __op__union_of(__op__union_of__parseEnum __VA_ARGS__)
#define __op__union_of(...)                 __op__union_of__emit(__VA_ARGS__)
#define __op__union_of__parseEnum(_Enum...) _Enum,
#define __op__union_of__emit(_Enum, _payload...) \
    { \
        .tag = _Enum, \
        .payload = { .pp_join($, tag, _Enum) $like_deref = _payload }, \
    }
#define union_of$(/*(_Alias)(_Enum){ _payload... }*/...) \
    __op__union_of$(__op__union_of$__parseAlias __VA_ARGS__)
#define __op__union_of$(...)                   __op__union_of$__emit(__VA_ARGS__)
#define __op__union_of$__parseAlias(_Alias...) _Alias,
#define __op__union_of$__emit(_Alias, _Enum_payload...) \
    lit$((_Alias)union_of(_Enum_payload))

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

#define comp_op__variant_of(_tag, _payload...) \
    { \
        .tag = _tag, \
        .payload = { .pp_join($, tagged, _tag) = { [0] = _payload } }, \
    }
#define comp_op__variant_of$(T_variant, _tag, _payload...) \
    ((T_variant)variant_of(_tag, _payload))
#define comp_op__variant_asg$(__addr_variant, T_Variant, var_addr_variant, val_variant...) blk({ \
    let __addr_variant = var_addr_variant; \
    debug_assert_nonnull(__addr_variant); \
    *__addr_variant = *(T_Variant[1]){ [0] = val_variant }; \
    blk_return __addr_variant; \
})
#define comp_op__variant_asg(var_addr_variant, val_variant...) \
    variant_asg$(TypeOf(*var_addr_variant), var_addr_variant, val_variant)
#define comp_op__variant_as(__addr_variant, _addr_variant, _tag...) blk({ \
    let __addr_variant = _addr_variant; \
    debug_assert(__addr_variant->tag == _tag); \
    blk_return __addr_variant->payload.pp_join($, tagged, _tag); \
})
#define comp_op__variant_extract(__var_variant, _var_variant, _tag...) blk({ \
    let __var_variant = _var_variant; \
    debug_assert(__var_variant.tag == _tag); \
    blk_return __var_variant.payload.pp_join($, tagged, _tag)[0]; \
})

// #define match_(_Variant) \
//     with_(let __variant = (_Variant)) switch (__variant.tag)
#define comp_syn__match_(_Variant, _Body...) blk({ \
    let __variant = _Variant; \
    switch (__variant.tag) \
        _Body \
})

#define comp_syn__pattern_1(_Tag, _Body...) \
    case _Tag: \
        _Body
#define comp_syn__pattern_2(_Tag, _Capture, _Body...) \
    case _Tag: { \
        let pp_Tuple_unwrap _Capture = __variant.payload.pp_join($, tag, _Tag); \
        _Body; \
    }

#define comp_syn__fallback_(_Body...) \
    default: \
        _Body

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* VARIANT_INCLUDED */
