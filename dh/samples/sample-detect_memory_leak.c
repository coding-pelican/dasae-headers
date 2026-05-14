#include "dh-main.h"
#include "dh/heap/Sys.h"

T_use_E$($set(mem_E)(S$u8));
T_use$((u8)(mem_Alctr_alloc));
fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var heap = heap_Sys_init();
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let_ignore = try_(mem_Alctr_alloc$u8($trace gpa, 1024));
    return_ok({});
} $unguarded(fn);
