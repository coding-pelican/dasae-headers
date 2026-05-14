#ifndef prl_tpl__included
#define prl_tpl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "tpl/Val.h"
#include "tpl/Ptr.h"
#include "tpl/Arr.h"
#include "tpl/Sli.h"
#include "tpl/Opt.h"
#include "tpl/ErrSet.h"
#include "tpl/ErrRes.h"
#include "tpl/variant.h"
#include "tpl/Tup.h"

/*========== Macros and Declarations ========================================*/

#define T_alias$(/*(_Alias)(_T)*/...) __stmt__T_alias$(__VA_ARGS__)
#define T_embed$(_T...) $supress_microsoft_anon_tag(_T)

#define $spec(_T, _attr...) (_T, _attr)
#define $via(_Use, _TypeStep...) ((_Use), (_TypeStep))
#define T_decl$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_decl$(__VA_ARGS__)
#define T_impl$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_impl$(__VA_ARGS__)
#define T_use$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_use$(__VA_ARGS__)
#define T_declBy$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_declBy$(__VA_ARGS__)
#define T_implBy$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_implBy$(__VA_ARGS__)
#define T_useBy$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_useBy$(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __stmt__T_alias$(...) __syn__T_alias$__emit(__pp__T_alias$__parseAlias __VA_ARGS__)
#define __pp__T_alias$__parseAlias(_Alias...) _Alias, __pp__T_alias$__parseT
#define __pp__T_alias$__parseT(_T...) _T
#define __syn__T_alias$__emit(...) __syn__T_alias$__impl(__VA_ARGS__)
#define __syn__T_alias$__impl(_Alias, _T...) typedef _T _Alias

#define __stmt__T_decl$(...) __pp__T_decl$__emit(__pp__T_decl$__parseBase __VA_ARGS__)
#define __pp__T_decl$__parseBase(_BaseT...) (_BaseT),
#define __pp__T_decl$__emit(...) __pp__T_decl$__impl(__VA_ARGS__)
#define __pp__T_decl$__impl(_BaseT, _List_Parenthesized...) \
    __T__foreach(__pp__T_decl$__each, _BaseT, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_decl$__each(_BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_decl_, _BaseT, __VA_ARGS__); \
)

#define __stmt__T_impl$(...) __pp__T_impl$__emit(__pp__T_impl$__parseBase __VA_ARGS__)
#define __pp__T_impl$__parseBase(_BaseT...) (_BaseT),
#define __pp__T_impl$__emit(...) __pp__T_impl$__impl(__VA_ARGS__)
#define __pp__T_impl$__impl(_BaseT, _List_Parenthesized...) \
    __T__foreach(__pp__T_impl$__each, _BaseT, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_impl$__each(_BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_impl_, _BaseT, __VA_ARGS__); \
)

