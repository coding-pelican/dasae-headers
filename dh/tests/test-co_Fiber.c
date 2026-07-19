#include "dh-main.h"
#include "dh/co/Fiber.h"
#include "dh/exec/Fiber.h"
#include "dh/exec/common.h"

T_alias$((test_co_Fiber_MovapsCtx)(struct test_co_Fiber_MovapsCtx {
    var_(ret, i32);
    var_(pad, u32);
    var_(block0, A$$(16, u8));
    var_(block1, A$$(16, u8));
}));

$static var_(test_co_Fiber_movaps_probe_ok, bool) = false;
$static var_(test_co_Fiber_probe_sched_ctx, co_Fiber_Context);
$static var_(test_co_Fiber_probe_fiber_ctx, co_Fiber_Context);

$static fn_((test_co_Fiber_movapsProbe(P$raw owner, P$raw ctx))(void)) {
    let_ignore = owner;
    var_(local, test_co_Fiber_MovapsCtx);
    local = *as$(const test_co_Fiber_MovapsCtx*)(ctx);
    test_co_Fiber_movaps_probe_ok = local.ret == 42;
    let sw = (co_Fiber){
        .old = &test_co_Fiber_probe_fiber_ctx,
        .new = &test_co_Fiber_probe_sched_ctx,
    };
    let_ignore = co_Fiber_contextSwitch(&sw);
    claim_unreachable;
};

$attr($callconv_naked)
$static fn_((test_co_Fiber_entry(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
    pp_if_(co_Fiber_supported)((asm_volatile(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(
            "leaq 8(%%rsp), %%rdi\n\t"
            "leaq 8(%%rsp), %%rcx\n\t"
            "movq %%rsi, %%rdx\n\t"
            "jmp " nameOf(exec_callFiber) "\n\t" : : : "memory"
        )),
        pp_case_((arch_type_aarch64)(
            "mov x0, sp\n\t"
            "b " nameOf(exec_callFiber) "\n\t" : : : "memory"
        )),
        pp_case_((arch_type_riscv64)(
            "mv a0, sp\n\t"
            "tail " nameOf(exec_callFiber) "\n\t" : : : "memory"
        ))
    )))));
} /* NOLINTEND(hicpp-no-assembler) */

$attr($suppressing_(frame_larger_than))
$static fn_((test_co_Fiber_runMovapsProbe(test_co_Fiber_MovapsCtx ctx))(void)) {
    var_(stack_storage, A$$(4096, u8)) = cleared();
    let stack = A_ref$((S$u8)(stack_storage));
    let start = unwrap_(co_Fiber_stackAllocArg(
        stack,
        sizeOf$(exec_Fiber_Starter),
        co_Fiber_stackArgAlign
    ));
    asg_l((as$(exec_Fiber_Starter*)(start))({
        .owner = null,
        .task = as$(P$raw)(&ctx),
        .workFn = test_co_Fiber_movapsProbe,
    }));
    co_Fiber_Context_from(&test_co_Fiber_probe_fiber_ctx, start, test_co_Fiber_entry);
    let_(sw, co_Fiber) = {
        .old = &test_co_Fiber_probe_sched_ctx,
        .new = &test_co_Fiber_probe_fiber_ctx,
    };
    let_ignore = co_Fiber_contextSwitch(&sw);
} $suppressed;

$attr($suppressing_(frame_larger_than))
TEST_fn_("co/Fiber: stack arg honors co_Fiber_stackArgAlign" $scope) {
    pp_if_(pp_not(co_Fiber_supported))(try_(TEST_skip()));
    var_(stack_storage, A$$(4096, u8)) = cleared();
    let stack = A_ref$((S$u8)(stack_storage));
    let arg = unwrap_(co_Fiber_stackAllocArg(stack, 24, co_Fiber_stackArgAlign));
    try_(TEST_expect(isAligned(ptrToInt(arg), co_Fiber_stackArgAlign)));
    return_ok({});
} $unscoped(TEST_fn) $suppressed;

