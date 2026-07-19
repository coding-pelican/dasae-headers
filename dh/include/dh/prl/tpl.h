#pragma once
#ifndef prl_tpl__included
#define prl_tpl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "tpl/V-Val.h"
#include "tpl/P-Ptr.h"

#include "tpl/A-Arr.h"
#include "tpl/S-Sli.h"

#include "tpl/O-Opt.h"
#include "tpl/E-ErrSet.h"
#include "tpl/E-ErrRes.h"

#include "tpl/enum.h"
#include "tpl/variant.h"

#include "tpl/Tup.h"

/*========== Macros and Declarations ========================================*/

#define T_alias$(/*(_$Alias)(_$T)*/...) __stmt__T_alias$(__VA_ARGS__)
#define T_embed$(_$T...) $suppress_microsoft_anon_tag(_$T)

#define $spec(_$T, _$attr...) (_$T, _$attr)
#define $via(_$Use, _$TypeStep...) ((_$Use), (_$TypeStep))
#define T_decl$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_decl$(__VA_ARGS__)
#define T_impl$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_impl$(__VA_ARGS__)
#define T_use$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_use$(__VA_ARGS__)

#define T_declBy$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_declBy$(__VA_ARGS__)
#define T_implBy$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_implBy$(__VA_ARGS__)
#define T_useBy$(/*(_$T)(_$List_Parenthesized)*/...) __stmt__T_useBy$(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __stmt__T_alias$(...) __syn__T_alias$__emit(__pp__T_alias$__parseAlias __VA_ARGS__)
#define __pp__T_alias$__parseAlias(_$Alias...) _$Alias, __pp__T_alias$__parseT
#define __pp__T_alias$__parseT(_$T...) _$T
#define __syn__T_alias$__emit(...) __syn__T_alias$__impl(__VA_ARGS__)
#define __syn__T_alias$__impl(_$Alias, _$T...) typedef _$T _$Alias

#define __stmt__T_decl$(...) __pp__T_decl$__emit(__pp__T_decl$__parseBase __VA_ARGS__)
#define __pp__T_decl$__parseBase(_$BaseT...) (_$BaseT),
#define __pp__T_decl$__emit(...) __pp__T_decl$__impl(__VA_ARGS__)
#define __pp__T_decl$__impl(_$BaseT, _$List_Parenthesized...) \
    __T__foreach(__pp__T_decl$__each, _$BaseT, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_decl$__each(_$BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_decl_, _$BaseT, __VA_ARGS__); \
)

#define __stmt__T_impl$(...) __pp__T_impl$__emit(__pp__T_impl$__parseBase __VA_ARGS__)
#define __pp__T_impl$__parseBase(_$BaseT...) (_$BaseT),
#define __pp__T_impl$__emit(...) __pp__T_impl$__impl(__VA_ARGS__)
#define __pp__T_impl$__impl(_$BaseT, _$List_Parenthesized...) \
    __T__foreach(__pp__T_impl$__each, _$BaseT, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_impl$__each(_$BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_impl_, _$BaseT, __VA_ARGS__); \
)

#define __stmt__T_use$(...) __pp__T_use$__emit(__pp__T_use$__parseBase __VA_ARGS__)
#define __pp__T_use$__parseBase(_$BaseT...) (_$BaseT),
#define __pp__T_use$__emit(...) __pp__T_use$__impl(__VA_ARGS__)
#define __pp__T_use$__impl(_$BaseT, _$List_Parenthesized...) \
    __T__foreach(__pp__T_use$__each, _$BaseT, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_use$__each(_$BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_use_, _$BaseT, __VA_ARGS__); \
)

