#include "Fiber.h"
#include "FiberGrow.h"

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

fn_((exec_Fiber_init(mem_Alctr gpa, P$raw owner, P$raw task, exec_Fiber_WorkFn workFn))(mem_E$P$exec_Fiber)) {
    return exec_Fiber_initWithPolicy(gpa, owner, task, workFn, exec_Fiber_defaultStackPolicy());
};

fn_((exec_Fiber_initWithPolicy(
    mem_Alctr gpa,
    P$raw owner,
    P$raw task,
    exec_Fiber_WorkFn workFn,
    exec_Fiber_StackPolicy policy
))(mem_E$P$exec_Fiber) $guard) {
    claim_assert_nonnull(owner), claim_assert_nonnull(task), claim_assert_nonnull(workFn);
    pp_if_(pp_not(Co_Fiber_supported))(
        /*pp_then_*/ ({
            let_ignore = gpa;
            let_ignore = owner;
            let_ignore = task;
            let_ignore = workFn;
            let_ignore = policy;
            return_err(mem_E_OutOfMemory());
        }),
        /*pp_else_*/ ({
            let fiber = u_castP$((P$exec_Fiber)(try_(mem_Alctr_create($trace gpa, typeInfo$(exec_Fiber)))));
            errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(fiber)));
            try_(exec_Fiber_initStorage(fiber, gpa, policy));
            let start = orelse_((Co_Fiber_stackAllocArg(fiber->stack, sizeOf$(exec_Fiber_Starter), alignOf$(exec_Fiber_Starter)))(
                return_err(mem_E_OutOfMemory())
            ));
            asg_l((as$(exec_Fiber_Starter*)(start))({
                .owner = owner,
                .task = task,
                .workFn = workFn,
            }));
            Co_Fiber_Context_from(&fiber->context, start, exec_Fiber__entry);
            return_ok(fiber);
        })
    );
} $unguarded(fn);

fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
    exec_Fiber_finiStorage(self, gpa);
    mem_Alctr_destroy($trace gpa, u_anyP(self));
};
