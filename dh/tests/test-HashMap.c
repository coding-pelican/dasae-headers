#include "dh/io/stream.h"
#include "dh/main.h"
#include "dh/HashMap.h"
#include "dh/heap/Page.h"

T_use$((usize, u16)(
    HashMap_Pair,
    HashMap_Entry,
    HashMap_Entry_key,
    HashMap_Entry_val,
    HashMap_Ensured
));
T_use$((usize, u16)(
    HashMap,
    HashMap_init,
    HashMap_fini,
    HashMap_clone,
    HashMap_clearRetainingCap,
    HashMap_clearAndFree,
    HashMap_count,
    HashMap_cap,
    HashMap_by,
    HashMap_entry,
    HashMap_contains,
    HashMap_put,
    HashMap_putNoClobber,
    HashMap_fetchPut,
    HashMap_ensure,
    HashMap_ensureValue,
    HashMap_remove,
    HashMap_fetchRemove,
    HashMap_rehash
));
T_use$((usize, u16)(
    HashMap_Iter,
    HashMap_iter,
    HashMap_Iter_next
));
T_use$((usize, u16)(
    HashMap_KeyIter,
    HashMap_keyIter,
    HashMap_KeyIter_next
));
T_use$((usize, u16)(
    HashMap_ValIter,
    HashMap_valIter,
    HashMap_ValIter_next
));

TEST_fn_("basic usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init$1usize$2u16(ctx, gpa, 256));
    defer_(HashMap_fini$1usize$2u16(&map_value, gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put$1usize$2u16(&map_value, gpa, i, i));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter$1usize$2u16(&map_value);
        while_some(HashMap_Iter_next$1usize$2u16(&it), entry) {
            sum += *HashMap_Entry_key$1usize$2u16(entry);
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(HashMap_contains$1usize$2u16(map_value, i)));
            try_(TEST_expect(i == unwrap_(HashMap_by$1usize$2u16(map_value, i))));
            sum += unwrap_(HashMap_by$1usize$2u16(map_value, i));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

TEST_fn_("basic usage - no templates used" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init(
        typeInfo$(usize), typeInfo$(u16), ctx, gpa, 256
    ));
    defer_(HashMap_fini(&map_value, typeInfo$(usize), typeInfo$(u16), gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put(&map_value, gpa, u_anyV(as$(usize)(i)), u_anyV(as$(u16)(i))));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter(&map_value, typeInfo$(usize), typeInfo$(u16));
        while_some(HashMap_Iter_next(&it, typeInfo$(usize), typeInfo$(u16)), entry) {
            sum += *u_castP$((const usize*)(HashMap_Entry_key(entry, typeInfo$(usize))));
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(HashMap_contains(map_value, typeInfo$(u16), u_anyV(as$(usize)(i)))));
            try_(TEST_expect(i == unwrap_(u_castO$((O$u16)(HashMap_by(map_value, u_anyV(as$(usize)(i)), u_retV$(u16)))))));
            sum += unwrap_(u_castO$((O$u16)(HashMap_by(map_value, u_anyV(as$(usize)(i)), u_retV$(u16)))));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

TEST_fn_("basic hash map usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();
    var map_value = try_(HashMap_init$1usize$2u16(ctx, gpa, 256));
    defer_(HashMap_fini$1usize$2u16(&map_value, gpa));

    try_(TEST_expect(isNone(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 1, 11)))));
    try_(TEST_expect(isNone(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 2, 22)))));
    try_(TEST_expect(isNone(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 3, 33)))));
    try_(TEST_expect(isNone(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 4, 44)))));

    try_(HashMap_putNoClobber$1usize$2u16(&map_value, gpa, 5, 55));
    try_(TEST_expect(unwrap_(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 5, 66))).val == 55));
    try_(TEST_expect(unwrap_(try_(HashMap_fetchPut$1usize$2u16(&map_value, gpa, 5, 55))).val == 66));

    let ensured1 = try_(HashMap_ensure$1usize$2u16(&map_value, gpa, 5));
    try_(TEST_expect(ensured1.found_existing));
    try_(TEST_expect(*ensured1.val == 55));
    *ensured1.val = 77;
    try_(TEST_expect(*unwrap_(HashMap_entry$1usize$2u16(map_value, 5)).val == 77));

    let ensured2 = try_(HashMap_ensure$1usize$2u16(&map_value, gpa, 99));
    try_(TEST_expect(!ensured2.found_existing));
    *ensured2.val = 42;
    try_(TEST_expect(*unwrap_(HashMap_entry$1usize$2u16(map_value, 99)).val == 42));

    let ensured3 = try_(HashMap_ensureValue$1usize$2u16(&map_value, gpa, 5, 5));
    try_(TEST_expect(*ensured3.val == 77));

    let ensured4 = try_(HashMap_ensureValue$1usize$2u16(&map_value, gpa, 100, 41));
    try_(TEST_expect(*ensured4.val == 41));

    try_(TEST_expect(HashMap_contains$1usize$2u16(map_value, 2)));
    try_(TEST_expect(*unwrap_(HashMap_entry$1usize$2u16(map_value, 2)).val == 22));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(map_value, 2)) == 22));

    let rmv = HashMap_fetchRemove$1usize$2u16(&map_value, 2);
    try_(TEST_expect(unwrap_(rmv).key == 2));
    try_(TEST_expect(unwrap_(rmv).val == 22));
    try_(TEST_expect(isNone(HashMap_fetchRemove$1usize$2u16(&map_value, 2))));
    try_(TEST_expect(!HashMap_remove$1usize$2u16(&map_value, 2)));
    try_(TEST_expect(isNone(HashMap_entry$1usize$2u16(map_value, 2))));
    try_(TEST_expect(isNone(HashMap_by$1usize$2u16(map_value, 2))));

    try_(TEST_expect(HashMap_remove$1usize$2u16(&map_value, 3)));
} $unguarded_(TEST_fn);

