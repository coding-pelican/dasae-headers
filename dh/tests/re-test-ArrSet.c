/**
 * @file test-ArrSet.c
 * @brief Test cases for ArrSet (Array-based hash set)
 */

#include "dh/main.h"
#include "dh/ArrSet.h"
#include "dh/heap/Page.h"
#include "dh/io/stream.h"

/* Hash and equality functions for u32 */
fn_((u32Hash(u_V$raw item, u_V$raw ctx))(u64)) {
    let_ignore = ctx;
    u32 val = u_castV$((u32)(item));
    // Simple hash function (FNV-1a style)
    u64 hash = 14695981039346656037ULL;
    hash ^= val;
    hash *= 1099511628211ULL;
    return hash;
}

fn_((u32Eql(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    let_ignore = ctx;
    return u_castV$((u32)(lhs)) == u_castV$((u32)(rhs));
}

/* Template instantiation for u32 */
T_use$((u32)(
    ArrSet,
    ArrSet_init,
    ArrSet_fini,
    ArrSet_len,
    ArrSet_insert,
    ArrSet_insertWithin,
    ArrSet_contains,
    ArrSet_get,
    ArrSet_getMut,
    ArrSet_getIndex,
    ArrSet_remove,
    ArrSet_items,
    ArrSet_getOrPut,
    ArrSet_getOrPutWithin,
    ArrSet_fetch,
    ArrSet_fetchSwap,
    ArrSet_clone,
    ArrSet_move,
    ArrSet_unionWith,
    ArrSet_intersectWith,
    ArrSet_differenceWith,
));

fn_((printSet(ArrSet$u32 set))(void)) {
    let items = ArrSet_items$u32(set);
    io_stream_print(u8_l("Set: {"));
    for_(($rf(0), $s(items))(i, val) {
        if (i > 0) {
            io_stream_print(u8_l(", "));
        }
        io_stream_print(u8_l("{:u}"), *val);
    });
    io_stream_println(u8_l("}"));
}

TEST_fn_("empty set" $guard) {
    let gpa = heap_Page_allocator(&lit0$((heap_Page){}));
    var set = try_(ArrSet_init$u32(gpa, 0, u32Hash, u32Eql, u_anyP(&lit0$((const Void)))));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(TEST_expect(ArrSet_len$u32(set) == 0));
    try_(TEST_expect(!ArrSet_contains$u32(set, 42)));
} $unguarded_(TEST_fn);

TEST_fn_("insert and contains - small set" $guard) {
    let gpa = heap_Page_allocator(&lit0$((heap_Page){}));
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&lit0$((const Void)))));
    defer_(ArrSet_fini$u32(&set, gpa));

    // Insert a few items (below threshold for hash index)
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 10)) == true));
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 20)) == true));
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 30)) == true));

    printSet(set);

    try_(TEST_expect(ArrSet_len$u32(set) == 3));
    try_(TEST_expect(ArrSet_contains$u32(set, 10)));
    try_(TEST_expect(ArrSet_contains$u32(set, 20)));
    try_(TEST_expect(ArrSet_contains$u32(set, 30)));
    try_(TEST_expect(!ArrSet_contains$u32(set, 40)));
} $unguarded_(TEST_fn);

TEST_fn_("insert duplicates" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    // Insert same item multiple times
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 42)) == true));
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 42)) == false));
    try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, 42)) == false));

    try_(TEST_expect(ArrSet_len$u32(set) == 1));
    try_(TEST_expect(ArrSet_contains$u32(set, 42)));
} $unguarded_(TEST_fn);

TEST_fn_("insert and contains - large set with hash index" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 16, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    // Insert enough items to trigger hash index (>= 8)
    for (usize i = 0; i < 12; i++) {
        try_(TEST_expect(try_(ArrSet_insert$u32(&set, gpa, i * 10)) == true));
    }

    printSet(set);

    try_(TEST_expect(ArrSet_len$u32(set) == 12));

    // Check all items exist
    for (usize i = 0; i < 12; i++) {
        try_(TEST_expect(ArrSet_contains$u32(set, i * 10)));
    }

    // Check non-existent items
    try_(TEST_expect(!ArrSet_contains$u32(set, 5)));
    try_(TEST_expect(!ArrSet_contains$u32(set, 125)));
} $unguarded_(TEST_fn);

TEST_fn_("get and getMut" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 100));
    try_(ArrSet_insert$u32(&set, gpa, 200));

    if_some((ArrSet_get$u32(set, 100))(ptr)) {
        try_(TEST_expect(*ptr == 100));
    } else_none {
        try_(TEST_fail(u8_l("Expected to find 100")));
    }

    if_some((ArrSet_getMut$u32(set, 200))(ptr)) {
        try_(TEST_expect(*ptr == 200));
        // Modify the value
        *ptr = 201;
    } else_none {
        try_(TEST_fail(u8_l("Expected to find 200")));
    }

    // Verify modification
    if_some((ArrSet_get$u32(set, 200))(ptr)) {
        try_(TEST_fail(u8_l("Should not find 200 anymore")));
    } else_none {}

    if_some((ArrSet_get$u32(set, 201))(ptr)) {
        try_(TEST_expect(*ptr == 201));
    } else_none {
        try_(TEST_fail(u8_l("Expected to find 201")));
    }
} $unguarded_(TEST_fn);

