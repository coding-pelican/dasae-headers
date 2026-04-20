#ifndef exec_Seq__included
#define exec_Seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Seq)(struct exec_Seq{}));
/* sequential scheduling */
$extern fn_((exec_Seq_sched(exec_Seq* self))(Sched));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Seq__included */
