#include "dh/main.h"
#include "dh/ArrPQue.h"
#include "dh/heap/Page.h"
#include "dh/io/stream.h"

T_use$((u32)(
    ArrPQue,
    ArrPQue_init,
    ArrPQue_fini,
    ArrPQue_len,
    ArrPQue_cap,
    ArrPQue_items,
    ArrPQue_peek,
    ArrPQue_ensureCap,
    ArrPQue_shrinkAndFree,
    ArrPQue_enque,
    ArrPQue_enqueWithin,
    ArrPQue_enqueS,
    ArrPQue_deque,
    ArrPQue_removeAt,
    ArrPQue_update,
    ArrPQue_Iter,
    ArrPQue_iter,
    ArrPQue_Iter_next
));

$attr($maybe_unused)
$static fn_((printHeap(ArrPQue$u32 pq))(void)) {
    let items = ArrPQue_items$u32(pq);
    io_stream_print(u8_l("Heap: ["));
    for_(($rf(0), $s(items))(i, val) {
        if (i > 0) {
            io_stream_print(u8_l(", "));
        }
        io_stream_print(u8_l("{:u}"), *val);
    });
    io_stream_println(u8_l("]"));
};

TEST_fn_("enque and deque min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 54);
    ArrPQue_enqueWithin$u32(&pq, 12);
    ArrPQue_enqueWithin$u32(&pq, 7);
    ArrPQue_enqueWithin$u32(&pq, 23);
    ArrPQue_enqueWithin$u32(&pq, 25);
    ArrPQue_enqueWithin$u32(&pq, 13);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 7));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 12));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 13));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 23));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 25));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 54));
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque same min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
} $unguarded_(TEST_fn);

TEST_fn_("deque on empty" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 0, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));
    try_(TEST_expect(isNone(ArrPQue_deque$u32(&pq))));
} $unguarded_(TEST_fn);

TEST_fn_("edge case 3 elements" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 3, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 9);
    ArrPQue_enqueWithin$u32(&pq, 3);
    ArrPQue_enqueWithin$u32(&pq, 2);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 3));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 9));
} $unguarded_(TEST_fn);

TEST_fn_("peek" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    // Empty queue should return none
    try_(TEST_expect(isNone(ArrPQue_peek$u32(pq))));

    ArrPQue_enqueWithin$u32(&pq, 9);
    ArrPQue_enqueWithin$u32(&pq, 3);
    ArrPQue_enqueWithin$u32(&pq, 2);

    // Peek should return minimum without removing
    try_(TEST_expect(*unwrap_(ArrPQue_peek$u32(pq)) == 2));
    try_(TEST_expect(*unwrap_(ArrPQue_peek$u32(pq)) == 2)); // Still 2, not removed
    try_(TEST_expect(ArrPQue_len$u32(pq) == 3));            // Count unchanged
} $unguarded_(TEST_fn);

TEST_fn_("sift up with odd indices" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 32, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    for_(($a(items))(item) {
        ArrPQue_enqueWithin$u32(&pq, *item);
    });

    let sorted_items = A_from$((u32){ 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("enqueS (addSlice)" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 32, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    let items = A_from$((u32){ 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 });
    try_(ArrPQue_enqueS$u32(&pq, gpa, A_ref$((S_const$u32)(items))));

    let sorted_items = A_from$((u32){ 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == *sorted));
    });
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque max heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultDesc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 54);
    ArrPQue_enqueWithin$u32(&pq, 12);
    ArrPQue_enqueWithin$u32(&pq, 7);
    ArrPQue_enqueWithin$u32(&pq, 23);
    ArrPQue_enqueWithin$u32(&pq, 25);
    ArrPQue_enqueWithin$u32(&pq, 13);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 54));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 25));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 23));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 13));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 12));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 7));
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque same max heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultDesc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("iterator" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    let items = A_from$((u32){ 54, 12, 7, 23, 25, 13 });
    var_(sum_expected, u32) = 0;
    for_(($a(items))(item) {
        ArrPQue_enqueWithin$u32(&pq, *item);
        sum_expected += *item;
    });

    // Iterator should visit all elements (in heap order, not sorted order)
    var_(count, usize) = 0;
    var_(sum, u32) = 0;
    var it = ArrPQue_iter$u32(&pq);
    while_some(ArrPQue_Iter_next$u32(&it), elem) {
        sum += *elem;
        count++;
    }

    try_(TEST_expect(count == A_len(items)));
    try_(TEST_expect(sum == sum_expected));
} $unguarded_(TEST_fn);

TEST_fn_("iterator while empty" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    var it = ArrPQue_iter$u32(&pq);
    try_(TEST_expect(isNone(ArrPQue_Iter_next$u32(&it))));
} $unguarded_(TEST_fn);

