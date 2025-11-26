#include "dh/prl.h"

#if UNUSED_CODE
fn_((sumProcedural(S_const$i32 items))(i32)) {
    var_(sum, i32) = 0;
    for_(($s(items))(item) {
        sum += *item;
    });
    return sum;
};

fn_((sumFunctionalInline(S_const$i32 items))(i32)) {
    return fold$((i32)($s(items), (0), (acc, item)(acc + *item)));
};
fn_((sumFunctionalInlineRaw(S_const$i32 items))(i32)) {
    return ({
        i32 acc = 0;
        for_(($s(items))(item) {
            acc = (acc + *item);
        });
        acc;
    });
};

fn_((sumFunctionalPrim(S_const$i32 items))(i32)) {
    return fold$((i32)($s(items), (0), (acc, item)prim_add(acc, *item)));
};
fn_((sumFunctionalPrimRaw(S_const$i32 items))(i32)) {
    return ({
        i32 acc = 0;
        for_(($s(items))(item) {
            acc = prim_add(acc, *item);
        });
        acc;
    });
};

#define fold$(/*(_T)(_s, _initial, (_acc, _p_e)) _combine*/...) \
    __step__fold$(__step__fold$__parse __VA_ARGS__)
#define __step__fold$(...) __step__fold$__emit(__VA_ARGS__)
#define __step__fold$__parse(_T...) _T, __step__fold$__parseNext
#define __step__fold$__parseNext(_s, _initial, _captures...) _s, _initial, __step__fold$__captures _captures
#define __step__fold$__captures(_acc, _p_e...) _acc, _p_e,
#define __step__fold$__emit(...) __fold$(__VA_ARGS__)
#define __fold$(_T, _s, _initial, _acc, _p_e, _combine...) ({ \
    _T _acc = _initial; \
    for_((_s)(_p_e) { \
        _acc = _combine; \
    }); \
    _acc; \
})
#endif /* UNUSED_CODE */

#if UNUSED_CODE
#define chain$(/*(_T)(_s)(_fnOps...)*/...) \
    __step__chain$(__step__chain$__parse __VA_ARGS__)
#define __step__chain$(...) __step__chain$__emit(__VA_ARGS__)
#define __step__chain$__parse(_T...) _T, __step__chain$__parseS
#define __step__chain$__parseS(_s...) _s, __step__chain$__parseFnOps
#define __step__chain$__parseFnOps(_fnOps...) _fnOps
#define __step__chain$__emit(...) __chain$(__VA_ARGS__)
#define __chain$(_T, _s, _fnOps...) ({ \
    _T __reserved_acc = {}; \
    $maybe_unused bool __assigned_default = false; \
    for_((_s)(__iter) { \
        _fnOps; \
    }); \
    __reserved_acc; \
})
#define filter_(/*(_p_e)(_pred)*/...) __step__filter_(__step__filter___capt __VA_ARGS__)
#define __step__filter_(...) __step__filter___emit(__VA_ARGS__)
#define __step__filter___capt(_p_e...) _p_e, __step__filter___pred
#define __step__filter___pred(_pred...) _pred
#define __step__filter___emit(...) __filter_(__VA_ARGS__)
#define __filter_(_p_e, _pred...) ({ \
    let _p_e = __iter; \
    if (!(_pred)) { continue; } \
})
#define fold_(/*(_default), (_acc, _p_e)_combine*/...) __step__fold_(__VA_ARGS__)
#define __step__fold_(_default, ...) __step__fold___emit(_default, __step__fold___capt __VA_ARGS__)
#define __step__fold___capt(_acc, _p_e...) _acc, _p_e,
#define __step__fold___emit(...) __fold_(__VA_ARGS__)
#define __fold_(_default, _acc, _p_e, _combine...) ({ \
    if (!__assigned_default) { \
        asg_lit((&__reserved_acc)_default); \
        __assigned_default = true; \
    } \
    let _acc = __reserved_acc; \
    let _p_e = __iter; \
    __reserved_acc = _combine; \
})

fn_((sumProcedural(S_const$i32 items))(i32)) {
    var_(sum, i32) = -123;
    for_(($s(items))(item) {
        if (!(*item > 0)) { continue; } // filter
        sum += *item;                   // fold
    });
    return sum;
};

