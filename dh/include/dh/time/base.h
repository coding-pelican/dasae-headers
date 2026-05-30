#ifndef time_base__included
#define time_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

/* --- Data Types --- */
T_alias$((time_Dur)(struct time_Dur));
T_decl_O$(time_Dur);
T_alias$((time_Inst)(struct time_Inst));
T_decl_O$(time_Inst);
T_alias$((time_SysTime)(struct time_SysTime));
T_decl_O$(time_SysTime);

/* --- Error Types --- */
errset_((time_E)(time_Unsupported));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_base__included */