TEST_fn_("getIndex" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 10));
    try_(ArrSet_insert$u32(&set, gpa, 20));
    try_(ArrSet_insert$u32(&set, gpa, 30));

    if_some((ArrSet_getIndex$u32(set, 20))(idx)) {
        try_(TEST_expect(idx == 1));
    } else_none {
        try_(TEST_fail(u8_l("Expected to find index for 20")));
    }

    if_some((ArrSet_getIndex$u32(set, 999))(idx)) {
        let_ignore = idx;
        try_(TEST_fail(u8_l("Should not find index for 999")));
    } else_none {}
} $unguarded_(TEST_fn);

TEST_fn_("remove" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 10));
    try_(ArrSet_insert$u32(&set, gpa, 20));
    try_(ArrSet_insert$u32(&set, gpa, 30));

    printSet(set);
    try_(TEST_expect(ArrSet_len$u32(set) == 3));

    // Remove existing item
    try_(TEST_expect(ArrSet_remove$u32(&set, 20) == true));
    printSet(set);
    try_(TEST_expect(ArrSet_len$u32(set) == 2));
    try_(TEST_expect(!ArrSet_contains$u32(set, 20)));
    try_(TEST_expect(ArrSet_contains$u32(set, 10)));
    try_(TEST_expect(ArrSet_contains$u32(set, 30)));

    // Remove non-existent item
    try_(TEST_expect(ArrSet_remove$u32(&set, 999) == false));
    try_(TEST_expect(ArrSet_len$u32(set) == 2));
} $unguarded_(TEST_fn);

TEST_fn_("fetch" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 10));
    try_(ArrSet_insert$u32(&set, gpa, 20));
    try_(ArrSet_insert$u32(&set, gpa, 30));

    // Fetch existing item
    if_some((ArrSet_fetch$u32(&set, 20))(val)) {
        try_(TEST_expect(val == 20));
    } else_none {
        try_(TEST_fail(u8_l("Expected to fetch 20")));
    }

    try_(TEST_expect(ArrSet_len$u32(set) == 2));
    try_(TEST_expect(!ArrSet_contains$u32(set, 20)));

    // Fetch non-existent item
    if_some((ArrSet_fetch$u32(&set, 999))(val)) {
        let_ignore = val;
        try_(TEST_fail(u8_l("Should not fetch 999")));
    } else_none {}
} $unguarded_(TEST_fn);

TEST_fn_("getOrPut - existing item" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 42));

    let result = try_(ArrSet_getOrPut$u32(&set, gpa, 42));
    try_(TEST_expect(result.found_existing == true));
    try_(TEST_expect(*result.item_ptr == 42));
    try_(TEST_expect(ArrSet_len$u32(set) == 1));
} $unguarded_(TEST_fn);

TEST_fn_("getOrPut - new item" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    let result = try_(ArrSet_getOrPut$u32(&set, gpa, 42));
    try_(TEST_expect(result.found_existing == false));
    *result.item_ptr = 42;  // Initialize the value
    try_(TEST_expect(ArrSet_len$u32(set) == 1));
    try_(TEST_expect(ArrSet_contains$u32(set, 42)));
} $unguarded_(TEST_fn);

TEST_fn_("clone" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    try_(ArrSet_insert$u32(&set, gpa, 10));
    try_(ArrSet_insert$u32(&set, gpa, 20));
    try_(ArrSet_insert$u32(&set, gpa, 30));

    var cloned = try_(ArrSet_clone$u32(set, gpa));
    defer_(ArrSet_fini$u32(&cloned, gpa));

    try_(TEST_expect(ArrSet_len$u32(cloned) == 3));
    try_(TEST_expect(ArrSet_contains$u32(cloned, 10)));
    try_(TEST_expect(ArrSet_contains$u32(cloned, 20)));
    try_(TEST_expect(ArrSet_contains$u32(cloned, 30)));

    // Modify original, clone should be unaffected
    try_(ArrSet_insert$u32(&set, gpa, 40));
    try_(TEST_expect(ArrSet_len$u32(set) == 4));
    try_(TEST_expect(ArrSet_len$u32(cloned) == 3));
    try_(TEST_expect(!ArrSet_contains$u32(cloned, 40)));
} $unguarded_(TEST_fn);