#define __stmt__T_use$(...) __pp__T_use$__emit(__pp__T_use$__parseBase __VA_ARGS__)
#define __pp__T_use$__parseBase(_BaseT...) (_BaseT),
#define __pp__T_use$__emit(...) __pp__T_use$__impl(__VA_ARGS__)
#define __pp__T_use$__impl(_BaseT, _List_Parenthesized...) \
    __T__foreach(__pp__T_use$__each, _BaseT, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_use$__each(_BaseT, ...) __VA_OPT__( \
    __pp__T__apply$__item(T_use_, _BaseT, __VA_ARGS__); \
)

#define __stmt__T_declBy$(...) __pp__T_declBy$__emit(__pp__T_declBy$__parseItem __VA_ARGS__)
#define __pp__T_declBy$__parseItem(_Item...) (_Item),
#define __pp__T_declBy$__emit(...) __pp__T_declBy$__impl(__VA_ARGS__)
#define __pp__T_declBy$__impl(_Item, _List_Parenthesized...) \
    __T__foreach(__pp__T_declBy$__each, _Item, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_declBy$__each(_Item, _Type...) \
    __pp__T__apply$__byItem(T_decl_, (_Type), _Item);

#define __stmt__T_implBy$(...) __pp__T_implBy$__emit(__pp__T_implBy$__parseItem __VA_ARGS__)
#define __pp__T_implBy$__parseItem(_Item...) (_Item),
#define __pp__T_implBy$__emit(...) __pp__T_implBy$__impl(__VA_ARGS__)
#define __pp__T_implBy$__impl(_Item, _List_Parenthesized...) \
    __T__foreach(__pp__T_implBy$__each, _Item, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_implBy$__each(_Item, _Type...) \
    __pp__T__apply$__byItem(T_impl_, (_Type), _Item);

#define __stmt__T_useBy$(...) __pp__T_useBy$__emit(__pp__T_useBy$__parseItem __VA_ARGS__)
#define __pp__T_useBy$__parseItem(_Item...) (_Item),
#define __pp__T_useBy$__emit(...) __pp__T_useBy$__impl(__VA_ARGS__)
#define __pp__T_useBy$__impl(_Item, _List_Parenthesized...) \
    __T__foreach(__pp__T_useBy$__each, _Item, __pp__T__apply$__expand _List_Parenthesized)
#define __pp__T_useBy$__each(_Item, _Type...) \
    __pp__T__apply$__byItem(T_use_, (_Type), _Item);

#define __pp__T__apply$__expand(_List...) _List

#define __pp__T__apply$__item(_Prefix, _BaseT, ...) __VA_OPT__( \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(__pp__T__apply$__emit), \
        pp_else_(__pp__T__apply$__item__paren) \
    )(_Prefix, __VA_ARGS__, _BaseT) \
)

#define __pp__T__apply$__item__paren(_Prefix, _ItemTuple, _BaseT...) \
    __pp__T__apply$__item__route(_Prefix, _BaseT, __pp__T__apply$__expand _ItemTuple)
#define __pp__T__apply$__item__route(_Prefix, _BaseT, ...) \
    pp_overload(__pp__T__apply$__item__case, __VA_ARGS__)(_Prefix, _BaseT, __VA_ARGS__)
#define __pp__T__apply$__item__case_1(_Prefix, _BaseT, _Use...) \
    __pp__T__apply$__emit(_Prefix, _Use, _BaseT)
#define __pp__T__apply$__item__case_2(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_3(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_4(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_5(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_6(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_7(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_8(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_9(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_10(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_11(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_12(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_13(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_14(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_15(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_16(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_17(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_18(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_19(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_20(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_21(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_22(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_23(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_24(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_25(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_26(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_27(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_28(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_29(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_30(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))
#define __pp__T__apply$__item__case_31(_Prefix, _BaseT, _Use, ...) \
    __pp__T__apply$__emit(_Prefix, _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __VA_ARGS__))

#define __pp__T__apply$__byItem(_Prefix, _BaseT, _Item...) \
    __pp__T__apply$__byItem__route(_Prefix, _BaseT, __pp__T__apply$__expand _Item)
#define __pp__T__apply$__byItem__route(_Prefix, _BaseT, ...) \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(__pp__T__apply$__emit), \
        pp_else_(__pp__T__apply$__byItem__parenPayload) \
    )(_Prefix, __VA_ARGS__, _BaseT)
#define __pp__T__apply$__byItem__parenPayload(_Prefix, _PayloadTuple, _BaseT...) \
    __pp__T__apply$__byItem__parenPayload__route(_Prefix, _BaseT, _PayloadTuple, __pp__T__apply$__expand _PayloadTuple)
#define __pp__T__apply$__byItem__parenPayload__route(_Prefix, _BaseT, _PayloadTuple, ...) \
    pp_overload(__pp__T__apply$__byItem__parenPayload__case, __VA_ARGS__)(_Prefix, _BaseT, _PayloadTuple, __VA_ARGS__)
#define __pp__T__apply$__byItem__parenPayload__case_1(_Prefix, _BaseT, _PayloadTuple, _Only...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_2(_Prefix, _BaseT, _PayloadTuple, _Use, _ViaList...) \
    pp_if_(pp_and(pp_isParen(_Use), pp_isParen(_ViaList)))( \
        pp_then_(__pp__T__apply$__byItem__via), \
        pp_else_(__pp__T__apply$__byItem__direct) \
    )(_Prefix, _BaseT, _PayloadTuple, _Use, _ViaList)
