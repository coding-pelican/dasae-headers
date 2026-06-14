#ifndef prl_tpl_Tup__included
#define prl_tpl_Tup__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/
/*========== Macros and Definitions =========================================*/

#define Tup$(_Ts...) tpl$(Tup, _Ts)
#define Tup$$(_Ts...) TypeOf(union { \
    T_embed$(struct { \
        pp_overload(__Tup$__impl, _Ts)(_Ts); \
    }); \
    var_(as_raw, Tup$raw) $flexible; \
})
#define T_use_Tup$(_Ts...) T_alias$((Tup$(_Ts))(union Tup$(_Ts)) { \
    T_embed$(struct { \
        pp_overload(__Tup$__impl, _Ts)(_Ts); \
    }); \
    var_(as_raw, Tup$raw) $flexible; \
})
#define __Tup$__impl_0(...) \
    T_embed$(Void)
#define __Tup$__impl_1(_T...) \
    _T $0
#define __Tup$__impl_2(_T, _U...) \
    __Tup$__impl_1(_T); \
    _U $1
#define __Tup$__impl_3(_T, _U, _V...) \
    __Tup$__impl_2(_T, _U); \
    _V $2
#define __Tup$__impl_4(_T, _U, _V, _W...) \
    __Tup$__impl_3(_T, _U, _V); \
    _W $3
#define __Tup$__impl_5(_T, _U, _V, _W, _X...) \
    __Tup$__impl_4(_T, _U, _V, _W); \
    _X $4
#define __Tup$__impl_6(_T, _U, _V, _W, _X, _Y...) \
    __Tup$__impl_5(_T, _U, _V, _W, _X); \
    _Y $5
#define __Tup$__impl_7(_T, _U, _V, _W, _X, _Y, _Z...) \
    __Tup$__impl_6(_T, _U, _V, _W, _X, _Y); \
    _Z $6
#define __Tup$__impl_8(_T, _U, _V, _W, _X, _Y, _Z, _A...) \
    __Tup$__impl_7(_T, _U, _V, _W, _X, _Y, _Z); \
    _A $7
#define __Tup$__impl_9(_T, _U, _V, _W, _X, _Y, _Z, _A, _B...) \
    __Tup$__impl_8(_T, _U, _V, _W, _X, _Y, _Z, _A); \
    _B $8
#define __Tup$__impl_10(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C...) \
    __Tup$__impl_9(_T, _U, _V, _W, _X, _Y, _Z, _A, _B); \
    _C $9
#define __Tup$__impl_11(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D...) \
    __Tup$__impl_10(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C); \
    _D $10
#define __Tup$__impl_12(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E...) \
    __Tup$__impl_11(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D); \
    _E $11
#define __Tup$__impl_13(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F...) \
    __Tup$__impl_12(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E); \
    _F $12
#define __Tup$__impl_14(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F, _G...) \
    __Tup$__impl_13(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F); \
    _G $13
#define __Tup$__impl_15(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F, _G, _H...) \
    __Tup$__impl_14(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F, _G); \
    _H $14
#define __Tup$__impl_16(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F, _G, _H, _I...) \
    __Tup$__impl_15(_T, _U, _V, _W, _X, _Y, _Z, _A, _B, _C, _D, _E, _F, _G, _H); \
    _I $15


#define tie_(_vals... /*: (_val)*/) { pp_overload(__tie_, _vals)(_vals) }
#define __tie___expand(...) __VA_ARGS__
#define __tie__0(...)
#define __tie__1(_val0...) \
    .$0 = __tie___expand _val0
#define __tie__2(_val0, _val1...) \
    __tie__1(_val0), .$1 = __tie___expand _val1
#define __tie__3(_val0, _val1, _val2...) \
    __tie__2(_val0, _val1), .$2 = __tie___expand _val2
#define __tie__4(_val0, _val1, _val2, _val3...) \
    __tie__3(_val0, _val1, _val2), .$3 = __tie___expand _val3
#define __tie__5(_val0, _val1, _val2, _val3, _val4...) \
    __tie__4(_val0, _val1, _val2, _val3), .$4 = __tie___expand _val4
#define __tie__6(_val0, _val1, _val2, _val3, _val4, _val5...) \
    __tie__5(_val0, _val1, _val2, _val3, _val4), .$5 = __tie___expand _val5
