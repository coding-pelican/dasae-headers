#pragma once
#ifndef proc_base__included
#define proc_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Handle)(usize));
T_use_prl$(proc_Handle);

errset_((proc_direct_E)(proc_direct_Unsupported));

T_alias$((proc_Self)(struct proc_Self));
T_decl_E$($set(proc_direct_E)(proc_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