#define __pp__T__apply$__byItem__parenPayload__case_3(_Prefix, _BaseT, _PayloadTuple, _A, _B, _C...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_4(_Prefix, _BaseT, _PayloadTuple, _A, _B, _C, _D...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_5(_Prefix, _BaseT, _PayloadTuple, _A, _B, _C, _D, _E...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__parenPayload__case_6(_Prefix, _BaseT, _PayloadTuple, _A, _B, _C, _D, _E, _F...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__direct(_Prefix, _BaseT, _PayloadTuple, _Use, _ViaList...) \
    __pp__T__apply$__emit(_Prefix, _PayloadTuple, _BaseT)
#define __pp__T__apply$__byItem__via(_Prefix, _BaseT, _PayloadTuple, _Use, _ViaList...) \
    __pp__T__apply$__emit(_Prefix, __pp__T__apply$__expand _Use, pp_foldr(__type__T_apply$__fold, _BaseT, __pp__T__apply$__expand _ViaList))

#define __pp__T__apply$__emit(_Prefix, _Use, _Type...) \
    pp_if_(pp_not(pp_isParen(_Use)))( \
        pp_then_(__pp__T__apply$__emit__plain), \
        pp_else_(__pp__T__apply$__emit__spec) \
    )(_Prefix, _Use, _Type)
#define __pp__T__apply$__emit__plain(_Prefix, _Use, _Type...) \
    pp_cat3(_Prefix, _Use, $) _Type
#define __pp__T__apply$__emit__spec(_Prefix, _SpecTuple, _Type...) \
    __pp__T__apply$__emit__spec__expand(_Prefix, __pp__T__apply$__expand _SpecTuple _Type)
#define __pp__T__apply$__emit__spec__expand(...) __pp__T__apply$__emit__spec__impl(__VA_ARGS__)
#define __pp__T__apply$__emit__spec__impl(_Prefix, _Use, _AttrType...) \
    pp_cat3(_Prefix, _Use, $)(_AttrType)

#define __type__T_apply$__apply(_TypeStep, _InnerType...) ( \
    pp_if_(pp_not(pp_isParen(_TypeStep)))( \
        pp_then_(__type__T_apply$__plain), \
        pp_else_(__type__T_apply$__spec) \
    )(_TypeStep, _InnerType) \
)
#define __type__T_apply$__plain(_TypeStep, _InnerType...) \
    pp_cat(_TypeStep, $) _InnerType
#define __type__T_apply$__spec(_SpecTuple, _InnerType...) \
    __type__T_apply$__spec__expand(__pp__T__apply$__expand _SpecTuple _InnerType)
#define __type__T_apply$__spec__expand(...) __type__T_apply$__spec__impl(__VA_ARGS__)
#define __type__T_apply$__spec__impl(_TypeStep, _AttrType...) \
    pp_cat(_TypeStep, $)(_AttrType)
#define __type__T_apply$__fold(_TypeStep, _AccumulatedType...) \
    __type__T_apply$__apply(_TypeStep, _AccumulatedType)

/* --- internal foreach --- */

#define __T__foreach(_Exec, _Iter, ...) \
    /** \
     * @note handles up to 32 arguments \
     * \
     * __T__foreach(exec, iter, x, y, z): \
     * => exec(iter, x) exec(iter, y) exec(iter, z) \
     */ \
    __T__exec_foreach(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach(_Exec, _Iter, ...) \
    pp_overload(__T__exec_foreach, __VA_ARGS__)(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_0(...)
#define __T__exec_foreach_1(_Exec, _Iter, _Arg) _Exec(_Iter, _Arg) __T__exec_foreach_0()
#define __T__exec_foreach_2(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_1(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_3(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_2(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_4(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_3(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_5(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_4(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_6(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_5(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_7(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_6(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_8(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_7(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_9(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_8(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_10(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_9(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_11(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_10(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_12(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_11(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_13(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_12(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_14(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_13(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_15(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_14(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_16(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_15(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_17(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_16(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_18(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_17(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_19(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_18(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_20(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_19(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_21(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_20(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_22(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_21(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_23(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_22(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_24(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_23(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_25(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_24(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_26(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_25(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_27(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_26(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_28(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_27(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_29(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_28(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_30(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_29(_Exec, _Iter, __VA_ARGS__)
#define __T__exec_foreach_31(_Exec, _Iter, _Arg, ...) _Exec(_Iter, _Arg) __T__exec_foreach_30(_Exec, _Iter, __VA_ARGS__)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl__included */
