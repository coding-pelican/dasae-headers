#include "dh/claim/unreachable.h"
#include "dh/core.h"
#include "dh/main.h"

#include "dh/fs/File.h"
#include "dh/io/Writer.h"
#include "dh/io/common.h"
#include "dh/opt.h"
#include "dh/prl.h"

#define biSearch$(T) pp_join($, BiSearch, T)
#define use_biSearch$(T) \
    fn_((biSearch$(T)(S$(T) sli, T val))(O$(usize))$scope) { \
        let_(len, usize) = sli.len; \
        var_(lhs, usize) = 0; \
        var_(rhs, usize) = len - 1; \
        while (lhs <= rhs) { \
            let_(mid, usize) = lhs + (rhs - lhs) / 2; \
            switch (prim_cmp(Sli_getAt(sli, mid), val)) { \
                case_(cmp_Ord_eq, return_some(mid)); \
                case_(cmp_Ord_lt, { rhs = mid - 1; } break); \
                case_(cmp_Ord_gt, { lhs = mid + 1; } break); \
                default_(claim_unreachable); \
            } \
        } \
        return_none(); \
    } \
    $unscoped_(fn)

use_biSearch$(i32);
fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let stream = fs_File_writer(io_getStdOut());
    try_(io_Writer_print(stream, u8_l("hello, world!\n")));

    let sli = A_ref$(S$(i32), A_from$(i32, { 1, 2, 3, 4, 5 }));
    let val = 3;
    let found = biSearch$(i32)(sli, val);
    if_some(found, index) {
        try_(io_Writer_print(stream, u8_l("found: {:zu}\n"), index));
    }
    else {
        try_(io_Writer_print(stream, u8_l("not found\n")));
    }

    return_ok({});
} $unscoped_(fn);


typedef void untyped;
typedef const untyped* P_const$(untyped);
typedef untyped* P$(untyped);
typedef struct S_const$(untyped) {
    P_const$(untyped) ptr;
    usize len;
} S_const$(untyped);
typedef union S$untyped {
    S_const$(untyped) as_const;
    struct {
        P$(untyped) ptr;
        usize len;
    };
} S$(untyped);


typedef enum PrimNum {
    PrimNum_none,
    PrimNum_u8,
    PrimNum_u16,
    PrimNum_u32,
    PrimNum_u64,
    PrimNum_i8,
    PrimNum_i16,
    PrimNum_i32,
    PrimNum_i64,
    PrimNum_f32,
    PrimNum_f64
} PrimNum;
#define PrimNum_from(_val...) _Generic( \
    TypeOf(_val), \
    u8: PrimNum_u8, \
    u16: PrimNum_u16, \
    u32: PrimNum_u32, \
    u64: PrimNum_u64, \
    i8: PrimNum_i8, \
    i16: PrimNum_i16, \
    i32: PrimNum_i32, \
    i64: PrimNum_i64, \
    f32: PrimNum_f32, \
    f64: PrimNum_f64, \
    default: PrimNum_none \
)
typedef variant_((TypeOrPrimNum $fits u8)(
    (TypeOrPrimNum_type, TypeInfo),
    (TypeOrPrimNum_prim_num, PrimNum)
)) TypeOrPrimNum;