#define __tie__7(_val0, _val1, _val2, _val3, _val4, _val5, _val6...) \
    __tie__6(_val0, _val1, _val2, _val3, _val4, _val5), .$6 = __tie___expand _val6
#define __tie__8(_val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7...) \
    __tie__7(_val0, _val1, _val2, _val3, _val4, _val5, _val6), .$7 = __tie___expand _val7
#define __tie__9( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8... \
) \
    __tie__8( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7 \
    ), \
        .$8 = __tie___expand _val8
#define __tie__10( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9... \
) \
    __tie__9( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8 \
    ), \
        .$9 = __tie___expand _val9
#define __tie__11( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10... \
) \
    __tie__10( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9 \
    ), \
        .$10 = __tie___expand _val10
#define __tie__12( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10, _val11... \
) \
    __tie__11( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9, _val10 \
    ), \
        .$11 = __tie___expand _val11
#define __tie__13( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10, _val11, _val12... \
) \
    __tie__12( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9, _val10, _val11 \
    ), \
        .$12 = __tie___expand _val12
#define __tie__14( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10, _val11, _val12, _val13... \
) \
    __tie__13( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9, _val10, _val11, _val12 \
    ), \
        .$13 = __tie___expand _val13
#define __tie__15( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10, _val11, _val12, _val13, _val14... \
) \
    __tie__14( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9, _val10, _val11, _val12, _val13 \
    ), \
        .$14 = __tie___expand _val14
#define __tie__16( \
    _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
    _val8, _val9, _val10, _val11, _val12, _val13, _val14, _val15... \
) \
    __tie__15( \
        _val0, _val1, _val2, _val3, _val4, _val5, _val6, _val7, \
        _val8, _val9, _val10, _val11, _val12, _val13, _val14 \
    ), \
        .$15 = __tie___expand _val15


#define tie$(/*(_T)(_vals...: (_val))*/... /*(_T)*/) __expr__tie$(__VA_ARGS__)
#define __expr__tie$(...) __step__tie$__emit(__step__tie$__parseT __VA_ARGS__)
#define __step__tie$__parseT(_T...) _T,
#define __step__tie$__emit(...) __step__tie$(__VA_ARGS__)
#define __step__tie$(_T, _vals...) ((_T)tie_ _vals)


#define $tup(_fields...) l$((Tup$$(pp_overload(__$tup__impl, _fields)(_fields)))tie_(_fields))
#define __$tup__impl_0(...)
#define __$tup__impl_1(_field0...) \
    TypeOf _field0
#define __$tup__impl_2(_field0, _field1...) \
    __$tup__impl_1(_field0), TypeOf _field1
#define __$tup__impl_3(_field0, _field1, _field2...) \
    __$tup__impl_2(_field0, _field1), TypeOf _field2
#define __$tup__impl_4(_field0, _field1, _field2, _field3...) \
    __$tup__impl_3(_field0, _field1, _field2), TypeOf _field3
#define __$tup__impl_5(_field0, _field1, _field2, _field3, _field4...) \
    __$tup__impl_4(_field0, _field1, _field2, _field3), TypeOf _field4
#define __$tup__impl_6(_field0, _field1, _field2, _field3, _field4, _field5...) \
    __$tup__impl_5(_field0, _field1, _field2, _field3, _field4), TypeOf _field5
#define __$tup__impl_7(_field0, _field1, _field2, _field3, _field4, _field5, _field6...) \
    __$tup__impl_6(_field0, _field1, _field2, _field3, _field4, _field5), TypeOf _field6
#define __$tup__impl_8(_field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7...) \
    __$tup__impl_7(_field0, _field1, _field2, _field3, _field4, _field5, _field6), TypeOf _field7
#define __$tup__impl_9( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8... \
) \
    __$tup__impl_8( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7 \
    ), \
        TypeOf _field8
#define __$tup__impl_10( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9... \
) \
    __$tup__impl_9( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8 \
    ), \
        TypeOf _field9
#define __$tup__impl_11( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10... \
) \
    __$tup__impl_10( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9 \
    ), \
        TypeOf _field10
