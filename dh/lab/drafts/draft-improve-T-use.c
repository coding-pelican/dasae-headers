#include "dh/prl.h"
#include "dh/mem.h"

T_alias$((Foo)(Opaq));

// T_use$((Foo)(P));
// T_use$((P$Foo)(O));
// T_use$((O$P$Foo)(E));

#define $spec(_T, _attr...) (_T, _attr)
#define T_use$(/*(_T)(_List_Parenthesized)*/...) __stmt__T_use$(__VA_ARGS__)
#define __stmt__T_use$(...) __step__T_use$__emit(__step__T_use$__parseT __VA_ARGS__)
#define __step__T_use$__parseT(_T...) (_T),
#define __step__T_use$__emit(...) ____T_use$(__VA_ARGS__)
#define ____T_use$(_T, _List_Parenthesized...) \
    __T__foreach(____T_use$__each, _T, __step__T_use$__unwrap _List_Parenthesized)
#define __step__T_use$__unwrap(_List...) _List
// #define ____T_use$__each(_T, /*_suffix*/...) __VA_OPT__( \
//     pp_cat3(T_use_, __VA_ARGS__, $) _T; \
// )
#define ____T_use$__each(_T, /*_suffix*/...) __VA_OPT__( \
    pp_if_(pp_not(pp_isParen(__VA_ARGS__)))( \
        pp_then_(____T_use$__each_1), \
        pp_else_(__step__T_use$__each_N) \
    )(_T, __VA_ARGS__); \
)
#define __step__T_use$__each_N(_T, ...) ____T_use$__each_N(_T, __step__T_use$__unwrap __VA_ARGS__)
#define ____T_use$__each_N(_T, ...) \
    pp_overload(____T_use$__each, __VA_ARGS__)(_T, __VA_ARGS__)
#define ____T_use$__each_1(_T, _U...) \
    __step__T_use$__each_to_tpl(_U, _T)
#define ____T_use$__each_2(_T, _U, _V...) \
    __step__T_use$__each_to_tpl( \
        _U, __step__T_use$__each_to_type(_V, _T) \
    )
#define ____T_use$__each_3(_T, _U, _V, _W...) \
    __step__T_use$__each_to_tpl( \
        _U, \
        __step__T_use$__each_to_type( \
            _V, __step__T_use$__each_to_type(_W, _T) \
        ) \
    )
#define ____T_use$__each_4(_T, _U, _V, _W, _X...) \
    __step__T_use$__each_to_tpl( \
        _U, \
        __step__T_use$__each_to_type( \
            _V, \
            __step__T_use$__each_to_type( \
                _W, __step__T_use$__each_to_type(_X, _T) \
            ) \
        ) \
    )
#define ____T_use$__each_5(_T, _U, _V, _W, _X, _Y...) \
    __step__T_use$__each_to_tpl( \
        _U, \
        __step__T_use$__each_to_type( \
            _V, \
            __step__T_use$__each_to_type( \
                _W, \
                __step__T_use$__each_to_type( \
                    _X, __step__T_use$__each_to_type(_Y, _T) \
                ) \
            ) \
        ) \
    )
#define ____T_use$__each_6(_T, _U, _V, _W, _X, _Y, _Z...) \
    __step__T_use$__each_to_tpl( \
        _U, \
        __step__T_use$__each_to_type( \
            _V, \
            __step__T_use$__each_to_type( \
                _W, \
                __step__T_use$__each_to_type( \
                    _X, \
                    __step__T_use$__each_to_type( \
                        _Y, __step__T_use$__each_to_type(_Z, _T) \
                    ) \
                ) \
            ) \
        ) \
    )
#define __step__T_use$__each_to_tpl(...) ____T_use$__each_to_tpl(__VA_ARGS__)
#define ____T_use$__each_to_tpl(_U, _T...) \
    pp_if_(pp_not(pp_isParen(_U)))( \
        pp_then_(__step__T_use$__each_to_tpl_plain), \
        pp_else_(__step__T_use$__each_to_tpl_spec) \
    )(_T, _U)
#define __step__T_use$__each_to_tpl_plain(...) ____T_use$__each_to_tpl_plain(__VA_ARGS__)
#define ____T_use$__each_to_tpl_plain(_T, _U...) \
    pp_cat3(T_use_, _U, $) _T
#define __step__T_use$__each_to_tpl_spec(_T, _U_attr...) __step__T_use$__each_to_tpl_spec__emit(__step__T_use$__unwrap _U_attr _T)
#define __step__T_use$__each_to_tpl_spec__emit(...) ____T_use$__each_to_tpl_spec(__VA_ARGS__)
#define ____T_use$__each_to_tpl_spec(_U, _attr_T...) \
    pp_cat3(T_use_, _U, $)(_attr_T)

#define __step__T_use$__each_to_type(...) ____T_use$__each_to_type(__VA_ARGS__)
#define ____T_use$__each_to_type(_U, _T...) ( \
    pp_if_(pp_not(pp_isParen(_U)))( \
        pp_then_(__step__T_use$__each_to_type_plain), \
        pp_else_(__step__T_use$__each_to_type_spec) \
    )(_T, _U) \
)
#define __step__T_use$__each_to_type_plain(...) ____T_use$__each_to_type_plain(__VA_ARGS__)
#define ____T_use$__each_to_type_plain(_T, _U...) \
    pp_cat(_U, $) _T
#define __step__T_use$__each_to_type_spec(_T, _U_attr...) __step__T_use$__each_to_type_spec__emit(__step__T_use$__unwrap _U_attr _T)
#define __step__T_use$__each_to_type_spec__emit(...) ____T_use$__each_to_type_spec(__VA_ARGS__)
#define ____T_use$__each_to_type_spec(_U, _attr_T...) \
    pp_cat(_U, $)(_attr_T)

// #define __step__T_use$__each_to_tpl(...) ____T_use$__each_to_tpl(__VA_ARGS__)
// #define ____T_use$__each_to_tpl(_U, _T...) \
//     pp_if_(pp_not(pp_isParen(_U)))( \
//         pp_then_(pp_cat3(T_use_, _U, $) _T), \
//         pp_else_(__step__T_use$__each_to_tpl_spec(_T, __step__T_use$__unwrap _U)) \
//     )
// #define __step__T_use$__each_to_tpl_spec(...) ____T_use$__each_to_tpl_spec(__VA_ARGS__)
// #define ____T_use$__each_to_tpl_spec(_T, _U, _attr...) \
//     pp_cat3(T_use_, _U, $)(_attr _T)

// #define __step__T_use$__each_to_type(...) ____T_use$__each_to_type(__VA_ARGS__)
// #define ____T_use$__each_to_type(_U, _T...) ( \
//     pp_if_(pp_not(pp_isParen(_U)))( \
//         pp_then_(pp_cat(_U, $) _T), \
//         pp_else_(__step__T_use$__each_to_type_spec(_T, __step__T_use$__unwrap _U)) \
//     ) \
// )
// #define __step__T_use$__each_to_type_spec(...) ____T_use$__each_to_type_spec(__VA_ARGS__)
// #define ____T_use$__each_to_type_spec(_T, _U, _attr...) \
//     pp_cat(_U, $)(_attr _T)

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
    __step__T_use$__each_to_tpl(_T, (__VA_ARGS__)); \
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
