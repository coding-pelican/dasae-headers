#pragma once
#ifndef prl_tpl_Tup__included
#define prl_tpl_Tup__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/
/*========== Macros and Definitions =========================================*/

#define Tup$(_$Ts...) tpl$(Tup, _$Ts)
#define Tup$$(_$Ts...) TypeOf(union { \
    T_embed$(struct { \
        pp_overload(__Tup$__impl, _$Ts)(_$Ts); \
    }); \
    var_(as_raw, Tup$raw) $flexible; \
})
#define T_use_Tup$(_$Ts...) T_alias$((Tup$(_$Ts))(union Tup$(_$Ts)) { \
    T_embed$(struct { \
        pp_overload(__Tup$__impl, _$Ts)(_$Ts); \
    }); \
    var_(as_raw, Tup$raw) $flexible; \
})
#define __Tup$__impl_0(...) \
    T_embed$(Void)
#define __Tup$__impl_1(_$T...) \
    _$T $0
#define __Tup$__impl_2(_$T, _$U...) \
    __Tup$__impl_1(_$T); \
    _$U $1
#define __Tup$__impl_3(_$T, _$U, _$V...) \
    __Tup$__impl_2(_$T, _$U); \
    _$V $2
#define __Tup$__impl_4(_$T, _$U, _$V, _$W...) \
    __Tup$__impl_3(_$T, _$U, _$V); \
    _$W $3
#define __Tup$__impl_5(_$T, _$U, _$V, _$W, _$X...) \
    __Tup$__impl_4(_$T, _$U, _$V, _$W); \
    _$X $4
#define __Tup$__impl_6(_$T, _$U, _$V, _$W, _$X, _$Y...) \
    __Tup$__impl_5(_$T, _$U, _$V, _$W, _$X); \
    _$Y $5
#define __Tup$__impl_7(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z...) \
    __Tup$__impl_6(_$T, _$U, _$V, _$W, _$X, _$Y); \
    _$Z $6
#define __Tup$__impl_8(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A...) \
    __Tup$__impl_7(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z); \
    _$A $7
#define __Tup$__impl_9(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B...) \
    __Tup$__impl_8(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A); \
    _$B $8
#define __Tup$__impl_10(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C...) \
    __Tup$__impl_9(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B); \
    _$C $9
#define __Tup$__impl_11(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D...) \
    __Tup$__impl_10(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C); \
    _$D $10
#define __Tup$__impl_12(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E...) \
    __Tup$__impl_11(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D); \
    _$E $11
#define __Tup$__impl_13(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F...) \
    __Tup$__impl_12(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E); \
    _$F $12
#define __Tup$__impl_14(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G...) \
    __Tup$__impl_13(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F); \
    _$G $13
#define __Tup$__impl_15(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H...) \
    __Tup$__impl_14(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G); \
    _$H $14
#define __Tup$__impl_16(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H, _$I...) \
    __Tup$__impl_15(_$T, _$U, _$V, _$W, _$X, _$Y, _$Z, _$A, _$B, _$C, _$D, _$E, _$F, _$G, _$H); \
    _$I $15


#define tie_(_$vals... /*: (_$val)*/) { pp_overload(__tie_, _$vals)(_$vals) }
#define __tie___expand(...) __VA_ARGS__
#define __tie__0(...)
#define __tie__1(_$val0...) \
    .$0 = __tie___expand _$val0
#define __tie__2(_$val0, _$val1...) \
    __tie__1(_$val0), .$1 = __tie___expand _$val1
#define __tie__3(_$val0, _$val1, _$val2...) \
    __tie__2(_$val0, _$val1), .$2 = __tie___expand _$val2
#define __tie__4(_$val0, _$val1, _$val2, _$val3...) \
    __tie__3(_$val0, _$val1, _$val2), .$3 = __tie___expand _$val3
#define __tie__5(_$val0, _$val1, _$val2, _$val3, _$val4...) \
    __tie__4(_$val0, _$val1, _$val2, _$val3), .$4 = __tie___expand _$val4
#define __tie__6(_$val0, _$val1, _$val2, _$val3, _$val4, _$val5...) \
    __tie__5(_$val0, _$val1, _$val2, _$val3, _$val4), .$5 = __tie___expand _$val5
#define __tie__7(_$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6...) \
    __tie__6(_$val0, _$val1, _$val2, _$val3, _$val4, _$val5), .$6 = __tie___expand _$val6
#define __tie__8(_$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7...) \
    __tie__7(_$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6), .$7 = __tie___expand _$val7
#define __tie__9( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8... \
) \
    __tie__8( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7 \
    ), \
        .$8 = __tie___expand _$val8
#define __tie__10( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9... \
) \
    __tie__9( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8 \
    ), \
        .$9 = __tie___expand _$val9
#define __tie__11( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10... \
) \
    __tie__10( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9 \
    ), \
        .$10 = __tie___expand _$val10
#define __tie__12( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10, _$val11... \
) \
    __tie__11( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9, _$val10 \
    ), \
        .$11 = __tie___expand _$val11
