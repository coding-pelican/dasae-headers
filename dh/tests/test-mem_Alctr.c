#include "dh-main.h"
#include "dh/heap/Sys.h"
#include "dh/mem/Alctr.h"

T_use_E$($set(mem_E)(P$u32));
T_use_E$($set(mem_E)(S$u32));
T_use$((u32)(
    mem_Alctr_create,
    mem_Alctr_destroy,
    mem_Alctr_clone,
    mem_Alctr_alloc,
    mem_Alctr_realloc,
    mem_Alctr_free,
    mem_Alctr_dupe
));

TEST_fn_("mem/Alctr: create zero initializes and clone owns an independent value" $guard) {
    var sys = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);

    let original = try_(mem_Alctr_create$u32($trace gpa));
    defer_(mem_Alctr_destroy$u32($trace gpa, original));
    try_(TEST_expect(*P_at((original)[0]) == 0));

    *P_at((original)[0]) = 0x12345678u;
    let clone = try_(mem_Alctr_clone$u32($trace gpa, original));
    defer_(mem_Alctr_destroy$u32($trace gpa, clone));

    try_(TEST_expect(clone != original));
    try_(TEST_expect(*P_at((clone)[0]) == 0x12345678u));
    *P_at((original)[0]) = 0;
    try_(TEST_expect(*P_at((clone)[0]) == 0x12345678u));
} $unguarded(TEST_fn);

TEST_fn_("mem/Alctr: typed realloc preserves prefix and clears grown elements" $guard) {
    var sys = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);

    var items = try_(mem_Alctr_alloc$u32($trace gpa, 3));
    defer_(mem_Alctr_free$u32($trace gpa, items));
    for_(($rf(0), $s(items))(idx, item)) { *item = intCast$((u32)(idx + 1)); } $end(for);

    items = try_(mem_Alctr_realloc$u32($trace gpa, items, 6));
    try_(TEST_expect(items.len == 6));
    for_(($rf(0), $s(S_prefix((items)(3))))(idx, item)) {
        try_(TEST_expect(*item == idx + 1));
    } $end(for);
    for_(($s(S_suffix((items)(3))))(item)) {
        try_(TEST_expect(*item == 0));
    } $end(for);

    let copy = try_(mem_Alctr_dupe$u32($trace gpa, items.as_const));
    defer_(mem_Alctr_free$u32($trace gpa, copy));
    *S_at((items)[0]) = 99;
    try_(TEST_expect(*S_at((copy)[0]) == 1));
} $unguarded(TEST_fn);

TEST_fn_("mem/Alctr: noop allocator accepts empty allocation but reports exhaustion" $guard) {
    let empty = try_(mem_Alctr_allocBytes($trace mem_Alctr_noop, 0));
    try_(TEST_expect(empty.len == 0));

    if_err((mem_Alctr_allocBytes($trace mem_Alctr_noop, 1))(err)) {
        try_(TEST_expect(E_eql(err.as_any, E_cause$OutOfMemory().as_any)));
    } else_ok(value) {
        let_ignore = value;
        try_(TEST_expect(false));
    }
} $unguarded(TEST_fn);
