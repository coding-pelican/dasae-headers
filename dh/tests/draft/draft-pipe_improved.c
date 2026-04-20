#include "dh-main.h"


#undef pipe_
#define pipe_(/*(_init)(<(_capt)_expr>...)*/...) __expr__pipe_(__VA_ARGS__)
#define __expr__pipe_(...) __inline__pipe___emit(__inline__pipe___parseInit __VA_ARGS__)
#define __inline__pipe___parseInit(_init...) pp_uniqTok(init), (_init), /*_steps*/
#define __inline__pipe___emit(...) __inline__pipe_(__VA_ARGS__)
#define __inline__pipe_(__init, _init, _steps...) local_({ \
    let __init = _init; \
    __inline__pipe___steps__expand( \
        __inline__pipe___steps__emit( \
            __init, \
            pp_uniqTok(step), \
            __inline__pipe___expandSteps _steps \
        ) \
    ); \
})
#define __inline__pipe___expandSteps(_steps...) _steps
#define __inline__pipe___steps__emit(...) __inline__pipe___steps(__VA_ARGS__)
#define __inline__pipe___steps(__curr, __step, _step, /*_next*/...) \
    __VA_OPT__( \
        __inline__pipe___stepLoop__emit(__curr, __step, __inline__pipe___expandStep _step); \
        pp_defer(__inline__pipe___stepsRecur)()(__step, pp_uniqTok(step), __VA_ARGS__) \
    ) \
    pp_if_(pp_not(pp_isParen(__VA_OPT__(()))))(pp_then_( \
        __inline__pipe___stepFinal___emit(__curr, __inline__pipe___expandStep _step) \
    ))
#define __inline__pipe___stepsRecur() __inline__pipe___steps
#define __inline__pipe___expandStep(_capt...) _capt, /*_expr*/
#define __inline__pipe___stepLoop__emit(...) __inline__pipe___stepLoop(__VA_ARGS__)
#define __inline__pipe___stepLoop(__prev, __curr, __capt, _expr...) \
    let __curr = local_({ \
        let __capt = __prev; \
        local_return_(__inline__pipe___step__expr(_expr)); \
    })
#define __inline__pipe___stepFinal___emit(...) __inline__pipe___stepFinal(__VA_ARGS__)
#define __inline__pipe___stepFinal(__prev, __capt, _expr...) \
    let __capt = __prev; \
    local_return_(__inline__pipe___step__expr(_expr))
#define __inline__pipe___step__expr(_expr...) T_switch$((TypeOf(_expr))( \
    T_case$((void)(($ignore_void _expr), l0$((Void)))), \
    T_default_(_expr) \
))
#define __inline__pipe___steps__expand(...) /* evaluate 2^5 */ __inline__pipe___steps__expand5(__VA_ARGS__)
#define __inline__pipe___steps__expand5(...) __inline__pipe___steps__expand4(__inline__pipe___steps__expand4(__VA_ARGS__))
#define __inline__pipe___steps__expand4(...) __inline__pipe___steps__expand3(__inline__pipe___steps__expand3(__VA_ARGS__))
#define __inline__pipe___steps__expand3(...) __inline__pipe___steps__expand2(__inline__pipe___steps__expand2(__VA_ARGS__))
#define __inline__pipe___steps__expand2(...) __inline__pipe___steps__expand1(__inline__pipe___steps__expand1(__VA_ARGS__))
#define __inline__pipe___steps__expand1(...) __VA_ARGS__


T_alias$((Foo)(struct Foo {
    var_(a, i32);
    var_(b, i32);
    var_(c, i32);
    var_(d, i32);
}));
$static fn_((Foo_splat(i32 scal))(Foo)) {
    return (Foo){
        .a = scal,
        .b = scal,
        .c = scal,
        .d = scal,
    };
};
$static fn_((Foo_withA(Foo self, i32 a))(Foo)) {
    return with_((self)((.a)(a)));
};
$static fn_((Foo_withB(Foo self, i32 b))(Foo)) {
    return with_((self)((.b)(b)));
};
$static fn_((Foo_withC(Foo self, i32 c))(Foo)) {
    return with_((self)((.c)(c)));
};
$static fn_((Foo_withD(Foo self, i32 d))(Foo)) {
    return with_((self)((.d)(d)));
};
$static fn_((Foo_discard(Foo self))(void)) {
    let_ignore = self;
};


fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let foo = Foo_splat(10);
    let_ignore = ({
        let __temp0 = (Foo_withA(foo, 10));
        let __temp1 = ({
            let a = __temp0;
            (Foo_withB(a, 20));
        });
        let __temp2 = ({
            let b = __temp1;
            (Foo_withC(b, 30));
        });
        let __temp3 = ({
            let c = __temp2;
            (Foo_withD(c, 40));
        });
        let d = __temp3;
        (Foo_discard(d)), l0$((Void));
    });
    let_ignore = pipe_((Foo_withA(foo, 10))(
        (a)(Foo_withB(a, 20)),
        (b)(Foo_withC(b, 30)),
        (c)(Foo_withD(c, 40)),
        (d)(Foo_discard(d))
    ));

    return_ok({});
} $unscoped(fn);
