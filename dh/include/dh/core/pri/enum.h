/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    enum.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-30 (date of creation)
 * @updated 2026-08-02 (date of last update)
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
 */
#pragma once
#ifndef core_pri_enum__included
#define core_pri_enum__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#define $fits(...) , $_fits, __VA_ARGS__

/*((_$Alias) $T)*/
/*((_$Alias $fits($packed))(...))*/
/*((_$Alias $fits($bits(8|16|32|64)))(...))*/
#define enum_(/*(_$Alias <$fits(...)>)((_$Enum)(_$Val),...) <$T>*/...) \
    __type__enum_(__VA_ARGS__)
#define enum_of$(/*(_$Alias)(_$val)*/...) \
    __step__enum_of$(__VA_ARGS__)

#define intToEnum$(/*(_$EnumType)(_$val: IntType)*/... /*(_$EnumType)*/) \
    __step__intToEnum$(__VA_ARGS__)
#define enumFromInt$ __alias__enumFromInt$
#define enumToInt$(/*(_$IntType)(_$val: EnumType)*/... /*(_$IntType)*/) \
    __step__enumToInt$(__VA_ARGS__)
#define intFromEnum$ __alias__intFromEnum$

#define switch_(/*(_$Init)(_$Cond...)*/...) __syn__switch_(__syn__switch___parseInit __VA_ARGS__)
// #define case_(/*(_$Enum)({...})*/...) __syn__case_(__syn__case___parseEnum __VA_ARGS__)
// #define default_(/*({...})*/...) __syn__default_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __type__enum_(...) \
    pp_overload(__step__enum_, __VA_ARGS__)(____enum___parseAlias __VA_ARGS__)

#define ____enum___parseAlias(...) __VA_ARGS__,
#define ____enum___parsePairEnumValueList(_$Pair_Enum_Value...) _$Pair_Enum_Value

#define __step__enum__1(...) \
    __gen__enum_(__VA_ARGS__)
#define __gen__enum_(...) \
    pp_overload(__gen__enum_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum__2(_$Alias, _$Pair_Enum_Value...) \
    __gen__enum___emit$_fits(_$Alias, , ____enum___parsePairEnumValueList _$Pair_Enum_Value)
#define __gen__enum__4(_$Alias, _Attr, _$Attr_Ctx, _$Pair_Enum_Value...) \
    pp_cat(__gen__enum___emit, _Attr)(_$Alias, _$Attr_Ctx, ____enum___parsePairEnumValueList _$Pair_Enum_Value)

#define __gen__enum___emit$_fits(_$Alias, _$Attr_Ctx, _$Pair_Enum_Value...) \
    enum _$Attr_Ctx _$Alias { \
        _$Pair_Enum_Value /* __gen__enum___emitEnumTags(_$Pair_Enum_Value) */ \
    }

#if UNUSED_CODE
#define __gen__enum___emitEnumTags(_$Pair_Enum_Value...) \
    ____enum___emitEnumTags(_$Pair_Enum_Value)
#define ____enum___emitEnumTags(_$Pair_Enum_Value...) \
    pp_foreach(__gen__enum___emitEnumTag__each_emit, ~, _$Pair_Enum_Value)
#define ____enum___expandPairEnumValue(_$Pair_Enum_Value...) _$Pair_Enum_Value
#define __gen__enum___emitEnumTag__each_emit(_$ignored, /*_$Pair_Enum_Value*/...) __VA_OPT__( \
    __gen__enum___emitEnumTag__each(____enum___expandPairEnumValue __VA_ARGS__) \
)
#define __gen__enum___emitEnumTag__each(/*_$Pair_Enum_Value*/...) \
    pp_overload(__gen__enum___emitEnumTag, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum___emitEnumTag_1(_$Enum...) \
    _$Enum, /* pp_join($, enum, _$Enum) = _$Enum, */
#define __gen__enum___emitEnumTag_2(_$Enum, _$Val...) \
    _$Enum = _$Val, /* pp_join($, enum, _$Enum) = _$Val, */
#endif /* UNUSED_CODE */

#define __step__enum__2(...) __gen__enum_raw(__VA_ARGS__)
#define __gen__enum_raw(_$Alias, _$T...) enum _$Alias
#define __step__enum_of$(...) (as$ __VA_ARGS__)

#define __step__enumToInt$(...) __step__enumToInt$__emit(__step__enumToInt$__parse __VA_ARGS__)
#define __step__enumToInt$__parse(_$IntType...) _$IntType,
#define __step__enumToInt$__emit(...) ____enumToInt$(__VA_ARGS__)
#define ____enumToInt$(_$IntType, _$val...) (as$(_$IntType)(_$val))
#define __alias__enumFromInt$ intToEnum$

#define __step__intToEnum$(...) __step__intToEnum$__emit(__step__intToEnum$__parse __VA_ARGS__)
#define __step__intToEnum$__parse(_$EnumType...) _$EnumType,
#define __step__intToEnum$__emit(...) ____intToEnum$(__VA_ARGS__)
#define ____intToEnum$(_$EnumType, _$val...) (as$(_$EnumType)(_$val))
#define __alias__intFromEnum$ enumToInt$

#define __syn__switch_(...) __syn__switch___emit(__VA_ARGS__)
#define __syn__switch___parseInit(_$Init...) pp_uniqTok(run_once), (_$Init), __syn__switch___parseCond
#define __syn__switch___parseCond(_$Cond...) (_$Cond)
#define __syn__switch___emit(__run_once, _$Init, _$Cond...) \
    for (bool __run_once = false; !__run_once;) \
        for (__syn__switch___expand _$Init; !__run_once; __run_once = true) switch (__syn__switch___expand _$Cond)
#define __syn__switch___expand(...) __VA_ARGS__

#define __syn__case_(...) __syn__case___emit(__VA_ARGS__)
#define __syn__case___parseEnum(_$Enum...) _$Enum,
#define __syn__case___emit(_$Enum, _$Body...) \
    case _$Enum: \
        __syn__case___expand _$Body
#define __syn__case___expand(...) __VA_ARGS__

#define __syn__default_(...) __syn__default___emit(__VA_ARGS__)
#define __syn__default___emit(_$Body...) \
    default: \
        __syn__default___expand _$Body
#define __syn__default___expand(...) __VA_ARGS__

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri_enum__included */
