#include "dh/exec/Fiber-growable.h"
#include "dh/heap/vmem.h"

#if plat_is_windows
#include "dh/sys/api/windows/except.h"
#include "dh/sys/api/windows/nt.h"
#elif plat_based_unix
#include "dh/sys/posix.h"
#endif

#if plat_is_windows || plat_based_unix
$thrd_local $static var_(exec_Fiber__current, exec_Fiber*) = null;
#endif

#define exec_Fiber__handler_state_uninit 0u
#define exec_Fiber__handler_state_installing 1u
#define exec_Fiber__handler_state_ready 2u
#define exec_Fiber__handler_state_failed 3u

#if plat_is_windows
$static var_(exec_Fiber__windows_handler_state, atom_V$$(u32)) = atom_V_init(exec_Fiber__handler_state_uninit);
$static var_(exec_Fiber__windows_exception_handler, P$raw) = null;
$static fn_((exec_Fiber__ensureWindowsExceptionHandler(void))(bool));
$static fn_((exec_Fiber__faultInFiberStack(exec_Fiber* self, usize fault_addr))(bool));
static LONG CALLBACK exec_Fiber__windowsHandleException(EXCEPTION_POINTERS* info);
#elif plat_based_unix
$static var_(exec_Fiber__unix_handler_state, atom_V$$(u32)) = atom_V_init(exec_Fiber__handler_state_uninit);
$static var_(exec_Fiber__unix_old_sigsegv, sys_posix_sigaction);
$static var_(exec_Fiber__unix_old_sigbus, sys_posix_sigaction);
$static fn_((exec_Fiber__ensureUnixSignalHandler(void))(bool));
$static fn_((exec_Fiber__faultInFiberStack(exec_Fiber* self, usize fault_addr))(bool));
static void exec_Fiber__unixHandleSignal(i32 sig, sys_posix_siginfo* info, P$raw uctx);
#endif

#if TEST_enabled
$thrd_local $static var_(exec_Fiber__ensureDiag, exec_Fiber_EnsureDiag);

$static fn_((exec_Fiber_ensureDiagSet(
    exec_Fiber_EnsureDiag_Stage stage,
    usize rsp,
    usize storage_begin,
    usize storage_end,
    usize stack_bottom,
    usize target,
    usize new_guard_begin,
    usize commit_len,
    usize fail_addr
))(void)) {
    asg_l((&exec_Fiber__ensureDiag)({
        .stage = stage,
        .rsp = rsp,
        .storage_begin = storage_begin,
        .storage_end = storage_end,
        .stack_bottom = stack_bottom,
        .target = target,
        .new_guard_begin = new_guard_begin,
        .commit_len = commit_len,
        .fail_addr = fail_addr,
    }));
};

fn_((exec_Fiber__ensureDiagLast(void))(exec_Fiber_EnsureDiag)) {
    return exec_Fiber__ensureDiag;
};
#else
#define exec_Fiber_ensureDiagSet(_stage, _rsp, _storage_begin, _storage_end, _stack_bottom, _target, _new_guard_begin, _commit_len, _fail_addr) \
    $do_nothing
#endif /* TEST_enabled */

#if plat_is_windows || plat_based_unix
$static fn_((exec_Fiber__faultInFiberStack(exec_Fiber* self, usize fault_addr))(bool)) {
    claim_assert_nonnull(self);
    if (!self->is_virtual || self->guard_size == 0) return false;
    let storage_begin = ptrToInt(self->storage.ptr);
    let storage_end = storage_begin + self->storage.len;
    if (fault_addr < storage_begin || fault_addr >= storage_end) return false;
    return fault_addr < ptrToInt(self->stack.ptr);
};

