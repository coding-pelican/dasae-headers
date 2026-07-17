#include "dh-main.h"
#include "dh/thrd.h"
#include "dh/clsr.h"
#include "dh/time/self.h"
#include "dh/heap/Sys.h"

T_use$((i32)(
    Clsr_Ctx, Clsr_Rtn, Clsr,
    thrd_spawn, thrd_join
));
$static fn_((test_thrd_ext__timesTwo(i32 input))(i32)) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    catch_((time_Awake_sleepMillis(clock, 1))($ignore, $do_nothing));
    return input * 2;
};
fn_use_Clsr_((test_thrd_ext__timesTwo)(i32)(i32));

TEST_fn_("thrd: ext - stack closure lifetime" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var clsr = clsr_((test_thrd_ext__timesTwo)(42));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr.ctx.ret == 84));
    return_ok({});
} $unguarded(TEST_fn);

typedef A$$(100, i32) test_thrd_ext__Values;

$static fn_((test_thrd_ext__sumValues(test_thrd_ext__Values values))(i32)) {
    i32 sum = 0;
    for_(($a(values))(value)) {
        sum += *value;
    } $end(for);
    return sum;
};
fn_use_Clsr_((test_thrd_ext__sumValues)(test_thrd_ext__Values)(i32));

TEST_fn_("thrd: ext - array argument synchronization" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    test_thrd_ext__Values values = A_zero();
    for_(($rf(0), $s(A_ref(values)))(i, value)) {
        *value = intCast$((i32)(i));
    } $end(for);

    var clsr = clsr_((test_thrd_ext__sumValues)(values));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(clsr.ctx.ret == (99 * 100) / 2));
    return_ok({});
} $unguarded(TEST_fn);

$static fn_((test_thrd_ext__square(i32 id))(i32)) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    catch_((time_Awake_sleepMillis(clock, 1))($ignore, $do_nothing));
    return id * id;
};
fn_use_Clsr_((test_thrd_ext__square)(i32)(i32));

typedef struct test_thrd_ext__Compound {
    var_(value, i32);
    var_(processed, i32);
} test_thrd_ext__Compound;

$static fn_((test_thrd_ext__processCompound(test_thrd_ext__Compound* ctx))(i32)) {
    let clock = catch_((time_Awake_direct())($ignore, time_Awake_noop));
    catch_((time_Awake_sleepMillis(clock, 1))($ignore, $do_nothing));
    ctx->value *= 2;
    ctx->processed = 1;
    return ctx->value;
};
fn_use_Clsr_((test_thrd_ext__processCompound)(test_thrd_ext__Compound*)(i32));

typedef struct test_thrd_ext__Stress {
    var_(iteration, i32);
    var_(computed, i32);
} test_thrd_ext__Stress;

$static fn_((test_thrd_ext__computeStress(test_thrd_ext__Stress* ctx))(i32)) {
    i32 sum = 0;
    for_(($r(0, 1000))(i)) {
        sum += intCast$((i32)(i));
    } $end(for);
    ctx->computed = ctx->iteration + sum;
    return ctx->computed;
};
fn_use_Clsr_((test_thrd_ext__computeStress)(test_thrd_ext__Stress*)(i32));

typedef struct test_thrd_ext__Fast {
    var_(ready, i32);
    var_(value, i32);
} test_thrd_ext__Fast;

$static fn_((test_thrd_ext__markFast(test_thrd_ext__Fast* ctx))(i32)) {
    ctx->ready = 1;
    ctx->value = 999;
    return ctx->value;
};
fn_use_Clsr_((test_thrd_ext__markFast)(test_thrd_ext__Fast*)(i32));

typedef A$$(256, u8) test_thrd_ext__Bytes;

typedef struct test_thrd_ext__Complex {
    var_(data_size, usize);
    var_(checksum, i32);
    var_(data, test_thrd_ext__Bytes);
} test_thrd_ext__Complex;

$static fn_((test_thrd_ext__checksumComplex(test_thrd_ext__Complex* ctx))(i32)) {
    i32 sum = 0;
    for_(($r(0, ctx->data_size))(i)) {
        sum += as$(i32)(*A_at((ctx->data)[i]));
    } $end(for);
    ctx->checksum = sum;
    return sum;
};
fn_use_Clsr_((test_thrd_ext__checksumComplex)(test_thrd_ext__Complex*)(i32));

