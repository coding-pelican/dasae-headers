/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    variant.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-02 (date of creation)
 * @updated 2025-06-15 (date of last update)
 * @version v0.1-alpha.3
 * @ingroup dasae-headers(dh)/prl/types
 * @prefix  (none)
 *
 * @brief   Variant implementation with pattern matching
 * @details Provides macros and utilities for creating and working with tagged unions (discriminated unions),
 *          including pattern matching syntax, payload extraction, and type-safe union construction.
 *          Supports compile-time type checking and exhaustive pattern matching.
 */
#pragma once
#ifndef prl_tpl_variant__included
#define prl_tpl_variant__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define $maps(...) , $_maps, __VA_ARGS__

/*((_$Alias) $T)*/
/*((_$Alias)(...))*/
/*((_$Alias $fits($packed))(...))*/
/*((_$Alias $fits($bits(8|16|32|64)))(...))*/
/*((_$Alias $maps(_$Tag))(...))*/
#define variant_(/*(_$Alias <$fits(...)|$maps(_$Tag)>)((_$Enum)(_$Type),...) <$T>*/...) \
    __type__variant_(__VA_ARGS__)
#define __type__variant_(...) \
    pp_overload(__step__variant_, __VA_ARGS__)(____variant___parseAlias __VA_ARGS__)

#define ____variant___parseAlias(...) __VA_ARGS__,
#define ____variant___parsePairEnumTypeList(_$Pair_Enum_Type...) _$Pair_Enum_Type

#define __step__variant__1(...) \
    __gen__variant_(__VA_ARGS__)
#define __gen__variant_(...) \
    pp_overload(__gen__variant_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__variant__2(_$Alias, _$Pair_Enum_Type...) \
    __gen__variant___emit$_fits(_$Alias, , ____variant___parsePairEnumTypeList _$Pair_Enum_Type)
#define __gen__variant__4(_$Alias, _$Attr, _$Attr_Ctx, _$Pair_Enum_Type...) \
    pp_cat(__gen__variant___emit, _$Attr)(_$Alias, _$Attr_Ctx, ____variant___parsePairEnumTypeList _$Pair_Enum_Type)

#define __gen__variant___emit$_fits(_$Alias, _$Attr_Ctx, _$Pair_Enum_Type...) \
    struct _$Alias { \
        enum _$Attr_Ctx { \
            __gen__variant___emitEnumTags(_$Pair_Enum_Type) \
        } tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_none(), _$Pair_Enum_Type) \
        } payload; \
    }
#define __gen__variant___emit$_maps(_$Alias, _$Attr_Ctx, _$Pair_Enum_Type...) \
    struct _$Alias { \
        _$Attr_Ctx tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_some(_$Attr_Ctx), _$Pair_Enum_Type) \
        } payload; \
    }

#define __gen__variant___emitEnumTags(_$Pair_Enum_Type...) \
    ____variant___emitEnumTags(_$Pair_Enum_Type)
#define ____variant___emitEnumTags(_$Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitEnumTag, ~, _$Pair_Enum_Type)
#define __gen__variant___emitEnumTag(_$ignored, /*_$Pair_Enum_Type*/...) __VA_OPT__( \
    pp_Tuple_get1st __VA_ARGS__, \
)

#define __gen__variant___emitUnionPayloads(_$Opt_UnderlayEnum, _$Pair_Enum_Type...) \
    ____variant___emitUnionPayloads(_$Opt_UnderlayEnum, _$Pair_Enum_Type)
#define ____variant___emitUnionPayloads(_$Opt_UnderlayEnum, _$Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitUnionPayload, _$Opt_UnderlayEnum, _$Pair_Enum_Type)
#define __gen__variant___emitUnionPayload(_$Opt_UnderlayEnum, /*_$Pair_Enum_Type*/...) __VA_OPT__( \
    var_(pp_join($, tag, pp_Tuple_get1st __VA_ARGS__), pp_Tuple_get2nd __VA_ARGS__); \
    claim_assert_static( \
        pp_Tuple_get1st __VA_ARGS__ \
        == as$(pp_orelse_((_$Opt_UnderlayEnum)(TypeOf(pp_Tuple_get1st __VA_ARGS__))))(pp_Tuple_get1st __VA_ARGS__) \
    ); /* claim_assert_static(pp_Tuple_get1st __VA_ARGS__ == pp_join($, enum, pp_Tuple_get1st __VA_ARGS__)); */ \
)

#define __step__variant__2(...) __gen__variant_raw(__VA_ARGS__)
#define __gen__variant_raw(_$Alias, _$T...) struct _$Alias

#define enum_Tag$(_$Alias...) FieldType$(_$Alias, tag)
#define enum_tag(_$tagged...) ((_$tagged).tag)

#define union_of(/*(_$Enum){ _$payload... }*/...) \
    __op__union_of(__op__union_of__parseEnum __VA_ARGS__)
