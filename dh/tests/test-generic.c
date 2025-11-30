#include "dh/prl.h"

let a = u32_n(123);
let a_is_u32 = T_switch$((TypeOf(a))(
    T_case$((u32)(true)),
    T_default_(false)
));
claim_assert_static(a_is_u32);
let a_is_const = T_switch$((TypeOf(a))(
    T_qual$((const u32)(true)),
    T_qual$((u32)(false))
));
claim_assert_static(a_is_const);

typedef struct {
    enum_(Tag $bits(8)) {
        Tag_A = 0,
    } tag;
    union {
        u8 A;
        u8 B;
    } payload;
} Tag_Struct;

typedef FieldType$(Tag_Struct, tag) MyTag;
let c = T_switch$((MyTag)(
    T_case$((u8)(0)),
    T_case$((u16)(1)),
    T_case$((u32)(2)),
    T_case$((u64)(3)),
    T_case$((i8)(4)),
    T_case$((i16)(5)),
    T_case$((i32)(6)),
    T_case$((i64)(7)),
    T_default_(false)
));