fn_((sumFunctionalInlineRaw(S_const$i32 items))(i32)) {
    return ({
        i32 __ret = -123;
        for_(($s(items))(item) {
            if (!(*item > 0)) { continue; } // filter
            let acc = __ret;
            __ret = (acc + *item); // fold
        });
        __ret;
    });
};

fn_((sumFunctionalInline(S_const$i32 items))(i32)) {
    return chain$((i32)($s(items))(
        filter_((x)(*x > 0));
        fold_((-123), (acc, item)(acc + *item));
    ));
};
#endif /* UNUSED_CODE */

typedef enum chain__State : u8 {
    chain__State_none = 0,
    chain__State_init = 1,
    chain__State_eval = 2
} chain__State;

#define chain$(/*(_T)(_s)(_fnOps...)*/...) \
    __step__chain$(__step__chain$__parse __VA_ARGS__)
#define __step__chain$(...) __step__chain$__emit(__VA_ARGS__)
#define __step__chain$__parse(_T...) _T, __step__chain$__parseS
#define __step__chain$__parseS(_s...) pp_uniqTok(s), _s, __step__chain$__parseFnOps
#define __step__chain$__parseFnOps(_fnOps...) _fnOps
#define __step__chain$__emit(...) __chain$(__VA_ARGS__)
#define __chain$(_T, __s, _s, _fnOps...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        let __s = _s; \
        var_(__reserved_acc, _T) = {}; \
        $maybe_unused bool __assigned_default = false; \
        var_(__chain_state, chain__State) = __s.len == 0 ? chain__State_none : chain__State_init; \
        do { \
            let_(__it, TypeOf(__s.ptr)) = __s.ptr; \
            _fnOps; \
        } while (false); \
        __chain_state = chain__State_eval; \
        for_(($s(__s))(__it)(_fnOps)); \
        __reserved_acc; \
    }) \
)
#define each_(/*(_p_e)_body*/...) __step__each_(__step__each___capt __VA_ARGS__)
#define __step__each_(...) __step__each___emit(__VA_ARGS__)
#define __step__each___capt(_p_e...) _p_e,
#define __step__each___emit(...) __each_(__VA_ARGS__)
#define __each_(_p_e, _body...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _p_e = __it; \
        _body; \
    }; \
})
#define filter_(/*(_p_e)_pred*/...) __step__filter_(__step__filter___capt __VA_ARGS__)
#define __step__filter_(...) __step__filter___emit(__VA_ARGS__)
#define __step__filter___capt(_p_e...) _p_e, __step__filter___pred
#define __step__filter___pred(_pred...) _pred
#define __step__filter___emit(...) __filter_(__VA_ARGS__)
#define __filter_(_p_e, _pred...) ({ \
    if (__chain_state != chain__State_eval) { \
    } else { \
        let _p_e = __it; \
        if (!(_pred)) { continue; }; \
    }; \
})
#define map$(/*(_T)(_p_e)_xform*/...) __step__map$(__step__map___parse __VA_ARGS__)
#define __step__map$(...) __step__map$__emit(__VA_ARGS__)
#define __step__map$__parse(_T...) _T, __step__map$__parseP
#define __step__map$__parseP(_p_e...) _p_e, __step__map$__parseXform
#define __step__map$__parseXform(_xform...) _xform
#define __step__map$__emit(...) __map$(__VA_ARGS__)
#define __map$(_T, _p_e, _xform...)

#define collect_(/*(_gpa)*/) __step__collect_(__step__collect___gpa __VA_ARGS__)
#define collectFixed_(/*(_out)*/) __step__collectFixed_(__step__collectFixed___out __VA_ARGS__)
#define collectWithin_(/*(_out)*/) __step__collectWithin_(__step__collectWithin___out __VA_ARGS__)