#define __op__union_of(...) __op__union_of__emit(__VA_ARGS__)
#define __op__union_of__parseEnum(_$Enum...) _$Enum,
#define __op__union_of__emit(_$Enum, _$payload...) \
    { \
        .tag = _$Enum, \
        .payload = { .pp_join($, tag, _$Enum) = _$payload }, \
    }
#define union_of$(/*(_$Alias)(_$Enum){ _$payload... }*/...) \
    __op__union_of$(__op__union_of$__parseAlias __VA_ARGS__)
#define __op__union_of$(...) __op__union_of$__emit(__VA_ARGS__)
#define __op__union_of$__parseAlias(_$Alias...) _$Alias,
#define __op__union_of$__emit(_$Alias, _Enum_payload...) \
    l$((_$Alias)union_of(_Enum_payload))
#define union_with$(/*(_$Alias)(_$enum_val)(_$payload_val)*/...) \
    __op__union_with$(__op__union_with$__parseAlias __VA_ARGS__)
#define __op__union_with$(...) __op__union_with$__emit(__VA_ARGS__)
#define __op__union_with$__parseAlias(_$Alias, ...) _$Alias, __op__union_with$__parseEnum __VA_ARGS__
#define __op__union_with$__parseEnum(_$Enum...) _$Enum,
#define __op__union_with$__emit(_$Alias, _$enum_val, _$payload_val...) l$((_$Alias){ \
    .tag = _$enum_val, \
    .payload = as$(FieldType$(_$Alias, payload))(_$payload_val), \
})

#define union_as(/*(_$p_tagged)(_$Enum)*/...) \
    __op__union_as(__op__union_as__parsePTagged __VA_ARGS__)
#define __op__union_as(...) __op__union_as__emit(__VA_ARGS__)
#define __op__union_as__parsePTagged(_$p_tagged...) pp_uniqTok(p_tagged), _$p_tagged,
#define __op__union_as__emit(__p_tagged, _$p_tagged, _$Enum...) local_({ \
    let __p_tagged = _$p_tagged; \
    claim_assert(matches(*__p_tagged, _$Enum)); \
    local_return_(&__p_tagged->payload.pp_join($, tag, pp_if_(pp_isParen(_$Enum))(pp_then_ _$Enum, pp_else_(_$Enum)))); \
})
#define union_to(/*(_$tagged)(_$Enum)*/...) \
    __op__union_to(__op__union_to__parseTagged __VA_ARGS__)
#define __op__union_to(...) __op__union_to__emit(__VA_ARGS__)
#define __op__union_to__parseTagged(_$tagged...) pp_uniqTok(tagged), _$tagged,
#define __op__union_to__emit(__tagged, _$tagged, _$Enum...) local_({ \
    let __tagged = _$tagged; \
    claim_assert(matches(__tagged, _$Enum)); \
    local_return_(__tagged.payload.pp_join($, tag, pp_if_(pp_isParen(_$Enum))(pp_then_ _$Enum, pp_else_(_$Enum)))); \
})
#define union_cast$(/*(_$T)(_$enum_val)(_$tagged)*/...) \
    __op__union_cast$(__op__union_cast$__parseT __VA_ARGS__)
#define __op__union_cast$(...) __op__union_cast$__emit(__VA_ARGS__)
#define __op__union_cast$__parseT(_$T, ...) _$T, __op__union_cast$__parseEnumVal __VA_ARGS__
#define __op__union_cast$__parseEnumVal(_$enum_val...) _$enum_val, pp_uniqTok(tagged),
#define __op__union_cast$__emit(_$T, _$enum_val, __tagged, _$tagged...) local_({ \
    let __tagged = _$tagged; \
    claim_assert(matches(__tagged, _$enum_val)); \
    local_return_(*ptrCast$((const _$T*)(&__tagged.payload))); \
})

#define matches(_$tagged, _$Enum...) \
    __expr__matches(_$tagged, _$Enum)
#define __expr__matches(_$tagged, _$Enum...) \
    pri_eql((_$tagged).tag, _$Enum)

/* clang-format off */
#define match_(/*<$deref> (_$tagged_val)|$ref (_$tagged_ptr)*/...) __stmt__match_(__VA_ARGS__)
#define __stmt__match_(...) pp_overload(__stmt__match_, __VA_ARGS__)(__VA_ARGS__)
#define __stmt__match__1(_$tagged_val...) __step__match_($deref _$tagged_val)
#define __stmt__match__2(_$opt, _$tagged...) __step__match_(_$opt, _$tagged)
#define __step__match_(...) ____match_(__VA_ARGS__)
#define ____match___tagged(_$opt...) pp_cat(____match___tagged, _$opt)
#define ____match___tagged$_deref(_$tagged_val...) &from$((TypeOf(_$tagged_val))_$tagged_val)
#define ____match___tagged$_ref(_$tagged_ptr...) _$tagged_ptr
#define ____match_(_$opt, _$tagged...) { \
    let __matching_tagged = ____match___tagged(_$opt)(_$tagged); \
    $attr($maybe_unused) \
    T_alias$((MatchingType)(TypeOfUnqual(*__matching_tagged))); \
    switch (__matching_tagged->tag)
