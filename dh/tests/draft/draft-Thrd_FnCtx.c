#include "dh/main.h"
#include "dh/time.h"
#include "dh/time/Duration.h"

typedef struct Thrd_FnCtx {
    u8 data[0];
} Thrd_FnCtx;
typedef struct Thrd_FnRet {
    u8 data[0];
} Thrd_FnRet;

typedef union FnCtx$demoThrd {
    Thrd_FnCtx base[1];
    struct {
        time_Duration wait;
    } arg;
    union {
        Thrd_FnRet base[1];
        i32        value;
    } ret;
} FnCtx$demoThrd;

fn_((demoThrd(Thrd_FnCtx* ctx))(Thrd_FnRet*)) {
    let self = as$((FnCtx$demoThrd*)(ctx));
    let arg  = self->arg;

    i32          cnt   = 0;
    time_Instant start = time_Instant_now();
    while (time_Duration_lt(time_Instant_elapsed(start), arg.wait)) { cnt++; }
    printf("cnt: %d\n", cnt);

    return self->ret.value = cnt, self->ret.base;
}

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    let direct_run = *as$((TypeOf(((FnCtx$demoThrd*)0)->ret.value)*)(
        demoThrd((FnCtx$demoThrd){ .arg = { time_Duration_secs } }.base)
    ));
    printf("direct_run: %d\n", direct_run);

    return_ok({});
} $unscoped_(fn);