TEST_fn_("thrd: ext - multiple stack closures" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    let cfg = (thrd_SpawnCfg){
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
    A$$(10, Clsr_(test_thrd_ext__square)) workers = A_zero();
    A$$(10, thrd_Self) threads = A_zero();

    for_(($rf(0), $s(A_ref(workers)), $s(A_ref(threads)))(i, worker, thread)) {
        *worker = clsr_((test_thrd_ext__square)(intCast$((i32)(i))));
        *thread = try_(thrd_spawn$i32(cfg, worker->as_base));
    } $end(for);
    for_(($s(A_ref(workers)), $s(A_ref(threads)))(worker, thread)) {
        let joined = thrd_join$i32(*thread);
        try_(TEST_expect(joined == worker->as_base));
    } $end(for);
    for_(($rf(0), $s(A_ref(workers)))(i, worker)) {
        try_(TEST_expect(worker->ctx.ret == intCast$((i32)(i * i))));
    } $end(for);
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: ext - compound stack context remains valid until join" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var ctx = (test_thrd_ext__Compound){
        .value = 100,
        .processed = 0,
    };
    var clsr = clsr_((test_thrd_ext__processCompound)(&ctx));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(ctx.value == 200));
    try_(TEST_expect(ctx.processed == 1));
    try_(TEST_expect(clsr.ctx.ret == 200));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: ext - repeated stress closures update caller contexts" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    let cfg = (thrd_SpawnCfg){
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
    let expected_base = (999 * 1000) / 2;

    for_(($r(0, 64))(iter)) {
        var ctx = (test_thrd_ext__Stress){
            .iteration = intCast$((i32)(iter)),
            .computed = 0,
        };
        var clsr = clsr_((test_thrd_ext__computeStress)(&ctx));
        let worker = try_(thrd_spawn$i32(cfg, clsr.as_base));
        let joined = thrd_join$i32(worker);
        let expected = intCast$((i32)(iter)) + expected_base;

        try_(TEST_expect(joined == clsr.as_base));
        try_(TEST_expect(ctx.computed == expected));
        try_(TEST_expect(clsr.ctx.ret == expected));
    } $end(for);
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: ext - fast spawn join observes stack writes" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    let cfg = (thrd_SpawnCfg){
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };

    for_(($r(0, 128))($ignore)) {
        var ctx = (test_thrd_ext__Fast){
            .ready = 0,
            .value = 0,
        };
        var clsr = clsr_((test_thrd_ext__markFast)(&ctx));
        let worker = try_(thrd_spawn$i32(cfg, clsr.as_base));
        let joined = thrd_join$i32(worker);

        try_(TEST_expect(joined == clsr.as_base));
        try_(TEST_expect(ctx.ready == 1));
        try_(TEST_expect(ctx.value == 999));
        try_(TEST_expect(clsr.ctx.ret == 999));
    } $end(for);
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: ext - complex structure stack context is processed" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    var ctx = (test_thrd_ext__Complex){
        .data_size = 100,
        .checksum = 0,
        .data = A_zero(),
    };
    i32 expected = 0;
    for_(($r(0, ctx.data_size))(i)) {
        let byte = as$(u8)(i % 256);
        *A_at((ctx.data)[i]) = byte;
        expected += as$(i32)(byte);
    } $end(for);

    var clsr = clsr_((test_thrd_ext__checksumComplex)(&ctx));
    let worker = try_(thrd_spawn$i32(
        (thrd_SpawnCfg){
            .gpa = heap_Sys_alctr(&heap),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        },
        clsr.as_base
    ));
    let joined = thrd_join$i32(worker);

    try_(TEST_expect(joined == clsr.as_base));
    try_(TEST_expect(ctx.checksum == expected));
    try_(TEST_expect(clsr.ctx.ret == expected));
    return_ok({});
} $unguarded(TEST_fn);

TEST_fn_("thrd: ext - concurrent stack closures all complete" $guard) {
    var heap = try_(heap_Sys_init());
    defer_(heap_Sys_fini(&heap));

    let cfg = (thrd_SpawnCfg){
        .gpa = heap_Sys_alctr(&heap),
        .stack_size = thrd_SpawnCfg_default_stack_size,
    };
    A$$(32, Clsr_(test_thrd_ext__square)) workers = A_zero();
    A$$(32, thrd_Self) threads = A_zero();

    for_(($rf(0), $s(A_ref(workers)), $s(A_ref(threads)))(i, worker, thread)) {
        *worker = clsr_((test_thrd_ext__square)(intCast$((i32)(i))));
        *thread = try_(thrd_spawn$i32(cfg, worker->as_base));
    } $end(for);
    for_(($s(A_ref(workers)), $s(A_ref(threads)))(worker, thread)) {
        let joined = thrd_join$i32(*thread);
        try_(TEST_expect(joined == worker->as_base));
    } $end(for);
    for_(($rf(0), $s(A_ref(workers)))(i, worker)) {
        try_(TEST_expect(worker->ctx.ret == intCast$((i32)(i * i))));
    } $end(for);
    return_ok({});
} $unguarded(TEST_fn);
