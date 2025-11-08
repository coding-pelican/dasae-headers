#if UNUSED_CODE
#include "dh/main.h"


// clang-format off
/* #define blk_(_label, _BreakType_and_Body...) \
    pp_overload(__blk, _BreakType_and_Body)(_label, _BreakType_and_Body)
#define __blk_1(_label, _Body...)                    comp_syn__blk_((_label, Void), _Body)
#define __blk_2(_label, _RetType, _Body...)          comp_syn__blk_((_label, _RetType), _Body)
#define comp_syn__blk_(_Label_and_RetType, _Body...) blk({ \
    $local_label pp_Tuple_get1st _Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _Label_and_RetType){} \
    ); \
    _Body; \
    pp_Tuple_get1st _Label_and_RetType: \
    pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType); \
}) */

#define break_(_label, ...) comp_syn__blk_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)
#define comp_syn__blk_break_(__reserved_val, _label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _label

#define for_(_Begin_to_End, _Iter, _Expr...) \
    comp_syn__for_(pp_uniqTok(range), pp_uniqTok(i), _Begin_to_End, _Iter, _Expr)
#define comp_syn__for_(__range, __i, _Begin_to_End, _Iter, _Expr...) blk({ \
    let_(__range, R) = _Begin_to_End; \
    for (usize __i = __range.begin; __i < __range.end; ++__i) { \
        let _Iter = __i; \
        _Expr; \
    } \
})
// clang-format on

#define While_(_init, _cond, _then, _cont, _expr...) ({ \
    pp_expand _init; \
    while (_cond) { \
        pp_expand _then; \
        _expr; \
        pp_expand _cont; \
    } \
})

#undef while_
#undef while_some
#undef while_none
#define while_(_init, _cond, _cont, _expr...) While_(_init, _cond, (), _cont, _expr)
#define while_some(_init, _opt, _capture, _cont, _expr...) \
    While_((var __opt = (_opt); pp_expand _init), (isSome(__opt)), (let pp_expand _capture = unwrap(__opt)), (pp_expand _cont; __opt = (_opt)), _expr)
#define while_none(_init, _opt, _cont, _expr...) \
    While_((var __opt = (_opt); pp_expand _init), (isNone(__opt)), (), (pp_expand _cont; __opt = (_opt)), _expr)

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    {
        i32 key   = 7;
        let value = eval_(i32 $scope)({
            i32 sum = 0;
            while_((i32 i = 0), (i < 10), (i += 1), {
                if (i == key) {
                    $break_(sum + 100);
                }
                sum += i;
            });
            $break_(sum);
        }) $unscoped_(expr);
        printf("value: %d\n", value);
    }

    extern fn_((getNext(void))(O$i32));
    while_some((), (getNext()), (value), (), {
        printf("value: %d\n", value);
    });
    while_none((), (getNext()), (), {
        printf("none\n");
    });

    {
        i32 key   = 23;
        let value = expr_(i32 $scope) {
            for_($r(0, 10), (i), {
                if (i == as$((usize)(key))) {
                    $break_(5);
                }
            });
            for_($r(10, 20), (i), {
                if (i == as$((usize)(key))) {
                    $break_(15);
                }
            });
            for_($r(20, 30), (i), {
                if (i == as$((usize)(key))) {
                    $break_(25);
                }
            });
            claim_unreachable;
        } $unscoped_(expr);
        printf("value: %d\n", value);
    }

    // For_((let slice = arr), (Slice_range(slice)), (i), (let p = Sli_at(slice, i)), {
    //     printf("value: %d\n", *p);
    // });
    // for_s((arr), (p), {
    //     printf("value: %d\n", *p);
    // });
    return_ok({});
} $unscoped_(fn);

// #define pp_ignore2_expand1_ignores(...) pp_ignore1_expand1_ignores
// #define pp_ignore1_expand1_ignores(...) pp_expand1_ignores
// #define pp_expand1_ignores(...)         __VA_ARGS__ pp_ignore
// pp_expand(pp_ignore2_expand1_ignores(a)(b)(c)())
#endif /* UNUSED_CODE */
