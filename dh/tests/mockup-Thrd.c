#include "dh/main.h"
#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/time/Instant.h"

typedef union FnCtx$demoThrd {
    Thrd_FnCtx base[1];
    struct {
        struct {
            time_Duration wait;
        } arg;
        union {
            Thrd_FnRet base[1];
            i32        value;
        } ret;
    };
} FnCtx$demoThrd;

fn_(demoThrd(Thrd_FnCtx* ctx), Thrd_FnRet*) {
    let self = as$(FnCtx$demoThrd*, ctx);
    let arg  = self->arg;

    i32 cnt   = 0;
    let start = time_Instant_now();
    while (time_Duration_lt(time_Instant_elapsed(start), arg.wait)) { cnt++; }
    printf("[demoThrd] cnt: %d\n", cnt);

    return self->ret.value = cnt, self->ret.base;
}

fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    $ignore = args;

    let direct_run = *as$(
        FieldType$(FnCtx$demoThrd, ret.value)*,
        demoThrd((FnCtx$demoThrd){ .arg = { time_Duration_secs } }.base)
    );
    printf("[main] direct_run: %d\n", direct_run);

    var thrd = try_(Thrd_spawn(
        Thrd_SpawnConfig_default,
        demoThrd,
        (FnCtx$demoThrd){ .arg = { time_Duration_secs } }.base
    ));
    defer_({
        let ret = *as$(
            FieldType$(FnCtx$demoThrd, ret.value)*,
            Thrd_join(thrd)
        );
        printf("ret: %d\n", ret);
    });

    printf("thrd: %llu\n", thrd.handle);
    for (usize i = 0; i < 20; ++i) {
        printf("[main] current: %zu\n", i);
        time_sleep(time_Duration_fromSecs_f64(0.1));
    }

    return_ok({});
} unscoped_ext;
