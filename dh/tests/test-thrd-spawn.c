#include "test-thrd/main.h"

TEST_fn_("thrd: spawn and join function closure" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var clsr = clsr_((test__double)(21));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr_kind$i32(joined) == Clsr_Kind_fn));
    try_(TEST_expect(clsr.ctx.ret == 42));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: spawn and join coroutine closure" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var clsr = clsr_((test__sumAfterSuspend)(19, 23));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr_kind$i32(joined) == Clsr_Kind_co));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ctrl.state == Co_State_ready));
    try_(TEST_expect(clsr.ctx.data.frame.ctx.ret == 42));
    return_ok({});
} $unguarded(TEST_fn);
