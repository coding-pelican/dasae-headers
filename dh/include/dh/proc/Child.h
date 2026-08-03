#pragma once
#ifndef proc_Child__included
#define proc_Child__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Child_Handle)(proc_Handle));
T_use_O$(proc_Child_Handle);
T_alias$((proc_Child_Id)(u64));
T_alias$((proc_Child_IO)(struct proc_Child_IO {
    var_(in, O$fs_File);
    var_(out, O$fs_File);
    var_(err, O$fs_File);
}));
T_use_prl$(proc_Child_IO);

struct proc_Child {
    /// Owned process handle. `none` means that wait or kill has completed.
    var_(handle, O$proc_Child_Handle);
    var_(id, proc_Child_Id);
    /// Owned pipe endpoints. Wait and kill close and clear every endpoint.
    var_(io, proc_Child_IO);
};
T_use_prl$(proc_Child);

T_alias$((proc_Child_Sig)(u8));
T_alias$((proc_Child_Trm)(variant_((proc_Child_Trm $fits($packed))(
    (proc_Child_Trm_exited, u8),
    (proc_Child_Trm_signal, proc_Child_Sig),
    (proc_Child_Trm_stopped, proc_Child_Sig),
    (proc_Child_Trm_unknown, u32)
))));
T_use_E$(proc_Child_Trm);
errset_((proc_Child_Wait_E)() $union_errset_(
    proc_AccessDeniedE,
    proc_SysResrcsE
));
T_use_E$($set(proc_Child_Wait_E)(proc_Child_Trm));
/// Wait for an active child. Calling this after completion is a contract error.
$attr($must_check)
$extern fn_((proc_Child_wait(proc_Child* self, proc_Self proc))(proc_Child_Wait_E$proc_Child_Trm));
/// Kill an active child and wait for cleanup. Idempotent after completion.
$extern fn_((proc_Child_kill(proc_Child* self, proc_Self proc))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Child__included */