#define __$tup__impl_12( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10, _field11... \
) \
    __$tup__impl_11( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9, _field10 \
    ), \
        TypeOf _field11
#define __$tup__impl_13( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10, _field11, _field12... \
) \
    __$tup__impl_12( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9, _field10, _field11 \
    ), \
        TypeOf _field12
#define __$tup__impl_14( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10, _field11, _field12, _field13... \
) \
    __$tup__impl_13( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9, _field10, _field11, _field12 \
    ), \
        TypeOf _field13
#define __$tup__impl_15( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10, _field11, _field12, _field13, _field14... \
) \
    __$tup__impl_14( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9, _field10, _field11, _field12, _field13 \
    ), \
        TypeOf _field14
#define __$tup__impl_16( \
    _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
    _field8, _field9, _field10, _field11, _field12, _field13, _field14, _field15... \
) \
    __$tup__impl_15( \
        _field0, _field1, _field2, _field3, _field4, _field5, _field6, _field7, \
        _field8, _field9, _field10, _field11, _field12, _field13, _field14 \
    ), \
        TypeOf _field15


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
#define ____untie___each_0(__tup, ...)
#define ____untie___each_1(__tup, _bind0...) \
    _bind0 = __tup.$0;
#define ____untie___each_2(__tup, _bind0, _bind1...) \
    ____untie___each_1(__tup, _bind0) _bind1 = __tup.$1;
#define ____untie___each_3(__tup, _bind0, _bind1, _bind2...) \
    ____untie___each_2(__tup, _bind0, _bind1) _bind2 = __tup.$2;
#define ____untie___each_4(__tup, _bind0, _bind1, _bind2, _bind3...) \
    ____untie___each_3(__tup, _bind0, _bind1, _bind2) _bind3 = __tup.$3;
#define ____untie___each_5(__tup, _bind0, _bind1, _bind2, _bind3, _bind4...) \
    ____untie___each_4(__tup, _bind0, _bind1, _bind2, _bind3) _bind4 = __tup.$4;
#define ____untie___each_6(__tup, _bind0, _bind1, _bind2, _bind3, _bind4, _bind5...) \
    ____untie___each_5(__tup, _bind0, _bind1, _bind2, _bind3, _bind4) _bind5 = __tup.$5;
#define ____untie___each_7(__tup, _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6...) \
    ____untie___each_6(__tup, _bind0, _bind1, _bind2, _bind3, _bind4, _bind5) _bind6 = __tup.$6;
#define ____untie___each_8(__tup, _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7...) \
    ____untie___each_7(__tup, _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6) _bind7 = __tup.$7;
#define ____untie___each_9( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8... \
) \
    ____untie___each_8( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7 \
    ) \
        _bind8 = __tup.$8;
#define ____untie___each_10( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9... \
) \
    ____untie___each_9( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8 \
    ) \
        _bind9 = __tup.$9;
#define ____untie___each_11( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10... \
) \
    ____untie___each_10( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9 \
    ) \
        _bind10 = __tup.$10;
#define ____untie___each_12( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10, _bind11... \
) \
    ____untie___each_11( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9, _bind10 \
    ) \
        _bind11 = __tup.$11;

#define ____untie___each_13( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10, _bind11, _bind12... \
) \
    ____untie___each_12( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9, _bind10, _bind11 \
    ) \
        _bind12 = __tup.$12;

#define ____untie___each_14( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10, _bind11, _bind12, _bind13... \
) \
    ____untie___each_13( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9, _bind10, _bind11, _bind12 \
    ) \
        _bind13 = __tup.$13;
#define ____untie___each_15( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10, _bind11, _bind12, _bind13, _bind14... \
) \
    ____untie___each_14( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9, _bind10, _bind11, _bind12, _bind13 \
    ) \
        _bind14 = __tup.$14;
#define ____untie___each_16( \
    __tup, \
    _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
    _bind8, _bind9, _bind10, _bind11, _bind12, _bind13, _bind14, _bind15... \
) \
    ____untie___each_15( \
        __tup, \
        _bind0, _bind1, _bind2, _bind3, _bind4, _bind5, _bind6, _bind7, \
        _bind8, _bind9, _bind10, _bind11, _bind12, _bind13, _bind14 \
    ) \
        _bind15 = __tup.$15;

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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Tup__included */
