#include "dh/main.h"
#include "dh/ArrPDeq.h"
#include "dh/heap/Page.h"

T_use$((u32)(
    ArrPDeq,
    ArrPDeq_init,
    ArrPDeq_fini,
    ArrPDeq_len,
    ArrPDeq_cap,
    ArrPDeq_peekMin,
    ArrPDeq_peekMax,
    ArrPDeq_ensureCap,
    ArrPDeq_shrinkAndFree,
    ArrPDeq_enque,
    ArrPDeq_enqueWithin,
    ArrPDeq_enqueS,
    ArrPDeq_dequeMin,
    ArrPDeq_dequeMax,
    ArrPDeq_removeAt,
    ArrPDeq_update,
    ArrPDeq_Iter,
    ArrPDeq_iter,
    ArrPDeq_Iter_next
));

TEST_fn_("enque and deque min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 54);
    ArrPDeq_enqueWithin$u32(&deq, 12);
    ArrPDeq_enqueWithin$u32(&deq, 7);
    ArrPDeq_enqueWithin$u32(&deq, 23);
    ArrPDeq_enqueWithin$u32(&deq, 25);
    ArrPDeq_enqueWithin$u32(&deq, 13);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 7));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 12));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 13));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 23));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 25));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 54));
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 54);
    ArrPDeq_enqueWithin$u32(&deq, 12);
    ArrPDeq_enqueWithin$u32(&deq, 7);
    ArrPDeq_enqueWithin$u32(&deq, 23);
    ArrPDeq_enqueWithin$u32(&deq, 25);
    ArrPDeq_enqueWithin$u32(&deq, 13);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 54));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 25));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 23));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 13));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 12));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 7));
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque same min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 2));
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque same max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("deque on empty" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 0, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));
    try_(TEST_expect(isNone(ArrPDeq_dequeMin$u32(&deq))));
    try_(TEST_expect(isNone(ArrPDeq_dequeMax$u32(&deq))));
} $unguarded_(TEST_fn);

TEST_fn_("edge case 3 elements min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 3, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 9);
    ArrPDeq_enqueWithin$u32(&deq, 3);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 3));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 9));
} $unguarded_(TEST_fn);

TEST_fn_("edge case 3 elements max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 3, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 9);
    ArrPDeq_enqueWithin$u32(&deq, 3);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 9));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 3));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 2));
} $unguarded_(TEST_fn);

TEST_fn_("peekMin" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    // Empty deque should return none
    try_(TEST_expect(isNone(ArrPDeq_peekMin$u32(deq))));

    ArrPDeq_enqueWithin$u32(&deq, 9);
    ArrPDeq_enqueWithin$u32(&deq, 3);
    ArrPDeq_enqueWithin$u32(&deq, 2);

    // Peek should return minimum without removing
    try_(TEST_expect(*unwrap_(ArrPDeq_peekMin$u32(deq)) == 2));
    try_(TEST_expect(*unwrap_(ArrPDeq_peekMin$u32(deq)) == 2)); // Still 2, not removed
} $unguarded_(TEST_fn);

TEST_fn_("peekMax" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    // Empty deque should return none
    try_(TEST_expect(isNone(ArrPDeq_peekMax$u32(deq))));

    ArrPDeq_enqueWithin$u32(&deq, 9);
    ArrPDeq_enqueWithin$u32(&deq, 3);
    ArrPDeq_enqueWithin$u32(&deq, 2);

    // Peek should return maximum without removing
    try_(TEST_expect(*unwrap_(ArrPDeq_peekMax$u32(deq)) == 9));
    try_(TEST_expect(*unwrap_(ArrPDeq_peekMax$u32(deq)) == 9)); // Still 9, not removed
} $unguarded_(TEST_fn);

