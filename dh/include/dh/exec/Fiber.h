#pragma once
#ifndef exec_Fiber__included
#define exec_Fiber__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../co/Fiber.h"
#include "../mem/Alctr.h"
#include "../mem/common.h"

/*========== Macros and Declarations ========================================*/

errset_((exec_Fiber_E)(exec_Fiber_Unsupported) $union_errset_(mem_E));

/*--- Stack policy ---*/

#define exec_Fiber_stack_reserve_size (usize_(1) * 1024 * 1024)
#define exec_Fiber_stack_commit_size (usize_(1) * mem_page_size)
#define exec_Fiber_stack_grow_size (usize_(1) * mem_page_size)
#define exec_Fiber_stack_guard_size (usize_(1) * mem_page_size)
#define exec_Fiber_stack_switch_headroom (usize_(64) * 1024)
#define exec_Fiber_stack_size exec_Fiber_stack_reserve_size

T_alias$((exec_Fiber_StackPolicy)(struct exec_Fiber_StackPolicy {
    var_(reserve_size, usize);
    var_(initial_commit_size, usize);
    var_(grow_commit_size, usize);
    var_(guard_size, usize);
}));
$attr($inline_always)
$static fn_((exec_Fiber_StackPolicy_default(void))(exec_Fiber_StackPolicy)) {
    return (exec_Fiber_StackPolicy){
        .reserve_size = exec_Fiber_stack_reserve_size,
        .initial_commit_size = exec_Fiber_stack_commit_size,
        .grow_commit_size = exec_Fiber_stack_grow_size,
        .guard_size = exec_Fiber_stack_guard_size,
    };
};

/*--- Worker function ---*/

T_alias$((exec_Fiber_WorkFn)(fn_(((*)(P$raw owner, P$raw task))(void) $T)));
T_alias$((exec_Fiber_Starter)(struct exec_Fiber_Starter {
    var_(owner, P$raw);
    var_(task, P$raw);
    var_(workFn, exec_Fiber_WorkFn);
}));

/*--- Types ---*/

T_alias$((exec_Fiber)(struct exec_Fiber {
    var_(storage, S$u8);
    var_(stack, S$u8);
    var_(grow_size, usize);
    var_(guard_size, usize);
    var_(is_virtual, bool);
    var_(context, co_Fiber_Context);
    var_(result_ty, debug_TypeInfo);
}));
T_use$((exec_Fiber)(P, (O, P)));
T_use_E$($set(exec_Fiber_E)(P$exec_Fiber));

/*--- Lifecycle ---*/

$attr($must_check)
$extern fn_((exec_Fiber_init(
    mem_Alctr gpa, P$raw owner,
    P$raw task, exec_Fiber_WorkFn workFn, TypeInfo result_ty
))(exec_Fiber_E$P$exec_Fiber));
$attr($must_check)
$extern fn_((exec_Fiber_initWithPolicy(
    mem_Alctr gpa, P$raw owner, P$raw task,
    exec_Fiber_WorkFn workFn, TypeInfo result_ty,
    exec_Fiber_StackPolicy policy
))(exec_Fiber_E$P$exec_Fiber));
$extern fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa, TypeInfo result_ty))(void));

/*--- Result access ---*/

$attr($inline_always)
$static fn_((exec_Fiber_slabBytes(TypeInfo result_ty))(usize));
$attr($inline_always)
$static fn_((exec_Fiber_result(exec_Fiber* self, TypeInfo type))(u_P_const$raw));
$attr($inline_always)
$static fn_((exec_Fiber_resultMut(exec_Fiber* self, TypeInfo type))(u_P$raw));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((exec_Fiber_slabBytes(TypeInfo result_ty))(usize)) {
    return mem_alignFwd(sizeOf$(exec_Fiber), mem_log2ToAlign(result_ty.log2_align)) + result_ty.size;
};

fn_((exec_Fiber_result(exec_Fiber* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->result_ty), type, TypeInfo_eql);
    return (u_P_const$raw){
        .raw = intToPtr$((u8*)(ptrToInt(self) + mem_alignFwd(sizeOf$(exec_Fiber), mem_log2ToAlign(type.log2_align)))),
        .type = type,
    };
};

fn_((exec_Fiber_resultMut(exec_Fiber* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->result_ty), type, TypeInfo_eql);
    return (u_P$raw){
        .raw = intToPtr$((u8*)(ptrToInt(self) + mem_alignFwd(sizeOf$(exec_Fiber), mem_log2ToAlign(type.log2_align)))),
        .type = type,
    };
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Fiber__included */
