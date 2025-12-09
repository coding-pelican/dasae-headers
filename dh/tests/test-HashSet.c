#include "dh/main.h"
#include "dh/HashSet.h"
#include "dh/heap/Page.h"

T_use$((usize)(
    HashSet_Sgl,
    HashSet_Entry,
    HashSet_Entry_key,
    HashSet_Ensured
));
T_use$((usize)(
    HashSet,
    HashSet_init,
    HashSet_fini,
    HashSet_clone,
    HashSet_clearRetainingCap,
    HashSet_clearAndFree,
    HashSet_count,
    HashSet_cap,
    HashSet_for,
    HashSet_entry,
    HashSet_contains,
    HashSet_put,
    HashSet_fetchPut,
    HashSet_ensure,
    HashSet_remove,
    HashSet_fetchRemove,
    HashSet_rehash,
    HashSet_isSubset,
    HashSet_isSuperset,
    HashSet_isDisjoint
));
T_use$((usize)(
    HashSet_Iter,
    HashSet_iter,
    HashSet_Iter_next
));
T_use$((usize)(
    HashSet_KeyIter,
    HashSet_keyIter,
    HashSet_KeyIter_next
));

TEST_fn_("basic usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();
    var set_value = try_(HashSet_init$usize(ctx, gpa, 256));
    defer_(HashSet_fini$usize(&set_value, gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashSet_put$usize(&set_value, gpa, i));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashSet_iter$usize(&set_value);
        while_some(HashSet_Iter_next$usize(&it), entry) {
            sum += *HashSet_Entry_key$usize(entry);
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(HashSet_contains$usize(set_value, i)));
            try_(TEST_expect(i == unwrap_(HashSet_for$usize(set_value, i))));
            sum += unwrap_(HashSet_for$usize(set_value, i));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

TEST_fn_("basic usage - no templates used" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();
    var set_value = try_(HashSet_init(typeInfo$(usize), ctx, gpa, 256));
    defer_(HashSet_fini(&set_value, typeInfo$(usize), gpa));

    let_(count, u32) = 128;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashSet_put(&set_value, gpa, u_anyV(as$(usize)(i))));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashSet_iter(&set_value, typeInfo$(usize));
        while_some(HashSet_Iter_next(&it, typeInfo$(usize)), entry) {
            sum += *u_castP$((const usize*)(HashSet_Entry_key(entry, typeInfo$(usize))));
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(HashSet_contains(set_value, u_anyV(as$(usize)(i)))));
            try_(TEST_expect(i == unwrap_(u_castO$((O$usize)(HashSet_for(set_value, u_anyV(as$(usize)(i)), u_retV$(usize)))))));
            sum += unwrap_(u_castO$((O$usize)(HashSet_for(set_value, u_anyV(as$(usize)(i)), u_retV$(usize)))));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

TEST_fn_("basic hash set usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();
    var set_value = try_(HashSet_init$usize(ctx, gpa, 256));
    defer_(HashSet_fini$usize(&set_value, gpa));

    // Test fetchPut returns None for new elements
    try_(TEST_expect(isNone(try_(HashSet_fetchPut$usize(&set_value, gpa, 1)))));
    try_(TEST_expect(isNone(try_(HashSet_fetchPut$usize(&set_value, gpa, 2)))));
    try_(TEST_expect(isNone(try_(HashSet_fetchPut$usize(&set_value, gpa, 3)))));
    try_(TEST_expect(isNone(try_(HashSet_fetchPut$usize(&set_value, gpa, 4)))));

    // Test fetchPut returns Some for new elements
    try_(TEST_expect(unwrap_(try_(HashSet_fetchPut$usize(&set_value, gpa, 1))).key == 1));
    try_(TEST_expect(unwrap_(try_(HashSet_fetchPut$usize(&set_value, gpa, 2))).key == 2));

    // Test ensure on existing element
    let ensured1 = try_(HashSet_ensure$usize(&set_value, gpa, 3));
    try_(TEST_expect(ensured1.found_existing));

    // Test ensure on new element
    let ensured2 = try_(HashSet_ensure$usize(&set_value, gpa, 99));
    try_(TEST_expect(!ensured2.found_existing));

    // Test contains
    try_(TEST_expect(HashSet_contains$usize(set_value, 1)));
    try_(TEST_expect(HashSet_contains$usize(set_value, 2)));
    try_(TEST_expect(HashSet_contains$usize(set_value, 3)));
    try_(TEST_expect(HashSet_contains$usize(set_value, 4)));
    try_(TEST_expect(HashSet_contains$usize(set_value, 99)));
    try_(TEST_expect(!HashSet_contains$usize(set_value, 100)));

    // Test entry
    try_(TEST_expect(isSome(HashSet_entry$usize(set_value, 2))));
    try_(TEST_expect(isNone(HashSet_entry$usize(set_value, 100))));

    // Test fetchRemove
    let rmv = HashSet_fetchRemove$usize(&set_value, 2);
    try_(TEST_expect(unwrap_(rmv).key == 2));
    try_(TEST_expect(isNone(HashSet_fetchRemove$usize(&set_value, 2))));
    try_(TEST_expect(!HashSet_remove$usize(&set_value, 2)));
    try_(TEST_expect(isNone(HashSet_entry$usize(set_value, 2))));

    // Test remove
    try_(TEST_expect(HashSet_remove$usize(&set_value, 3)));
    try_(TEST_expect(!HashSet_remove$usize(&set_value, 3)));
} $unguarded_(TEST_fn);

TEST_fn_("set operations" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();

    // Create set A = {1, 2, 3}
    var setA = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&setA, gpa));
    try_(HashSet_put$usize(&setA, gpa, 1));
    try_(HashSet_put$usize(&setA, gpa, 2));
    try_(HashSet_put$usize(&setA, gpa, 3));

    // Create set B = {2, 3, 4}
    var setB = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&setB, gpa));
    try_(HashSet_put$usize(&setB, gpa, 2));
    try_(HashSet_put$usize(&setB, gpa, 3));
    try_(HashSet_put$usize(&setB, gpa, 4));

    // Create set C = {1, 2, 3} (same as A)
    var setC = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&setC, gpa));
    try_(HashSet_put$usize(&setC, gpa, 1));
    try_(HashSet_put$usize(&setC, gpa, 2));
    try_(HashSet_put$usize(&setC, gpa, 3));

    // Create set D = {1} (subset of A)
    var setD = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&setD, gpa));
    try_(HashSet_put$usize(&setD, gpa, 1));

    // Create set E = {5, 6} (disjoint from A)
    var setE = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&setE, gpa));
    try_(HashSet_put$usize(&setE, gpa, 5));
    try_(HashSet_put$usize(&setE, gpa, 6));

    // Test isSubset
    try_(TEST_expect(HashSet_isSubset$usize(setD, setA)));  // {1} ⊆ {1,2,3}
    try_(TEST_expect(!HashSet_isSubset$usize(setA, setD))); // {1,2,3} ⊄ {1}
    try_(TEST_expect(HashSet_isSubset$usize(setA, setC)));  // {1,2,3} ⊆ {1,2,3}
    try_(TEST_expect(!HashSet_isSubset$usize(setA, setB))); // {1,2,3} ⊄ {2,3,4}

    // Test isSuperset
    try_(TEST_expect(HashSet_isSuperset$usize(setA, setD)));  // {1,2,3} ⊇ {1}
    try_(TEST_expect(!HashSet_isSuperset$usize(setD, setA))); // {1} ⊅ {1,2,3}
    try_(TEST_expect(HashSet_isSuperset$usize(setA, setC)));  // {1,2,3} ⊇ {1,2,3}

    // Test isDisjoint
    try_(TEST_expect(HashSet_isDisjoint$usize(setA, setE)));  // {1,2,3} ∩ {5,6} = ∅
    try_(TEST_expect(HashSet_isDisjoint$usize(setE, setA)));  // {5,6} ∩ {1,2,3} = ∅
    try_(TEST_expect(!HashSet_isDisjoint$usize(setA, setB))); // {1,2,3} ∩ {2,3,4} ≠ ∅
    try_(TEST_expect(!HashSet_isDisjoint$usize(setA, setC))); // {1,2,3} ∩ {1,2,3} ≠ ∅
} $unguarded_(TEST_fn);