$attr($suppressing_(frame_larger_than))
TEST_fn_("co/Fiber: x86_64 context entry rsp is 8 mod 16" $scope) {
    pp_if_(pp_not(co_Fiber_supported))(try_(TEST_skip()));
    pp_if_(pp_not(arch_type_x86_64))(try_(TEST_skip()));
    var_(stack_storage, A$$(4096, u8)) = cleared();
    let stack = A_ref$((S$u8)(stack_storage));
    let start = unwrap_(co_Fiber_stackAllocArg(
        stack,
        sizeOf$(exec_Fiber_Starter),
        co_Fiber_stackArgAlign
    ));
    var_(ctx, co_Fiber_Context) = cleared();
    co_Fiber_Context_from(&ctx, start, test_co_Fiber_entry);
    try_(TEST_expect(ctx.rsp % 16 == 8));
    return_ok({});
} $unscoped(TEST_fn) $suppressed;

$attr($suppressing_(frame_larger_than))
TEST_fn_("co/Fiber: context switch survives movaps stack copies" $scope) {
    pp_if_(pp_not(co_Fiber_supported))(try_(TEST_skip()));
    test_co_Fiber_movaps_probe_ok = false;
    var_(ctx, test_co_Fiber_MovapsCtx) = {
        .ret = 42,
        .block0 = A_init({ [0] = 0xAA, [15] = 0xBB }),
        .block1 = A_init({ [0] = 0xCC, [15] = 0xDD }),
    };
    test_co_Fiber_runMovapsProbe(ctx);
    try_(TEST_expect(test_co_Fiber_movaps_probe_ok));
    return_ok({});
} $unscoped(TEST_fn) $suppressed;

/*--- Struct argument preservation (16 / 24 / 32 bytes) across contextSwitch ---*/

T_alias$((test_co_Fiber_Probe16)(struct test_co_Fiber_Probe16 {
    var_(w0, u64);
    var_(w1, u64);
}));
claim_assert_static(sizeOf$(test_co_Fiber_Probe16) == usize_(16));

T_alias$((test_co_Fiber_Probe24)(struct test_co_Fiber_Probe24 {
    var_(w0, u64);
    var_(w1, u64);
    var_(w2, u64);
}));
claim_assert_static(sizeOf$(test_co_Fiber_Probe24) == usize_(24));

T_alias$((test_co_Fiber_Probe32)(struct test_co_Fiber_Probe32 {
    var_(w0, u64);
    var_(w1, u64);
    var_(w2, u64);
    var_(w3, u64);
}));
claim_assert_static(sizeOf$(test_co_Fiber_Probe32) == usize_(32));

#define test_co_Fiber_argProbe_yield_count u32_(32)

T_alias$((test_co_Fiber_ArgProbeState)(struct test_co_Fiber_ArgProbeState {
    var_(p16, test_co_Fiber_Probe16);
    var_(p24, test_co_Fiber_Probe24);
    var_(p32, test_co_Fiber_Probe32);
    var_(done, bool);
    var_(ok, bool);
}));

$static var_(test_co_Fiber_argProbe_sched_ctx, co_Fiber_Context);
$static var_(test_co_Fiber_argProbe_fiber_ctx, co_Fiber_Context);

$static fn_((test_co_Fiber_argProbe_expect16(test_co_Fiber_Probe16 p))(bool)) {
    return p.w0 == u64_(0xA16A16A16A16A160) && p.w1 == u64_(0xA16A16A16A16A161);
};

$static fn_((test_co_Fiber_argProbe_expect24(test_co_Fiber_Probe24 p))(bool)) {
    return p.w0 == u64_(0xB24B24B24B24B240)
        && p.w1 == u64_(0xB24B24B24B24B241)
        && p.w2 == u64_(0xB24B24B24B24B242);
};

$static fn_((test_co_Fiber_argProbe_expect32(test_co_Fiber_Probe32 p))(bool)) {
    return p.w0 == u64_(0xC32C32C32C32C320)
        && p.w1 == u64_(0xC32C32C32C32C321)
        && p.w2 == u64_(0xC32C32C32C32C322)
        && p.w3 == u64_(0xC32C32C32C32C323);
};

$static fn_((test_co_Fiber_argProbe_expect(
    test_co_Fiber_Probe16 p16, test_co_Fiber_Probe24 p24, test_co_Fiber_Probe32 p32
))(bool)) {
    return test_co_Fiber_argProbe_expect16(p16)
        && test_co_Fiber_argProbe_expect24(p24)
        && test_co_Fiber_argProbe_expect32(p32);
};

