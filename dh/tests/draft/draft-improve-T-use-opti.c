#include "dh/prl.h"
#include "dh/mem.h"

T_alias$((Foo)(Opaq));

// T_use$((Foo)(P));
// T_use$((P$Foo)(O));
// T_use$((O$P$Foo)(E));

#define $spec(_T, _attr...) (_T, _attr)

/*
 * Macro naming convention used in this prototype:
 *
 * - public entry: `T_use$`, `T_useBy$`, `pp_foldl`, `pp_foldr`
 * - stage helper: `__step__<family>__<role>`
 * - bounded overload arm: `__step__<family>__<role>_<N>`
 * - role words:
 *   - `parse`: turn user syntax into internal shape
 *   - `item`: one list item from `T_use$`
 *   - `use`: final `T_use_<suffix>$` emission
 *   - `type`: type-level transformation such as `P$T`, `E$T`
 *   - `fold`: ordered accumulation over a list
 *
 * The goal is that a helper name answers:
 *   family -> role -> bounded arity
 */

#define pp_foldl(_Step, _Init, ...) \
    pp_overload(__step__pp_foldl, __VA_ARGS__)(_Step, _Init, __VA_ARGS__)
#define __step__pp_foldl_0(_Step, _Init...) _Init
#define __step__pp_foldl_1(_Step, _Init, _A...) _Step(_Init, _A)
#define __step__pp_foldl_2(_Step, _Init, _A, _B...) _Step(__step__pp_foldl_1(_Step, _Init, _A), _B)
#define __step__pp_foldl_3(_Step, _Init, _A, _B, _C...) _Step(__step__pp_foldl_2(_Step, _Init, _A, _B), _C)
#define __step__pp_foldl_4(_Step, _Init, _A, _B, _C, _D...) _Step(__step__pp_foldl_3(_Step, _Init, _A, _B, _C), _D)
#define __step__pp_foldl_5(_Step, _Init, _A, _B, _C, _D, _E...) _Step(__step__pp_foldl_4(_Step, _Init, _A, _B, _C, _D), _E)
#define __step__pp_foldl_6(_Step, _Init, _A, _B, _C, _D, _E, _F...) _Step(__step__pp_foldl_5(_Step, _Init, _A, _B, _C, _D, _E), _F)

#define pp_foldr(_Step, _Init, ...) \
    pp_overload(__step__pp_foldr, __VA_ARGS__)(_Step, _Init, __VA_ARGS__)
#define __step__pp_foldr_0(_Step, _Init...) _Init
#define __step__pp_foldr_1(_Step, _Init, _A...) _Step(_A, _Init)
#define __step__pp_foldr_2(_Step, _Init, _A, _B...) _Step(_A, __step__pp_foldr_1(_Step, _Init, _B))
#define __step__pp_foldr_3(_Step, _Init, _A, _B, _C...) _Step(_A, __step__pp_foldr_2(_Step, _Init, _B, _C))
#define __step__pp_foldr_4(_Step, _Init, _A, _B, _C, _D...) _Step(_A, __step__pp_foldr_3(_Step, _Init, _B, _C, _D))
#define __step__pp_foldr_5(_Step, _Init, _A, _B, _C, _D, _E...) _Step(_A, __step__pp_foldr_4(_Step, _Init, _B, _C, _D, _E))
#define __step__pp_foldr_6(_Step, _Init, _A, _B, _C, _D, _E, _F...) _Step(_A, __step__pp_foldr_5(_Step, _Init, _B, _C, _D, _E, _F))

#define T_use$(/*(_BaseT)(_List_Parenthesized)*/...) \
    __step__T_use$__emit(__step__T_use$__parseBase __VA_ARGS__)
#define __step__T_use$__parseBase(_BaseT...) (_BaseT),
#define __step__T_use$__emit(...) __step__T_use$__emit__impl(__VA_ARGS__)
#define __step__T_use$__emit__impl(_BaseT, _UseList_Parenthesized...) \
    __T__foreach(__step__T_use$__item, _BaseT, __step__T_use$__unwrap _UseList_Parenthesized)
#define __step__T_use$__unwrap(_List...) _List

#define __step__T_use$__item(_BaseT, ...) __VA_OPT__( \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(__step__T_use$__use), \
        pp_else_(__step__T_use$__item__paren) \
    )(__VA_ARGS__, _BaseT); \
)

#define __step__T_use$__item__paren(_ItemTuple, _BaseT...) \
    __step__T_use$__item__paren__dispatch(_BaseT, __step__T_use$__unwrap _ItemTuple)
#define __step__T_use$__item__paren__dispatch(_BaseT, ...) \
    pp_overload(__step__T_use$__item__paren__case, __VA_ARGS__)(_BaseT, __VA_ARGS__)
