#ifndef draft_tup__included
#define draft_tup__included 1

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

#endif /* draft_tup__included */
