#ifndef exec_Preem__included
#define exec_Preem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Thrd_SpawnCfg)(Void));
T_alias$((exec_Preem)(struct exec_Preem {
    var_(gpa, mem_Alctr);
    var_(spawn_cfg, Thrd_SpawnCfg);
}));
$extern fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem));
$extern fn_((exec_Preem_fini(exec_Preem* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Preem__included */