TEST_fn_("clone and clear operations" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();

    var set = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&set, gpa));

    // Add some elements
    try_(HashSet_put$usize(&set, gpa, 1));
    try_(HashSet_put$usize(&set, gpa, 2));
    try_(HashSet_put$usize(&set, gpa, 3));

    // Clone and verify
    var cloned = try_(HashSet_clone$usize(set, gpa));
    defer_(HashSet_fini$usize(&cloned, gpa));

    try_(TEST_expect(HashSet_count$usize(cloned) == 3));
    try_(TEST_expect(HashSet_contains$usize(cloned, 1)));
    try_(TEST_expect(HashSet_contains$usize(cloned, 2)));
    try_(TEST_expect(HashSet_contains$usize(cloned, 3)));

    // Clear retaining capacity
    let cap_before = HashSet_cap$usize(set);
    HashSet_clearRetainingCap$usize(&set);
    try_(TEST_expect(HashSet_count$usize(set) == 0));
    try_(TEST_expect(HashSet_cap$usize(set) == cap_before));
    try_(TEST_expect(!HashSet_contains$usize(set, 1)));

    // Clear and free
    HashSet_clearAndFree$usize(&cloned, gpa);
    try_(TEST_expect(HashSet_count$usize(cloned) == 0));
    try_(TEST_expect(HashSet_cap$usize(cloned) == 0));
} $unguarded_(TEST_fn);