TEST_fn_("clone and clear operations" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 16));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Add some elements
    try_(HashMap_put$1usize$2u16(&map, gpa, 1, 10));
    try_(HashMap_put$1usize$2u16(&map, gpa, 2, 20));
    try_(HashMap_put$1usize$2u16(&map, gpa, 3, 30));

    // Clone and verify
    var cloned = try_(HashMap_clone$1usize$2u16(map, gpa));
    defer_(HashMap_fini$1usize$2u16(&cloned, gpa));

    try_(TEST_expect(HashMap_count$1usize$2u16(cloned) == 3));
    try_(TEST_expect(HashMap_contains$1usize$2u16(cloned, 1)));
    try_(TEST_expect(HashMap_contains$1usize$2u16(cloned, 2)));
    try_(TEST_expect(HashMap_contains$1usize$2u16(cloned, 3)));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(cloned, 1)) == 10));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(cloned, 2)) == 20));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(cloned, 3)) == 30));

    // Modify original, cloned should be unaffected
    try_(HashMap_put$1usize$2u16(&map, gpa, 1, 100));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(map, 1)) == 100));
    try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(cloned, 1)) == 10));

    // Clear retaining capacity
    let cap_before = HashMap_cap$1usize$2u16(map);
    HashMap_clearRetainingCap$1usize$2u16(&map);
    try_(TEST_expect(HashMap_count$1usize$2u16(map) == 0));
    try_(TEST_expect(HashMap_cap$1usize$2u16(map) == cap_before));
    try_(TEST_expect(!HashMap_contains$1usize$2u16(map, 1)));

    // Clear and free
    HashMap_clearAndFree$1usize$2u16(&cloned, gpa);
    try_(TEST_expect(HashMap_count$1usize$2u16(cloned) == 0));
    try_(TEST_expect(HashMap_cap$1usize$2u16(cloned) == 0));
} $unguarded_(TEST_fn);

TEST_fn_("rehash" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 64));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Add elements: keys 0-31, values are key * 10
    for_(($r(0, 32))(i) {
        try_(HashMap_put$1usize$2u16(&map, gpa, i, as$(u16)(i * 10)));
    });

    // Remove even keys to create tombstones
    for_(($r(0, 32 / 2))(i) {
        HashMap_remove$1usize$2u16(&map, i * 2);
    });

    try_(TEST_expect(HashMap_count$1usize$2u16(map) == 16));

    // Rehash to remove tombstones
    HashMap_rehash$1usize$2u16(&map);

    // Verify count is preserved
    try_(TEST_expect(HashMap_count$1usize$2u16(map) == 16));

    // Verify odd keys (1, 3, 5, ..., 31) are still present with correct values
    for_(($r(0, 32 / 2))(i) {
        let key = i * 2 + 1;
        try_(TEST_expect(HashMap_contains$1usize$2u16(map, key)));
        try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(map, key)) == as$(u16)(key * 10)));
    });

    // Verify even keys (0, 2, 4, ..., 30) are removed
    for_(($r(0, 32 / 2))(i) {
        try_(TEST_expect(!HashMap_contains$1usize$2u16(map, i * 2)));
    });
} $unguarded_(TEST_fn);

