#pragma once
#ifndef proc_std__included
#define proc_std__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

/// Return borrowed handles for the current process standard streams.
$extern fn_((proc_std_in(void))(fs_File));
$extern fn_((proc_std_out(void))(fs_File));
$extern fn_((proc_std_err(void))(fs_File));

/// Standard-stream disposition for a spawned child process.
T_alias$((proc_std_IO)(variant_((proc_std_IO $fits($packed))(
    (proc_std_IO_inherit, Void),
    (proc_std_IO_file, fs_File),
    (proc_std_IO_ignore, Void),
    (proc_std_IO_pipe, Void),
    (proc_std_IO_close, Void)
))));
$attr($inline_always)
$static fn_((proc_std_IO_fromFile(fs_File file))(proc_std_IO));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_std_IO_fromFile(fs_File file))(proc_std_IO)) {
    return union_of$((proc_std_IO)(proc_std_IO_file)(file));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_std__included */