TEST_fn_("sift up with odd indices, dequeMin" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 32, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    for_(($a(items))(item) {
        ArrPDeq_enqueWithin$u32(&deq, *item);
    });

    let sorted_items = A_from$((u32){ 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("sift up with odd indices, dequeMax" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 32, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    for_(($a(items))(item) {
        ArrPDeq_enqueWithin$u32(&deq, *item);
    });

    let sorted_items = A_from$((u32){ 25, 24, 24, 22, 21, 16, 15, 15, 14, 13, 12, 11, 7, 7, 6, 5, 2, 1 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("enqueS min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 32, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    try_(ArrPDeq_enqueS$u32(&deq, gpa, A_ref$((S_const$u32)(items))));

    let sorted_items = A_from$((u32){ 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("enqueS max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 32, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    try_(ArrPDeq_enqueS$u32(&deq, gpa, A_ref$((S_const$u32)(items))));

    let sorted_items = A_from$((u32){ 25, 24, 24, 22, 21, 16, 15, 15, 14, 13, 12, 11, 7, 7, 6, 5, 2, 1 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("update min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 55);
    ArrPDeq_enqueWithin$u32(&deq, 44);
    ArrPDeq_enqueWithin$u32(&deq, 11);
    try_(ArrPDeq_update$u32(&deq, 55, 5));
    try_(ArrPDeq_update$u32(&deq, 44, 4));
    try_(ArrPDeq_update$u32(&deq, 11, 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 5));
} $unguarded_(TEST_fn);

TEST_fn_("update same min" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    try_(ArrPDeq_update$u32(&deq, 1, 5));
    try_(ArrPDeq_update$u32(&deq, 2, 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 5));
} $unguarded_(TEST_fn);

TEST_fn_("update max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 55);
    ArrPDeq_enqueWithin$u32(&deq, 44);
    ArrPDeq_enqueWithin$u32(&deq, 11);
    try_(ArrPDeq_update$u32(&deq, 55, 5));
    try_(ArrPDeq_update$u32(&deq, 44, 1));
    try_(ArrPDeq_update$u32(&deq, 11, 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 5));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("update same max" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 1);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    try_(ArrPDeq_update$u32(&deq, 1, 5));
    try_(ArrPDeq_update$u32(&deq, 2, 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 5));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 4));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("iterator" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    let items = A_from$((u32){ 54, 12, 7, 23, 25, 13 });
    var_(sum_expected, u32) = 0;
    for_(($a(items))(item) {
        ArrPDeq_enqueWithin$u32(&deq, *item);
        sum_expected += *item;
    });

    // Iterator should visit all elements (in heap order, not sorted order)
    var_(count, usize) = 0;
    var_(sum, u32) = 0;
    var it = ArrPDeq_iter$u32(&deq);
    while_some(ArrPDeq_Iter_next$u32(&it), elem) {
        sum += *elem;
        count++;
    }

    try_(TEST_expect(count == A_len(items)));
    try_(TEST_expect(sum == sum_expected));
} $unguarded_(TEST_fn);

TEST_fn_("iterator while empty" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    var it = ArrPDeq_iter$u32(&deq);
    try_(TEST_expect(isNone(ArrPDeq_Iter_next$u32(&it))));
} $unguarded_(TEST_fn);

TEST_fn_("remove at index" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    ArrPDeq_enqueWithin$u32(&deq, 3);
    ArrPDeq_enqueWithin$u32(&deq, 2);
    ArrPDeq_enqueWithin$u32(&deq, 1);

    // Find the index of element 2
    let two_idx = eval_(O$usize $scope)({
        var_(idx_to_find, usize) = 0;
        var it = ArrPDeq_iter$u32(&deq);
        while_some(ArrPDeq_Iter_next$u32(&it), elem) {
            if (*elem == 2) { $break_(some(idx_to_find)); }
            idx_to_find++;
        }
    }) eval_(else)($break_(none())) $unscoped_(eval);
    try_(TEST_expect(isSome(two_idx)));

    // Remove element 2
    let removed = ArrPDeq_removeAt$u32(&deq, unwrap_(two_idx));
    try_(TEST_expect(removed == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 1));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$u32(&deq)) == 3));
    try_(TEST_expect(isNone(ArrPDeq_dequeMin$u32(&deq))));
} $unguarded_(TEST_fn);

TEST_fn_("shrinkAndFree" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPDeq_Ctx_default(cmp_MathType_u32);
    var deq = try_(ArrPDeq_init$u32(gpa, 0, &ctx));
    defer_(ArrPDeq_fini$u32(&deq, gpa));

    try_(ArrPDeq_ensureCap$u32(&deq, gpa, 4));
    try_(TEST_expect(ArrPDeq_cap$u32(deq) >= 4));

    try_(ArrPDeq_enque$u32(&deq, gpa, 1));
    try_(ArrPDeq_enque$u32(&deq, gpa, 2));
    try_(ArrPDeq_enque$u32(&deq, gpa, 3));
    try_(TEST_expect(ArrPDeq_cap$u32(deq) >= 4));
    try_(TEST_expect(ArrPDeq_len$u32(deq) == 3));

    ArrPDeq_shrinkAndFree$u32(&deq, gpa, 3);
    try_(TEST_expect(ArrPDeq_cap$u32(deq) == 3));
    try_(TEST_expect(ArrPDeq_len$u32(deq) == 3));

    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 3));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 2));
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$u32(&deq)) == 1));
    try_(TEST_expect(isNone(ArrPDeq_dequeMax$u32(&deq))));
} $unguarded_(TEST_fn);

// Test with context comparator (indices into external array)
T_use$((usize)(
    ArrPDeq,
    ArrPDeq_init,
    ArrPDeq_fini,
    ArrPDeq_enqueWithin,
    ArrPDeq_dequeMin,
    ArrPDeq_dequeMax
));

$static fn_((test_ascCtx(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(cmp_Ord)) {
    let c = u_castV$((S_const$u32)(ctx));
    let l = u_cast((usize)(lhs));
    let r = u_cast((usize)(rhs));
    return prim_ord(*S_at((c)[l]), *S_at((c)[r]));
};

TEST_fn_("enque and deque with context comparator" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx_arr = A_from$((u32){ 5, 3, 4, 2, 2, 8, 0 });
    let ctx_sli = A_ref$((S_const$u32)(ctx_arr));
    let ctx = (ArrPDeq_Ctx){
        .inner = u_anyP(&ctx_sli),
        .ordFn = test_ascCtx,
    };
    var deq = try_(ArrPDeq_init$usize(gpa, 8, &ctx));
    defer_(ArrPDeq_fini$usize(&deq, gpa));

    ArrPDeq_enqueWithin$usize(&deq, 0);
    ArrPDeq_enqueWithin$usize(&deq, 1);
    ArrPDeq_enqueWithin$usize(&deq, 2);
    ArrPDeq_enqueWithin$usize(&deq, 3);
    ArrPDeq_enqueWithin$usize(&deq, 4);
    ArrPDeq_enqueWithin$usize(&deq, 5);
    ArrPDeq_enqueWithin$usize(&deq, 6);
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$usize(&deq)) == 6)); // context[6] = 0 (smallest)
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$usize(&deq)) == 5)); // context[5] = 8 (largest)
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$usize(&deq)) == 3)); // context[3] = 2
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$usize(&deq)) == 0)); // context[0] = 5
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$usize(&deq)) == 4)); // context[4] = 2
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMax$usize(&deq)) == 2)); // context[2] = 4
    try_(TEST_expect(unwrap_(ArrPDeq_dequeMin$usize(&deq)) == 1)); // context[1] = 3 (last one)
} $unguarded_(TEST_fn);
