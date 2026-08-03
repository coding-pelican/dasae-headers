#pragma once
#ifndef proc_base__included
#define proc_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

/*--- Principal process types ----------------------------------------------*/

T_alias$((proc_Handle)(usize));
T_use_prl$(proc_Handle);

T_alias$((proc_Args)(struct proc_Args));
T_alias$((proc_Env)(struct proc_Env));
T_alias$((proc_Preopens)(struct proc_Preopens));
T_alias$((proc_Entry)(struct proc_Entry));

T_alias$((proc_Child)(struct proc_Child));
T_alias$((proc_Self)(struct proc_Self));
T_alias$((proc_Mem)(struct proc_Mem));

/*--- Cross-cutting process errors -----------------------------------------*/

/* These singleton sets are retained only because they are genuinely shared
 * building blocks of multiple independently precise process contracts. */
errset_((proc_AccessDeniedE)(proc_AccessDenied));
errset_((proc_ResrcLimitReachedE)(proc_ResrcLimitReached));
errset_((proc_SysResrcsE)(proc_SysResrcs));

/*--- Direct capability ----------------------------------------------------*/

errset_((proc_direct_E)(proc_direct_Unsupported));
T_decl_E$($set(proc_direct_E)(proc_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
