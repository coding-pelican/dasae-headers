#include "dh-main.h"
#include "dh/ArrDeq.h"
#include "dh/heap/Sys.h"

T_use$((u32)(
    ArrDeq,
    ArrDeq_fixed,
    ArrDeq_init,
    ArrDeq_fini,
    ArrDeq_len,
    ArrDeq_cap,
    ArrDeq_at,
    ArrDeq_atMut,
    ArrDeq_front,
    ArrDeq_frontMut,
    ArrDeq_back,
    ArrDeq_backMut,
    ArrDeq_ensureCapPrecise,
    ArrDeq_ensureUnusedCap,
    ArrDeq_clearRetainingCap,
    ArrDeq_clearAndFree,
    ArrDeq_appendFixed,
    ArrDeq_append,
    ArrDeq_appendWithin,
    ArrDeq_prependFixed,
    ArrDeq_prepend,
    ArrDeq_pop,
    ArrDeq_shift,
    ArrDeq_Iter,
    ArrDeq_iter,
    ArrDeq_Iter_next,
    ArrDeq_Iter_nextMut
));

TEST_fn_("ArrDeq: fixed buffer pushes and pops both ends" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0 });
    var deq = ArrDeq_fixed$u32(A_ref$((S$u32)(buf)));

    try_(TEST_expect(ArrDeq_len$u32(deq) == 0));
    try_(TEST_expect(isNone(ArrDeq_front$u32(deq))));
    try_(TEST_expect(isNone(ArrDeq_back$u32(deq))));

    try_(ArrDeq_appendFixed$u32(&deq, 2));
    try_(ArrDeq_appendFixed$u32(&deq, 3));
    try_(ArrDeq_prependFixed$u32(&deq, 1));

    try_(TEST_expect(ArrDeq_len$u32(deq) == 3));
    try_(TEST_expect(*unwrap_(ArrDeq_front$u32(deq)) == 1));
    try_(TEST_expect(*unwrap_(ArrDeq_back$u32(deq)) == 3));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 0) == 1));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 1) == 2));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 2) == 3));

    try_(TEST_expect(unwrap_(ArrDeq_pop$u32(&deq)) == 3));
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 2));
    try_(TEST_expect(isNone(ArrDeq_pop$u32(&deq))));
    try_(TEST_expect(isNone(ArrDeq_shift$u32(&deq))));
} $unscoped(TEST_fn);

TEST_fn_("ArrDeq: fixed buffer preserves logical order across wrap" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0 });
    var deq = ArrDeq_fixed$u32(A_ref$((S$u32)(buf)));

    try_(ArrDeq_appendFixed$u32(&deq, 1));
    try_(ArrDeq_appendFixed$u32(&deq, 2));
    try_(ArrDeq_appendFixed$u32(&deq, 3));
    try_(ArrDeq_appendFixed$u32(&deq, 4));
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 2));
    try_(ArrDeq_appendFixed$u32(&deq, 5));
    try_(ArrDeq_appendFixed$u32(&deq, 6));

    try_(TEST_expect(*ArrDeq_at$u32(deq, 0) == 3));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 1) == 4));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 2) == 5));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 3) == 6));
    try_(TEST_expect(isErr(ArrDeq_appendFixed$u32(&deq, 7))));
    try_(TEST_expect(isErr(ArrDeq_prependFixed$u32(&deq, 0))));

    var it = ArrDeq_iter$u32(&deq);
    var expected = as$(u32)(3);
    while_some(ArrDeq_Iter_next$u32(&it), item) {
        try_(TEST_expect(*item == expected));
        expected += 1;
    }
    try_(TEST_expect(expected == 7));
} $unscoped(TEST_fn);

