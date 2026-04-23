#ifndef exec_Fiber__included
#define exec_Fiber__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl/Co/Fiber.h"
#include "dh/mem/Alctr.h"

/*========== Macros and Declarations ========================================*/

#define exec_Fiber_stk_size (usize_(1) * 1024 * 1024)
T_alias$((exec_Fiber)(struct exec_Fiber {
    var_(stk, S$u8);
    var_(ctx, Co_Fiber_Context);
}));
T_use_P$(exec_Fiber);
T_use_O$(P$exec_Fiber);
T_use_E$($set(mem_E)(P$exec_Fiber));
T_alias$((exec_Fiber_WorkFn)(fn_(((*)(P$raw owner, P$raw task))(void) $T)));
$attr($must_check)
$extern fn_((exec_Fiber_init(mem_Alctr gpa, P$raw owner, P$raw task, exec_Fiber_WorkFn workFn))(mem_E$P$exec_Fiber));
$extern fn_((exec_Fiber_fini(exec_Fiber* self, mem_Alctr gpa))(void));

T_alias$((exec_Fiber_Starter)(struct exec_Fiber_Starter {
    var_(owner, P$raw);
    var_(task, P$raw);
    var_(workFn, exec_Fiber_WorkFn);
}));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Fiber__included */
