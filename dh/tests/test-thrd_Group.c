#include "test-thrd/main.h"

TEST_fn_("thrd/Group: spawned closure completes before protected wait returns" $guard) {
    var sys_heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&sys_heap));

    var thrd_safe_heap = (heap_ThrdSafe){
        .child_alctr = heap_Sys_alctr(&sys_heap),
        .mtx = thrd_Mtx_init(),
    };
    defer_(thrd_Mtx_fini(&thrd_safe_heap.mtx));

    var group = thrd_Group_init();
    defer_(thrd_Group_fini(&group));

    var done_count = atom_V_init$(atom_V$usize, 0);
    var clsr = clsr_((test__markDone)(&done_count));

    try_(thrd_Group_spawn(
        &group, heap_ThrdSafe_alctr(&thrd_safe_heap), clsr.as_base
    ));
    thrd_Group_waitProtcd(&group);

    try_(TEST_expect(atom_V_load(&done_count, atom_MemOrd_acquire) == 1));
    return_ok({});
} $unguarded(TEST_fn);
