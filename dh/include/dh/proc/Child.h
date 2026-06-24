#pragma once
#ifndef proc_Child__included
#define proc_Child__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Child_Handle)(proc_Handle));
T_alias$((proc_Child_Id)(u64));
T_use_O$(proc_Child_Handle);
struct proc_Child {
    var_(handle, O$proc_Child_Handle);
    var_(id, proc_Child_Id);
    var_(std_in, O$fs_File);
    var_(std_out, O$fs_File);
    var_(std_err, O$fs_File);
};
T_use_prl$(proc_Child);

$extern fn_((proc_Child_wait(proc_Child* self))(E$proc_Ter));
$extern fn_((proc_Child_kill(proc_Child self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Child__included */
