#include "dh-main.h"
#include "dh/ArrStk.h"
#include "dh/heap/Sys.h"

T_use_P$(u32);
T_use_E$($set(mem_E)(P$u32));
T_use_E$($set(mem_E)(S$u32));
T_use$((u32)(
    ArrStk,
    ArrStk_fixed,
    ArrStk_init,
    ArrStk_fini,
    ArrStk_clone,
    ArrStk_len,
    ArrStk_cap,
    ArrStk_at,
    ArrStk_atMut,
    ArrStk_top,
    ArrStk_topMut,
    ArrStk_resize,
    ArrStk_shrinkRetainingCap,
    ArrStk_clearRetainingCap,
    ArrStk_clearAndFree,
    ArrStk_addFixed,
    ArrStk_addNFixed,
    ArrStk_pushFixed,
    ArrStk_pushSFixed,
    ArrStk_pushNFixed,
    ArrStk_pop
));

TEST_fn_("ArrStk: fixed buffer pops LIFO" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0 });
    var stk = ArrStk_fixed$u32(A_ref$((S$u32)(buf)));

    try_(TEST_expect(ArrStk_len$u32(stk) == 0));
    try_(TEST_expect(ArrStk_cap$u32(stk) == 4));
    try_(TEST_expect(isNone(ArrStk_top$u32(stk))));

    try_(ArrStk_pushFixed$u32(&stk, 1));
    try_(ArrStk_pushFixed$u32(&stk, 2));
    try_(ArrStk_pushFixed$u32(&stk, 3));

    try_(TEST_expect(ArrStk_len$u32(stk) == 3));
    try_(TEST_expect(*ArrStk_at$u32(stk, 0) == 1));
    try_(TEST_expect(*ArrStk_at$u32(stk, 1) == 2));
    try_(TEST_expect(*ArrStk_at$u32(stk, 2) == 3));
    try_(TEST_expect(*unwrap_(ArrStk_top$u32(stk)) == 3));

    try_(TEST_expect(unwrap_(ArrStk_pop$u32(&stk)) == 3));
    try_(TEST_expect(unwrap_(ArrStk_pop$u32(&stk)) == 2));
    try_(TEST_expect(unwrap_(ArrStk_pop$u32(&stk)) == 1));
    try_(TEST_expect(isNone(ArrStk_pop$u32(&stk))));
} $unscoped(TEST_fn);

TEST_fn_("ArrStk: slice and repeated pushes preserve order" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0, 0 });
    var stk = ArrStk_fixed$u32(A_ref$((S$u32)(buf)));
    let items = A_from$((u32){ 1, 2, 3 });

    try_(ArrStk_pushSFixed$u32(&stk, A_ref$((S_const$u32)(items))));
    try_(ArrStk_pushNFixed$u32(&stk, 4, 2));
    let result = ArrStk_pushFixed$u32(&stk, 6);
    try_(TEST_expect(isErr(result)));

    try_(TEST_expect(ArrStk_len$u32(stk) == 5));
    try_(TEST_expect(*ArrStk_at$u32(stk, 0) == 1));
    try_(TEST_expect(*ArrStk_at$u32(stk, 1) == 2));
    try_(TEST_expect(*ArrStk_at$u32(stk, 2) == 3));
    try_(TEST_expect(*ArrStk_at$u32(stk, 3) == 4));
    try_(TEST_expect(*ArrStk_at$u32(stk, 4) == 4));
} $unscoped(TEST_fn);

TEST_fn_("ArrStk: add APIs return writable slots" $scope) {
    var buf = A_from$((u32){ 0, 0, 0 });
    var stk = ArrStk_fixed$u32(A_ref$((S$u32)(buf)));

    *try_(ArrStk_addFixed$u32(&stk)) = 1;
    let slots = try_(ArrStk_addNFixed$u32(&stk, 2));
    slots.ptr[0] = 2;
    slots.ptr[1] = 3;

    try_(TEST_expect(ArrStk_len$u32(stk) == 3));
    try_(TEST_expect(*ArrStk_at$u32(stk, 0) == 1));
    try_(TEST_expect(*ArrStk_at$u32(stk, 1) == 2));
    try_(TEST_expect(*ArrStk_at$u32(stk, 2) == 3));
} $unscoped(TEST_fn);

TEST_fn_("ArrStk: dynamic resize clear and clone define ownership" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var stk = try_(ArrStk_init$u32(gpa, 2));
    defer_(ArrStk_fini$u32(&stk, gpa));

    try_(ArrStk_pushFixed$u32(&stk, 1));
    try_(ArrStk_pushFixed$u32(&stk, 2));
    try_(ArrStk_resize$u32(&stk, gpa, 4));
    *ArrStk_atMut$u32(stk, 2) = 3;
    *unwrap_(ArrStk_topMut$u32(stk)) = 4;
    try_(TEST_expect(ArrStk_len$u32(stk) == 4));

    var cloned = try_(ArrStk_clone$u32(stk, gpa));
    defer_(ArrStk_fini$u32(&cloned, gpa));
    *ArrStk_atMut$u32(cloned, 0) = 10;
    try_(TEST_expect(*ArrStk_at$u32(stk, 0) == 1));
    try_(TEST_expect(*ArrStk_at$u32(cloned, 0) == 10));

    ArrStk_shrinkRetainingCap$u32(&stk, 2);
    let retained_cap = ArrStk_cap$u32(stk);
    ArrStk_clearRetainingCap$u32(&stk);
    try_(TEST_expect(ArrStk_len$u32(stk) == 0));
    try_(TEST_expect(ArrStk_cap$u32(stk) == retained_cap));
    ArrStk_clearAndFree$u32(&stk, gpa);
    try_(TEST_expect(ArrStk_len$u32(stk) == 0));
    try_(TEST_expect(ArrStk_cap$u32(stk) == 0));
} $unguarded(TEST_fn);