TEST_fn_("ArrDeq: grow preserves wrapped logical order" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var deq = try_(ArrDeq_init$u32(gpa, 4));
    defer_(ArrDeq_fini$u32(&deq, gpa));

    ArrDeq_appendWithin$u32(&deq, 1);
    ArrDeq_appendWithin$u32(&deq, 2);
    ArrDeq_appendWithin$u32(&deq, 3);
    ArrDeq_appendWithin$u32(&deq, 4);
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrDeq_shift$u32(&deq)) == 2));
    ArrDeq_appendWithin$u32(&deq, 5);
    ArrDeq_appendWithin$u32(&deq, 6);

    try_(ArrDeq_ensureCapPrecise$u32(&deq, gpa, 8));

    try_(TEST_expect(ArrDeq_cap$u32(deq) >= 8));
    try_(TEST_expect(ArrDeq_len$u32(deq) == 4));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 0) == 3));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 1) == 4));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 2) == 5));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 3) == 6));
} $unguarded(TEST_fn);

TEST_fn_("ArrDeq: dynamic prepend append and growth preserve both endpoints" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var deq = try_(ArrDeq_init$u32(gpa, 0));
    defer_(ArrDeq_fini$u32(&deq, gpa));

    try_(ArrDeq_append$u32(&deq, gpa, 2));
    try_(ArrDeq_prepend$u32(&deq, gpa, 1));
    try_(ArrDeq_append$u32(&deq, gpa, 3));
    try_(ArrDeq_prepend$u32(&deq, gpa, 0));

    try_(TEST_expect(ArrDeq_len$u32(deq) == 4));
    try_(TEST_expect(ArrDeq_cap$u32(deq) >= 4));
    try_(TEST_expect(*unwrap_(ArrDeq_front$u32(deq)) == 0));
    try_(TEST_expect(*unwrap_(ArrDeq_back$u32(deq)) == 3));
    for (usize i = 0; i < 4; i++) {
        try_(TEST_expect(*ArrDeq_at$u32(deq, i) == as$(u32)(i)));
    }
} $unguarded(TEST_fn);

TEST_fn_("ArrDeq: mutable iterator and endpoint access mutate logical items" $scope) {
    var buf = A_from$((u32){ 0, 0, 0, 0 });
    var deq = ArrDeq_fixed$u32(A_ref$((S$u32)(buf)));
    try_(ArrDeq_appendFixed$u32(&deq, 1));
    try_(ArrDeq_appendFixed$u32(&deq, 2));
    try_(ArrDeq_appendFixed$u32(&deq, 3));

    *unwrap_(ArrDeq_frontMut$u32(deq)) = 10;
    *unwrap_(ArrDeq_backMut$u32(deq)) = 30;
    *ArrDeq_atMut$u32(deq, 1) = 20;

    var it = ArrDeq_iter$u32(&deq);
    while_some(ArrDeq_Iter_nextMut$u32(&it), item) {
        *item += 1;
    }

    try_(TEST_expect(*ArrDeq_at$u32(deq, 0) == 11));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 1) == 21));
    try_(TEST_expect(*ArrDeq_at$u32(deq, 2) == 31));
} $unscoped(TEST_fn);

TEST_fn_("ArrDeq: clear retaining capacity and clear free define capacity ownership" $guard) {
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    var deq = try_(ArrDeq_init$u32(gpa, 4));

    ArrDeq_appendWithin$u32(&deq, 1);
    ArrDeq_appendWithin$u32(&deq, 2);
    let retained_cap = ArrDeq_cap$u32(deq);
    ArrDeq_clearRetainingCap$u32(&deq);
    try_(TEST_expect(ArrDeq_len$u32(deq) == 0));
    try_(TEST_expect(ArrDeq_cap$u32(deq) == retained_cap));
    try_(TEST_expect(isNone(ArrDeq_pop$u32(&deq))));
    try_(TEST_expect(isNone(ArrDeq_shift$u32(&deq))));

    try_(ArrDeq_ensureUnusedCap$u32(&deq, gpa, 2));
    try_(TEST_expect(ArrDeq_cap$u32(deq) >= 2));
    ArrDeq_clearAndFree$u32(&deq, gpa);
    try_(TEST_expect(ArrDeq_len$u32(deq) == 0));
    try_(TEST_expect(ArrDeq_cap$u32(deq) == 0));
} $unguarded(TEST_fn);