#define __tie__13( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10, _$val11, _$val12... \
) \
    __tie__12( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9, _$val10, _$val11 \
    ), \
        .$12 = __tie___expand _$val12
#define __tie__14( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10, _$val11, _$val12, _$val13... \
) \
    __tie__13( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9, _$val10, _$val11, _$val12 \
    ), \
        .$13 = __tie___expand _$val13
#define __tie__15( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10, _$val11, _$val12, _$val13, _$val14... \
) \
    __tie__14( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9, _$val10, _$val11, _$val12, _$val13 \
    ), \
        .$14 = __tie___expand _$val14
#define __tie__16( \
    _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
    _$val8, _$val9, _$val10, _$val11, _$val12, _$val13, _$val14, _$val15... \
) \
    __tie__15( \
        _$val0, _$val1, _$val2, _$val3, _$val4, _$val5, _$val6, _$val7, \
        _$val8, _$val9, _$val10, _$val11, _$val12, _$val13, _$val14 \
    ), \
        .$15 = __tie___expand _$val15


#define tie$(/*(_$T)(_$vals...: (_$val))*/... /*(_$T)*/) __expr__tie$(__VA_ARGS__)
#define __expr__tie$(...) __step__tie$__emit(__step__tie$__parseT __VA_ARGS__)
#define __step__tie$__parseT(_$T...) _$T,
#define __step__tie$__emit(...) __step__tie$(__VA_ARGS__)
#define __step__tie$(_$T, _$vals...) ((_$T)tie_ _$vals)


#define $tup(_$fields...) l$((Tup$$(pp_overload(__$tup__impl, _$fields)(_$fields)))tie_(_$fields))
#define __$tup__impl_0(...)
#define __$tup__impl_1(_$field0...) \
    TypeOf _$field0
#define __$tup__impl_2(_$field0, _$field1...) \
    __$tup__impl_1(_$field0), TypeOf _$field1
#define __$tup__impl_3(_$field0, _$field1, _$field2...) \
    __$tup__impl_2(_$field0, _$field1), TypeOf _$field2
#define __$tup__impl_4(_$field0, _$field1, _$field2, _$field3...) \
    __$tup__impl_3(_$field0, _$field1, _$field2), TypeOf _$field3
#define __$tup__impl_5(_$field0, _$field1, _$field2, _$field3, _$field4...) \
    __$tup__impl_4(_$field0, _$field1, _$field2, _$field3), TypeOf _$field4
#define __$tup__impl_6(_$field0, _$field1, _$field2, _$field3, _$field4, _$field5...) \
    __$tup__impl_5(_$field0, _$field1, _$field2, _$field3, _$field4), TypeOf _$field5
#define __$tup__impl_7(_$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6...) \
    __$tup__impl_6(_$field0, _$field1, _$field2, _$field3, _$field4, _$field5), TypeOf _$field6
#define __$tup__impl_8(_$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7...) \
    __$tup__impl_7(_$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6), TypeOf _$field7
#define __$tup__impl_9( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8... \
) \
    __$tup__impl_8( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7 \
    ), \
        TypeOf _$field8
#define __$tup__impl_10( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9... \
) \
    __$tup__impl_9( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8 \
    ), \
        TypeOf _$field9
#define __$tup__impl_11( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10... \
) \
    __$tup__impl_10( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9 \
    ), \
        TypeOf _$field10
#define __$tup__impl_12( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10, _$field11... \
) \
    __$tup__impl_11( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9, _$field10 \
    ), \
        TypeOf _$field11
#define __$tup__impl_13( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10, _$field11, _$field12... \
) \
    __$tup__impl_12( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9, _$field10, _$field11 \
    ), \
        TypeOf _$field12
#define __$tup__impl_14( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10, _$field11, _$field12, _$field13... \
) \
    __$tup__impl_13( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9, _$field10, _$field11, _$field12 \
    ), \
        TypeOf _$field13
#define __$tup__impl_15( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10, _$field11, _$field12, _$field13, _$field14... \
) \
    __$tup__impl_14( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9, _$field10, _$field11, _$field12, _$field13 \
    ), \
        TypeOf _$field14
#define __$tup__impl_16( \
    _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
    _$field8, _$field9, _$field10, _$field11, _$field12, _$field13, _$field14, _$field15... \
) \
    __$tup__impl_15( \
        _$field0, _$field1, _$field2, _$field3, _$field4, _$field5, _$field6, _$field7, \
        _$field8, _$field9, _$field10, _$field11, _$field12, _$field13, _$field14 \
    ), \
        TypeOf _$field15


#define untie_(/*(_$binds...: (var|let _$ident))(_$tup)*/...) __stmt__untie_(__VA_ARGS__)
#define __stmt__untie_(...) __step__untie___emit(__step__untie___parseBinds __VA_ARGS__)
#define __step__untie___parseBinds(_$binds...) (_$binds), pp_uniqTok(tup),
#define __step__untie___emit(...) __step__untie_(__VA_ARGS__)
#define __step__untie_(_$binds, __tup, _$tup...) \
    let __tup = _$tup; \
    ____untie___each_emit(__tup, ____untie___expandBinds _$binds)
