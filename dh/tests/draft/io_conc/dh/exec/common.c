#include "common.h"

$attr(__attribute__((naked)))
$static fn_((exec_Fiber_entry(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
    pp_if_(Co_Fiber_supported)((__asm__ __volatile__(pp_switch_((arch_type)(
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

fn_((exec_callFiber(exec_Fiber_Starter* start, const Co_Fiber* first_switch))(void)) {
    claim_assert_nonnull(start), claim_assert_nonnull(first_switch);
    let_ignore = first_switch;
    start->workFn(start->owner, start->task);
    claim_unreachable;
};

fn_((exec_createFiber(mem_Alctr gpa, P$raw owner, P$raw task, exec_Fiber_WorkFn workFn))(mem_E$P$exec_Fiber) $guard) {
    claim_assert_nonnull(owner), claim_assert_nonnull(task), claim_assert_nonnull(workFn);
    pp_if_(pp_not(Co_Fiber_supported))(
        pp_then_({
            let_ignore = gpa;
            let_ignore = owner;
            let_ignore = task;
            let_ignore = workFn;
            return_err(mem_E_OutOfMemory());
        }),
        pp_else_({
            let fiber = u_castP$((P$exec_Fiber)(try_(mem_Alctr_create($trace gpa, typeInfo$(exec_Fiber)))));
            errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(fiber)));
            let stk = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), exec_Fiber_stk_size))));
            errdefer_($ignore, mem_Alctr_free($trace gpa, u_anyS(stk)));
            fiber->stk = stk;
            let start = orelse_((Co_Fiber_stackAllocArg(stk, sizeOf$(exec_Fiber_Starter), alignOf$(exec_Fiber_Starter)))(
                return_err(mem_E_OutOfMemory())
            ));
            asg_l((as$(exec_Fiber_Starter*)(start))({
                .owner = owner,
                .task = task,
                .workFn = workFn,
            }));
            Co_Fiber_Context_init(&fiber->ctx, start, exec_Fiber_entry);
            return_ok(fiber);
        })
    );
} $unguarded(fn);

fn_((exec_destroyFiber(mem_Alctr gpa, exec_Fiber* fiber))(void)) {
    claim_assert_nonnull(fiber);
    mem_Alctr_free($trace gpa, u_anyS(fiber->stk));
    mem_Alctr_destroy($trace gpa, u_anyP(fiber));
};

fn_((exec_switchToFiber(Co_Fiber_Context* sched_ctx, exec_Fiber* fiber))(void)) {
    claim_assert_nonnull(sched_ctx), claim_assert_nonnull(fiber);
    let_(switch_to, Co_Fiber) = {
        .old = sched_ctx,
        .new = &fiber->ctx,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_to);
};

fn_((exec_switchFromFiber(Co_Fiber_Context* fiber_ctx, Co_Fiber_Context* sched_ctx))(void)) {
    claim_assert_nonnull(fiber_ctx), claim_assert_nonnull(sched_ctx);
    let_(switch_back, Co_Fiber) = {
        .old = fiber_ctx,
        .new = sched_ctx,
    };
    let_ignore = Co_Fiber_contextSwitch(&switch_back);
};
