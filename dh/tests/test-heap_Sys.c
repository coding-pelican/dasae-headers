#include "dh-main.h"
#include "dh/heap/Sys.h"

TEST_fn_("heap/Sys: header initializer yields a usable instance" $guard) {
    var sys = heap_Sys_init();
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);

    let memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 1))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);

TEST_fn_("heap/Sys: fini restores default state" $guard) {
    var sys = heap_Sys_init();
    using_() blk_defer {
        defer_(heap_Sys_fini(&sys));
        let gpa = heap_Sys_alctr(&sys);
        let memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 1))));
        defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));
    } blk_deferral;

    sys = heap_Sys_init();
    defer_(heap_Sys_fini(&sys));
    let gpa = heap_Sys_alctr(&sys);
    let memory = try_(u_castE$((E$S$u8)(mem_Alctr_alloc($trace gpa, typeInfo$(u8), 1))));
    defer_(mem_Alctr_free($trace gpa, u_anyS(memory)));

    try_(TEST_expect(memory.len == 1));
} $unguarded(TEST_fn);