TEST_fn_("move" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));

    try_(ArrSet_insert$u32(&set, gpa, 10));
    try_(ArrSet_insert$u32(&set, gpa, 20));
    try_(ArrSet_insert$u32(&set, gpa, 30));

    var moved = ArrSet_move$u32(&set);
    defer_(ArrSet_fini$u32(&moved, gpa));

    // Original should be empty
    try_(TEST_expect(ArrSet_len$u32(set) == 0));

    // Moved should have all items
    try_(TEST_expect(ArrSet_len$u32(moved) == 3));
    try_(TEST_expect(ArrSet_contains$u32(moved, 10)));
    try_(TEST_expect(ArrSet_contains$u32(moved, 20)));
    try_(TEST_expect(ArrSet_contains$u32(moved, 30)));

    // Clean up empty original
    ArrSet_fini$u32(&set, gpa);
} $unguarded_(TEST_fn);

TEST_fn_("unionWith" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set1 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set1, gpa));
    var set2 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set2, gpa));

    try_(ArrSet_insert$u32(&set1, gpa, 10));
    try_(ArrSet_insert$u32(&set1, gpa, 20));

    try_(ArrSet_insert$u32(&set2, gpa, 20));
    try_(ArrSet_insert$u32(&set2, gpa, 30));

    try_(ArrSet_unionWith$u32(&set1, gpa, *set2.as_raw));

    io_stream_print(u8_l("Union result: "));
    printSet(set1);

    try_(TEST_expect(ArrSet_len$u32(set1) == 3));
    try_(TEST_expect(ArrSet_contains$u32(set1, 10)));
    try_(TEST_expect(ArrSet_contains$u32(set1, 20)));
    try_(TEST_expect(ArrSet_contains$u32(set1, 30)));
} $unguarded_(TEST_fn);

TEST_fn_("intersectWith" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set1 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set1, gpa));
    var set2 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set2, gpa));

    try_(ArrSet_insert$u32(&set1, gpa, 10));
    try_(ArrSet_insert$u32(&set1, gpa, 20));
    try_(ArrSet_insert$u32(&set1, gpa, 30));

    try_(ArrSet_insert$u32(&set2, gpa, 20));
    try_(ArrSet_insert$u32(&set2, gpa, 30));
    try_(ArrSet_insert$u32(&set2, gpa, 40));

    ArrSet_intersectWith$u32(&set1, *set2.as_raw);

    io_stream_print(u8_l("Intersect result: "));
    printSet(set1);

    try_(TEST_expect(ArrSet_len$u32(set1) == 2));
    try_(TEST_expect(!ArrSet_contains$u32(set1, 10)));
    try_(TEST_expect(ArrSet_contains$u32(set1, 20)));
    try_(TEST_expect(ArrSet_contains$u32(set1, 30)));
    try_(TEST_expect(!ArrSet_contains$u32(set1, 40)));
} $unguarded_(TEST_fn);

TEST_fn_("differenceWith" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set1 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set1, gpa));
    var set2 = try_(ArrSet_init$u32(gpa, 8, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set2, gpa));

    try_(ArrSet_insert$u32(&set1, gpa, 10));
    try_(ArrSet_insert$u32(&set1, gpa, 20));
    try_(ArrSet_insert$u32(&set1, gpa, 30));

    try_(ArrSet_insert$u32(&set2, gpa, 20));
    try_(ArrSet_insert$u32(&set2, gpa, 40));

    ArrSet_differenceWith$u32(&set1, *set2.as_raw);

    io_stream_print(u8_l("Difference result: "));
    printSet(set1);

    try_(TEST_expect(ArrSet_len$u32(set1) == 2));
    try_(TEST_expect(ArrSet_contains$u32(set1, 10)));
    try_(TEST_expect(!ArrSet_contains$u32(set1, 20)));
    try_(TEST_expect(ArrSet_contains$u32(set1, 30)));
    try_(TEST_expect(!ArrSet_contains$u32(set1, 40)));
} $unguarded_(TEST_fn);

TEST_fn_("stress test - many insertions" $guard) {
    let gpa = heap_Page_allocator(&(heap_Page){});
    var set = try_(ArrSet_init$u32(gpa, 128, u32Hash, u32Eql, u_anyP(&(const Void){})));
    defer_(ArrSet_fini$u32(&set, gpa));

    // Insert 100 items
    for (usize i = 0; i < 100; i++) {
        try_(ArrSet_insert$u32(&set, gpa, i));
    }

    try_(TEST_expect(ArrSet_len$u32(set) == 100));

    // Verify all items exist
    for (usize i = 0; i < 100; i++) {
        try_(TEST_expect(ArrSet_contains$u32(set, i)));
    }

    // Remove every other item
    for (usize i = 0; i < 100; i += 2) {
        try_(TEST_expect(ArrSet_remove$u32(&set, i) == true));
    }

    try_(TEST_expect(ArrSet_len$u32(set) == 50));

    // Verify correct items remain
    for (usize i = 0; i < 100; i++) {
        if (i % 2 == 0) {
            try_(TEST_expect(!ArrSet_contains$u32(set, i)));
        } else {
            try_(TEST_expect(ArrSet_contains$u32(set, i)));
        }
    }
} $unguarded_(TEST_fn);