TEST_fn_("remove at index" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    let items = A_from$((u32){ 2, 1, 8, 9, 3, 4, 5 });
    for_(($a(items))(item) {
        ArrPQue_enqueWithin$u32(&pq, *item);
    });

    // Find the index of element 2
    let two_idx = eval_(O$usize $scope)({
        var_(idx_to_find, usize) = 0;
        var it = ArrPQue_iter$u32(&pq);
        while_some(ArrPQue_Iter_next$u32(&it), elem) {
            if (*elem == 2) { $break_(some(idx_to_find)); }
            idx_to_find++;
        }
    }) eval_(else)($break_(none())) $unscoped_(eval);
    try_(TEST_expect(isSome(two_idx)));

    // Remove element 2
    let removed = ArrPQue_removeAt$u32(&pq, unwrap_(two_idx));
    try_(TEST_expect(removed == 2));

    // Remaining elements should be sorted correctly
    let sorted_items = A_from$((u32){ 1, 3, 4, 5, 8, 9 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == *sorted));
    });
    try_(TEST_expect(isNone(ArrPQue_deque$u32(&pq))));
} $unguarded_(TEST_fn);

TEST_fn_("shrinkAndFree" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 0, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    try_(ArrPQue_ensureCap$u32(&pq, gpa, 4));
    try_(TEST_expect(ArrPQue_cap$u32(pq) >= 4));

    try_(ArrPQue_enque$u32(&pq, gpa, 1));
    try_(ArrPQue_enque$u32(&pq, gpa, 2));
    try_(ArrPQue_enque$u32(&pq, gpa, 3));
    try_(TEST_expect(ArrPQue_cap$u32(pq) >= 4));
    try_(TEST_expect(ArrPQue_len$u32(pq) == 3));

    ArrPQue_shrinkAndFree$u32(&pq, gpa, 3);
    try_(TEST_expect(ArrPQue_cap$u32(pq) == 3));
    try_(TEST_expect(ArrPQue_len$u32(pq) == 3));

    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 3));
    try_(TEST_expect(isNone(ArrPQue_deque$u32(&pq))));
} $unguarded_(TEST_fn);

TEST_fn_("update min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 55);
    ArrPQue_enqueWithin$u32(&pq, 44);
    ArrPQue_enqueWithin$u32(&pq, 11);
    try_(ArrPQue_update$u32(&pq, 55, 5));
    try_(ArrPQue_update$u32(&pq, 44, 4));
    try_(ArrPQue_update$u32(&pq, 11, 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 5));
} $unguarded_(TEST_fn);

TEST_fn_("update same min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 2);
    try_(ArrPQue_update$u32(&pq, 1, 5));
    try_(ArrPQue_update$u32(&pq, 2, 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 5));
} $unguarded_(TEST_fn);

TEST_fn_("update max heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultDesc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 55);
    ArrPQue_enqueWithin$u32(&pq, 44);
    ArrPQue_enqueWithin$u32(&pq, 11);
    try_(ArrPQue_update$u32(&pq, 55, 5));
    try_(ArrPQue_update$u32(&pq, 44, 1));
    try_(ArrPQue_update$u32(&pq, 11, 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 5));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("update same max heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultDesc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 8, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 1);
    ArrPQue_enqueWithin$u32(&pq, 2);
    ArrPQue_enqueWithin$u32(&pq, 2);
    try_(ArrPQue_update$u32(&pq, 1, 5));
    try_(ArrPQue_update$u32(&pq, 2, 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 5));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 4));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("siftUp in remove" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    let ctx = ArrPQue_Ctx_defaultAsc(cmp_MathType_u32);
    var pq = try_(ArrPQue_init$u32(gpa, 32, &ctx));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    let items = A_from$((u32){ 0, 1, 100, 2, 3, 101, 102, 4, 5, 6, 7, 103, 104, 105, 106, 8 });
    try_(ArrPQue_enqueS$u32(&pq, gpa, A_ref$((S_const$u32)(items))));

    // Find index of 102
    let target_idx = eval_(usize $scope)({
        var_(idx_to_find, usize) = 0;
        var it = ArrPQue_iter$u32(&pq);
        while_some(ArrPQue_Iter_next$u32(&it), elem) {
            if (*elem == 102) { $break_(idx_to_find); }
            idx_to_find++;
        }
    }) eval_(else)(claim_unreachable) $unscoped_(eval);

    // Remove 102
    let removed = ArrPQue_removeAt$u32(&pq, target_idx);
    try_(TEST_expect(removed == 102));

    // Check remaining elements come out sorted
    let sorted_items = A_from$((u32){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 100, 101, 103, 104, 105, 106 });
    for_(($a(sorted_items))(sorted) {
        try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == *sorted));
    });
} $unguarded_(TEST_fn);
