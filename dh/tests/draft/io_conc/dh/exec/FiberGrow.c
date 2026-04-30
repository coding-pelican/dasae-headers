#include "FiberGrow.h"
#include "dh/heap/vmem.h"

#if plat_is_windows
#include "dh/os/windows/except.h"
#elif plat_based_unix
#include <signal.h>
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
$static fn_((exec_Fiber__windowsGrow(exec_Fiber* self))(bool));
static LONG CALLBACK exec_Fiber__windowsHandleException(EXCEPTION_POINTERS* info);
#elif plat_based_unix
$static var_(exec_Fiber__unix_handler_state, atom_V$$(u32)) = atom_V_init(exec_Fiber__handler_state_uninit);
$static var_(exec_Fiber__unix_old_sigsegv, struct sigaction);
$static var_(exec_Fiber__unix_old_sigbus, struct sigaction);
$static fn_((exec_Fiber__ensureUnixSignalHandler(void))(bool));
$static fn_((exec_Fiber__unixGrow(exec_Fiber* self))(bool));
static void exec_Fiber__unixHandleSignal(i32 sig, siginfo_t* info, P$raw uctx);
#endif

#if plat_is_windows
fn_((exec_Fiber__ensureWindowsExceptionHandler(void))(bool)) {
    while (true) {
        let state = atom_V_load(&exec_Fiber__windows_handler_state, atom_MemOrd_acquire);
        switch (state) {
        case exec_Fiber__handler_state_ready: return true;
        case exec_Fiber__handler_state_failed: return false;
        case exec_Fiber__handler_state_installing:
            atom_spinLoopHint();
            continue;
        default: break;
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

fn_((exec_Fiber__windowsGrow(exec_Fiber* self))(bool)) {
    claim_assert_nonnull(self), claim_assert(self->is_virtual), claim_assert(self->guard_size != 0);
    let storage_begin = ptrToInt(self->storage.ptr);
    let guard_begin = ptrToInt(self->stack.ptr) - self->guard_size;
    if (guard_begin <= storage_begin || self->grow_size == 0) return false;
    let available = guard_begin - storage_begin;
    let grow_size = self->grow_size <= available ? self->grow_size : available;
    let new_guard_begin = guard_begin - grow_size;
    if (!heap_vmem_commit(intToPtr$((P$raw)(new_guard_begin)), grow_size)) {
        return false;
    }
    if (!heap_vmem_protect(intToPtr$((P$raw)(new_guard_begin)), self->guard_size, heap_vmem_Protect_read_write_guard)) {
        return false;
    }
    self->stack.ptr = intToPtr$((u8*)(new_guard_begin + self->guard_size));
    self->stack.len += grow_size;
    return true;
};

static LONG CALLBACK exec_Fiber__windowsHandleException(EXCEPTION_POINTERS* info) {
    if (info == null || info->ExceptionRecord == null) return EXCEPTION_CONTINUE_SEARCH;
    if (info->ExceptionRecord->ExceptionCode != STATUS_GUARD_PAGE_VIOLATION) return EXCEPTION_CONTINUE_SEARCH;
    let fiber = exec_Fiber__current;
    if (fiber == null || !fiber->is_virtual || fiber->guard_size == 0) return EXCEPTION_CONTINUE_SEARCH;
    let guard_begin = ptrToInt(fiber->stack.ptr) - fiber->guard_size;
    let guard_end = guard_begin + fiber->guard_size;
    let fault_addr = info->ExceptionRecord->NumberParameters >= 2
                       ? as$(usize)(info->ExceptionRecord->ExceptionInformation[1])
                       : ptrToInt(info->ExceptionRecord->ExceptionAddress);
    if (fault_addr < guard_begin || fault_addr >= guard_end) return EXCEPTION_CONTINUE_SEARCH;
    return exec_Fiber__windowsGrow(fiber) ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_CONTINUE_SEARCH;
}
#elif plat_based_unix
fn_((exec_Fiber__ensureUnixSignalHandler(void))(bool)) {
    while (true) {
        let state = atom_V_load(&exec_Fiber__unix_handler_state, atom_MemOrd_acquire);
        switch (state) {
        case exec_Fiber__handler_state_ready: return true;
        case exec_Fiber__handler_state_failed: return false;
        case exec_Fiber__handler_state_installing:
            atom_spinLoopHint();
            continue;
        default: break;
        }
        if_some((atom_V_cmpXchgStrong(
               &exec_Fiber__unix_handler_state,
               exec_Fiber__handler_state_uninit,
               exec_Fiber__handler_state_installing,
               atom_MemOrd_acq_rel,
               atom_MemOrd_acquire
           ))(continue))
            ;

        struct sigaction current = cleared();
        if (sigaction(SIGSEGV, null, &current) != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (current.sa_handler != SIG_DFL) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        exec_Fiber__unix_old_sigsegv = current;
        if (sigaction(SIGBUS, null, &current) != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (current.sa_handler != SIG_DFL) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        exec_Fiber__unix_old_sigbus = current;

        struct sigaction action = cleared();
        action.sa_flags = SA_SIGINFO;
        action.sa_sigaction = exec_Fiber__unixHandleSignal;
        sigemptyset(&action.sa_mask);
        if (sigaction(SIGSEGV, &action, null) != 0) {
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        if (sigaction(SIGBUS, &action, null) != 0) {
            let_ignore = sigaction(SIGSEGV, &exec_Fiber__unix_old_sigsegv, null);
            atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_failed, atom_MemOrd_release);
            return false;
        }
        atom_V_store(&exec_Fiber__unix_handler_state, exec_Fiber__handler_state_ready, atom_MemOrd_release);
        return true;
    }
};

fn_((exec_Fiber__unixGrow(exec_Fiber* self))(bool)) {
    claim_assert_nonnull(self), claim_assert(self->is_virtual), claim_assert(self->guard_size != 0);
    let storage_begin = ptrToInt(self->storage.ptr);
    let guard_begin = ptrToInt(self->stack.ptr) - self->guard_size;
    if (guard_begin <= storage_begin || self->grow_size == 0) return false;
    let available = guard_begin - storage_begin;
    let grow_size = self->grow_size <= available ? self->grow_size : available;
    let new_guard_begin = guard_begin - grow_size;
    if (!heap_vmem_commit(intToPtr$((P$raw)(new_guard_begin)), grow_size)) {
        return false;
    }
    if (!heap_vmem_protect(intToPtr$((P$raw)(new_guard_begin)), self->guard_size, heap_vmem_Protect_none)) {
        return false;
    }
    self->stack.ptr = intToPtr$((u8*)(new_guard_begin + self->guard_size));
    self->stack.len += grow_size;
    return true;
};

static void exec_Fiber__unixHandleSignal(i32 sig, siginfo_t* info, P$raw uctx) {
    let_ignore = uctx;
    let fiber = exec_Fiber__current;
    if (info != null && fiber != null && fiber->is_virtual && fiber->guard_size != 0) {
        let guard_begin = ptrToInt(fiber->stack.ptr) - fiber->guard_size;
        let guard_end = guard_begin + fiber->guard_size;
        let fault_addr = ptrToInt(info->si_addr);
        if (fault_addr >= guard_begin && fault_addr < guard_end && exec_Fiber__unixGrow(fiber)) return;
    }
    struct sigaction old = sig == SIGBUS ? exec_Fiber__unix_old_sigbus : exec_Fiber__unix_old_sigsegv;
    let_ignore = sigaction(sig, &old, null);
    raise(sig);
}
#endif

fn_((exec_Fiber_initStorage(exec_Fiber* self, mem_Alctr gpa, exec_Fiber_StackPolicy policy))(mem_E$void) $guard) {
    claim_assert_nonnull(self);
#if plat_is_windows
    let_ignore = gpa;
    let reserve_size = exec_Fiber__alignPage(policy.reserve_size);
    var guard_size = exec_Fiber__alignPage(policy.guard_size);
    var commit_size = exec_Fiber__alignPage(policy.initial_commit_size);
    var grow_size = exec_Fiber__alignPage(policy.grow_commit_size);
    if (guard_size >= commit_size) commit_size = guard_size + mem_page_size;
    if (reserve_size < commit_size) commit_size = reserve_size;
    let can_grow = grow_size != 0
                && guard_size != 0
                && commit_size < reserve_size
                && exec_Fiber__ensureWindowsExceptionHandler();
    if (!can_grow) {
        guard_size = 0;
        grow_size = 0;
        commit_size = reserve_size;
    }
    let storage_ptr = orelse_((heap_vmem_reserve(null, reserve_size))(return_err(mem_E_OutOfMemory())));
    errdefer_($ignore, heap_vmem_release(storage_ptr, reserve_size));
    let commit_begin = ptrToInt(storage_ptr) + (reserve_size - commit_size);
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_begin)), commit_size)) {
        return_err(mem_E_OutOfMemory());
    }
    if (guard_size != 0) {
        if (!heap_vmem_protect(intToPtr$((P$raw)(commit_begin)), guard_size, heap_vmem_Protect_read_write_guard)) {
            return_err(mem_E_OutOfMemory());
        }
    }
    self->storage = (S$u8){ .ptr = storage_ptr, .len = reserve_size };
    self->stack = exec_Fiber__usableStack(self, commit_size);
    self->grow_size = grow_size;
    self->guard_size = guard_size;
    self->is_virtual = true;
    return_ok({});
#elif plat_based_unix
    let reserve_size = exec_Fiber__alignPage(policy.reserve_size);
    var guard_size = exec_Fiber__alignPage(policy.guard_size);
    var commit_size = exec_Fiber__alignPage(policy.initial_commit_size);
    var grow_size = exec_Fiber__alignPage(policy.grow_commit_size);
    if (guard_size >= commit_size) commit_size = guard_size + mem_page_size;
    if (reserve_size < commit_size) commit_size = reserve_size;
    let can_grow = grow_size != 0
                && guard_size != 0
                && commit_size < reserve_size
                && exec_Fiber__ensureUnixSignalHandler();
    if (!can_grow) {
        guard_size = 0;
        grow_size = 0;
        commit_size = reserve_size;
    }
    let storage_ptr = orelse_((heap_vmem_reserve(null, reserve_size))(return_err(mem_E_OutOfMemory())));
    errdefer_($ignore, heap_vmem_release(storage_ptr, reserve_size));
    let commit_begin = ptrToInt(storage_ptr) + (reserve_size - commit_size);
    if (!heap_vmem_commit(intToPtr$((P$raw)(commit_begin)), commit_size)) {
        return_err(mem_E_OutOfMemory());
    }
    if (guard_size != 0 && !heap_vmem_protect(intToPtr$((P$raw)(commit_begin)), guard_size, heap_vmem_Protect_none)) {
        return_err(mem_E_OutOfMemory());
    }
    self->storage = (S$u8){ .ptr = storage_ptr, .len = reserve_size };
    self->stack = exec_Fiber__usableStack(self, commit_size);
    self->grow_size = grow_size;
    self->guard_size = guard_size;
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
