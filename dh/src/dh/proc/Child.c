#include "dh/proc/Child.h"

$attr($maybe_unused)
$static fn_((proc_Child__unsupported_wait(proc_Child* self))(proc_Child_Wait_E$proc_Child_Ter) $scope) {
    claim_assert_nonnull(self);
    let_ignore = self;
    return_err(E_cause$proc_OperationUnsupported());
} $unscoped(fn);
$attr($maybe_unused)
$static fn_((proc_Child__unsupported_kill(proc_Child self))(void)) {
    let_ignore = self;
};

#if plat_is_windows
#include "dh/sys/api/windows/handle.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/sync.h"

$static fn_((proc_Child__windows_wait(proc_Child* self))(proc_Child_Wait_E$proc_Child_Ter) $scope) {
    claim_assert_nonnull(self);
    let handle = orelse_((self->handle)(return_err(E_cause$proc_ProcessAlreadyExited())));
    switch (WaitForSingleObject(as$(HANDLE)(handle), INFINITE)) {
    case WAIT_OBJECT_0: break;
    case WAIT_ABANDONED: $fallthrough;
    case WAIT_TIMEOUT: $fallthrough;
    case WAIT_FAILED: $fallthrough;
    default_() return_err(E_cause$proc_SystemResources()) $end(default);
    }
    var_(exit_code, DWORD) = 0;
    if (!GetExitCodeProcess(as$(HANDLE)(handle), &exit_code)) {
        return_err(E_cause$proc_SystemResources());
    }
    claim_assert(CloseHandle(as$(HANDLE)(handle)));
    asg_l((&self->handle)(none()));
    return_ok(union_of$((proc_Child_Ter)(proc_Child_Ter_Tag_exited)( as$(u8)(exit_code))));
} $unscoped(fn);
$static fn_((proc_Child__windows_kill(proc_Child self))(void)) {
    if_some((self.handle)(handle)) {
        let_ignore = TerminateProcess(as$(HANDLE)(handle), 1);
    };
};
#endif /* plat_is_windows */

#if plat_is_linux
#include "dh/sys/call/linux.h"

$static fn_((proc_Child__linux_wait(proc_Child* self))(proc_Child_Wait_E$proc_Child_Ter) $scope) {
    claim_assert_nonnull(self);
    if (self->id == 0) return_err(E_cause$proc_ProcessAlreadyExited());
    var_(status, int) = 0;
    let waited = sys_call_linux_wait4(as$(sys_call_linux_pid_t)(self->id), &status, 0, null);
    if (sys_call_linux_syscall_isErr(waited)) return_err(E_cause$proc_SystemResources());
    self->id = 0;
    if ((status & 0x7f) == 0) {
        return_ok(union_of$((proc_Child_Ter)(proc_Child_Ter_Tag_exited)( as$(u8)((status >> 8) & 0xff))));
    }
    if ((status & 0x7f) != 0x7f) {
        return_ok(union_of$((proc_Child_Ter)(proc_Child_Ter_Tag_signal)( as$(sys_posix_signal_t)(status & 0x7f))));
    }
    return_ok(union_of$((proc_Child_Ter)(proc_Child_Ter_Tag_stopped)( as$(sys_posix_signal_t)((status >> 8) & 0xff))));
} $unscoped(fn);
$static fn_((proc_Child__linux_kill(proc_Child self))(void)) {
    if (self.id != 0) {
        let_ignore = sys_call_linux_kill(as$(sys_call_linux_pid_t)(self.id), 9);
    }
};
#endif /* plat_is_linux */

$static let proc_Child__wait = pp_if_(plat_is_windows)(
    pp_then_(proc_Child__windows_wait),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Child__linux_wait),
        pp_else_(proc_Child__unsupported_wait)
    )));
$static let proc_Child__kill = pp_if_(plat_is_windows)(
    pp_then_(proc_Child__windows_kill),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(proc_Child__linux_kill),
        pp_else_(proc_Child__unsupported_kill)
    )));

fn_((proc_Child_wait(proc_Child* self))(proc_Child_Wait_E$proc_Child_Ter)) {
    claim_assert_nonnull(self);
    return proc_Child__wait(self);
};
fn_((proc_Child_kill(proc_Child self))(void)) {
    proc_Child__kill(self);
};
