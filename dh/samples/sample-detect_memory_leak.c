#include "dh-main.h"
#include "dh/heap/Sys.h"

fn_((main(proc_Entry entry))(E$void) $guard) {
    let_ignore = entry;
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    let gpa = heap_Sys_alctr(&heap);
    let_ignore = try_(mem_Alctr_allocBytes($trace gpa, 1024));
    return_ok({});
} $unguarded(fn);
