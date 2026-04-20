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
#ifndef prl_types_variant__included
#define prl_types_variant__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "raw.h"

/*========== Macros and Declarations ========================================*/

#define $maps(...) , $_maps, __VA_ARGS__

/*((_Alias) $T)*/
/*((_Alias)(...))*/
/*((_Alias $fits($packed))(...))*/
/*((_Alias $fits($bits(8|16|32|64)))(...))*/
/*((_Alias $maps(_Tag))(...))*/
#define variant_(/*(_Alias <$fits(...)|$maps(_Tag)>)((_Enum)(_Type),...) <$T>*/...) \
    __type__variant_(__VA_ARGS__)
#define __type__variant_(...) \
    pp_overload(__step__variant_, __VA_ARGS__)(____variant___parseAlias __VA_ARGS__)

#define ____variant___parseAlias(...) __VA_ARGS__,
#define ____variant___parsePairEnumTypeList(_Pair_Enum_Type...) _Pair_Enum_Type

#define __step__variant__1(...) \
    __gen__variant_(__VA_ARGS__)
#define __gen__variant_(...) \
    pp_overload(__gen__variant_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__variant__2(_Alias, _Pair_Enum_Type...) \
    __gen__variant___emit$_fits(_Alias, , ____variant___parsePairEnumTypeList _Pair_Enum_Type)
#define __gen__variant__4(_Alias, _Attr, _Attr_Ctx, _Pair_Enum_Type...) \
    pp_cat(__gen__variant___emit, _Attr)(_Alias, _Attr_Ctx, ____variant___parsePairEnumTypeList _Pair_Enum_Type)

#define __gen__variant___emit$_fits(_Alias, _Attr_Ctx, _Pair_Enum_Type...) \
    struct _Alias { \
        enum _Attr_Ctx { \
            __gen__variant___emitEnumTags(_Pair_Enum_Type) \
        } tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_none(), _Pair_Enum_Type) \
        } payload; \
    }
#define __gen__variant___emit$_maps(_Alias, _Attr_Ctx, _Pair_Enum_Type...) \
    struct _Alias { \
        _Attr_Ctx tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_some(_Attr_Ctx), _Pair_Enum_Type) \
        } payload; \
    }

#define __gen__variant___emitEnumTags(_Pair_Enum_Type...) \
    ____variant___emitEnumTags(_Pair_Enum_Type)
#define ____variant___emitEnumTags(_Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitEnumTag, ~, _Pair_Enum_Type)
#define __gen__variant___emitEnumTag(_$ignored, /*_Pair_Enum_Type*/...) __VA_OPT__( \
    pp_Tuple_get1st __VA_ARGS__, \
)

#define __gen__variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type...) \
    ____variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type)
#define ____variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitUnionPayload, _Opt_UnderlayEnum, _Pair_Enum_Type)
#define __gen__variant___emitUnionPayload(_Opt_UnderlayEnum, /*_Pair_Enum_Type*/...) __VA_OPT__( \
    var_(pp_join($, tag, pp_Tuple_get1st __VA_ARGS__), pp_Tuple_get2nd __VA_ARGS__); \
    claim_assert_static( \
        pp_Tuple_get1st __VA_ARGS__ \
        == as$(pp_orelse_((_Opt_UnderlayEnum)(TypeOf(pp_Tuple_get1st __VA_ARGS__))))(pp_Tuple_get1st __VA_ARGS__) \
    ); /* claim_assert_static(pp_Tuple_get1st __VA_ARGS__ == pp_join($, enum, pp_Tuple_get1st __VA_ARGS__)); */ \
)

#define __step__variant__2(...) __gen__variant_raw(__VA_ARGS__)
#define __gen__variant_raw(_Alias, _$T...) struct _Alias

#define union_of(/*(_Enum){ _payload... }*/...) \
    __op__union_of(__op__union_of__parseEnum __VA_ARGS__)
#define __op__union_of(...) __op__union_of__emit(__VA_ARGS__)
#define __op__union_of__parseEnum(_Enum...) _Enum,
#define __op__union_of__emit(_Enum, _payload...) \
    { \
        .tag = _Enum, \
        .payload = { .pp_join($, tag, _Enum) = _payload }, \
    }
#define union_of$(/*(_Alias)(_Enum){ _payload... }*/...) \
    __op__union_of$(__op__union_of$__parseAlias __VA_ARGS__)
