#include "dh/prl.h"
#include "dh/main.h"

typedef struct Foo {
    i32 a;
    f32 b;
} Foo;
prl_T_use$(Foo);

op_fn_addWith$(((Foo, f32)(lhs, rhs))(Foo)) {
    return (Foo){
        .a = lhs.a + rhs,
        .b = lhs.b + rhs,
    };
}
op_fn_addWith$(addFlt, ((Foo, f32)(lhs, rhs))(Foo));

#define pp_openBrace() {
#define pp_closeBrace() }

#define $with $_with,
#define $some $_some,
#define $none $_none,

#undef if_
#undef else_
#define if_(/*$extn(_stmt|_expr)(_cond|_capture) | _cond)*/...) \
    pp_overload(__if_, __VA_ARGS__)(__VA_ARGS__)
#define __if__1(_cond...) __syn__if_$_cond(_cond)
#define __if__2(_extn, ...) pp_openBrace() pp_cat(__syn__if_, _extn)(__if__expand __VA_ARGS__)
#define __if__expand(...) __VA_ARGS__,
#define else_(...) \
    pp_overload(__else_, __VA_ARGS__)(__VA_ARGS__)
#define __else__0(_cond...) __syn__else_$_cond(_cond)
#define __else__2(_extn, ...) pp_cat(__syn__else_, _extn)(__else__expand __VA_ARGS__)
#define __else__expand(...) __VA_ARGS__,
#define $done(...) \
    pp_overload(__$done, __VA_ARGS__)(__VA_ARGS__)
#define __$done_0(...) pp_closeBrace()
#define __$done_2(_extn, _$ignore...) pp_cat(__syn__$done, _extn)()

#define __syn__if_$_cond(...) __syn__if_$_cond__emit(__VA_ARGS__)
#define __syn__if_$_cond__emit(_cond...) \
    if (_cond)
#define __syn__else_$_cond(...) __syn__else_$_cond__emit()
#define __syn__else_$_cond__emit() \
    else
#define __syn__if_$_with(...) __syn__if_$_with__emit(__VA_ARGS__)
#define __syn__if_$_with__emit(_init, _cond...) \
    _init; \
    if (_cond)
#define __syn__else_$_with(...) __syn__else_$_with__emit(__VA_ARGS__)
#define __syn__else_$_with__emit(_init...) \
    else pp_openBrace() \
        _init;

/* clang-format off */
#define __syn__if_$_some(...) __syn__if_$_some__emit(__VA_ARGS__)
#define __syn__if_$_some__emit(_expr, _capture...) \
    let __result = _expr; \
    if (__result.is_some) pp_openBrace() \
        let pp_expand _capture = __result.payload.some;
#define __syn__else_$_none(...) __syn__else_$_none__emit()
#define __syn__else_$_none__emit() \
    pp_closeBrace() else
#define __syn__$done$_none(...) __syn__$done$_none__emit()
#define __syn__$done$_none__emit() \
    pp_closeBrace()
#define __syn__if_$_none(...) __syn__if_$_none__emit(__VA_ARGS__)
#define __syn__if_$_none__emit(_expr, _$ignore...) \
    let __result = _expr; \
    if (!__result.is_some)
#define __syn__else_$_some(...) __syn__else_$_some__emit(__VA_ARGS__)
#define __syn__else_$_some__emit(_capture, _$ignore...) \
    else pp_openBrace() \
        let _capture = __result.payload.some;
#define __syn__$done$_some(...) __syn__$done$_some__emit()
#define __syn__$done$_some__emit() \
    pp_closeBrace() pp_closeBrace()
/* clang-format on */

$extern fn_((getValue(void))(i32));
$extern fn_((maybeFoo(void))(O$Foo));

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    if_(args.len > 0) {
        debug_assert(true);
    }
    else {
        claim_unreachable;
    }

    if_($with(let value = getValue())(value > 0)) {
        debug_assert(value > 0);
    }
    else {
        debug_assert(value <= 0);
    }
    $done();

    if_($with(let value = getValue())(value > 0)) {
        debug_assert(value > 0);
    }
    else_() {
        debug_assert(value <= 0);
    }
    $done();

    if_($some(maybeFoo())(foo)) {
        let_ignore = foo;
    }
    $done($some);

    if_($some(maybeFoo())($ignore)) {
        debug_assert(true);
    }
    else_($none) {
        debug_assert(true);
    }
    $done($none);

    if_($none(maybeFoo())) {
        debug_assert(true);
    }
    $done($none);

    if_($none(maybeFoo())) {
        debug_assert(true);
    }
    else_($some(foo)) {
        let_ignore = foo;
    }
    $done($some);

    if_($none(maybeFoo())) {
        debug_assert(true);
    }
    else_($some($ignore)) {
        debug_assert(true);
    }
    $done($some);

    return_ok({});
} $unscoped_(fn);