#define __step__T_use$__item__paren__case_1(_BaseT, _Use...) \
    __step__T_use$__use(_Use, _BaseT)
#define __step__T_use$__item__paren__case_2(_BaseT, _Use, ...) \
    __step__T_use$__use(_Use, pp_foldr(__step__T_use$__type__foldStep, _BaseT, __VA_ARGS__))
#define __step__T_use$__item__paren__case_3(_BaseT, _Use, ...) \
    __step__T_use$__use(_Use, pp_foldr(__step__T_use$__type__foldStep, _BaseT, __VA_ARGS__))
#define __step__T_use$__item__paren__case_4(_BaseT, _Use, ...) \
    __step__T_use$__use(_Use, pp_foldr(__step__T_use$__type__foldStep, _BaseT, __VA_ARGS__))
#define __step__T_use$__item__paren__case_5(_BaseT, _Use, ...) \
    __step__T_use$__use(_Use, pp_foldr(__step__T_use$__type__foldStep, _BaseT, __VA_ARGS__))
#define __step__T_use$__item__paren__case_6(_BaseT, _Use, ...) \
    __step__T_use$__use(_Use, pp_foldr(__step__T_use$__type__foldStep, _BaseT, __VA_ARGS__))

#define __step__T_use$__use(_Use, _Type...) \
    pp_if_(pp_not(pp_isParen(_Use)))( \
        pp_then_(__step__T_use$__use__plain), \
        pp_else_(__step__T_use$__use__spec) \
    )(_Use, _Type)
#define __step__T_use$__use__plain(_Use, _Type...) \
    pp_cat3(T_use_, _Use, $) _Type
#define __step__T_use$__use__spec(_SpecTuple, _Type...) \
    __step__T_use$__use__spec__emit(__step__T_use$__unwrap _SpecTuple _Type)
#define __step__T_use$__use__spec__emit(...) \
    __step__T_use$__use__spec__impl(__VA_ARGS__)
#define __step__T_use$__use__spec__impl(_Use, _AttrType...) \
    pp_cat3(T_use_, _Use, $)(_AttrType)

#define __step__T_use$__type(_TypeStep, _InnerType...) ( \
    pp_if_(pp_not(pp_isParen(_TypeStep)))( \
        pp_then_(__step__T_use$__type__plain), \
        pp_else_(__step__T_use$__type__spec) \
    )(_TypeStep, _InnerType) \
)
#define __step__T_use$__type__plain(_TypeStep, _InnerType...) \
    pp_cat(_TypeStep, $) _InnerType
#define __step__T_use$__type__spec(_SpecTuple, _InnerType...) \
    __step__T_use$__type__spec__emit(__step__T_use$__unwrap _SpecTuple _InnerType)
#define __step__T_use$__type__spec__emit(...) \
    __step__T_use$__type__spec__impl(__VA_ARGS__)
#define __step__T_use$__type__spec__impl(_TypeStep, _AttrType...) \
    pp_cat(_TypeStep, $)(_AttrType)
#define __step__T_use$__type__foldStep(_TypeStep, _AccumulatedType...) \
    __step__T_use$__type(_TypeStep, _AccumulatedType)

T_use$((Foo)(
    P,
    (O, P),
    (E, O, P),
    ($spec(E, $set(mem_E))),
    ($spec(E, $set(mem_E)), O, P)
));

// T_use_P$(Foo);
// T_use_O$(P$(Foo));
// T_use_E$(O$(P$(Foo)));
// T_use_E$($set(mem_E)(Foo))
// T_use_E$($set(mem_E)(O$(P$(Foo))));

#define T_useBy$(/*(_T...)(_List_Parenthesized)*/...) __stmt__T_useBy$(__VA_ARGS__)
#define __stmt__T_useBy$(...) __step__T_useBy$__emit(__step__T_useBy$__parseT __VA_ARGS__)
#define __step__T_useBy$__parseT(_T...) _T,
#define __step__T_useBy$__emit(...) ____T_useBy$(__VA_ARGS__)
#define ____T_useBy$(_T, _List_Parenthesized...) \
    __T__foreach(____T_useBy$__each, _T, __step__T_useBy$__unwrap _List_Parenthesized)
#define __step__T_useBy$__unwrap(_List...) _List
#define ____T_useBy$__each(_T, /*_suffix*/...) __VA_OPT__( \
    __step__T_use$__use(_T, (__VA_ARGS__)); \
)

T_useBy$((S)(
    Foo,
    P$Foo,
    O$P$Foo
));
T_useBy$(($spec(E, $set(mem_E)))(
    P$Foo,
    S$Foo
));
// T_use_E$($set(mem_E)(Foo));
// T_use_E$($set(mem_E)(P$Foo));
// T_use_E$($set(mem_E)(O$P$Foo));