#define __op__union_of$(...) __op__union_of$__emit(__VA_ARGS__)
#define __op__union_of$__parseAlias(_Alias...) _Alias,
#define __op__union_of$__emit(_Alias, _Enum_payload...) \
    l$((_Alias)union_of(_Enum_payload))
#define union_with$(/*(_Alias)(_enum_val)(_payload_val)*/...) \
    __op__union_with$(__op__union_with$__parseAlias __VA_ARGS__)
#define __op__union_with$(...) __op__union_with$__emit(__VA_ARGS__)
#define __op__union_with$__parseAlias(_Alias, ...) _Alias, __op__union_with$__parseEnum __VA_ARGS__
#define __op__union_with$__parseEnum(_Enum...) _Enum,
#define __op__union_with$__emit(_Alias, _enum_val, _payload_val...) l$((_Alias){ \
    .tag = _enum_val, \
    .payload = as$(FieldType$(_Alias, payload))(_payload_val), \
})

#define union_as(/*(_p_tagged)(_Enum)*/...) \
    __op__union_as(__op__union_as__parsePTagged __VA_ARGS__)
#define __op__union_as(...) __op__union_as__emit(__VA_ARGS__)
#define __op__union_as__parsePTagged(_p_tagged...) pp_uniqTok(p_tagged), _p_tagged,
#define __op__union_as__emit(__p_tagged, _p_tagged, _Enum...) local_({ \
    let __p_tagged = _p_tagged; \
    claim_assert(matches(*__p_tagged, _Enum)); \
    local_return_(&__p_tagged->payload.pp_join($, tag, pp_if_(pp_isParen(_Enum))(pp_then_ _Enum, pp_else_(_Enum)))); \
})
#define union_to(/*(_tagged)(_Enum)*/...) \
    __op__union_to(__op__union_to__parseTagged __VA_ARGS__)
#define __op__union_to(...) __op__union_to__emit(__VA_ARGS__)
#define __op__union_to__parseTagged(_tagged...) pp_uniqTok(tagged), _tagged,
#define __op__union_to__emit(__tagged, _tagged, _Enum...) local_({ \
    let __tagged = _tagged; \
    claim_assert(matches(__tagged, _Enum)); \
    local_return_(__tagged.payload.pp_join($, tag, pp_if_(pp_isParen(_Enum))(pp_then_ _Enum, pp_else_(_Enum)))); \
})
#define union_cast$(/*(_T)(_enum_val)(_tagged)*/...) \
    __op__union_cast$(__op__union_cast$__parseT __VA_ARGS__)
#define __op__union_cast$(...) __op__union_cast$__emit(__VA_ARGS__)
#define __op__union_cast$__parseT(_T, ...) _T, __op__union_cast$__parseEnumVal __VA_ARGS__
#define __op__union_cast$__parseEnumVal(_enum_val...) _enum_val, pp_uniqTok(tagged),
#define __op__union_cast$__emit(_T, _enum_val, __tagged, _tagged...) local_({ \
    let __tagged = _tagged; \
    claim_assert(matches(__tagged, _enum_val)); \
    local_return_(*ptrCast$((const _T*)(&__tagged.payload))); \
})

#define matches(_tagged, _Enum...) \
    __expr__matches(_tagged, _Enum)
#define __expr__matches(_tagged, _Enum...) \
    pri_eql((_tagged).tag, _Enum)

/* clang-format off */
#define match_(/*<$deref> (_tagged_val)|$ref (_tagged_ptr)*/...) __stmt__match_(__VA_ARGS__)
#define __stmt__match_(...) pp_overload(__stmt__match_, __VA_ARGS__)(__VA_ARGS__)
#define __stmt__match__1(_tagged_val...) __step__match_($deref _tagged_val)
#define __stmt__match__2(_$opt, _tagged...) __step__match_(_$opt, _tagged)
#define __step__match_(...) ____match_(__VA_ARGS__)
#define ____match___tagged(_$opt...) pp_cat(____match___tagged, _$opt)
#define ____match___tagged$_deref(_tagged_val...) &from$((TypeOf(_tagged_val))_tagged_val)
#define ____match___tagged$_ref(_tagged_ptr...) _tagged_ptr
#define ____match_(_$opt, _tagged...) { \
    let __matching_tagged = ____match___tagged(_$opt)(_tagged); \
    $attr($maybe_unused) \
    T_alias$((MatchingType)(TypeOfUnqual(*__matching_tagged))); \
    switch (__matching_tagged->tag)
#define $end_match \
}

