#include "dh/main.h"
#include "dh/heap/Fixed.h"
#include "dh/mem/Allocator.h"

TEST_fn_("Basic Allocator Reallocation Usage" $guard) {
    var_(buffer, A$$(1024, u8)) = A_zero();
    var_(fixed, heap_Fixed) = heap_Fixed_from(A_ref$((S$u8)(buffer)));

    let gpa = heap_Fixed_allocator(&fixed);
    var items = try_(u_castE$((E$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), 10))));
    defer_(mem_Allocator_free(gpa, u_anyS(items)));

    try_(TEST_expect(items.ptr != null));
    try_(TEST_expect(items.len == 10));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, items.as_const)));

    for_(($rf(0), $s(items))(idx, item) { *item = intCast$((u8)(idx)); });
    let extended = try_(u_castE$((E$S$u8)(mem_Allocator_realloc(gpa, u_anyS(items), 20))));
    try_(TEST_expect(extended.ptr != null));
    try_(TEST_expect(extended.len == 20));
    try_(TEST_expect(items.ptr == extended.ptr));
    try_(TEST_expect(items.len < extended.len));
    try_(TEST_expect(!heap_Fixed_isLastAllocation(&fixed, items.as_const)));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, extended.as_const)));

    items = extended;
    try_(TEST_expect(items.ptr != null));
    try_(TEST_expect(items.len == 20));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, items.as_const)));

    for_(($rf(0), $s(items))(idx, item) { try_(TEST_expect(*item == idx)); });
} $unguarded_(TEST_fn);