TEST_fn_("rehash" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();

    var set = try_(HashSet_init$usize(ctx, gpa, 64));
    defer_(HashSet_fini$usize(&set, gpa));

    // Add and remove elements to create tombstones
    for_(($r(0, 32))(i) {
        try_(HashSet_put$usize(&set, gpa, i));
    });

    // Remove half to create tombstones
    for_(($r(0, 16))(i) {
        HashSet_remove$usize(&set, i * 2);
    });

    try_(TEST_expect(HashSet_count$usize(set) == 16));

    // Rehash to remove tombstones
    HashSet_rehash$usize(&set);

    // Verify all expected elements are still present
    try_(TEST_expect(HashSet_count$usize(set) == 16));
    for_(($r(0, 16))(i) {
        try_(TEST_expect(HashSet_contains$usize(set, i * 2 + 1)));
    });
    for_(($r(0, 16))(i) {
        try_(TEST_expect(!HashSet_contains$usize(set, i * 2)));
    });
} $unguarded_(TEST_fn);

TEST_fn_("key iterator" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    let ctx = HashSet_Ctx_default();

    var set = try_(HashSet_init$usize(ctx, gpa, 16));
    defer_(HashSet_fini$usize(&set, gpa));

    // Add elements
    let_(count, u32) = 10;
    var_(expected_total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashSet_put$usize(&set, gpa, i));
        expected_total += i;
    });

    // Iterate and sum
    var_(actual_total, u32) = 0;
    var_(iter_count, u32) = 0;
    var key_it = HashSet_keyIter$usize(set);
    while_some(HashSet_KeyIter_next$usize(&key_it), key_ptr) {
        actual_total += *key_ptr;
        iter_count++;
    }

    try_(TEST_expect(iter_count == count));
    try_(TEST_expect(actual_total == expected_total));
} $unguarded_(TEST_fn);

#if UNUSED_CODE
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    return_ok({});
} $unguarded_(fn);
#endif /* UNUSED_CODE */
