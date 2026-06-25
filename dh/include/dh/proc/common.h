#pragma once
#ifndef proc_common__included
#define proc_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "dh/fs.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_E)(
    proc_AccessDenied,
    proc_FileNotFound,
    proc_InvalidExe,
    proc_InvalidName,
    proc_NotDir,
    proc_OperationUnsupported,
    proc_PermissionDenied,
    proc_ProcessAlreadyExited,
    proc_ResourceLimitReached,
    proc_SystemResources
));

T_alias$((proc_ArgExpansion)(enum_((proc_ArgExpansion $fits($packed))(
    proc_ArgExpansion_no_expand = 0,
    proc_ArgExpansion_expand
))));

T_alias$((proc_StdIO_Tag)(enum_((proc_StdIO_Tag $fits($packed))(
    proc_StdIO_Tag_inherit = 0,
    proc_StdIO_Tag_file,
    proc_StdIO_Tag_ignore,
    proc_StdIO_Tag_pipe,
    proc_StdIO_Tag_close
))));
T_alias$((proc_StdIO)(struct proc_StdIO {
    var_(tag, proc_StdIO_Tag);
    var_(file, fs_File);
}));

T_alias$((proc_Ter_Tag)(enum_((proc_Ter_Tag $fits($packed))(
    proc_Ter_Tag_exited = 0,
    proc_Ter_Tag_signal,
    proc_Ter_Tag_stopped,
    proc_Ter_Tag_unknown
))));
T_alias$((proc_Ter)(struct proc_Ter {
    var_(tag, proc_Ter_Tag);
    var_(code, u32);
}));
T_use_E$(proc_Ter);

T_alias$((proc_Env)(S$S_const$u8));
T_use_O$(proc_Env);
T_use_O$(fs_Dir);

T_alias$((proc_Cmd)(struct proc_Cmd {
    var_(argv, S$S_const$u8);
    var_(env, O$proc_Env);
    var_(cwd, O$fs_Dir);
    var_(std_in, proc_StdIO);
    var_(std_out, proc_StdIO);
    var_(std_err, proc_StdIO);
    var_(expand_arg0, proc_ArgExpansion);
    var_(start_suspended, bool);
    var_(create_no_window, bool);
}));

$static let_(proc_StdIO_inherit, proc_StdIO) = {
    .tag = proc_StdIO_Tag_inherit,
    .file = cleared(),
};
$static let_(proc_StdIO_ignore, proc_StdIO) = {
    .tag = proc_StdIO_Tag_ignore,
    .file = cleared(),
};
$static let_(proc_StdIO_pipe, proc_StdIO) = {
    .tag = proc_StdIO_Tag_pipe,
    .file = cleared(),
};
$static let_(proc_StdIO_close, proc_StdIO) = {
    .tag = proc_StdIO_Tag_close,
    .file = cleared(),
};
$attr($inline_always)
$static fn_((proc_StdIO_file(fs_File file))(proc_StdIO));

$attr($must_check)
$extern fn_((proc_executablePath(S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((proc_currentPath(S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((proc_setCurrentPath(S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((proc_spawn(proc_Cmd cmd))(E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(fs_Dir dir, proc_Cmd cmd))(E$proc_Child));
$attr($must_check)
$extern fn_((proc_replace(proc_Cmd cmd))(E$void));
$attr($must_check)
$extern fn_((proc_replacePath(fs_Dir dir, proc_Cmd cmd))(E$void));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((proc_StdIO_file(fs_File file))(proc_StdIO)) {
    return (proc_StdIO){
        .tag = proc_StdIO_Tag_file,
        .file = file,
    };
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_common__included */
