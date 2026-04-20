/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    enum.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-30 (date of creation)
 * @updated 2025-12-30 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
 */
#ifndef core_pri_enum__included
#define core_pri_enum__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#define $fits(...) , $_fits, __VA_ARGS__

/*((_Alias) $T)*/
/*((_Alias $fits($packed))(...))*/
/*((_Alias $fits($bits(8|16|32|64)))(...))*/
#define enum_(/*(_Alias <$fits(...)>)((_Enum)(_Value),...) <$T>*/...) \
    __type__enum_(__VA_ARGS__)

#define enum_of$(/*(_Alias)(_value)*/...) \
    pp_expand(pp_defer(block_inline__enum_of$)(comp_param__enum_of$ _value))

/*========== Macros and Definitions =========================================*/

#define __type__enum_(...) \
    pp_overload(__step__enum_, __VA_ARGS__)(____enum___parseAlias __VA_ARGS__)

#define ____enum___parseAlias(...) __VA_ARGS__,
#define ____enum___parsePairEnumValueList(_Pair_Enum_Value...) _Pair_Enum_Value

#define __step__enum__1(...) \
    __gen__enum_(__VA_ARGS__)
#define __gen__enum_(...) \
    pp_overload(__gen__enum_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum__2(_Alias, _Pair_Enum_Value...) \
    __gen__enum___emit$_fits(_Alias, , ____enum___parsePairEnumValueList _Pair_Enum_Value)
#define __gen__enum__4(_Alias, _Attr, _Attr_Ctx, _Pair_Enum_Value...) \
    pp_cat(__gen__enum___emit, _Attr)(_Alias, _Attr_Ctx, ____enum___parsePairEnumValueList _Pair_Enum_Value)

#define __gen__enum___emit$_fits(_Alias, _Attr_Ctx, _Pair_Enum_Value...) \
    enum _Attr_Ctx _Alias { \
        _Pair_Enum_Value /* __gen__enum___emitEnumTags(_Pair_Enum_Value) */ \
    }

#if UNUSED_CODE
#define __gen__enum___emitEnumTags(_Pair_Enum_Value...) \
    ____enum___emitEnumTags(_Pair_Enum_Value)
#define ____enum___emitEnumTags(_Pair_Enum_Value...) \
    pp_foreach(__gen__enum___emitEnumTag__each_emit, ~, _Pair_Enum_Value)
#define ____enum___expandPairEnumValue(_Pair_Enum_Value...) _Pair_Enum_Value
#define __gen__enum___emitEnumTag__each_emit(_$ignored, /*_Pair_Enum_Value*/...) __VA_OPT__( \
    __gen__enum___emitEnumTag__each(____enum___expandPairEnumValue __VA_ARGS__) \
)
#define __gen__enum___emitEnumTag__each(/*_Pair_Enum_Value*/...) \
    pp_overload(__gen__enum___emitEnumTag, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum___emitEnumTag_1(_Enum...) \
    _Enum, /* pp_join($, enum, _Enum) = _Enum, */
#define __gen__enum___emitEnumTag_2(_Enum, _Value...) \
    _Enum = _Value, /* pp_join($, enum, _Enum) = _Value, */
#endif /* UNUSED_CODE */

#define __step__enum__2(...) __gen__enum_raw(__VA_ARGS__)
#define __gen__enum_raw(_Alias, _$T...) enum _Alias

#define comp_param__enum_of$(_value...) _value, pp_expand
#define block_inline__enum_of$(_value...) (as$(_Alias)(_value))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri_enum__included */
