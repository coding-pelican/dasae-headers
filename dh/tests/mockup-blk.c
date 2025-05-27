#include "dh/main.h"


// clang-format off
#define blk_(_label, _BreakType_and_Body...) \
    pp_overload(__blk, _BreakType_and_Body)(_label, _BreakType_and_Body)
#define __blk_1(_label, _Body...)                    comp_syn__blk_((_label, Void), _Body)
#define __blk_2(_label, _RetType, _Body...)          comp_syn__blk_((_label, _RetType), _Body)
#define comp_syn__blk_(_Label_and_RetType, _Body...) eval({ \
    local_label pp_Tuple_get1st _Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _Label_and_RetType){} \
    ); \
    _Body; \
    pp_Tuple_get1st _Label_and_RetType: \
    pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType); \
})

#define blk_break_(_label, ...) comp_syn__blk_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)
#define comp_syn__blk_break_(__reserved_val, _label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _label

#define for_(_Begin_to_End, _Iter, _Expr...) \
    comp_syn__for_(pp_uniqTok(range), pp_uniqTok(i), _Begin_to_End, _Iter, _Expr)
#define comp_syn__for_(__range, __i, _Begin_to_End, _Iter, _Expr...) eval({ \
    let_(__range, Range) = Range_from _Begin_to_End; \
    for (usize __i = __range.begin; __i < __range.end; ++__i) { \
        let _Iter = __i; \
        _Expr; \
    } \
})
// clang-format on



fn_(dh_main(Sli$Str_const args), Err$void, $scope) {
    $ignore = args;

    {
        i32 key   = 7;
        let value = blk_(blk_label, i32, {
            i32 sum = 0;
            while_(i32 i = 0, i < 10) {
                if (i == key) {
                    blk_break_(blk_label, sum + 100);
                }
                sum += i;
                i += 1;
            }
            blk_break_(blk_label, sum);
        });
        printf("value: %d\n", value);
    }

    {
        i32 key   = 23;
        let value = blk_(blk_label, i32, {
            for_((0, 10), (i), {
                if (i == as$(usize, key)) {
                    blk_break_(blk_label, 5);
                }
            });
            for_((10, 20), (i), {
                if (i == as$(usize, key)) {
                    blk_break_(blk_label, 15);
                }
            });
            for_((20, 30), (i), {
                if (i == as$(usize, key)) {
                    blk_break_(blk_label, 25);
                }
            });
            claim_unreachable;
        });
        printf("value: %d\n", value);
    }

    return_ok({});
} $unscoped;
