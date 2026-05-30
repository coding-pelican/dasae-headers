#ifndef exec_Seq__included
#define exec_Seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Lane.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Seq)(struct exec_Seq {
    var_(lane, exec_Lane);
}));
$extern fn_((exec_Seq_init(mem_Alctr gpa))(exec_Seq));
$extern fn_((exec_Seq_fini(exec_Seq* self))(void));
$extern fn_((exec_Seq_awaitUntilDone(exec_Seq* self, exec_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Seq__included */
