#include "dh/prl.h"


#define Tup$(_Ts...) pp_overload(__Tup$, _Ts)(_Ts)
#define __Tup$_0(...) claim_assert_static_msg(false, "Tup requires at least one type. Use `Void` instead.")
#define __Tup$_1(_Ts...) tpl_id$1T(Tup, _Ts)
#define __Tup$_2(_Ts...) tpl_id$1T$2U(Tup, _Ts)
#define __Tup$_3(_Ts...) tpl_id$2T$2U$3V(Tup, _Ts)
#define __Tup$_4(_Ts...) tpl_id$2T$2U$3V$4W(Tup, _Ts)
#define Tup$$(_Ts...) \
    struct { \
        pp_overload(__Tup$__impl, _Ts)(_Ts); \
    }
#define T_use_Tup$(_Ts...) T_alias$((Tup$(_Ts))(struct Tup$(_Ts)) { \
    pp_overload(__Tup$__impl, _Ts)(_Ts); \
})
#define __Tup$__impl_1(_T...) \
    _T $0
#define __Tup$__impl_2(_T, _U...) \
    _T $0; \
    _U $1
#define __Tup$__impl_3(_T, _U, _V...) \
    _T $0; \
    _U $1; \
    _V $2
#define __Tup$__impl_4(_T, _U, _V, _W...) \
    _T $0; \
    _U $1; \
    _V $2; \
    _W $3


#define tie_(_vals... /*: (_val)*/) pp_overload(__tie_, _vals)(_vals)
#define __tie___expand(...) __VA_ARGS__
#define __tie__1(_val0...) { \
    .$0 = __tie___expand _val0, \
}
#define __tie__2(_val0, _val1...) { \
    .$0 = __tie___expand _val0, \
    .$1 = __tie___expand _val1, \
}
#define __tie__3(_val0, _val1, _val2...) { \
    .$0 = __tie___expand _val0, \
    .$1 = __tie___expand _val1, \
    .$2 = __tie___expand _val2, \
}
#define __tie__4(_val0, _val1, _val2, _val3...) { \
    .$0 = __tie___expand _val0, \
    .$1 = __tie___expand _val1, \
    .$2 = __tie___expand _val2, \
    .$3 = __tie___expand _val3, \
}
#define tie$(/*(_T)(_vals...: (_val))*/... /*(_T)*/) __expr__tie$(__VA_ARGS__)
#define __expr__tie$(...) __step__tie$__emit(__step__tie$__parseT __VA_ARGS__)
#define __step__tie$__parseT(_T...) _T,
#define __step__tie$__emit(...) __step__tie$(__VA_ARGS__)
#define __step__tie$(_T, _vals...) ((_T)tie_ _vals)


#define $tup(_fields...) l$((Tup$$(pp_overload(__$tup__impl, _fields)(_fields)))tie_(_fields))
#define __$tup__impl_1(_field0...) \
    TypeOf _field0
#define __$tup__impl_2(_field0, _field1...) \
    TypeOf _field0, TypeOf _field1
#define __$tup__impl_3(_field0, _field1, _field2...) \
    TypeOf _field0, TypeOf _field1, TypeOf _field2
#define __$tup__impl_4(_field0, _field1, _field2, _field3...) \
    TypeOf _field0, TypeOf _field1, TypeOf _field2, TypeOf _field3


#define untie_(/*(_binds...: (var|let _ident))(_tup)*/...) __stmt__untie_(__VA_ARGS__)
#define __stmt__untie_(...) __step__untie___emit(__step__untie___parseBinds __VA_ARGS__)
#define __step__untie___parseBinds(_binds...) (_binds), pp_uniqTok(tup),
#define __step__untie___emit(...) __step__untie_(__VA_ARGS__)
#define __step__untie_(_binds, __tup, _tup...) \
    let __tup = _tup; \
    ____untie___each_emit(__tup, ____untie___expandBinds _binds)
#define ____untie___expandBinds(_binds...) _binds
#define ____untie___each_emit(...) ____untie___each(__VA_ARGS__)
#define ____untie___each(__tup, _binds...) pp_overload(____untie___each, _binds)(__tup, _binds)
#define ____untie___each_1(__tup, _bind0...) \
    _bind0 = __tup.$0;
#define ____untie___each_2(__tup, _bind0, _bind1...) \
    _bind0 = __tup.$0; \
    _bind1 = __tup.$1;
#define ____untie___each_3(__tup, _bind0, _bind1, _bind2...) \
    _bind0 = __tup.$0; \
    _bind1 = __tup.$1; \
    _bind2 = __tup.$2;
#define ____untie___each_4(__tup, _bind0, _bind1, _bind2, _bind3...) \
    _bind0 = __tup.$0; \
    _bind1 = __tup.$1; \
    _bind2 = __tup.$2; \
    _bind3 = __tup.$3;


