#pragma once
#ifndef exec_Fiber_growable__included
#define exec_Fiber_growable__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Fiber.h"
#include "../atom.h"
#include "../TEST/cfg.h"

/*========== Macros and Declarations ========================================*/

/* --- Growable stack --- */

$extern fn_((exec_Fiber_initStorage(exec_Fiber* self, mem_Alctr gpa, exec_Fiber_StackPolicy policy))(mem_E$void));
$extern fn_((exec_Fiber_finiStorage(exec_Fiber* self, mem_Alctr gpa))(void));
$extern fn_((exec_Fiber_ensureStackHeadroom(exec_Fiber* self, usize rsp, usize margin))(void));

$attr($inline_always)
$static fn_((exec_Fiber_alignPage(usize size))(usize));
$attr($inline_always)
$static fn_((exec_Fiber_usableStack(exec_Fiber* self, usize commit_size))(S$u8));

/* --- Context switch --- */

$extern fn_((exec_Fiber_enter(exec_Fiber* self))(O$P$exec_Fiber));
$extern fn_((exec_Fiber_restore(O$P$exec_Fiber prev))(void));

#if TEST_enabled
/* --- Diagnostics --- */

T_alias$((exec_Fiber_EnsureDiag_Stage)(enum exec_Fiber_EnsureDiag_Stage{
    exec_Fiber_EnsureDiag_Stage_none = 0,
    exec_Fiber_EnsureDiag_Stage_skip_not_virtual,
    exec_Fiber_EnsureDiag_Stage_skip_sufficient,
    exec_Fiber_EnsureDiag_Stage_ok,
    exec_Fiber_EnsureDiag_Stage_fail_rsp_below_storage,
    exec_Fiber_EnsureDiag_Stage_fail_rsp_above_storage,
    exec_Fiber_EnsureDiag_Stage_fail_new_guard_below_storage,
    exec_Fiber_EnsureDiag_Stage_fail_commit,
    exec_Fiber_EnsureDiag_Stage_fail_protect_rw,
    exec_Fiber_EnsureDiag_Stage_fail_protect_guard,
    exec_Fiber_EnsureDiag_Stage_fail_post_fault,
}));
T_alias$((exec_Fiber_EnsureDiag)(struct exec_Fiber_EnsureDiag {
    var_(stage, exec_Fiber_EnsureDiag_Stage);
    var_(rsp, usize);
    var_(storage_begin, usize);
    var_(storage_end, usize);
    var_(stack_bottom, usize);
    var_(target, usize);
    var_(new_guard_begin, usize);
    var_(commit_len, usize);
    var_(fail_addr, usize);
}));
$extern fn_((exec_Fiber_ensureDiagLast(void))(exec_Fiber_EnsureDiag));
#endif /* TEST_enabled */

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((exec_Fiber_alignPage(usize size))(usize)) {
    return mem_alignFwd(size == 0 ? mem_page_size : size, mem_page_size);
};

fn_((exec_Fiber_usableStack(exec_Fiber* self, usize commit_size))(S$u8)) {
    claim_assert_nonnull(self), claim_assert(commit_size <= self->storage.len), claim_assert(commit_size >= self->guard_size);
    let storage_begin = ptrToInt(self->storage.ptr);
    let commit_begin = storage_begin + (self->storage.len - commit_size);
    return (S$u8){
        .ptr = intToPtr$((u8*)(commit_begin + self->guard_size)),
        .len = commit_size - self->guard_size,
    };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Fiber_growable__included */