fn_((biSearch_raw(
    TypeOrPrimNum type_or_prim_num,
    S_const$untyped sli,
    P_const$untyped val
))(O$usize) $scope) {
    let_(len, usize) = sli.len;
    var_(lhs, usize) = 0;
    var_(rhs, usize) = len - 1;
    while (lhs <= rhs) {
        let_(mid, usize) = lhs + (rhs - lhs) / 2;
        switch (blk_(cmp, cmp_Ord, {
            match_(type_or_prim_num, {
                pattern_(TypeOrPrimNum_type, (type), {
                    blk_break_(cmp, prim_memcmp(Sli_at(sli, mid), val, type->size));
                });
                pattern_(TypeOrPrimNum_prim_num, (type), switch (*type) {
                    case_(PrimNum_u8, blk_break_(cmp, prim_cmp(*as$(u8*)(Sli_at(sli, mid)), *as$(u8*)(val))));
                    case_(PrimNum_u16, blk_break_(cmp, prim_cmp(*as$(u16*)(Sli_at(sli, mid)), *as$(u16*)(val))));
                    case_(PrimNum_u32, blk_break_(cmp, prim_cmp(*as$(u32*)(Sli_at(sli, mid)), *as$(u32*)(val))));
                    case_(PrimNum_u64, blk_break_(cmp, prim_cmp(*as$(u64*)(Sli_at(sli, mid)), *as$(u64*)(val))));
                    case_(PrimNum_i8, blk_break_(cmp, prim_cmp(*as$(i8*)(Sli_at(sli, mid)), *as$(i8*)(val))));
                    case_(PrimNum_i16, blk_break_(cmp, prim_cmp(*as$(i16*)(Sli_at(sli, mid)), *as$(i16*)(val))));
                    case_(PrimNum_i32, blk_break_(cmp, prim_cmp(*as$(i32*)(Sli_at(sli, mid)), *as$(i32*)(val))));
                    case_(PrimNum_i64, blk_break_(cmp, prim_cmp(*as$(i64*)(Sli_at(sli, mid)), *as$(i64*)(val))));
                    case_(PrimNum_f32, blk_break_(cmp, prim_cmp(*as$(f32*)(Sli_at(sli, mid)), *as$(f32*)(val))));
                    case_(PrimNum_f64, blk_break_(cmp, prim_cmp(*as$(f64*)(Sli_at(sli, mid)), *as$(f64*)(val))));
                    case_(PrimNum_none, $fallthrough);
                    default_(claim_unreachable);
                });
            });
        })) {
            case_(cmp_Ord_eq, return_some(mid));
            case_(cmp_Ord_lt, { rhs = mid - 1; } continue);
            case_(cmp_Ord_gt, { lhs = mid + 1; } continue);
            default_(claim_unreachable);
        }
    }
    return_none();
} $unscoped_(fn);

#define valueOrDefault$(T, val...) \
    _Generic(TypeOfUnqual(val), T: val, default: make$(T))

#define addVoid(lhs, rhs...) ({ \
    (void)lhs; \
    (void)rhs; \
    (void)0; \
})

#define addInt(lhs, rhs...) ({ \
    isize _lhs = valueOrDefault$(isize, lhs); \
    isize _rhs = valueOrDefault$(isize, rhs); \
    _lhs + _rhs; \
})

#define addFloat(lhs, rhs...) ({ \
    f64 _lhs = valueOrDefault$(f64, lhs); \
    f64 _rhs = valueOrDefault$(f64, rhs); \
    _lhs + _rhs; \
})

#define genAdd(lhs, rhs...) _Generic( \
    TypeOfUnqual(lhs), \
    i32: addInt(lhs, rhs), \
    f32: addFloat(lhs, rhs), \
    void: addVoid(lhs, rhs), \
    default: claim_unreachable \
)

#define genNewAdd(lhs, rhs...) genericIf$( \
    lhs, void, ({ \
        $ignore_void lhs; \
        $ignore_void rhs; \
        (void)0; \
    }), \
    ({ \
        let _lhs = genericExprOrDefaultEvaledIfVoid$(lhs, TypeOf(rhs)); \
        let _rhs = genericExprOrDefaultEvaledIfVoid$(rhs, TypeOf(rhs)); \
        _lhs + _rhs; \
    }) \
)

#define genOldAdd(lhs, rhs...) ({ \
    __builtin_choose_expr( \
        __builtin_types_compatible_p(TypeOfUnqual(lhs), isize), addInt(lhs, rhs), \
        __builtin_choose_expr( \
            __builtin_types_compatible_p(TypeOfUnqual(lhs), f64), addFloat(lhs, rhs), \
            __builtin_choose_expr( \
                __builtin_types_compatible_p(TypeOfUnqual(lhs), void), addVoid(lhs, rhs), \
                claim_unreachable \
            ) \
        ) \
    ); \
})

fn_((testVoid(void))(void));
fn_((genericTest(void))(void)) {
    genAdd(testVoid(), 2);
    genOldAdd(testVoid(), 2.0f);
    genNewAdd(testVoid(), 2.0f);
}