#define bind_(/*(_binds...: (var|let, _field))(_record)*/...) __stmt__bind_(__VA_ARGS__)
#define __stmt__bind_(...) __step__bind$__emit(__step__bind$__parseBinds __VA_ARGS__)
#define __step__bind$__parseBinds(_binds...) (_binds), pp_uniqTok(record),
#define __step__bind$__emit(...) __step__bind_(__VA_ARGS__)
#define __step__bind_(_binds, __record, _record...) \
    let __record = _record; \
    pp_foreach(__stmt__bind___each, __record, ____bind___expandBinds _binds)
#define ____bind___expandBinds(_binds...) _binds
#define __stmt__bind___each(__record, _bind...) __step__bind___each__emit(__record, ____bind___each__expandBind _bind)
#define __step__bind___each__emit(...) __step__bind___each(__VA_ARGS__)
#define __step__bind___each(__record, _decl, _field...) \
    _decl _field = (__record)._field;
#define ____bind___each__expandBind(_bind...) _bind


#include "dh-main.h"
#include "dh/io/stream.h"

T_alias$((Foo)(struct Foo {
    var_(a, i32);
    var_(b, i32);
}));
T_use_prl$(Foo);

T_use$((P$Foo, P$usize)(Tup));
$static fn_((getModel(void))(Tup$1P$Foo$2P$usize) $scope) {
    $static var_(bar, usize) = 0;
    let tup = tie$((ReturnT)((cleared()), (&bar)));
    $static var_(foo, Foo) = {
        .a = 1,
        .b = 2,
    };
    return_(with_((tup)((.$0)(&foo))));
} $unscoped(fn);

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    {
        untie_((let foo, let bar)(getModel()));
        foo->a++;
        foo->b--;
        *bar = intCast$((usize)(foo->a)) + intCast$((usize)(foo->b));
    }
    {
        bind_(((var, $0), (let, $1))(getModel()));
        $0->a++;
        $0->b--;
        bind_(((let, a), (let, b))(*$0));
        *$1 = intCast$((usize)(a)) + intCast$((usize)(b));
    }
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


#define EMPTY()
#define DEFER(m...) m EMPTY()
#define EXPAND(...) __VA_ARGS__

/* 3^5 = 243회 확장을 보장하는 EVAL 스택 */
#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))

#undef bind_
#define bind_(/*(_binds...: (var|let, _field))(_record)*/...) EVAL(__stmt__bind_(__VA_ARGS__))
/* * pattern example: (let, (a, b) sub_struct)
 * _decl: let
 * _target: (a, b) sub_struct
 */
#undef __stmt__bind___each
#define __stmt__bind___each(__record, _bind...) \
    __step__bind___recursive_dispatch_emit(__record, ____bind___each__expandBind _bind)
#define __step__bind___recursive_dispatch_emit(...) \
    __step__bind___recursive_dispatch(__VA_ARGS__)
#define __bind__target__sep(...) (__VA_ARGS__),
#define __step__bind___recursive_dispatch(__record, _decl, _target...) \
    pp_if_(pp_isParen(_target))( \
        pp_then_() DEFER(__RECURSIVE_BIND_NESTED)(__record, _decl, __bind__target__sep _target), \
        pp_else_(_decl _target = (__record)._target;) \
    )

/* nested binding expansion: bind_((sub_patterns)(record.field)) */
#define __RECURSIVE_BIND_NESTED(...) ____RECURSIVE_BIND_NESTED(__VA_ARGS__)
#define ____RECURSIVE_BIND_NESTED(__record, _decl, _nested_pattern, _field...) \
    DEFER(__redirect__bind_)(_nested_pattern((__record)._field))
#define __redirect__bind_(...) DEFER(pp_cat(__stmt__bind, _))(__VA_ARGS__)


$attr($maybe_unused)
$static fn_((demoRecursiveBind(void))(void)) {
    T_alias$((Inner)(struct Inner {
        var_(x, i32);
        var_(y, struct {
            var_(z, struct {
                var_(w, i32);
            });
        });
    }));
    T_alias$((Outer)(struct Outer {
        T_embed$(union {
            T_embed$(Foo);
            T_embed$(struct {
                var_(id, FieldType$(Foo, a));
                var_(num, FieldType$(Foo, b));
            });
        });
        var_(data, Inner);
    }));
    claim_assert_static(offsetTo(Outer, id) == offsetTo(Foo, a));
    claim_assert_static(offsetTo(Outer, num) == offsetTo(Foo, b));

    var_(obj, Outer) = {
        .id = 1,
        .num = 2,
        .data = { .x = 10, .y = { .z = { .w = 20 } } }
    };
    bind_((
        (let, id),
        (let, num),
        (let, ((let, x), (let, ((let, ((let, w))z))y))data)
    )(obj));
    /* // Expands to
    __auto_type const __218_27_record = (obj);
    __auto_type const id = (__218_27_record).id;
    __auto_type const __218_28_record = ((__218_27_record).data);
    __auto_type const x = (__218_28_record).x;
    __auto_type const __218_29_record = ((__218_28_record).y);
    __auto_type const __218_30_record = ((__218_29_record).z);
    __auto_type const w = (__218_30_record).w; */

    let_ignore = id;
    let_ignore = num;
    let_ignore = x;
    let_ignore = w;
};
