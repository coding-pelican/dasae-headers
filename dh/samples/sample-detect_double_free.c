#include "dh-main.h"
#include "dh/heap/Sys.h"

fn_((main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let mem = try_(mem_Alctr_allocBytes($trace gpa, 1024));
    mem_Alctr_freeBytes($trace gpa, mem);
    mem_Alctr_freeBytes($trace gpa, mem); // Double free
    return_ok({});
} $unguarded(fn);
