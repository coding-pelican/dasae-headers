#include "dh/main.h"
#include "dh/ArrPQue.h"
#include "dh/heap/Page.h"
#include "dh/io/stream.h"

fn_((ascOrd(u_V$raw l, u_V$raw r, u_V$raw ctx))(cmp_Ord)) {
    let_ignore = ctx;
    return prim_ord(u_castV$((u32)(l)), u_castV$((u32)(r)));
}

fn_((descOrd(u_V$raw l, u_V$raw r, u_V$raw ctx))(cmp_Ord)) {
    let_ignore = ctx;
    return cmp_Ord_inv(prim_ord(u_castV$((u32)(l)), u_castV$((u32)(r))));
}

T_use$((u32)(
    ArrPQue,
    ArrPQue_init,
    ArrPQue_fini,
    ArrPQue_enqueWithin,
    ArrPQue_deque,
    ArrPQue_items,
));

fn_((printHeap(ArrPQue$u32 pq))(void)) {
    let items = ArrPQue_items$u32(pq);
    io_stream_print(u8_l("Heap: ["));
    for_(($rf(0), $s(items))(i, val) {
        if (i > 0) {
            io_stream_print(u8_l(", "));
        }
        io_stream_print(u8_l("{:u}"), *val);
    });
    io_stream_println(u8_l("]"));
}

TEST_fn_("enque and deque min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var pq = try_(ArrPQue_init$u32(gpa, 8, ascOrd, u_anyP(&(const Void){})));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 54);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 12);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 7);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 23);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 25);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 13);
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 7));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 12));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 13));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 23));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 25));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 54));
    printHeap(pq);
} $unguarded_(TEST_fn);

TEST_fn_("enque and deque same min heap" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var pq = try_(ArrPQue_init$u32(gpa, 8, ascOrd, u_anyP(&(const Void){})));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 1);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 1);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 2);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 2);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 1);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 1);
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 1));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    printHeap(pq);
} $unguarded_(TEST_fn);

TEST_fn_("deque on empty" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var pq = try_(ArrPQue_init$u32(gpa, 0, ascOrd, u_anyP(&(const Void){})));
    defer_(ArrPQue_fini$u32(&pq, gpa));
    try_(TEST_expect(isNone(ArrPQue_deque$u32(&pq))));
} $unguarded_(TEST_fn);

TEST_fn_("edge case 3 elements" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var pq = try_(ArrPQue_init$u32(gpa, 3, ascOrd, u_anyP(&(const Void){})));
    defer_(ArrPQue_fini$u32(&pq, gpa));

    ArrPQue_enqueWithin$u32(&pq, 9);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 3);
    printHeap(pq);
    ArrPQue_enqueWithin$u32(&pq, 2);
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 2));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 3));
    printHeap(pq);
    try_(TEST_expect(unwrap_(ArrPQue_deque$u32(&pq)) == 9));
    printHeap(pq);
} $unguarded_(TEST_fn);

#if UNUSED_CODE
test "peek" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try expect(queue.peek() == null);
    try queue.add(9);
    try queue.add(3);
    try queue.add(2);
    try expectEqual(@as(u32, 2), queue.peek().?);
    try expectEqual(@as(u32, 2), queue.peek().?);
}

test "sift up with odd indices" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();
    const items = [_] u32 { 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 };
    for (items) {
        | e | {
            try queue.add(e);
        }
    }

    const sorted_items = [_] u32 { 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 };
    for (sorted_items) {
        | e | {
            try expectEqual(e, queue.remove());
        }
    }
}

test "addSlice" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();
    const items = [_] u32 { 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 };
    try queue.addSlice(items[0..]);

    const sorted_items = [_] u32 { 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 };
    for (sorted_items) {
        | e | {
            try expectEqual(e, queue.remove());
        }
    }
}

test "fromOwnedSlice trivial case 0" {
    const items = [0] u32{};
    const queue_items = try testing.allocator.dupe(u32, &items);
    var queue = PQlt.fromOwnedSlice(testing.allocator, queue_items[0..], {});
    defer queue.deinit();
    try expectEqual(@as(usize, 0), queue.count());
    try expect(queue.removeOrNull() == null);
}

test "fromOwnedSlice trivial case 1" {
    const items = [1] u32{ 1 };
    const queue_items = try testing.allocator.dupe(u32, &items);
    var queue = PQlt.fromOwnedSlice(testing.allocator, queue_items[0..], {});
    defer queue.deinit();

    try expectEqual(@as(usize, 1), queue.count());
    try expectEqual(items[0], queue.remove());
    try expect(queue.removeOrNull() == null);
}

test "fromOwnedSlice" {
    const items = [_] u32 { 15, 7, 21, 14, 13, 22, 12, 6, 7, 25, 5, 24, 11, 16, 15, 24, 2, 1 };
    const heap_items = try testing.allocator.dupe(u32, items[0..]);
    var queue = PQlt.fromOwnedSlice(testing.allocator, heap_items[0..], {});
    defer queue.deinit();

    const sorted_items = [_] u32 { 1, 2, 5, 6, 7, 7, 11, 12, 13, 14, 15, 15, 16, 21, 22, 24, 24, 25 };
    for (sorted_items) {
        | e | {
            try expectEqual(e, queue.remove());
        }
    }
}

test "add and remove max heap" {
    var queue = PQgt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(54);
    try queue.add(12);
    try queue.add(7);
    try queue.add(23);
    try queue.add(25);
    try queue.add(13);
    try expectEqual(@as(u32, 54), queue.remove());
    try expectEqual(@as(u32, 25), queue.remove());
    try expectEqual(@as(u32, 23), queue.remove());
    try expectEqual(@as(u32, 13), queue.remove());
    try expectEqual(@as(u32, 12), queue.remove());
    try expectEqual(@as(u32, 7), queue.remove());
}

