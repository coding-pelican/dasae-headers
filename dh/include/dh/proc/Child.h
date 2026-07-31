#pragma once
#ifndef proc_Child__included
#define proc_Child__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../fs/File/self.h"
#include "../sys/posix.h"

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

T_alias$((proc_Child_Ter)(variant_((proc_Child_Ter $fits($packed))(
    (proc_Child_Ter_Tag_exited, u8),
    (proc_Child_Ter_Tag_signal, sys_posix_signal_t),
    (proc_Child_Ter_Tag_stopped, sys_posix_signal_t),
    (proc_Child_Ter_Tag_unknown, u32)
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

$extern fn_((proc_Child_wait(proc_Child* self))(proc_Child_Wait_E$proc_Child_Ter));
$extern fn_((proc_Child_kill(proc_Child self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Child__included */
