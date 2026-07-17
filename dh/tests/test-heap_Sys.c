#include "dh-main.h"
#include "dh/heap/Sys.h"

TEST_fn_("heap/Sys: header initializer yields a usable instance" $guard) {
    var sys = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);

    let memory = try_(mem_Alctr_allocBytes($trace gpa, 1));
    defer_(mem_Alctr_freeBytes($trace gpa, memory));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);

TEST_fn_("heap/Sys: fini restores default state" $guard) {
    var sys = try_(heap_Sys_init());
    using_() blk_defer {
        defer_(heap_Sys_fini(&sys));
        let gpa = heap_Sys_alctr(&sys);
        let memory = try_(mem_Alctr_allocBytes($trace gpa, 1));
        defer_(mem_Alctr_freeBytes($trace gpa, memory));
    } blk_deferral;

    sys = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);
    let memory = try_(mem_Alctr_allocBytes($trace gpa, 1));
    defer_(mem_Alctr_freeBytes($trace gpa, memory));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);

TEST_fn_("heap/Sys: thread-safe allocator surface is usable" $guard) {
    var sys = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys));
    let gpa = try_(heap_Sys_thrdSafeAlctr(&sys));

    let memory = try_(mem_Alctr_allocBytes($trace gpa, 1));
    defer_(mem_Alctr_freeBytes($trace gpa, memory));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);
