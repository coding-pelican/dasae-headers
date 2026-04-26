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
T_alias$((proc_Child_Pipe)(struct proc_Child_Pipe {
    var_(is_present, bool);
    var_(file, fs_File);
}));
struct proc_Child {
    var_(handle, proc_Child_Handle);
    var_(id, proc_Child_Id);
    var_(std_in, proc_Child_Pipe);
    var_(std_out, proc_Child_Pipe);
    var_(std_err, proc_Child_Pipe);
};
T_use_prl$(proc_Child);

$extern fn_((proc_Child_wait(proc_Child* self, proc_Self proc))(E$proc_Term));
$extern fn_((proc_Child_kill(proc_Child* self, proc_Self proc))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Child__included */
