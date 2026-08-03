#include "dh-main.h"
#include "dh/heap/ThrdSafe.h"
#include "dh/heap/Sys.h"

TEST_fn_("heap/ThrdSafe: synchronized wrapper delegates allocation ownership" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));
    var safe = (heap_ThrdSafe){
        .child_alctr = heap_Sys_alctr(&heap),
        .mtx = thrd_Mtx_init(),
    };
    defer_(thrd_Mtx_fini(&safe.mtx));
    let alctr = heap_ThrdSafe_alctr(&safe);

    let mem = try_(mem_Alctr_allocBytes($trace alctr, 32));
    try_(TEST_expect(mem.len == 32));
    mem_Alctr_freeBytes($trace alctr, mem);
    return_ok({});
} $unguarded(TEST_fn);
