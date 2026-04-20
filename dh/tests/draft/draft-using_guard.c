#include "dh-main.h"
#include "dh/fs/File.h"
#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/io/Reader.h"
#include "dh/mem/common.h"
#include "dh/fmt/common.h"


$attr($maybe_unused)
$static fn_((oldIfSome(O$S$u8 cond))(S_const$u8)) {
    if_some((cond)(str)) {
        return str.as_const;
    } else_none {
        return u8_l("default");
    };
    claim_unreachable;
};


#define $defer , $_defer
#define $defer_(/*_fini*/...) , $_defer_(__VA_ARGS__)
#define $errdefer_(/*(_capt)(_fini...)*/...) , $_errdefer_(__VA_ARGS__)

/* clang-format off */
#define $end(_keyword) ; \
    pp_cat($end_, _keyword)
#define $deferral(_keyword) ; \
    pp_cat($deferral_, _keyword)
/* clang-format on */

#define using_(/*(_init...) <$defer|$defer_(_fini...)|$errdefer_(_fini...)>*/...) __stmt__using_(__VA_ARGS__)
#define $end_using __stmt__$end_using
#define $deferral_using __stmt__$deferral_using

#define __stmt__using_(...) pp_overload(__stmt__using_, __VA_ARGS__)(__VA_ARGS__)
#define __stmt__using__1(_init...) __step__using___emitForDefault( \
    ____using___expandInit _init \
)
#define ____using___expandInit(_init...) _init
#define __step__using___emitForDefault(...) ____using___emitForDefault(__VA_ARGS__)
/* clang-format off */
#define ____using___emitForDefault(_init...) { \
    _init;
#define __stmt__$end_using \
}
/* clang-format on */
#define __stmt__using__2(_init, _ext...) __step__using___emitForExt( \
    _init, ____using___expandExt(_ext) \
)
#define ____using___expandExt(_ext...) pp_cat(____using___expandExt, _ext)
#define ____using___expandExt$_defer $_defer, ~
#define ____using___expandExt$_defer_(...) $_defer_, (__VA_ARGS__)
#define ____using___expandExt$_errdefer_(...) $_errdefer_, (__VA_ARGS__)
#define __step__using___emitForExt(...) ____using___emitForExt(__VA_ARGS__)
#define ____using___emitForExt(_init, _ext, _ext_ctx...) \
    pp_cat(____using___emitForExt, _ext)(_init, _ext_ctx)
#define ____using___emitForExt$_defer(_init, _$ignored...) \
    comp_syn__blk_defer__code __stmt__using__1(_init)
#define ____using___emitForExt$_defer_(_init, _ext_ctx...) \
    ____using___emitForExt$_defer(_init) defer_ _ext_ctx
#define ____using___emitForExt$_errdefer_(_init, _ext_ctx...) \
    ____using___emitForExt$_defer(_init) errdefer_ _ext_ctx
#define __stmt__$deferral_using \
    __stmt__$end_using comp_syn__blk_deferral__code


$attr($maybe_unused)
$static fn_((genNum(void))(i32)) {
    $static var_(num, i32) = 0;
    return num++;
};

/* $attr($maybe_unused)
$static fn_((demoUsing(void))(i32) $guard) {
    var cnt = 0;
    defer_(io_stream_println(u8_l("cnt: {:i}"), cnt));

    using_((let n = genNum())) {
        if (n > 3) return_(n);
    } $end(using);
    using_((let n = genNum())) if (n > 3) return_(n) $end(using);

    using_((let n = genNum()) $defer) {
        defer_(cnt++);
        if (n > 3) io_stream_println(u8_l("n > 3: {:i}"), n);
    } $deferral(using);
    using_((let n = genNum()) $defer_(cnt++)) if (n > 3) io_stream_println(u8_l("n > 3: {:i}"), n) $deferral(using);

    return_(cnt);
} $unguarded(fn); */