#define fold_(/*(_default), (_acc, _p_e)_combine*/...) __step__fold_(__VA_ARGS__)
#define __step__fold_(_default, ...) __step__fold___emit(_default, __step__fold___capt __VA_ARGS__)
#define __step__fold___capt(_acc, _p_e...) _acc, _p_e,
#define __step__fold___emit(...) __fold_(__VA_ARGS__)
#define __fold_(_default, _acc, _p_e, _combine...) ({ \
    if (__chain_state != chain__State_eval) { \
        asg_lit((&__reserved_acc)(_default)); \
    } else { \
        let _acc = __reserved_acc; \
        let _p_e = __it; \
        asg_lit((&__reserved_acc)_combine); \
    }; \
})
#define reduce_(/*(_acc, _p_e)_combine*/...) __step__reduce_(__VA_ARGS__)
#define __step__reduce_(...) __step__reduce___emit(__step__reduce___capt __VA_ARGS__)
#define __step__reduce___capt(_acc, _p_e...) _acc, _p_e,
#define __step__reduce___emit(...) __reduce_(__VA_ARGS__)
#define __reduce_(_acc, _p_e, _combine...) ({ \
    if (__chain_state != chain__State_eval) { \
        asg_lit((&__reserved_acc)(none())); \
    } else { \
        if (isNone(__reserved_acc)) { \
            asg_lit((&__reserved_acc)(some(*__it))); \
            continue; \
        }; \
        let _acc = orelse_((__reserved_acc)(*__it)); \
        let _p_e = __it; \
        asg_lit((&__reserved_acc)(some(_combine))); \
    }; \
})

fn_((sumProcedural(S_const$i32 items))(i32)) {
    var_(sum, i32) = -123;
    for_(($s(items))(item) {
        if (!(*item > 0)) { continue; } // filter
        sum += *item;                   // fold
    });
    return sum;
};

fn_((sumFunctionalFoldInlineRaw(S_const$i32 items))(i32)) {
    return ({
        i32 __ret = -123;
        for_(($s(items))(item) {
            if (!(*item > 0)) { continue; } // filter
            let acc = __ret;
            __ret = (acc + *item); // fold
        });
        __ret;
    });
};

fn_((sumFunctionalFoldInline(S_const$i32 items))(i32)) {
    return chain$((i32)(items)(
        filter_((x)(*x > 0)),
        fold_((-123), (acc, item)(acc + *item))
    ));
};

fn_((sumFunctionalReduceInline(S_const$i32 items))(O$i32)) {
    return chain$((O$i32)(items)(
        filter_((x)(*x > 0)),
        reduce_((acc, item)(acc + *item))
    ));
};

fn_((sumFunctionalFilterMapReduce(S_const$i32 items))(O$u32)) {
    return chain$((O$u32)(items)(
        filter_((x)(*x > 0)),
        map_((x)(as$(u32)(*x))),
        reduce_((acc, item)(prim_min(acc, *item)))
    ));
};

fn_((sumFunctionalFilterMapReduceInline(S_const$i32 items))(O$u32)) {
    return ({
        O$u32 __ret = none();
        for_(($s(items))(item) {
            if (!(*item > 0)) { continue; } // filter
            let x = as$(u32)(*item);
            if (isNone(__ret)) {
                asg_lit((&__ret)(some(x)));
                continue;
            };
            let acc = orelse_((__ret)(x));
            asg_lit((&__ret)(some(prim_min(acc, x))));
        });
        __ret;
    });
};

fn_((sumFunctionalFilterMapReduceInline(S_const$i32 items))(O$u32)) {
    return ({
        O$u32 __ret = none();
        for_(($s(items))(__curr) {
            let __reserved = __curr;
            let __curr = ({ // filter
                let item = __reserved;
                if (!(*item > 0)) { continue; }
                item;
            });
            {
                let __reserved = __curr;
                let __curr = ({ // map
                    let item = __reserved;
                    as$(u32)(*item);
                });
                {
                    let __reserved = __curr;
                    let __curr = ({ // reduce
                        let x = __reserved;
                        if (isNone(__ret)) {
                            asg_lit((&__ret)(some(x)));
                            continue;
                        };
                        let acc = orelse_((__ret)(x));
                        asg_lit((&__ret)(some(prim_min(acc, x))));
                    });
                }
            }
        });
        __ret;
    });
};

fn_((sumFunctionalFilterMapCollect(S_const$i32 items, S$u32 out))(S$u32)) {
    return chain$((O$u32)(items)(
        filter_((x)(*x > 0)),
        map_((x)(as$(u32)(*x))),
        collectWithin_(out)
    ));
};