test "add and remove same max heap" {
    var queue = PQgt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(1);
    try queue.add(1);
    try queue.add(2);
    try queue.add(2);
    try queue.add(1);
    try queue.add(1);
    try expectEqual(@as(u32, 2), queue.remove());
    try expectEqual(@as(u32, 2), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
}

test "iterator" {
    var queue = PQlt.init(testing.allocator, {});
    var map = std.AutoHashMap(u32, void).init(testing.allocator);
    defer {
        queue.deinit();
        map.deinit();
    }

    const items = [_] u32 { 54, 12, 7, 23, 25, 13 };
    for (items) {
        | e | {
            _ = try queue.add(e);
            try map.put(e, {});
        }
    }

    var it = queue.iterator();
    while (it.next()) {
        | e | {
            _ = map.remove(e);
        }
    }

    try expectEqual(@as(usize, 0), map.count());
}

test "remove at index" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    const items = [_] u32 { 2, 1, 8, 9, 3, 4, 5 };
    for (items) {
        | e | {
            _ = try queue.add(e);
        }
    }

    var it = queue.iterator();
    var idx : usize = 0;
    const two_idx = while (it.next()) | elem | {
        if (elem == 2) {
            break idx;
        }
        idx += 1;
    }
    else unreachable;
    const sorted_items = [_] u32 { 1, 3, 4, 5, 8, 9 };
    try expectEqual(queue.removeIndex(two_idx), 2);

    var i : usize = 0;
    while (queue.removeOrNull()) {
        | n | : (i += 1) {
            try expectEqual(n, sorted_items[i]);
        }
    }
    try expectEqual(queue.removeOrNull(), null);
}

test "iterator while empty" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    var it = queue.iterator();

    try expectEqual(it.next(), null);
}

test "shrinkAndFree" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.ensureTotalCapacity(4);
    try expect(queue.capacity() >= 4);

    try queue.add(1);
    try queue.add(2);
    try queue.add(3);
    try expect(queue.capacity() >= 4);
    try expectEqual(@as(usize, 3), queue.count());

    queue.shrinkAndFree(3);
    try expectEqual(@as(usize, 3), queue.capacity());
    try expectEqual(@as(usize, 3), queue.count());

    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 2), queue.remove());
    try expectEqual(@as(u32, 3), queue.remove());
    try expect(queue.removeOrNull() == null);
}

test "update min heap" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(55);
    try queue.add(44);
    try queue.add(11);
    try queue.update(55, 5);
    try queue.update(44, 4);
    try queue.update(11, 1);
    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 4), queue.remove());
    try expectEqual(@as(u32, 5), queue.remove());
}

test "update same min heap" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(1);
    try queue.add(1);
    try queue.add(2);
    try queue.add(2);
    try queue.update(1, 5);
    try queue.update(2, 4);
    try expectEqual(@as(u32, 1), queue.remove());
    try expectEqual(@as(u32, 2), queue.remove());
    try expectEqual(@as(u32, 4), queue.remove());
    try expectEqual(@as(u32, 5), queue.remove());
}

test "update max heap" {
    var queue = PQgt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(55);
    try queue.add(44);
    try queue.add(11);
    try queue.update(55, 5);
    try queue.update(44, 1);
    try queue.update(11, 4);
    try expectEqual(@as(u32, 5), queue.remove());
    try expectEqual(@as(u32, 4), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
}

test "update same max heap" {
    var queue = PQgt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(1);
    try queue.add(1);
    try queue.add(2);
    try queue.add(2);
    try queue.update(1, 5);
    try queue.update(2, 4);
    try expectEqual(@as(u32, 5), queue.remove());
    try expectEqual(@as(u32, 4), queue.remove());
    try expectEqual(@as(u32, 2), queue.remove());
    try expectEqual(@as(u32, 1), queue.remove());
}

test "update after remove" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.add(1);
    try expectEqual(@as(u32, 1), queue.remove());
    try expectError(error.ElementNotFound, queue.update(1, 1));
}

test "siftUp in remove" {
    var queue = PQlt.init(testing.allocator, {});
    defer queue.deinit();

    try queue.addSlice(&.{ 0, 1, 100, 2, 3, 101, 102, 4, 5, 6, 7, 103, 104, 105, 106, 8 });

    _ = queue.removeIndex(std.mem.indexOfScalar(u32, queue.items[0..queue.count()], 102).?);

    const sorted_items = [_] u32 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 100, 101, 103, 104, 105, 106 };
    for (sorted_items) {
        | e | {
            try expectEqual(e, queue.remove());
        }
    }
}

fn contextLessThan(context : [] const u32, a : usize, b : usize) Order {
    return std.math.order(context[a], context[b]);
}

const CPQlt = PriorityQueue(usize, [] const u32, contextLessThan);

test "add and remove min heap with context comparator" {
    const context = [_] u32 { 5, 3, 4, 2, 2, 8, 0 };

    var queue = CPQlt.init(testing.allocator, context[0..]);
    defer queue.deinit();

    try queue.add(0);
    try queue.add(1);
    try queue.add(2);
    try queue.add(3);
    try queue.add(4);
    try queue.add(5);
    try queue.add(6);
    try expectEqual(@as(usize, 6), queue.remove());
    try expectEqual(@as(usize, 4), queue.remove());
    try expectEqual(@as(usize, 3), queue.remove());
    try expectEqual(@as(usize, 1), queue.remove());
    try expectEqual(@as(usize, 2), queue.remove());
    try expectEqual(@as(usize, 0), queue.remove());
    try expectEqual(@as(usize, 5), queue.remove());
}
#endif /* UNUSED_CODE */
