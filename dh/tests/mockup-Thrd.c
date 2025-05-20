#include "dh/main.h"
#include "dh/time.h"
#include "dh/time/Duration.h"

typedef struct Thrd_FnCtx {
    u8 data[];
} Thrd_FnCtx;
typedef struct Thrd_FnRet {
    u8 data[];
} Thrd_FnRet;

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

typedef fn_((*Thrd_StartFn)(Thrd_FnCtx* ctx), Thrd_FnRet*);
#include <pthread.h>
fn_(Thrd_spawn(Thrd_StartFn startFn, Thrd_FnCtx* ctx), pthread_t) {
    pthread_t thrd = {};
    pthread_create(&thrd, null, (void* (*)(void*))startFn, ctx);
    return thrd;
}
fn_(Thrd_join(pthread_t thrd), Thrd_FnRet*) {
    void* ret = null;
    pthread_join(thrd, &ret);
    return as$(Thrd_FnRet*, ret);
}

fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    $ignore = args;

    let direct_run = *as$(
        TypeOf(((FnCtx$demoThrd*)0)->ret.value)*,
        demoThrd((FnCtx$demoThrd){ .arg = { time_Duration_secs } }.base)
    );
    printf("[main] direct_run: %d\n", direct_run);

    let thrd = Thrd_spawn(demoThrd, (FnCtx$demoThrd){ .arg = { time_Duration_secs } }.base);
    defer_({
        let ret = *as$(TypeOf(((FnCtx$demoThrd*)0)->ret.value)*, Thrd_join(thrd));
        printf("ret: %d\n", ret);
    });

    printf("thrd: %llu\n", thrd);
    for (usize i = 0; i < 20; ++i) {
        printf("[main] current: %zu\n", i);
        time_sleep(time_Duration_fromSecs_f64(0.1));
    }

    return_ok({});
} unscoped_ext;