#define case_(/*(_Enum)*/...) __stmt__case_(__VA_ARGS__)
#define __stmt__case_(...) __step__case_(__inline__case___parseEnum __VA_ARGS__)
#define __inline__case___parseEnum(_Enum...) _Enum
#define __step__case_(...) __inline__case_(__VA_ARGS__)
#define __inline__case_(_Enum...) case _Enum: { \
    $attr($maybe_unused) \
    $static let __matched_enum = _Enum;
#define $end_case \
} break

#define cases_(/*(_Enums...) stmts...*/...) __stmt__cases_(__VA_ARGS__)
#define __stmt__cases_(...) __step__cases_(__inline__cases___parseEnums __VA_ARGS__)
#define __inline__cases___parseEnums(_Enums...) (_Enums),
#define __step__cases_(...) __inline__cases_(__VA_ARGS__)
#define __inline__cases_(_Enums, ...) \
    pp_foreach(__cases___stmt__each, (__VA_ARGS__), __cases___expand _Enums)
#define __cases___stmt__each(_stmts, /*_Enum*/...) __VA_OPT__( \
    __cases___step__each(__VA_ARGS__, __cases___expand _stmts) \
)
#define __cases___step__each(...) __cases___inline__each(__VA_ARGS__)
#define __cases___inline__each(_Enum, stmts...) \
    case_((_Enum)) stmts $end(case);
#define __cases___expand(...) __VA_ARGS__
#define $end_cases

#define pattern_(/*(_Enum)(_capt)*/...) __step__pattern_(__step__pattern___parseEnum __VA_ARGS__)
#define __step__pattern_(...) __step__pattern___emit(__VA_ARGS__)
#define __step__pattern___parseEnum(_Enum...) _Enum, __step__pattern___parseCapt
#define __step__pattern___parseCapt(_capt...) pp_overload(__step__pattern___parseCapt, _capt)(_capt)
#define __step__pattern___parseCapt_1(_capt...) $deref _capt
#define __step__pattern___parseCapt_2(_$opt, _capt...) _$opt, _capt
#define __step__pattern___capt(_$opt...) pp_cat(__step__pattern___capt, _$opt)
#define __step__pattern___capt$_ref(_Enum...) union_as((__matching_tagged)(_Enum))
#define __step__pattern___capt$_deref(_Enum...) union_to((*__matching_tagged)(_Enum))
#define __step__pattern___emit(_Enum, _$opt, _capt...) case _Enum: { \
    $attr($maybe_unused) \
    $static let __matched_enum = _Enum; \
    $attr($maybe_unused) \
    T_alias$((MatchedType)(TypeOfUnqual(union_to((*__matching_tagged)(_Enum))))); \
    let _capt = __step__pattern___capt(_$opt(_Enum));
#define $end_pattern \
} break

#define patterns_(/*(_Enums...)(_capt) stmts...*/...) __step__patterns_(__step__patterns___parseEnums __VA_ARGS__)
#define __step__patterns_(...) __step__patterns___emit(__VA_ARGS__)
#define __step__patterns___parseEnums(_Enums...) (_Enums), __step__patterns___parseCapt
#define __step__patterns___parseCapt(_capt...) (pp_overload(__step__patterns___parseCapt, _capt)(_capt)),
#define __step__patterns___parseCapt_1(_capt...) $deref _capt
#define __step__patterns___parseCapt_2(_$opt, _capt...) _$opt, _capt
#define __step__patterns___emit(_Enums, ...) \
    pp_foreach(__step__patterns___each, (__VA_ARGS__), __patterns___expand _Enums)
#define __step__patterns___each(_$opt_w_capt_w_stmts, /*_Enum*/...) __VA_OPT__( \
    __step__patterns___each_emit(__VA_ARGS__, __patterns___expand _$opt_w_capt_w_stmts) \
)
#define __step__patterns___each_emit(...) ____patterns___each(__VA_ARGS__)
#define ____patterns___each(_Enum, _$opt_w_capt, stmts...) \
    pattern_((_Enum)_$opt_w_capt) stmts $end(pattern);
#define __patterns___expand(_Enum...) _Enum
#define $end_patterns

#define default_(...) default __VA_ARGS__: {
#define $end_default \
} break
/* clang-format on */

#define matchedEnum() (__matched_enum)
#define union_matched(/*{ _payload... }*/...) \
    union_with$((MatchingType)(matchedEnum())from$((MatchedType)__VA_ARGS__))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_types_variant__included */
