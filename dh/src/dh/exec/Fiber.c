#include "dh/exec/Fiber.h"
#include "dh/exec/Fiber-growable.h"
#include "dh/exec/common.h"

$static fn_((exec_Fiber__freeSlab(exec_Fiber* self, mem_Alctr gpa, TypeInfo result_ty))(void));
$attr($callconv_naked)
$static fn_((exec_Fiber__entry(void))(void)) { /* NOLINTBEGIN(hicpp-no-assembler) */
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

/*========== External Definitions ===========================================*/

fn_((exec_Fiber_init(
    mem_Alctr gpa, P$raw owner,
    P$raw task, exec_Fiber_WorkFn workFn, TypeInfo result_ty
))(exec_Fiber_E$P$exec_Fiber)) {
    return exec_Fiber_initWithPolicy(
        gpa, owner,
        task, workFn, result_ty,
        exec_Fiber_StackPolicy_default()
    );
};

fn_((exec_Fiber_initWithPolicy(
    mem_Alctr gpa, P$raw owner,
    P$raw task, exec_Fiber_WorkFn workFn, TypeInfo result_ty,
    exec_Fiber_StackPolicy policy
))(exec_Fiber_E$P$exec_Fiber) $guard) {
    claim_assert_nonnull(owner), claim_assert_nonnull(task), claim_assert_nonnull(workFn);
    pp_if_(pp_not(co_Fiber_supported))(
        /*pp_then_*/ ({
            let_ignore = gpa;
            let_ignore = owner;
            let_ignore = task;
            let_ignore = workFn;
            let_ignore = result_ty;
            let_ignore = policy;
            return_err(E_cause$exec_Fiber_Unsupported());
        }),
        /*pp_else_*/ ({
            let bytes = exec_Fiber_slabBytes(result_ty);
            let mem = orelse_((mem_Alctr_rawAlloc($trace gpa, bytes, alignOfLog2$(exec_Fiber)))(
                return_err(E_cause$OutOfMemory())
            ));
            let fiber = ptrAlignCast$((exec_Fiber*)(mem));
            errdefer_($ignore, exec_Fiber__freeSlab(fiber, gpa, result_ty));
            mem_set0Bytes(P_prefix$((S$u8)(mem)(bytes)));
            asg_l((fiber)({
                .storage = cleared(),
                .stack = cleared(),
                .grow_size = 0,
                .guard_size = 0,
                .is_virtual = false,
                .context = cleared(),
                .result_ty = $typing(result_ty),
            }));
            try_(exec_Fiber_initStorage(fiber, gpa, policy));
            let start = orelse_((co_Fiber_stackAllocArg(
                fiber->stack,
                sizeOf$(exec_Fiber_Starter),
                co_Fiber_stackArgAlign
            ))(return_err(E_cause$OutOfMemory())));
            asg_l((as$(exec_Fiber_Starter*)(start))({
                .owner = owner,
                .task = task,
                .workFn = workFn,
            }));
            co_Fiber_Context_from(&fiber->context, start, exec_Fiber__entry);
            return_ok(fiber);
        })
    );
} $unguarded(fn);

fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa, TypeInfo result_ty))(void)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->result_ty), result_ty, TypeInfo_eql);
    exec_Fiber_finiStorage(self, gpa);
    exec_Fiber__freeSlab(self, gpa, result_ty);
};

/*========== Internal Definitions ===========================================*/

fn_((exec_Fiber__freeSlab(exec_Fiber* self, mem_Alctr gpa, TypeInfo result_ty))(void)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->result_ty), result_ty, TypeInfo_eql);
    let bytes = exec_Fiber_slabBytes(result_ty);
    mem_Alctr_rawFree($trace gpa, P_prefix$((S$u8)(as$(u8*)(self))(bytes)), alignOfLog2$(exec_Fiber));
};
