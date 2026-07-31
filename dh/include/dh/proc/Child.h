#pragma once
#ifndef proc_Child__included
#define proc_Child__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Child_Handle)(proc_Handle));
T_use_O$(proc_Child_Handle);

T_alias$((proc_Child_Id)(u64));

/// Process-domain signal number reported by a child termination.
///
/// Native providers normalize their platform representation to this public
/// value; no `sys` module type crosses the process boundary.
T_alias$((proc_Child_Sig)(u8));

T_alias$((proc_Child_IO)(struct proc_Child_IO {
    var_(in, O$fs_File);
    var_(out, O$fs_File);
    var_(err, O$fs_File);
}));
T_use_prl$(proc_Child_IO);

T_alias$((proc_Child_Ter)(variant_((proc_Child_Ter $fits($packed))(
    (proc_Child_Ter_exited, u8),
    (proc_Child_Ter_signal, proc_Child_Sig),
    (proc_Child_Ter_stopped, proc_Child_Sig),
    (proc_Child_Ter_unknown, u32)
))));
T_use_E$(proc_Child_Ter);

T_alias$((proc_Child)(struct proc_Child {
    var_(handle, O$proc_Child_Handle);
    var_(id, proc_Child_Id);
    var_(io, proc_Child_IO);
}));
T_use_prl$(proc_Child);
T_use_E$($set(proc_Spawn_E)(proc_Child));
T_use_E$($set(proc_Child_Wait_E)(proc_Child_Ter));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Child__included */
