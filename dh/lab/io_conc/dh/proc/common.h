#pragma once
#ifndef proc_common__included
#define proc_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../fs.h"

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
    proc_SystemResources,
));

T_alias$((proc_ArgExpsn)(enum_((proc_ArgExpsn $fits($packed))(
    proc_ArgExpsn_no_expand = 0,
    proc_ArgExpsn_expand,
))));

T_alias$((proc_std_IO_Tag)(enum_((proc_std_IO_Tag $fits($packed))(
    proc_std_IO_Tag_inherit = 0,
    proc_std_IO_Tag_file,
    proc_std_IO_Tag_ignore,
    proc_std_IO_Tag_pipe,
    proc_std_IO_Tag_close,
))));
T_alias$((proc_std_IO)(struct proc_std_IO {
    var_(tag, proc_std_IO_Tag);
    var_(file, fs_File);
}));

T_alias$((proc_Child_Ter_Tag)(enum_((proc_Child_Ter_Tag $fits($packed))(
    proc_Child_Ter_Tag_exited = 0,
    proc_Child_Ter_Tag_signal,
    proc_Child_Ter_Tag_stopped,
    proc_Child_Ter_Tag_unknown,
))));
T_alias$((proc_Child_Ter)(struct proc_Child_Ter {
    var_(tag, proc_Child_Ter_Tag);
    var_(code, u32);
}));
T_use_E$(proc_Child_Ter);

T_alias$((proc_Cmd)(struct proc_Cmd {
    var_(argv, S$S_const$u8);
    var_(env, S$S_const$u8);
    var_(cwd, fs_Dir*);
    var_(std_in, proc_std_IO);
    var_(std_out, proc_std_IO);
    var_(std_err, proc_std_IO);
    var_(expand_arg0, proc_ArgExpsn);
    var_(start_suspended, bool);
    var_(create_no_window, bool);
}));

$static let_(proc_std_IO_inherit, proc_std_IO) = {
    .tag = proc_std_IO_Tag_inherit,
    .file = cleared(),
};
$static let_(proc_std_IO_ignore, proc_std_IO) = {
    .tag = proc_std_IO_Tag_ignore,
    .file = cleared(),
};
$static let_(proc_std_IO_pipe, proc_std_IO) = {
    .tag = proc_std_IO_Tag_pipe,
    .file = cleared(),
};
$static let_(proc_std_IO_close, proc_std_IO) = {
    .tag = proc_std_IO_Tag_close,
    .file = cleared(),
};
$attr($inline_always)
$static fn_((proc_std_IO_file(fs_File file))(proc_std_IO));

fn_((proc_std_IO_file(fs_File file))(proc_std_IO)) {
    return (proc_std_IO){
        .tag = proc_std_IO_Tag_file,
        .file = file,
    };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_common__included */