#define __stmt__T_declBy$(...) __pp__T_declBy$__emit(__pp__T_declBy$__parseItem __VA_ARGS__)
#define __pp__T_declBy$__parseItem(_$Item...) (_$Item),
#define __pp__T_declBy$__emit(...) __pp__T_declBy$__impl(__VA_ARGS__)
#define __pp__T_declBy$__impl(_$Item, _$List_Parenthesized...) \
    __T__foreach(__pp__T_declBy$__each, _$Item, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_declBy$__each(_$Item, _$Type...) \
    __pp__T__apply$__byItem(T_decl_, (_$Type), _$Item);

#define __stmt__T_implBy$(...) __pp__T_implBy$__emit(__pp__T_implBy$__parseItem __VA_ARGS__)
#define __pp__T_implBy$__parseItem(_$Item...) (_$Item),
#define __pp__T_implBy$__emit(...) __pp__T_implBy$__impl(__VA_ARGS__)
#define __pp__T_implBy$__impl(_$Item, _$List_Parenthesized...) \
    __T__foreach(__pp__T_implBy$__each, _$Item, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_implBy$__each(_$Item, _$Type...) \
    __pp__T__apply$__byItem(T_impl_, (_$Type), _$Item);

#define __stmt__T_useBy$(...) __pp__T_useBy$__emit(__pp__T_useBy$__parseItem __VA_ARGS__)
#define __pp__T_useBy$__parseItem(_$Item...) (_$Item),
#define __pp__T_useBy$__emit(...) __pp__T_useBy$__impl(__VA_ARGS__)
#define __pp__T_useBy$__impl(_$Item, _$List_Parenthesized...) \
    __T__foreach(__pp__T_useBy$__each, _$Item, __pp__T__apply$__expand _$List_Parenthesized)
#define __pp__T_useBy$__each(_$Item, _$Type...) \
    __pp__T__apply$__byItem(T_use_, (_$Type), _$Item);

#define __pp__T__apply$__expand(_$List...) _$List

#define __pp__T__apply$__item(_$Prefix, _$BaseT, ...) __VA_OPT__( \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(__pp__T__apply$__emit), \
        pp_else_(__pp__T__apply$__item__paren) \
    )(_$Prefix, __VA_ARGS__, _$BaseT) \
)

#define __pp__T__apply$__item__paren(_$Prefix, _$ItemTuple, _$BaseT...) \
    __pp__T__apply$__item__route(_$Prefix, _$BaseT, __pp__T__apply$__expand _$ItemTuple)
#define __pp__T__apply$__item__route(_$Prefix, _$BaseT, ...) \
    pp_overload(__pp__T__apply$__item__case, __VA_ARGS__)(_$Prefix, _$BaseT, __VA_ARGS__)
#define __pp__T__apply$__item__case_1(_$Prefix, _$BaseT, _$Use...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, _$BaseT)
#define __pp__T__apply$__item__case_2(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_3(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_4(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_5(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_6(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_7(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_8(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_9(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_10(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_11(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_12(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_13(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_14(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_15(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_16(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_17(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_18(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_19(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_20(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_21(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_22(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_23(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_24(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_25(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_26(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_27(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_28(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_29(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_30(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_31(_$Prefix, _$BaseT, _$Use, ...) \
    __pp__T__apply$__emit(_$Prefix, _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __VA_ARGS__))

#define __pp__T__apply$__byItem(_$Prefix, _$BaseT, _$Item...) \
    __pp__T__apply$__byItem__route(_$Prefix, _$BaseT, __pp__T__apply$__expand _$Item)
#define __pp__T__apply$__byItem__route(_$Prefix, _$BaseT, ...) \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(__pp__T__apply$__emit), \
        pp_else_(__pp__T__apply$__byItem__parenPayload) \
    )(_$Prefix, __VA_ARGS__, _$BaseT)
#define __pp__T__apply$__byItem__parenPayload(_$Prefix, _$PayloadTuple, _$BaseT...) \
    __pp__T__apply$__byItem__parenPayload__route(_$Prefix, _$BaseT, _$PayloadTuple, __pp__T__apply$__expand _$PayloadTuple)
#define __pp__T__apply$__byItem__parenPayload__route(_$Prefix, _$BaseT, _$PayloadTuple, ...) \
    pp_overload(__pp__T__apply$__byItem__parenPayload__case, __VA_ARGS__)(_$Prefix, _$BaseT, _$PayloadTuple, __VA_ARGS__)
#define __pp__T__apply$__byItem__parenPayload__case_1(_$Prefix, _$BaseT, _$PayloadTuple, _Only...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_2(_$Prefix, _$BaseT, _$PayloadTuple, _$Use, _$ViaList...) \
    pp_if_(pp_and(pp_isParen(_$Use), pp_isParen(_$ViaList)))( \
        pp_then_(__pp__T__apply$__byItem__via), \
        pp_else_(__pp__T__apply$__byItem__direct) \
    )(_$Prefix, _$BaseT, _$PayloadTuple, _$Use, _$ViaList)
