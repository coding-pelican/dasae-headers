#ifndef exec_Timer__included
#define exec_Timer__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../time/Inst.h"
#include "dh/ArrPQue.h"

/*========== Macros and Declarations ========================================*/

struct exec_Timer {
    var_(deadline, time_Inst);
    var_(task, P$exec_Task);
};
T_use_prl$(exec_Timer);
T_use_ArrPQue$(exec_Timer);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Timer__included */
