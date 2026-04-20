#ifndef exec_Para__included
#define exec_Para__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Para)(struct exec_Para{}));
/* parallel scheduling */
$extern fn_((exec_Para_sched(exec_Para* self))(Sched));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Para__included */
