#ifndef proc_Self__included
#define proc_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Child.h"

/*========== Macros and Declarations ========================================*/

#define proc_direct_supported __bool__proc_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__proc_direct_supported pp_true
#else
#define __bool__proc_direct_supported pp_false
#endif

T_alias$((proc_Self_VTbl)(struct proc_Self_VTbl));
$extern let_(proc_VTbl_noop, proc_Self_VTbl);
$extern let_(proc_VTbl_failing, proc_Self_VTbl);

struct proc_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Self_VTbl));
};
T_impl_E$($set(proc_direct_E)(proc_Self));
$extern let_(proc_noop, proc_Self);
$extern let_(proc_failing, proc_Self);

$attr($must_check)
$extern fn_((proc_executablePath(proc_Self self, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((proc_currentPath(proc_Self self, S$u8 out_buf))(E$S$u8));
$attr($must_check)
$extern fn_((proc_setCurrentPath(proc_Self self, S_const$u8 path))(E$void));
$attr($must_check)
$extern fn_((proc_spawn(proc_Self self, proc_Cmd cmd))(E$proc_Child));
$attr($must_check)
$extern fn_((proc_spawnPath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(E$proc_Child));
$attr($must_check)
$extern fn_((proc_replace(proc_Self self, proc_Cmd cmd))(E$void));
$attr($must_check)
$extern fn_((proc_replacePath(proc_Self self, fs_Dir dir, proc_Cmd cmd))(E$void));

$attr($must_check)
$extern fn_((proc_direct(void))(proc_direct_E$proc_Self));

struct proc_Self_VTbl {
    fn_(((*executablePathFn)(P$raw ctx, S$u8 out_buf))(E$S$u8));
    fn_(((*currentPathFn)(P$raw ctx, S$u8 out_buf))(E$S$u8));
    fn_(((*setCurrentPathFn)(P$raw ctx, S_const$u8 path))(E$void));
    fn_(((*spawnFn)(P$raw ctx, proc_Cmd cmd))(E$proc_Child));
    fn_(((*spawnPathFn)(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(E$proc_Child));
    fn_(((*replaceFn)(P$raw ctx, proc_Cmd cmd))(E$void));
    fn_(((*replacePathFn)(P$raw ctx, fs_Dir dir, proc_Cmd cmd))(E$void));
    fn_(((*waitFn)(P$raw ctx, proc_Child* child))(E$proc_Term));
    fn_(((*killFn)(P$raw ctx, proc_Child* child))(void));
};

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Self__included */
