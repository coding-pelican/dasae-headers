#ifndef proc_base__included
#define proc_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Handle)(posix_fd_t));

T_alias$((proc_Child)(struct proc_Child));
T_decl_E$(proc_Child);

errset_((proc_direct_E)(Unsupported));
T_alias$((proc_Self)(struct proc_Self));
T_decl_E$($set(proc_direct_E)(proc_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