#define __pp__T__apply$__byItem__parenPayload__case_3(_$Prefix, _$BaseT, _$PayloadTuple, _$A, _$B, _$C...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_4(_$Prefix, _$BaseT, _$PayloadTuple, _$A, _$B, _$C, _$D...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_5(_$Prefix, _$BaseT, _$PayloadTuple, _$A, _$B, _$C, _$D, _$E...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_6(_$Prefix, _$BaseT, _$PayloadTuple, _$A, _$B, _$C, _$D, _$E, _$F...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__direct(_$Prefix, _$BaseT, _$PayloadTuple, _$Use, _$ViaList...) \
    __pp__T__apply$__emit(_$Prefix, _$PayloadTuple, _$BaseT)
#define __pp__T__apply$__byItem__via(_$Prefix, _$BaseT, _$PayloadTuple, _$Use, _$ViaList...) \
    __pp__T__apply$__emit(_$Prefix, __pp__T__apply$__expand _$Use, pp_foldr(__type__T_apply$__fold, _$BaseT, __pp__T__apply$__expand _$ViaList))

#define __pp__T__apply$__emit(_$Prefix, _$Use, _$Type...) \
    pp_if_(pp_not(pp_isParen(_$Use)))( \
        pp_then_(__pp__T__apply$__emit__plain), \
        pp_else_(__pp__T__apply$__emit__spec) \
    )(_$Prefix, _$Use, _$Type)
#define __pp__T__apply$__emit__plain(_$Prefix, _$Use, _$Type...) \
    pp_cat3(_$Prefix, _$Use, $) _$Type
#define __pp__T__apply$__emit__spec(_$Prefix, _$SpecTuple, _$Type...) \
    __pp__T__apply$__emit__spec__expand(_$Prefix, __pp__T__apply$__expand _$SpecTuple _$Type)
#define __pp__T__apply$__emit__spec__expand(...) __pp__T__apply$__emit__spec__impl(__VA_ARGS__)
#define __pp__T__apply$__emit__spec__impl(_$Prefix, _$Use, _$AttrType...) \
    pp_cat3(_$Prefix, _$Use, $)(_$AttrType)

#define __type__T_apply$__apply(_$TypeStep, _$InnerType...) ( \
    pp_if_(pp_not(pp_isParen(_$TypeStep)))( \
        pp_then_(__type__T_apply$__plain), \
        pp_else_(__type__T_apply$__spec) \
    )(_$TypeStep, _$InnerType) \
)
#define __type__T_apply$__plain(_$TypeStep, _$InnerType...) \
    pp_cat(_$TypeStep, $) _$InnerType
#define __type__T_apply$__spec(_$SpecTuple, _$InnerType...) \
    __type__T_apply$__spec__expand(__pp__T__apply$__expand _$SpecTuple _$InnerType)
#define __type__T_apply$__spec__expand(...) __type__T_apply$__spec__impl(__VA_ARGS__)
#define __type__T_apply$__spec__impl(_$TypeStep, _$AttrType...) \
    pp_cat(_$TypeStep, $)(_$AttrType)
#define __type__T_apply$__fold(_$TypeStep, _$AccumulatedType...) \
    __type__T_apply$__apply(_$TypeStep, _$AccumulatedType)

/*--- internal foreach ---*/

#define __T__foreach(_$Exec, _$Iter, ...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * __T__foreach(exec, iter, x, y, z): \
     * => exec(iter, x) exec(iter, y) exec(iter, z) \
     */ \
    __T__exec_foreach(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach(_$Exec, _$Iter, ...) \
    pp_overload(__T__exec_foreach, __VA_ARGS__)(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_0(...)
#define __T__exec_foreach_1(_$Exec, _$Iter, _$Arg) _$Exec(_$Iter, _$Arg) __T__exec_foreach_0()
#define __T__exec_foreach_2(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_1(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_3(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_2(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_4(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_3(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_5(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_4(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_6(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_5(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_7(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_6(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_8(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_7(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_9(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_8(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_10(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_9(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_11(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_10(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_12(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_11(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_13(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_12(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_14(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_13(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_15(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_14(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_16(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_15(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_17(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_16(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_18(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_17(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_19(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_18(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_20(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_19(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_21(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_20(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_22(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_21(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_23(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_22(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_24(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_23(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_25(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_24(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_26(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_25(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_27(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_26(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_28(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_27(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_29(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_28(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_30(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_29(_$Exec, _$Iter, __VA_ARGS__)
#define __T__exec_foreach_31(_$Exec, _$Iter, _$Arg, ...) _$Exec(_$Iter, _$Arg) __T__exec_foreach_30(_$Exec, _$Iter, __VA_ARGS__)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl__included */