fn_((exec_Fiber_ensureStackHeadroom(exec_Fiber* self, usize rsp, usize margin))(void)) {
    claim_assert_nonnull(self);
    if (!self->is_virtual || self->guard_size == 0) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_skip_not_virtual,
            rsp, 0, 0, 0, 0, 0, 0, 0
        );
        return;
    }
    let storage_begin = ptrToInt(self->storage.ptr);
    let storage_end = storage_begin + self->storage.len;
    var stack_bottom = ptrToInt(self->stack.ptr);
    if (rsp < storage_begin) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_fail_rsp_below_storage,
            rsp, storage_begin, storage_end, stack_bottom, 0, 0, 0, 0
        );
        claim_unreachable_fmt(
            "fiber ensure rsp {:#x} below storage {:#x}",
            rsp, storage_begin
        );
    }
    if (rsp > storage_end) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_fail_rsp_above_storage,
            rsp, storage_begin, storage_end, stack_bottom, 0, 0, 0, 0
        );
        claim_unreachable_fmt(
            "fiber ensure rsp {:#x} above storage end {:#x}",
            rsp, storage_end
        );
    }
    let min_stack_ptr = storage_begin + self->guard_size;
    var target = rsp > margin ? rsp - margin : min_stack_ptr;
    if (target < min_stack_ptr) target = min_stack_ptr;
    target = mem_alignBwd(target, mem_page_size);
    if (target >= stack_bottom) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_skip_sufficient,
            rsp, storage_begin, storage_end, stack_bottom, target, 0, 0, 0
        );
        return;
    }
    let new_stack_bottom = target;
    let new_guard_begin = new_stack_bottom - self->guard_size;
    if (new_guard_begin < storage_begin) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_fail_new_guard_below_storage,
            rsp, storage_begin, storage_end, stack_bottom, target, new_guard_begin, 0, 0
        );
        claim_unreachable_fmt(
            "fiber ensure target {:#x} exceeds reserve (guard {:#x} storage {:#x})",
            target, new_guard_begin, storage_begin
        );
    }
    let old_guard_begin = stack_bottom - self->guard_size;
    if (new_guard_begin >= old_guard_begin) {
        exec_Fiber_ensureDiagSet(
            exec_Fiber_EnsureDiag_Stage_skip_sufficient,
            rsp, storage_begin, storage_end, stack_bottom, target, new_guard_begin, 0, 0
        );
        return;
    }
    claim_assert(self->grow_size != 0);
    while (stack_bottom > new_stack_bottom) {
        let cur_guard_begin = stack_bottom - self->guard_size;
        let available = cur_guard_begin - storage_begin;
        if (available == 0) {
            exec_Fiber_ensureDiagSet(
                exec_Fiber_EnsureDiag_Stage_fail_new_guard_below_storage,
                rsp, storage_begin, storage_end, stack_bottom, target, cur_guard_begin, 0, 0
            );
            claim_unreachable_fmt(
                "fiber ensure target {:#x} exceeds reserve (guard {:#x} storage {:#x})",
                target, cur_guard_begin, storage_begin
            );
        }
        var grow_size = self->grow_size <= available ? self->grow_size : available;
        var chunk_guard_begin = cur_guard_begin - grow_size;
        if (chunk_guard_begin < new_guard_begin) {
            grow_size = cur_guard_begin - new_guard_begin;
            chunk_guard_begin = new_guard_begin;
        }
        let commit_len = cur_guard_begin - chunk_guard_begin;
        if (commit_len == 0) break;
        if (!heap_vmem_commit(intToPtr$((P$raw)(chunk_guard_begin)), commit_len)) {
            exec_Fiber_ensureDiagSet(
                exec_Fiber_EnsureDiag_Stage_fail_commit,
                rsp, storage_begin, storage_end, stack_bottom, target, chunk_guard_begin, commit_len, 0
            );
            claim_unreachable_fmt(
                "fiber ensure commit failed at {:#x} len {:#x}",
                chunk_guard_begin, commit_len
            );
        }
        if (!heap_vmem_protect(
                intToPtr$((P$raw)(cur_guard_begin)), self->guard_size, heap_vmem_Protcn_read_write)) {
            exec_Fiber_ensureDiagSet(
                exec_Fiber_EnsureDiag_Stage_fail_protect_rw,
                rsp, storage_begin, storage_end, stack_bottom, target, chunk_guard_begin, commit_len, cur_guard_begin
            );
            claim_unreachable_fmt(
                "fiber ensure old guard RW protect failed at {:#x}",
                cur_guard_begin
            );
        }
        if (!heap_vmem_protect(
                intToPtr$((P$raw)(chunk_guard_begin)), self->guard_size,
#if plat_is_windows
                               heap_vmem_Protcn_read_write_guard
#else
                               heap_vmem_Protcn_none
#endif
            )) {
            exec_Fiber_ensureDiagSet(
                exec_Fiber_EnsureDiag_Stage_fail_protect_guard,
                rsp, storage_begin, storage_end, stack_bottom, target, chunk_guard_begin, commit_len, chunk_guard_begin
            );
            claim_unreachable_fmt(
                "fiber ensure guard protect failed at {:#x}",
                chunk_guard_begin
            );
        }
        let prev_stack_bottom = stack_bottom;
        stack_bottom = chunk_guard_begin + self->guard_size;
        self->stack.ptr = intToPtr$((u8*)(stack_bottom));
        self->stack.len += prev_stack_bottom - stack_bottom;
    }
    exec_Fiber_ensureDiagSet(
        exec_Fiber_EnsureDiag_Stage_ok,
        rsp, storage_begin, storage_end, stack_bottom, target, new_guard_begin, 0, 0
    );
};
#endif

