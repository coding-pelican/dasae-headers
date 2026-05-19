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
T_use_E$(proc_Handle);
T_alias$((proc_Child)(struct proc_Child));
T_decl_E$(proc_Child);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
