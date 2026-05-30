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

$attr(__attribute__((naked)))
$static fn_((test_co_Fiber_entry(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
    pp_if_(co_Fiber_supported)((asm_volatile(pp_switch_((arch_type)(
        pp_case_((arch_type_x86_64)(
            "leaq 8(%%rsp), %%rdi\n\t"
            "leaq 8(%%rsp), %%rcx\n\t"
            "movq %%rsi, %%rdx\n\t"
            "jmp exec_callFiber\n\t" : : : "memory"
        )),
        pp_case_((arch_type_aarch64)(
            "mov x0, sp\n\t"
            "b exec_callFiber\n\t" : : : "memory"
        )),
        pp_case_((arch_type_riscv64)(
            "mv a0, sp\n\t"
            "tail exec_callFiber\n\t" : : : "memory"
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
