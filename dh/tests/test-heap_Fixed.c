#include "dh-main.h"
#include "dh/heap/Fixed.h"
#include "dh/mem/Alctr.h"

TEST_fn_("heap/Fixed: basic reallocation usage" $guard) {
    var_(buffer, A$$(1024, u8)) $undefined;
    var_(fixed, heap_Fixed) = heap_Fixed_from(A_ref$((S$u8)(buffer)));

    let gpa = heap_Fixed_alctr(&fixed);
    var items = try_(mem_Alctr_allocBytes($trace gpa, 10));
    defer_(mem_Alctr_freeBytes($trace gpa, items));

    try_(TEST_expect(items.ptr != null));
    try_(TEST_expect(items.len == 10));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, items.as_const)));

    for_(($rf(0), $s(items))(idx, item)) { *item = intCast$((u8)(idx)); } $end(for);
    let extended = try_(mem_Alctr_reallocBytes($trace gpa, items, 20));
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

    for_(($rf(0), $s(S_prefix((items)(10))))(idx, item)) { try_(TEST_expect(*item == idx)); } $end(for);
    for_(($rf(10), $s(S_prefix((S_suffix((items)(10)))(10))))($ignore, item)) { try_(TEST_expect(*item == 0)); } $end(for);
} $unguarded(TEST_fn);

TEST_fn_("heap/Fixed: basic resize growth zero fills tail" $guard) {
    var_(buffer, A$$(1024, u8)) $undefined;
    var_(fixed, heap_Fixed) = heap_Fixed_from(A_ref$((S$u8)(buffer)));

    let gpa = heap_Fixed_alctr(&fixed);
    var items = try_(mem_Alctr_allocBytes($trace gpa, 8));
    defer_(mem_Alctr_freeBytes($trace gpa, items));

    for_(($rf(0), $s(items))(idx, item)) { *item = intCast$((u8)(idx + 1)); } $end(for);
    try_(TEST_expect(mem_Alctr_resizeBytes($trace gpa, items, 16)));
    items = l$((S$u8){ .ptr = items.ptr, .len = 16 });

    for_(($rf(0), $s(S_prefix((items)(8))))(idx, item)) { try_(TEST_expect(*item == (idx + 1))); } $end(for);
    for_(($rf(8), $s(S_prefix((S_suffix((items)(8)))(8))))(idx, item)) {
        let_ignore = idx;
        try_(TEST_expect(*item == 0));
    } $end(for);
} $unguarded(TEST_fn);

TEST_fn_("heap/Fixed: thread-safe allocator supports reallocation" $guard) {
    var_(buffer, A$$(1024, u8)) $undefined;
    var_(fixed, heap_Fixed) = heap_Fixed_from(A_ref$((S$u8)(buffer)));

    let gpa = heap_Fixed_thrdSafeAlctr(&fixed);
    var items = try_(mem_Alctr_allocBytes($trace gpa, 10));
    defer_(mem_Alctr_freeBytes($trace gpa, items));

    for_(($rf(0), $s(items))(idx, item)) { *item = intCast$((u8)(idx)); } $end(for);

    let extended = try_(mem_Alctr_reallocBytes($trace gpa, items, 20));
    try_(TEST_expect(extended.ptr == items.ptr));
    try_(TEST_expect(extended.len == 20));
    try_(TEST_expect(heap_Fixed_isLastAllocation(&fixed, extended.as_const)));

    items = extended;
    for_(($rf(0), $s(S_prefix((items)(10))))(idx, item)) { try_(TEST_expect(*item == idx)); } $end(for);
    for_(($rf(10), $s(S_prefix((S_suffix((items)(10)))(10))))($ignore, item)) {
        try_(TEST_expect(*item == 0));
    } $end(for);
} $unguarded(TEST_fn);

TEST_fn_("heap/Fixed: thread-safe allocator reclaims last allocation" $guard) {
    var_(buffer, A$$(16, u8)) $undefined;
    var_(fixed, heap_Fixed) = heap_Fixed_from(A_ref$((S$u8)(buffer)));

    let gpa = heap_Fixed_thrdSafeAlctr(&fixed);
    let first = try_(mem_Alctr_allocBytes($trace gpa, 8));
    let second = try_(mem_Alctr_allocBytes($trace gpa, 8));

    try_(TEST_expect(isNone(mem_Alctr_rawAlloc($trace gpa, 1, alignOfLog2$(u8)))));
    mem_Alctr_freeBytes($trace gpa, second);

    let reused = try_(mem_Alctr_allocBytes($trace gpa, 8));
    try_(TEST_expect(reused.ptr == second.ptr));

    mem_Alctr_freeBytes($trace gpa, reused);
    mem_Alctr_freeBytes($trace gpa, first);
} $unguarded(TEST_fn);