#if plat_is_windows
fn_((exec_Fiber__ensureWindowsExceptionHandler(void))(bool)) {
    while (true) {
        let state = atom_V_load(&exec_Fiber__windows_handler_state, atom_MemOrd_acquire);
        switch (state) {
        case_((exec_Fiber__handler_state_ready)) return true $end(case);
        case_((exec_Fiber__handler_state_failed)) return false $end(case);
        case_((exec_Fiber__handler_state_installing)) $continue_(atom_spinLoopHint()) $end(case);
        default_() $do_nothing $end(default);
        }
        if (isSome(atom_V_cmpXchgStrong(
                &exec_Fiber__windows_handler_state,
                exec_Fiber__handler_state_uninit,
                exec_Fiber__handler_state_installing,
                atom_MemOrd_acq_rel,
                atom_MemOrd_acquire
            ))) continue;
        let handler = AddVectoredExceptionHandler(1, exec_Fiber__windowsHandleException);
        if (handler == null) {
            atom_V_store(&exec_Fiber__windows_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        exec_Fiber__windows_exception_handler = handler;
        atom_V_store(&exec_Fiber__windows_handler_state, exec_Fiber__handler_state_ready, atom_MemOrd_release);
        return true;
    }
};

static LONG CALLBACK exec_Fiber__windowsHandleException(EXCEPTION_POINTERS* info) {
    if (info == null || info->ExceptionRecord == null) return EXCEPTION_CONTINUE_SEARCH;
    let fiber = exec_Fiber__current;
    if (fiber == null) return EXCEPTION_CONTINUE_SEARCH;
    let fault_addr = info->ExceptionRecord->NumberParameters >= 2
                       ? as$(usize)(info->ExceptionRecord->ExceptionInformation[1])
                       : ptrToInt(info->ExceptionRecord->ExceptionAddress);
    if (!exec_Fiber__faultInFiberStack(fiber, fault_addr)) return EXCEPTION_CONTINUE_SEARCH;
    exec_Fiber_ensureDiagSet(
        exec_Fiber_EnsureDiag_Stage_fail_post_fault,
        0, 0, 0, ptrToInt(fiber->stack.ptr), 0, 0, 0, fault_addr
    );
    claim_unreachable_fmt(
        "fiber stack fault at {:#x} without scheduler headroom grow",
        fault_addr
    );
    return EXCEPTION_CONTINUE_SEARCH;
}
#elif plat_based_unix
fn_((exec_Fiber__ensureUnixSignalHandler(void))(bool)) {
    while (true) {
        let state = atom_V_load(&exec_Fiber__unix_handler_state, atom_MemOrd_acquire);
        switch (state) {
        case_((exec_Fiber__handler_state_ready)) return true $end(case);
        case_((exec_Fiber__handler_state_failed)) return false $end(case);
        case_((exec_Fiber__handler_state_installing)) $continue_(atom_spinLoopHint()) $end(case);
        default_() $do_nothing $end(default);
        }
        if (isSome(atom_V_cmpXchgStrong(
                &exec_Fiber__unix_handler_state,
                exec_Fiber__handler_state_uninit,
                exec_Fiber__handler_state_installing,
                atom_MemOrd_acq_rel,
                atom_MemOrd_acquire
            ))) continue;

        sys_posix_sigaction current = cleared();
        if (sys_posix_sigaction_set(
                sys_posix_SIGSEGV,
                none$((O$P$raw)),
                some$((O$P$raw)(&current))
            )
            != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (current.sa_handler != sys_posix_SIG_DFL) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        exec_Fiber__unix_old_sigsegv = current;
        if (sys_posix_sigaction_set(
                sys_posix_SIGBUS,
                none$((O$P$raw)),
                some$((O$P$raw)(&current))
            )
            != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (current.sa_handler != sys_posix_SIG_DFL) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        exec_Fiber__unix_old_sigbus = current;

        sys_posix_sigaction action = cleared();
        action.sa_flags = sys_posix_SA_SIGINFO;
        action.sa_sigaction = exec_Fiber__unixHandleSignal;
        sys_posix_sigemptyset(&action.sa_mask);
        if (sys_posix_sigaction_set(
                sys_posix_SIGSEGV,
                some$((O$P$raw)(&action)),
                none$((O$P$raw))
            )
            != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (sys_posix_sigaction_set(
                sys_posix_SIGBUS,
                some$((O$P$raw)(&action)),
                none$((O$P$raw))
            )
            != 0) {
            let_ignore = sys_posix_sigaction_set(
                sys_posix_SIGSEGV,
                some$((O$P$raw)(&exec_Fiber__unix_old_sigsegv)),
                none$((O$P$raw))
            );
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_ready, atom_MemOrd_release);
        return true;
    }
};

static void exec_Fiber__unixHandleSignal(i32 sig, sys_posix_siginfo* info, P$raw uctx) {
    let_ignore = uctx;
    let fiber = exec_Fiber__current;
    if (info != null && fiber != null) {
        let fault_addr = ptrToInt(sys_posix_siginfo_addr(info));
        if (exec_Fiber__faultInFiberStack(fiber, fault_addr)) {
            exec_Fiber_ensureDiagSet(
                exec_Fiber_EnsureDiag_Stage_fail_post_fault,
                0, 0, 0, ptrToInt(fiber->stack.ptr), 0, 0, 0, fault_addr
            );
            claim_unreachable_fmt(
                "fiber stack fault at {:#x} without scheduler headroom grow",
                fault_addr
            );
        }
    }
    sys_posix_sigaction old = sig == sys_posix_SIGBUS ? exec_Fiber__unix_old_sigbus : exec_Fiber__unix_old_sigsegv;
    let_ignore = sys_posix_sigaction_set(
        sig,
        some$((O$P$raw)(&old)),
        none$((O$P$raw))
    );
    sys_posix_raise(sig);
}
#endif

fn_((exec_Fiber_initStorage(exec_Fiber* self, mem_Alctr gpa, exec_Fiber_StackPolicy policy))(mem_E$void) $guard) {
    claim_assert_nonnull(self);
#if plat_is_windows
    let_ignore = gpa;
    let reserve_size = exec_Fiber_alignPage(policy.reserve_size);
    var guard_size = exec_Fiber_alignPage(policy.guard_size);
    var commit_size = exec_Fiber_alignPage(policy.initial_commit_size);
    var grow_size = exec_Fiber_alignPage(policy.grow_commit_size);
    if (guard_size >= commit_size) commit_size = guard_size + mem_page_size;
    if (reserve_size < commit_size) commit_size = reserve_size;
    let can_grow = grow_size != 0 && guard_size != 0 && commit_size < reserve_size;
    if (can_grow) {
        let_ignore = exec_Fiber__ensureWindowsExceptionHandler();
    } else {
        guard_size = 0;
        grow_size = 0;
        commit_size = reserve_size;
    }
    let storage_ptr = orelse_((heap_vmem_reserve(null, reserve_size))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, heap_vmem_release(storage_ptr, reserve_size));
    let commit_begin = ptrToInt(storage_ptr) + (reserve_size - commit_size);
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_begin)), commit_size)) {
        return_err(E_cause$OutOfMemory());
    }
    if (guard_size != 0) {
        if (!heap_vmem_protect(intToPtr$((P$raw)(commit_begin)), guard_size, heap_vmem_Protcn_read_write_guard)) {
            return_err(E_cause$OutOfMemory());
        }
    }
    self->storage = (S$u8){ .ptr = storage_ptr, .len = reserve_size };
    self->guard_size = guard_size;
    self->grow_size = grow_size;
    self->stack = exec_Fiber_usableStack(self, commit_size);
    self->is_virtual = true;
    return_ok({});
#elif plat_based_unix
    let_ignore = gpa;
    let reserve_size = exec_Fiber_alignPage(policy.reserve_size);
    var guard_size = exec_Fiber_alignPage(policy.guard_size);
    var commit_size = exec_Fiber_alignPage(policy.initial_commit_size);
    var grow_size = exec_Fiber_alignPage(policy.grow_commit_size);
    if (guard_size >= commit_size) commit_size = guard_size + mem_page_size;
    if (reserve_size < commit_size) commit_size = reserve_size;
    let can_grow = grow_size != 0 && guard_size != 0 && commit_size < reserve_size;
    if (can_grow) {
        let_ignore = exec_Fiber__ensureUnixSignalHandler();
    } else {
        guard_size = 0;
        grow_size = 0;
        commit_size = reserve_size;
    }
    let storage_ptr = orelse_((heap_vmem_reserve(null, reserve_size))(return_err(E_cause$OutOfMemory())));
    errdefer_($ignore, heap_vmem_release(storage_ptr, reserve_size));
    let commit_begin = ptrToInt(storage_ptr) + (reserve_size - commit_size);
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_begin)), commit_size)) {
        return_err(E_cause$OutOfMemory());
    }
    if (guard_size != 0 && !heap_vmem_protect(intToPtr$((P$raw)(commit_begin)), guard_size, heap_vmem_Protcn_none)) {
        return_err(E_cause$OutOfMemory());
    }
    self->storage = (S$u8){ .ptr = storage_ptr, .len = reserve_size };
    self->guard_size = guard_size;
    self->grow_size = grow_size;
    self->stack = exec_Fiber_usableStack(self, commit_size);
    self->is_virtual = true;
    return_ok({});