TEST_fn_("key iterator" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 16));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Add elements
    let_(count, u32) = 10;
    var_(expected_key_total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put$1usize$2u16(&map, gpa, i, as$(u16)(i * 10)));
        expected_key_total += i;
    });

    // Iterate over keys and sum
    var_(actual_key_total, u32) = 0;
    var_(iter_count, u32) = 0;
    var key_it = HashMap_keyIter$1usize$2u16(map);
    while_some(HashMap_KeyIter_next$1usize$2u16(&key_it), key_ptr) {
        actual_key_total += *key_ptr;
        iter_count++;
    }

    try_(TEST_expect(iter_count == count));
    try_(TEST_expect(actual_key_total == expected_key_total));
} $unguarded_(TEST_fn);

TEST_fn_("value iterator" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 16));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Add elements: value = key * 10
    let_(count, u32) = 10;
    var_(expected_val_total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put$1usize$2u16(&map, gpa, i, as$(u16)(i * 10)));
        expected_val_total += i * 10;
    });

    // Iterate over values and sum
    var_(actual_val_total, u32) = 0;
    var_(iter_count, u32) = 0;
    var val_it = HashMap_valIter$1usize$2u16(map);
    while_some(HashMap_ValIter_next$1usize$2u16(&val_it), val_ptr) {
        actual_val_total += *val_ptr;
        iter_count++;
    }

    try_(TEST_expect(iter_count == count));
    try_(TEST_expect(actual_val_total == expected_val_total));
} $unguarded_(TEST_fn);

TEST_fn_("entry iterator" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 16));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Add elements
    let_(count, u32) = 10;
    for_(($r(0, count))(i) {
        try_(HashMap_put$1usize$2u16(&map, gpa, i, as$(u16)(i * 10)));
    });

    // Iterate over entries and verify key-value relationship
    var_(iter_count, u32) = 0;
    var it = HashMap_iter$1usize$2u16(&map);
    while_some(HashMap_Iter_next$1usize$2u16(&it), entry) {
        let key = *HashMap_Entry_key$1usize$2u16(entry);
        let val = *HashMap_Entry_val$1usize$2u16(entry);
        try_(TEST_expect(val == as$(u16)(key * 10)));
        iter_count++;
    }

    try_(TEST_expect(iter_count == count));
} $unguarded_(TEST_fn);

#define test_HashMap_enable_debug_print 0
TEST_fn_("stress test with many insertions and deletions" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashMap_Ctx_default();

    var map = try_(HashMap_init$1usize$2u16(ctx, gpa, 16));
    defer_(HashMap_fini$1usize$2u16(&map, gpa));

    // Insert many elements
    let_(total_insertions, u32) = 1000;
    for_(($r(0, total_insertions))(i) {
        try_(HashMap_put$1usize$2u16(&map, gpa, i, as$(u16)(i % 65536)));
    });
    try_(TEST_expect(HashMap_count$1usize$2u16(map) == total_insertions));

    // Remove every third element
    var_(removed, u32) = 0;
    for_(($r(0, total_insertions))(i) {
        let should_remove = (i % 3) == 0;
        if (should_remove && HashMap_remove$1usize$2u16(&map, i)) {
            removed++;
        }
    });
    try_(TEST_expect(HashMap_count$1usize$2u16(map) == total_insertions - removed));

    // Verify remaining elements
    for_(($r(0, total_insertions))(i) {
        let should_exist = (i % 3) != 0;
        try_(TEST_expect(HashMap_contains$1usize$2u16(map, i) == should_exist));
    });

    // Rehash and verify again
    HashMap_rehash$1usize$2u16(&map);
    try_(TEST_expect(HashMap_count$1usize$2u16(map) == total_insertions - removed));

    for_(($r(0, total_insertions))(i) {
        let should_exist = (i % 3) != 0;
        pp_if_(test_HashMap_enable_debug_print)(pp_then_(io_stream_println), pp_else_(pp_ignore))(
            u8_l("i: {:ul}, should_exist: {:B}"), i, should_exist
        );
        try_(TEST_expect(HashMap_contains$1usize$2u16(map, i) == should_exist));
        if (should_exist) {
            pp_if_(test_HashMap_enable_debug_print)(pp_then_(io_stream_println), pp_else_(pp_ignore))(
                u8_l("-> HashMap_by$1usize$2u16(map, i): {:?uh}"), HashMap_by$1usize$2u16(map, i)
            );
            try_(TEST_expect(unwrap_(HashMap_by$1usize$2u16(map, i)) == as$(u16)(i % 65536)));
        }
    });
} $unguarded_(TEST_fn);

#if UNUSED_CODE
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    return_ok({});
} $unguarded_(fn);
#endif /* UNUSED_CODE */
