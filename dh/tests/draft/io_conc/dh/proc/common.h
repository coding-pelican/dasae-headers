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
    AccessDenied,
    FileNotFound,
    InvalidExe,
    InvalidName,
    NotDir,
    OperationUnsupported,
    PermissionDenied,
    ProcessAlreadyExited,
    ResourceLimitReached,
    SystemResources,
    Unsupported
));

T_alias$((proc_ArgExpansion)(enum_((proc_ArgExpansion $fits($packed))(
    proc_ArgExpansion_no_expand = 0,
    proc_ArgExpansion_expand,
))));

T_alias$((proc_StdIO_Tag)(enum_((proc_StdIO_Tag $fits($packed))(
    proc_StdIO_Tag_inherit = 0,
    proc_StdIO_Tag_file,
    proc_StdIO_Tag_ignore,
    proc_StdIO_Tag_pipe,
    proc_StdIO_Tag_close,
))));
T_alias$((proc_StdIO)(struct proc_StdIO {
    var_(tag, proc_StdIO_Tag);
    var_(file, fs_File);
}));

T_alias$((proc_Term_Tag)(enum_((proc_Term_Tag $fits($packed))(
    proc_Term_Tag_exited = 0,
    proc_Term_Tag_signal,
    proc_Term_Tag_stopped,
    proc_Term_Tag_unknown,
))));
T_alias$((proc_Term)(struct proc_Term {
    var_(tag, proc_Term_Tag);
    var_(code, u32);
}));
T_use_E$(proc_Term);

T_alias$((proc_Cmd)(struct proc_Cmd {
    var_(argv, S$S_const$u8);
    var_(env, S$S_const$u8);
    var_(cwd, fs_Dir*);
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

fn_((proc_StdIO_file(fs_File file))(proc_StdIO)) {
    return (proc_StdIO){
        .tag = proc_StdIO_Tag_file,
        .file = file,
    };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_common__included */
