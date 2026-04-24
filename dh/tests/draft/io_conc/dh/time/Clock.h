#ifndef time_Clock__included
#define time_Clock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((time_Clock)(variant_((time_Clock $fits($packed))(
    (time_Clock_real, time_Real),
    (time_Clock_awake, time_Awake),
    (time_Clock_boot, time_Boot),
    (time_Clock_proc, time_Proc),
    (time_Clock_thrd, time_Thrd)
))));

T_alias$((time_Clock_Inst)(variant_((time_Clock_Inst $fits($packed))(
    (time_Clock_Inst_real, time_Real_Inst),
    (time_Clock_Inst_awake, time_Awake_Inst),
    (time_Clock_Inst_boot, time_Boot_Inst),
    (time_Clock_Inst_proc, time_Proc_Inst),
    (time_Clock_Inst_thrd, time_Thrd_Inst)
))));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Clock__included */
