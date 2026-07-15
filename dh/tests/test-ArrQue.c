#include "dh-main.h"
#include "dh/ArrQue.h"
#include "dh/heap/Sys.h"

T_use$((u32)(
    ArrQue,
    ArrQue_fixed,
    ArrQue_init,
    ArrQue_fini,
    ArrQue_len,
    ArrQue_cap,
    ArrQue_at,
    ArrQue_atMut,
    ArrQue_front,
    ArrQue_frontMut,
    ArrQue_back,
    ArrQue_backMut,
    ArrQue_clearRetainingCap,
    ArrQue_clearAndFree,
    ArrQue_enque,
    ArrQue_enqueFixed,
    ArrQue_deque,
    ArrQue_Iter,
    ArrQue_iter,
    ArrQue_Iter_next,
    ArrQue_Iter_nextMut
));

TEST_fn_("ArrQue: fixed buffer dequeues FIFO across wrap" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0 });
    var que = ArrQue_fixed$u32(A_ref$((S$u32)(buf)));

    try_(TEST_expect(isNone(ArrQue_front$u32(que))));
    try_(TEST_expect(isNone(ArrQue_back$u32(que))));

    try_(ArrQue_enqueFixed$u32(&que, 1));
    try_(ArrQue_enqueFixed$u32(&que, 2));
    try_(ArrQue_enqueFixed$u32(&que, 3));
    try_(ArrQue_enqueFixed$u32(&que, 4));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 1));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 2));
    try_(ArrQue_enqueFixed$u32(&que, 5));
    try_(ArrQue_enqueFixed$u32(&que, 6));

    try_(TEST_expect(ArrQue_len$u32(que) == 4));
    try_(TEST_expect(*unwrap_(ArrQue_front$u32(que)) == 3));
    try_(TEST_expect(*unwrap_(ArrQue_back$u32(que)) == 6));
    try_(TEST_expect(*ArrQue_at$u32(que, 0) == 3));
    try_(TEST_expect(*ArrQue_at$u32(que, 1) == 4));
    try_(TEST_expect(*ArrQue_at$u32(que, 2) == 5));
    try_(TEST_expect(*ArrQue_at$u32(que, 3) == 6));
    try_(TEST_expect(isErr(ArrQue_enqueFixed$u32(&que, 7))));

    var it = ArrQue_iter$u32(&que);
    var expected = as$(u32)(3);
    while_some(ArrQue_Iter_next$u32(&it), item) {
        try_(TEST_expect(*item == expected));
        expected += 1;
    }
    try_(TEST_expect(expected == 7));

    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 3));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 4));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 5));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 6));
    try_(TEST_expect(isNone(ArrQue_deque$u32(&que))));
} $unscoped(TEST_fn);

TEST_fn_("ArrQue: dynamic enqueue grows and preserves FIFO order" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var que = try_(ArrQue_init$u32(gpa, 0));
    defer_(ArrQue_fini$u32(&que, gpa));

    for (usize i = 0; i < 8; i++) {
        try_(ArrQue_enque$u32(&que, gpa, as$(u32)(i + 1)));
    }

    try_(TEST_expect(ArrQue_len$u32(que) == 8));
    try_(TEST_expect(ArrQue_cap$u32(que) >= 8));
    for (usize i = 0; i < 8; i++) {
        try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == as$(u32)(i + 1)));
    }
    try_(TEST_expect(isNone(ArrQue_deque$u32(&que))));
} $unguarded(TEST_fn);

TEST_fn_("ArrQue: mutable access mutates queue logical order" $scope) {
    var buf = A_from$((u32){ 0, 0, 0 });
    var que = ArrQue_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrQue_enqueFixed$u32(&que, 1));
    try_(ArrQue_enqueFixed$u32(&que, 2));
    try_(ArrQue_enqueFixed$u32(&que, 3));

    *unwrap_(ArrQue_frontMut$u32(que)) = 10;
    *ArrQue_atMut$u32(que, 1) = 20;
    *unwrap_(ArrQue_backMut$u32(que)) = 30;
    var it = ArrQue_iter$u32(&que);
    while_some(ArrQue_Iter_nextMut$u32(&it), item) {
        *item += 1;
    }

    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 11));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 21));
    try_(TEST_expect(unwrap_(ArrQue_deque$u32(&que)) == 31));
} $unscoped(TEST_fn);

TEST_fn_("ArrQue: clear retaining capacity and clear free define capacity ownership" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var que = try_(ArrQue_init$u32(gpa, 4));

    try_(ArrQue_enqueFixed$u32(&que, 1));
    try_(ArrQue_enqueFixed$u32(&que, 2));
    let retained_cap = ArrQue_cap$u32(que);
    ArrQue_clearRetainingCap$u32(&que);
    try_(TEST_expect(ArrQue_len$u32(que) == 0));
    try_(TEST_expect(ArrQue_cap$u32(que) == retained_cap));
    try_(TEST_expect(isNone(ArrQue_deque$u32(&que))));

    ArrQue_clearAndFree$u32(&que, gpa);
    try_(TEST_expect(ArrQue_len$u32(que) == 0));
    try_(TEST_expect(ArrQue_cap$u32(que) == 0));
} $unguarded(TEST_fn);
