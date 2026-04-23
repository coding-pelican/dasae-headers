#include "Fiber.h"

$attr(__attribute__((naked)))
$static fn_((exec_Fiber__entry(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
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

fn_((exec_Fiber_init(mem_Alctr gpa, P$raw owner, P$raw task, exec_Fiber_WorkFn workFn))(mem_E$P$exec_Fiber) $guard) {
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
            Co_Fiber_Context_from(&fiber->ctx, start, exec_Fiber__entry);
            return_ok(fiber);
        })
    );
} $unguarded(fn);

fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
    mem_Alctr_free($trace gpa, u_anyS(self->stk));
    mem_Alctr_destroy($trace gpa, u_anyP(self));
    asg_l((self)(cleared()));
};