#define if_(/*(_init...)(_cond)*/...) __stmt__if_(__VA_ARGS__)
#define $end_if __stmt__$end_if
#define if_some(/*(_o...)(_capt)*/...) __stmt__if_some(__VA_ARGS__)
#define if_none(/*(_o...)*/...) __stmt__if_none(__VA_ARGS__)
#define if_ok(/*(_e...)(_capt)*/...) __stmt__if_ok(__VA_ARGS__)
#define if_err(/*(_e...)(_capt)*/...) __stmt__if_err(__VA_ARGS__)
#define else_(/*(_init...)*/...) __stmt__else_(__VA_ARGS__)
#define else_some(/*(_capt)*/...) __stmt__else_some(__VA_ARGS__)
#define else_none __stmt__else_none
#define else_ok(/*(_capt)*/...) __stmt__else_ok(__VA_ARGS__)
#define else_err(/*(_capt)*/...) __stmt__else_err(__VA_ARGS__)

#define $cont(/*_expr*/...) , $_cont(__VA_ARGS__)
#define while_(/*(_cond...) <$cont(_expr...)>*/...) __stmt__while_(__VA_ARGS__)
#define $end_while __stmt__$end_while
#define while_some(/*(_o...)(_capt) <$cont(_expr...)>*/...) __stmt__while_some(__VA_ARGS__)
#define while_none(/*(_o...) <$cont(_expr...)>*/...) __stmt__while_none(__VA_ARGS__)
#define while_ok(/*(_e...)(_capt) <$cont(_expr...)>*/...) __stmt__while_ok(__VA_ARGS__)
#define while_err(/*(_e...)(_capt) <$cont(_expr...)>*/...) __stmt__while_err(__VA_ARGS__)


/* clang-format off */
#define __stmt__if_(...) pp_overload(__stmt__if_, __VA_ARGS__)(__VA_ARGS__)
#define __stmt__if__1(_cond...) __step__if___emitForDefault(_cond)
#define __step__if___emitForDefault(_cond...) if _cond
#define __stmt__if__2(_cond, _ext...) __step__if___emitForExt(_cond, ____if___expandExt(_ext))
#define ____if___expandExt(_ext...) pp_cat(____if___expandExt, _ext)
#define ____if___expandExt$_defer $_defer, ~
#define ____if___expandExt$_defer_(...) $_defer, __VA_ARGS__
#define ____if___expandExt$_errdefer_(...) $_errdefer, __VA_ARGS__
#define __stmt__$end_if } $end_using
/* clang-format on */