#else
    let stack_size = policy.reserve_size == 0 ? exec_Fiber_stack_reserve_size : policy.reserve_size;
    let stack = u_castS$((S$u8)(try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), stack_size))));
    self->storage = stack;
    self->stack = stack;
    self->grow_size = 0;
    self->guard_size = 0;
    self->is_virtual = false;
    return_ok({});
#endif
} $unguarded(fn);

fn_((exec_Fiber_finiStorage(exec_Fiber* self, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows || plat_based_unix
    let_ignore = gpa;
    if (self->is_virtual) {
        let_ignore = heap_vmem_release(self->storage.ptr, self->storage.len);
        return;
    }
#endif
    mem_Alctr_free($trace gpa, u_anyS(self->storage));
};

fn_((exec_Fiber_enter(exec_Fiber* self))(O$P$exec_Fiber) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows || plat_based_unix
    let prev = exec_Fiber__current;
    exec_Fiber__current = self;
    prev == null ? return_none() : return_some(prev);
#else
    return_none();
#endif
} $unscoped(fn);

fn_((exec_Fiber_restore(O$P$exec_Fiber prev))(void)) {
#if plat_is_windows || plat_based_unix
    exec_Fiber__current = orelse_((prev)(null));
#else
    let_ignore = prev;
#endif
};