#define $end_match \
}

#define case_(/*(_$Enum)*/...) __stmt__case_(__VA_ARGS__)
#define __stmt__case_(...) __step__case_(__inline__case___parseEnum __VA_ARGS__)
#define __inline__case___parseEnum(_$Enum...) _$Enum
#define __step__case_(...) __inline__case_(__VA_ARGS__)
#define __inline__case_(_$Enum...) case _$Enum: { \
    $attr($maybe_unused) \
    $static let __matched_enum = _$Enum;
#define $end_case \
} break

#define cases_(/*(_$Enums...) _$stmts...*/...) __stmt__cases_(__VA_ARGS__)
#define __stmt__cases_(...) __step__cases_(__inline__cases___parseEnums __VA_ARGS__)
#define __inline__cases___parseEnums(_$Enums...) (_$Enums),
#define __step__cases_(...) __inline__cases_(__VA_ARGS__)
#define __inline__cases_(_$Enums, ...) \
    pp_foreach(__cases___stmt__each, (__VA_ARGS__), __cases___expand _$Enums)
#define __cases___stmt__each(_$stmts, /*_$Enum*/...) __VA_OPT__( \
    __cases___step__each(__VA_ARGS__, __cases___expand _$stmts) \
)
#define __cases___step__each(...) __cases___inline__each(__VA_ARGS__)
#define __cases___inline__each(_$Enum, _$stmts...) \
    case_((_$Enum)) _$stmts $end(case);
#define __cases___expand(...) __VA_ARGS__
#define $end_cases

#define patt_(/*(_$Enum)(_$capt)*/...) __step__patt_(__step__patt___parseEnum __VA_ARGS__)
#define __step__patt_(...) __step__patt___emit(__VA_ARGS__)
#define __step__patt___parseEnum(_$Enum...) _$Enum, __step__patt___parseCapt
#define __step__patt___parseCapt(_$capt...) pp_overload(__step__patt___parseCapt, _$capt)(_$capt)
#define __step__patt___parseCapt_1(_$capt...) $deref _$capt
#define __step__patt___parseCapt_2(_$opt, _$capt...) _$opt, _$capt
#define __step__patt___capt(_$opt...) pp_cat(__step__patt___capt, _$opt)
#define __step__patt___capt$_ref(_$Enum...) union_as((__matching_tagged)(_$Enum))
#define __step__patt___capt$_deref(_$Enum...) union_to((*__matching_tagged)(_$Enum))
#define __step__patt___emit(_$Enum, _$opt, _$capt...) case _$Enum: { \
    $attr($maybe_unused) \
    $static let __matched_enum = _$Enum; \
    $attr($maybe_unused) \
    T_alias$((MatchedType)(TypeOfUnqual(union_to((*__matching_tagged)(_$Enum))))); \
    let _$capt = __step__patt___capt(_$opt(_$Enum));
#define $end_patt \
} break

#define patts_(/*(_$Enums...)(_$capt) _$stmts...*/...) __step__patts_(__step__patts___parseEnums __VA_ARGS__)
#define __step__patts_(...) __step__patts___emit(__VA_ARGS__)
#define __step__patts___parseEnums(_$Enums...) (_$Enums), __step__patts___parseCapt
#define __step__patts___parseCapt(_$capt...) (pp_overload(__step__patts___parseCapt, _$capt)(_$capt)),
#define __step__patts___parseCapt_1(_$capt...) $deref _$capt
#define __step__patts___parseCapt_2(_$opt, _$capt...) _$opt, _$capt
#define __step__patts___emit(_$Enums, ...) \
    pp_foreach(__step__patts___each, (__VA_ARGS__), __patts___expand _$Enums)
#define __step__patts___each(_$opt_w_capt_w_stmts, /*_$Enum*/...) __VA_OPT__( \
    __step__patts___each_emit(__VA_ARGS__, __patts___expand _$opt_w_capt_w_stmts) \
)
#define __step__patts___each_emit(...) ____patts___each(__VA_ARGS__)
#define ____patts___each(_$Enum, _$opt_w_capt, _$stmts...) \
    patt_((_$Enum)_$opt_w_capt) _$stmts $end(patt);
#define __patts___expand(_$Enum...) _$Enum
#define $end_patts

#define default_(/*(_$Enums)*/...) \
    __VA_OPT__(__step__default___casesFallthrough __VA_ARGS__) \
    default: {
#define $end_default \
    } break
#define __step__default___casesFallthrough(...) \
    pp_foreach(__default___eachFallthrough, ~, __VA_ARGS__)
#define __default___eachFallthrough(_$ignored, /*_$Enum*/...)  __VA_OPT__( \
    case __VA_ARGS__: $fallthrough; \
)
/* clang-format on */

#define matchedEnum() (__matched_enum)
#define union_matched(/*{ _$payload... }*/...) \
    union_with$((MatchingType)(matchedEnum())from$((MatchedType)__VA_ARGS__))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_variant__included */