/* clang-format off */
#define if_some(/*(_expr...)(_capt)*/...) __if_some__step(pp_defer(__if_some__emit)(__if_some__parseExpr __VA_ARGS__))
#define __if_some__step(...) __VA_ARGS__
#define __if_some__parseExpr(_expr...) (_expr), __if_some__parseCapture
#define __if_some__parseCapture(_capt...) _capt
#define __if_some__emit(_expr, _capt...) \
    using_((let __monad_cond = _expr)) if (isSome(__monad_cond)) { \
        let _capt = __monad_cond.payload.some;
#define else_none ; } else {
#define if_none(/*(_expr...)*/...) __stmt__if_none(__VA_ARGS__)
#define __stmt__if_none(_expr...) \
    using_((let __monad_cond = _expr)) if (isNone(__monad_cond)) {
#define else_some(_capt...) ; } else { \
        let _capt = __monad_cond.payload.some;
#define if_ok(/*(_expr...)(_capt)*/...) __if_ok__step(pp_defer(__if_ok__emit)(__if_ok__parseExpr __VA_ARGS__))
#define __if_ok__step(...) __VA_ARGS__
#define __if_ok__parseExpr(_expr...) (_expr), __if_ok__parseCapture
#define __if_ok__parseCapture(_capt...) _capt
#define __if_ok__emit(_expr, _capt...) \
    using_((let __monad_cond = _expr)) if (isOk(__monad_cond)) { \
        let _capt = __monad_cond.payload.ok;
#define else_err(_capt...) ; } else { \
        let _capt = __monad_cond.payload.err;
#define if_err(/*(_expr...)(_capt)*/...) __if_err__step(pp_defer(__if_err__emit)(__if_err__parseExpr __VA_ARGS__))
#define __if_err__step(...) __VA_ARGS__
#define __if_err__parseExpr(_expr...) (_expr), __if_err__parseCapture
#define __if_err__parseCapture(_capt...) _capt
#define __if_err__emit(_expr, _capt...) \
    using_((let __monad_cond = _expr)) if (isErr(__monad_cond)) { \
        let _capt = __monad_cond.payload.err;
#define else_ok(_capt...) ; } else { \
        let _capt = __monad_cond.payload.ok;
/* clang-format on */

/* clang-format off */
#define while_(/*(_cond...)*/...) __stmt__while_(__VA_ARGS__)
#define __stmt__while_(...) __step__while___emit(__step__while___parseCond __VA_ARGS__)
#define __step__while___parseCond(_cond...) (_cond)
#define __step__while___emit(...) ____while___emit(__VA_ARGS__)
#define ____while___emit(_cond...) using_((var __latch = true; var __monad_cond = l0$((TypeOf(_cond))))) { \
    while (__latch && ((__monad_cond = _cond), (__monad_cond ? true : (__latch ? ((__latch = false), true) : false)))) { \
        if (__monad_cond) {
#define hence_(_keyword...) __stmt__hence_(_keyword)
#define __stmt__hence_(_keyword...) ; } _keyword {
#define $end_hence __stmt__$end_hence
#define __stmt__$end_while ; } $end_if; $end_using
/* clang-format on */

/* clang-format off */
#define while_some(/*(_expr...)(_capt)*/...) __stmt__while_some(__VA_ARGS__)
#define __stmt__while_some(...) __step__while_some__emit(__step__while_some__parseExpr __VA_ARGS__)
#define __step__while_some__parseExpr(_expr...) (_expr), __step__while_some__parseCapt
#define __step__while_some__parseCapt(_capt...) _capt
#define __step__while_some__emit(...) ____while_some(__VA_ARGS__)
#define ____while_some(_expr, _capt...) using_((var __latch = true; var __monad_iter = l0$((TypeOf(_expr))))) \
    while (__latch && ((__monad_iter = _expr), (isSome(__monad_iter) ? true : (__latch ? ((__latch = false), true) : false)))) \
        if_some((__monad_iter)(_capt)) {
#define while_none(/*(_expr...)*/...) __stmt__while_none(__VA_ARGS__)
#define __stmt__while_none(...) __step__while_none__emit(__step__while_none__parseExpr __VA_ARGS__)
#define __step__while_none__parseExpr(_expr...) (_expr)
#define __step__while_none__emit(...) ____while_none(__VA_ARGS__)
#define ____while_none(_expr...) using_((var __latch = true; var __monad_iter = l0$((TypeOf(_expr))))) \
    while (__latch && ((__monad_iter = _expr), (isNone(__monad_iter) ? true : (__latch ? ((__latch = false), true) : false)))) \
        if_none((__monad_iter)) {
#define while_ok(/*(_expr...)(_capt)*/...) __stmt__while_ok(__VA_ARGS__)
#define __stmt__while_ok(...) __step__while_ok__emit(__step__while_ok__parseExpr __VA_ARGS__)
#define __step__while_ok__parseExpr(_expr...) (_expr), __step__while_ok__parseCapt
#define __step__while_ok__parseCapt(_capt...) _capt
#define __step__while_ok__emit(...) ____while_ok(__VA_ARGS__)
#define ____while_ok(_expr, _capt...) using_((var __latch = true; var __monad_iter = l0$((TypeOf(_expr))))) \
    while (__latch && ((__monad_iter = _expr), (isOk(__monad_iter) ? true : (__latch ? ((__latch = false), true) : false)))) \
        if_ok((__monad_iter)(_capt)) {
#define while_err(/*(_expr...)(_capt)*/...) __stmt__while_err(__VA_ARGS__)
#define __stmt__while_err(...) __step__while_err__emit(__step__while_err__parseExpr __VA_ARGS__)
#define __step__while_err__parseExpr(_expr...) (_expr), __step__while_err__parseCapt
#define __step__while_err__parseCapt(_capt...) _capt
#define __step__while_err__emit(...) ____while_err(__VA_ARGS__)
#define ____while_err(_expr, _capt...) using_((var __latch = true; var __monad_iter = l0$((TypeOf(_expr))))) \
    while (__latch && ((__monad_iter = _expr), (isErr(__monad_iter) ? true : (__latch ? ((__latch = false), true) : false)))) \
        if_err((__monad_iter)(_capt)) {
/* clang-format on */

$extern fn_((getNextBool(void))(bool));
$attr($maybe_unused)
$static fn_((newCtrlFlowScalar(bool cond))(S_const$u8 $guard)) {
    if (cond) {
        let_ignore = true;
    } else {
        let_ignore = false;
    }

    var true_count = 0;
    while_((getNextBool() == true)) {
        true_count++;
    } hence_(else) {
        io_stream_println(u8_l("default"));
    } $end(while);
    let_ignore = true_count;

    var false_count = 0;
    while_((getNextBool() == false)) {
        false_count++;
    } hence_(else) {
        io_stream_println(u8_l("default"));
    } $end(while);
    let_ignore = false_count;

    return u8_l("done");
} $unguarded(fn);

$extern fn_((getNextOpt(void))(O$S$u8));
$attr($maybe_unused)
$static fn_((newCtrlFlow4Opt(O$S$u8 cond))(S_const$u8)) {
    if_some((cond)(str)) {
        let_ignore = str.as_const;
    } else_none {
        let_ignore = u8_l("default");
    } $end(if);

    if_none((cond)) {
        let_ignore = u8_l("default");
    } else_some((str)) {
        let_ignore = str.as_const;
    } $end(if);

    var some_count = 0;
    while_some((getNextOpt())($ignore)) {
        some_count++;
    } hence_(else_none) {
        io_stream_println(u8_l("none"));
    } $end(while);
    let_ignore = some_count;

    var none_count = 0;
    while_none((getNextOpt())) {
        none_count++;
    } hence_(else_some((str))) {
        io_stream_println(u8_l("str: {:s}"), str.as_const);
    } $end(while);
    let_ignore = none_count;

#if UNUSED_CODE
    // while_none((getNextOpt())) {
    // } else_some((str)) {
    // } $end(while);

    {
        var __monad_iter = getNextOpt();
        while (true)
            if_none((__monad_iter)) {
            } $end(if);
    };

    // var none_count = 0;
    // while_none((getNextOpt())) {
    //     none_count++;
    // }
    // hence_(else_some((str))) {
    //     return str.as_const;
    // } $end(while);
#endif /* UNUSED_CODE */
    return u8_l("done");
};

$extern fn_((getNextErr(void))(E$S$u8));
$attr($maybe_unused)
$static fn_((newCtrlFlow4Err(E$S$u8 cond))(S_const$u8)) {
    if_ok((cond)(str)) {
        return str.as_const;
    } else_err((err)) {
        return Err_codeToStr(err);
    } $end(if);

    if_err((cond)(err)) {
        return Err_codeToStr(err);
    } else_ok((str)) {
        return str.as_const;
    } $end(if);

    var ok_count = 0;
    while_ok((getNextErr())($ignore)) {
        ok_count++;
    } hence_(else_err((err))) {
        io_stream_println(u8_l("err: {:s}"), Err_codeToStr(err));
    } $end(while);
    let_ignore = ok_count;

    var err_count = 0;
    while_err((getNextErr())($ignore)) {
        err_count++;
    } hence_(else_ok((str))) {
        io_stream_println(u8_l("str: {:s}"), str.as_const);
    } $end(while);
    let_ignore = err_count;

    return u8_l("done");
};


#include "draft-tup.h"

T_alias$((Foo)(struct Foo {
    var_(a, i32);
    var_(b, i32);
}));
T_use_prl$(Foo);

T_use$((P$Foo, P$usize)(Tup));
$static fn_((getModel(void))(Tup$(P$Foo, P$usize)) $scope) {
    $static var_(bar, usize) = 0;
    let tup = tie$((ReturnT)((cleared()), (&bar)));
    $static var_(foo, Foo) = {
        .a = 1,
        .b = 2,
    };
    return_(with_((tup)((.$0)(&foo))));
} $unscoped(fn);
T_use$((Tup$(P$Foo, P$usize))(O));

$extern fn_((getValue(void))(O$(Tup$(P$Foo, P$usize))));
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    using_((untie_((let foo, let bar)(getModel())))) {
        foo->a++;
        foo->b--;
        *bar = intCast$((usize)(foo->a)) + intCast$((usize)(foo->b));
    } $end(using);

    using_((bind_(((var, $0), (let, $1))(getModel())))) {
        $0->a++;
        $0->b--;
        bind_(((let, a), (let, b))(*$0));
        *$1 = intCast$((usize)(a)) + intCast$((usize)(b));
    } $end(using);

    if (args.len <= 1) {
        untie_((let foo, let bar)({
            bind_(((let, $0), (let, $1))(getModel()));
            local_return_($tup((*$0), (*$1)));
        }));
        io_stream_println(u8_l("model: {:i}, {:i}, {:uz}"), foo.a, foo.b, bar);
    } else {
        let model = getModel();
        let foo = *model.$0;
        let bar = *model.$1;
        io_stream_println(u8_l("model: {:i}, {:i}, {:uz}"), foo.a, foo.b, bar);
    }
    return_ok({});
} $unscoped(fn);

/*===========================================================================*/

#define $defer , $_defer
#define $defer_(/*_fini*/...) , $_defer_(__VA_ARGS__)
#define $errdefer_(/*(_capt)(_fini...)*/...) , $_errdefer_(__VA_ARGS__)

#define $end(_keyword) \
    ; \
    pp_cat($end_, _keyword)
#define $deferral(_keyword) \
    ; \
    pp_cat($deferral_, _keyword)

#define using_(/*(_init...) <$defer|$defer_(_fini...)|$errdefer_(_fini...)>*/...) __stmt__using_(__VA_ARGS__)
#define $end_using __stmt__$end_using
#define $deferral_using __stmt__$deferral_using

#define __stmt__using_(...) pp_overload(__stmt__using_, __VA_ARGS__)(__VA_ARGS__)
#define __stmt__using__1(_init...) __step__using___emitForDefault( \
    ____using___expandInit _init \
)
#define ____using___expandInit(_init...) _init
#define __step__using___emitForDefault(...) ____using___emitForDefault(__VA_ARGS__)
/* clang-format off */
#define ____using___emitForDefault(_init...) { \
    _init;
#define __stmt__$end_using \
}
/* clang-format on */
#define __stmt__using__2(_init, _ext...) __step__using___emitForExt( \
    _init, ____using___expandExt(_ext) \
)
#define ____using___expandExt(_ext...) pp_cat(____using___expandExt, _ext)
#define ____using___expandExt$_defer $_defer, ~
#define ____using___expandExt$_defer_(...) $_defer_, __VA_ARGS__
#define ____using___expandExt$_errdefer_(...) $_errdefer_, __VA_ARGS__
#define __step__using___emitForExt(...) ____using___emitForExt(__VA_ARGS__)
#define ____using___emitForExt(_init, _ext, _ext_ctx...) \
    pp_cat(____using___emitForExt, _ext)(_init, _ext_ctx)
#define ____using___emitForExt$_defer(_init, _$ignored...) \
    comp_syn__blk_defer__code __stmt__using__1(_init)
#define ____using___emitForExt$_defer_(_init, _ext_ctx...) \
    comp_syn__blk_defer__codeForExt$_defer_( \
        pp_uniqTok(snapshot_reserved), pp_uniqTok(snapshot_deferred), \
        ____using___expandInit _init, \
        _ext_ctx \
    )
#define ____using___emitForExt$_errdefer_(_init, _ext_ctx...) \
    comp_syn__blk_defer__codeForExt$_errdefer_( \
        pp_uniqTok(snapshot_reserved), pp_uniqTok(snapshot_deferred), \
        ____using___expandInit _init, \
        _ext_ctx \
    )
#define __stmt__$deferral_using \
    __stmt__$end_using comp_syn__blk_deferral__code

// clang-format off
#define comp_syn__blk_defer__codeForExt$_defer_( \
    __snapshot_reserved, __snapshot_deferred, \
    _init, _Expr...\
) { do { \
    comp_syn__defer__op_snapshot( \
        goto __snapshot_reserved; \
    __snapshot_deferred: \
        if (__scope_counter.is_returning) { \
            goto __step_deferred; \
        } else { \
            continue; \
        } \
    ); \
    _init; \
    if (false) { \
    __snapshot_reserved: \
    _Expr; goto __snapshot_deferred; \
    }; \
    do ____using___emitForDefault()
#define comp_syn__blk_defer__codeForExt$_errdefer_( \
    __snapshot_reserved, __snapshot_deferred, \
    _init, _Payload_Capture, _Expr...\
) { do { \
    comp_syn__defer__op_snapshot( \
        goto __snapshot_reserved; \
    __snapshot_deferred: \
        if (__scope_counter.is_returning) { \
            goto __step_deferred; \
        } else { \
            continue; \
        } \
    ); \
    _init; \
    if (false) { \
    __snapshot_reserved: \
        if (!__reserved_return->is_ok) { \
            claim_assert(__reserved_return->payload.err.ctx != 0); \
            claim_assert_nonnull(__reserved_return->payload.err.vt); \
            let _Payload_Capture = __reserved_return->payload.err; \
            _Expr; \
        } \
        goto __snapshot_deferred; \
    }; \
    do ____using___emitForDefault()
// clang-format on

#include "dh/Thrd/common.h"
$attr($maybe_unused)
$static fn_((report(S_const$u8 label, S_const$u8 fmt, ...))(void) $guard) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currentId(), label);
    using_((var args = l0$((va_list)); va_start(args, fmt)) $defer_(va_end(args))) {
        io_stream_printVaArgs(fmt, args);
    } $deferral(using);
    io_stream_nl();
} $unguarded(fn);

$attr($maybe_unused)
$static fn_((reportOther(S_const$u8 label, S_const$u8 fmt, ...))(void) $guard) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currentId(), label);
    var args = l0$((va_list));
    va_start(args, fmt);
    blk_defer_({
        defer_(va_end(args));
        io_stream_printVaArgs(fmt, args);
    }) blk_deferral;
    io_stream_nl();
} $unguarded(fn);

$attr($maybe_unused)
$static fn_((reportFlat(S_const$u8 label, S_const$u8 fmt, ...))(void) $guard) {
    io_stream_print(u8_l("[ThrdId({:uz}): {:s}] "), Thrd_currentId(), label);
    var args = l0$((va_list));
    va_start(args, fmt);
    defer_(va_end(args));
    io_stream_printVaArgs(fmt, args);
    io_stream_nl();
} $unguarded(fn);