#define ____untie___expandBinds(_$binds...) _$binds
#define ____untie___each_emit(...) ____untie___each(__VA_ARGS__)
#define ____untie___each(__tup, _$binds...) pp_overload(____untie___each, _$binds)(__tup, _$binds)
#define ____untie___each_0(__tup, ...)
#define ____untie___each_1(__tup, _$bind0...) \
    _$bind0 = __tup.$0;
#define ____untie___each_2(__tup, _$bind0, _$bind1...) \
    ____untie___each_1(__tup, _$bind0) _$bind1 = __tup.$1;
#define ____untie___each_3(__tup, _$bind0, _$bind1, _$bind2...) \
    ____untie___each_2(__tup, _$bind0, _$bind1) _$bind2 = __tup.$2;
#define ____untie___each_4(__tup, _$bind0, _$bind1, _$bind2, _$bind3...) \
    ____untie___each_3(__tup, _$bind0, _$bind1, _$bind2) _$bind3 = __tup.$3;
#define ____untie___each_5(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4...) \
    ____untie___each_4(__tup, _$bind0, _$bind1, _$bind2, _$bind3) _$bind4 = __tup.$4;
#define ____untie___each_6(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5...) \
    ____untie___each_5(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4) _$bind5 = __tup.$5;
#define ____untie___each_7(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6...) \
    ____untie___each_6(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5) _$bind6 = __tup.$6;
#define ____untie___each_8(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7...) \
    ____untie___each_7(__tup, _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6) _$bind7 = __tup.$7;
#define ____untie___each_9( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8... \
) \
    ____untie___each_8( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7 \
    ) \
        _$bind8 = __tup.$8;
#define ____untie___each_10( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9... \
) \
    ____untie___each_9( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8 \
    ) \
        _$bind9 = __tup.$9;
#define ____untie___each_11( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10... \
) \
    ____untie___each_10( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9 \
    ) \
        _$bind10 = __tup.$10;
#define ____untie___each_12( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10, _$bind11... \
) \
    ____untie___each_11( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9, _$bind10 \
    ) \
        _$bind11 = __tup.$11;

#define ____untie___each_13( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10, _$bind11, _$bind12... \
) \
    ____untie___each_12( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9, _$bind10, _$bind11 \
    ) \
        _$bind12 = __tup.$12;

#define ____untie___each_14( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10, _$bind11, _$bind12, _$bind13... \
) \
    ____untie___each_13( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9, _$bind10, _$bind11, _$bind12 \
    ) \
        _$bind13 = __tup.$13;
#define ____untie___each_15( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10, _$bind11, _$bind12, _$bind13, _$bind14... \
) \
    ____untie___each_14( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9, _$bind10, _$bind11, _$bind12, _$bind13 \
    ) \
        _$bind14 = __tup.$14;
#define ____untie___each_16( \
    __tup, \
    _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
    _$bind8, _$bind9, _$bind10, _$bind11, _$bind12, _$bind13, _$bind14, _$bind15... \
) \
    ____untie___each_15( \
        __tup, \
        _$bind0, _$bind1, _$bind2, _$bind3, _$bind4, _$bind5, _$bind6, _$bind7, \
        _$bind8, _$bind9, _$bind10, _$bind11, _$bind12, _$bind13, _$bind14 \
    ) \
        _$bind15 = __tup.$15;

#define $skip /*_$decl*/ let_ignore, /*field_as*/,
#define bind_(/*(_$binds...: (var|let, _$field)|(var|let, _$field_as, _$field))(_$record)*/...) __stmt__bind_(__VA_ARGS__)
#define __stmt__bind_(...) __step__bind$__emit(__step__bind$__parseBinds __VA_ARGS__)
#define __step__bind$__parseBinds(_$binds...) (_$binds), pp_uniqTok(record),
#define __step__bind$__emit(...) __step__bind_(__VA_ARGS__)
#define __step__bind_(_$binds, __record, _$record...) \
    let __record = _$record; \
    pp_foreach(__stmt__bind___each, __record, ____bind___expandBinds _$binds)
#define ____bind___expandBinds(_$binds...) _$binds
#define __stmt__bind___each(__record, _$bind...) __step__bind___each__emit(__record, ____bind___each__expandBind _$bind)
#define __step__bind___each__emit(...) __step__bind___each(__VA_ARGS__)
#define __step__bind___each(__record, ...) \
    pp_overload(__step__bind___each, __VA_ARGS__)(__record, __VA_ARGS__)
#define __step__bind___each_2(__record, _$decl, _$field...) \
    _$decl _$field = (__record)._$field;
#define __step__bind___each_3(__record, _$decl, _$field_as, _$field...) \
    _$decl _$field_as = (__record)._$field;
#define ____bind___each__expandBind(_$bind...) _$bind

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Tup__included */