$static fn_((test_co_Fiber_argProbeBody(
    test_co_Fiber_Probe16 p16, test_co_Fiber_Probe24 p24, test_co_Fiber_Probe32 p32,
    co_Fiber_Context* sched_ctx, co_Fiber_Context* fiber_ctx, u32 yield_count
))(bool)) {
    claim_assert_nonnull(sched_ctx), claim_assert_nonnull(fiber_ctx);
    while_(var_(i, u32) = 0, i < yield_count, ++i) {
        if (!test_co_Fiber_argProbe_expect(p16, p24, p32)) return false;
        let_(sw, co_Fiber) = {
            .old = fiber_ctx,
            .new = sched_ctx,
        };
        let_ignore = co_Fiber_contextSwitch(&sw);
    };
    return test_co_Fiber_argProbe_expect(p16, p24, p32);
};

$static fn_((test_co_Fiber_argProbeWork(P$raw owner, P$raw ctx))(void)) {
    let_ignore = owner;
    let state = as$(test_co_Fiber_ArgProbeState*)(ctx);
    state->ok = test_co_Fiber_argProbeBody(
        state->p16, state->p24, state->p32,
        &test_co_Fiber_argProbe_sched_ctx, &test_co_Fiber_argProbe_fiber_ctx,
        test_co_Fiber_argProbe_yield_count
    );
    state->done = true;
    let_(sw, co_Fiber) = {
        .old = &test_co_Fiber_argProbe_fiber_ctx,
        .new = &test_co_Fiber_argProbe_sched_ctx,
    };
    let_ignore = co_Fiber_contextSwitch(&sw);
    claim_unreachable;
};

$attr($suppressing_(frame_larger_than))
$static fn_((test_co_Fiber_runArgProbe(test_co_Fiber_ArgProbeState* state))(void)) {
    claim_assert_nonnull(state);
    state->done = false;
    state->ok = false;
    var_(stack_storage, A$$(8192, u8)) = cleared();
    let stack = A_ref$((S$u8)(stack_storage));
    let start = unwrap_(co_Fiber_stackAllocArg(
        stack,
        sizeOf$(exec_Fiber_Starter),
        co_Fiber_stackArgAlign
    ));
    asg_l((as$(exec_Fiber_Starter*)(start))({
        .owner = null,
        .task = as$(P$raw)(state),
        .workFn = test_co_Fiber_argProbeWork,
    }));
    co_Fiber_Context_from(&test_co_Fiber_argProbe_fiber_ctx, start, test_co_Fiber_entry);
    while (!state->done) {
        let_(sw, co_Fiber) = {
            .old = &test_co_Fiber_argProbe_sched_ctx,
            .new = &test_co_Fiber_argProbe_fiber_ctx,
        };
        let_ignore = co_Fiber_contextSwitch(&sw);
    }
} $suppressed;

$attr($suppressing_(frame_larger_than))
TEST_fn_("co/Fiber: context switch preserves 16-, 24-, and 32-byte struct arguments" $scope) {
    pp_if_(pp_not(co_Fiber_supported))(try_(TEST_skip()));
    var_(state, test_co_Fiber_ArgProbeState) = {
        .p16 = { .w0 = u64_(0xA16A16A16A16A160), .w1 = u64_(0xA16A16A16A16A161) },
        .p24 = {
            .w0 = u64_(0xB24B24B24B24B240),
            .w1 = u64_(0xB24B24B24B24B241),
            .w2 = u64_(0xB24B24B24B24B242),
        },
        .p32 = {
            .w0 = u64_(0xC32C32C32C32C320),
            .w1 = u64_(0xC32C32C32C32C321),
            .w2 = u64_(0xC32C32C32C32C322),
            .w3 = u64_(0xC32C32C32C32C323),
        },
        .done = false,
        .ok = false,
    };
    test_co_Fiber_runArgProbe(&state);
    try_(TEST_expect(state.done));
    try_(TEST_expect(state.ok));
    return_ok({});
} $unscoped(TEST_fn) $suppressed;
