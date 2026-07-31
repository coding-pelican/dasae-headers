#pragma once
#ifndef proc_Cmd__included
#define proc_Cmd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "std.h"
#include "../fs/Dir.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_ArgExpsn)(enum_((proc_ArgExpsn $fits($packed))(
    proc_ArgExpsn_no_expand = 0,
    proc_ArgExpsn_expand
))));

/// Explicit child-process environment override.
///
/// `none()` inherits the environment present at process entry; `some(entries)`
/// replaces it with the exact `KEY=VALUE` entries in this slice.
T_alias$((proc_Cmd_Env)(S$S_const$u8));
T_use_O$(proc_Cmd_Env);

T_alias$((proc_Cmd)(struct proc_Cmd {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_Cmd_Env);
    var_(cwd, O$fs_Dir);
    var_(std_in, proc_std_IO);
    var_(std_out, proc_std_IO);
    var_(std_err, proc_std_IO);
    var_(expand_arg0, proc_ArgExpsn);
    var_(start_suspended, bool);
    var_(create_no_window, bool);
}));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Cmd__included */
