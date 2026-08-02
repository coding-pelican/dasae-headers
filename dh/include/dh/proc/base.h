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

T_alias$((proc_Self)(struct proc_Self));
T_alias$((proc_Child)(struct proc_Child));
T_alias$((proc_Args)(struct proc_Args));
T_alias$((proc_Env)(struct proc_Env));
T_alias$((proc_Preopens)(struct proc_Preopens));
T_alias$((proc_Mem)(struct proc_Mem));
T_alias$((proc_Entry)(struct proc_Entry));

/*--- Cross-cutting process errors -----------------------------------------*/

/* These singleton sets are retained only because they are genuinely shared
 * building blocks of multiple independently precise process contracts. */
errset_((proc_AccessDenied_E)(proc_AccessDenied));
errset_((proc_ResourceLimitReached_E)(proc_ResourceLimitReached));
errset_((proc_SystemResources_E)(proc_SystemResources));

/*--- Direct capability ----------------------------------------------------*/

errset_((proc_direct_E)(proc_direct_Unsupported));
T_decl_E$($set(proc_direct_E)(proc_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
