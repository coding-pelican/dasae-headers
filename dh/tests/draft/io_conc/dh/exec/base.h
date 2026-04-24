#ifndef exec_base__included
#define exec_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Fiber.h"
#include "Task.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Lane)(struct exec_Lane));
T_alias$((exec_Timer)(struct exec_Timer));
T_alias$((exec_LaneTimed)(struct exec_LaneTimed));
T_alias$((exec_Seq)(struct exec_Seq));
T_alias$((exec_Coop)(struct exec_Coop));
T_alias$((exec_Preem)(struct exec_Preem));
T_alias$((exec_Para)(struct exec_Para));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_base__included */
