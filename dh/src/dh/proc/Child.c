#include "dh/proc/Child.h"

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/proc.h"
#endif

fn_((proc_Child_wait(proc_Child* self))(E$proc_Term) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (self->handle == null || self->handle == INVALID_HANDLE_VALUE) {
        return_err(E_cause$proc_ProcessAlreadyExited());
    }
    switch (WaitForSingleObject(self->handle, INFINITE)) {
    case WAIT_OBJECT_0: break;
    case WAIT_ABANDONED: $fallthrough;
    case WAIT_TIMEOUT: $fallthrough;
    case WAIT_FAILED: $fallthrough;
    default_() return_err(E_cause$proc_SystemResources()) $end(default);
    }
    var_(exit_code, DWORD) = 0;
    if (!GetExitCodeProcess(self->handle, &exit_code)) {
        return_err(E_cause$proc_SystemResources());
    }
    claim_assert(CloseHandle(self->handle));
    self->handle = INVALID_HANDLE_VALUE;
    return_ok((proc_Term){
        .tag = proc_Term_Tag_exited,
        .code = as$(u32)(exit_code),
    });
#else
    let_ignore = self;
    return_err(E_cause$proc_OperationUnsupported());
#endif
} $unscoped(fn);

fn_((proc_Child_kill(proc_Child* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_windows
    if (self->handle != null && self->handle != INVALID_HANDLE_VALUE) {
        let_ignore = TerminateProcess(self->handle, 1);
    }
#else
    let_ignore = self;
#endif
}
