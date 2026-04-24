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

/* --- Error Types --- */
errset_((time_E)(Unsupported));

/* --- Self Types --- */
T_alias$((time_Real)(struct time_Real));
T_alias$((time_Real_Inst)(struct time_Real_Inst));
T_decl_O$(time_Real_Inst);
T_decl_E$($set(time_E)(time_Real));

T_alias$((time_Awake)(struct time_Awake));
T_alias$((time_Awake_Inst)(struct time_Awake_Inst));
T_decl_O$(time_Awake_Inst);
T_decl_E$($set(time_E)(time_Awake));

T_alias$((time_Boot)(struct time_Boot));
T_alias$((time_Boot_Inst)(struct time_Boot_Inst));
T_decl_O$(time_Boot_Inst);
T_decl_E$($set(time_E)(time_Boot));

T_alias$((time_Proc)(struct time_Proc));
T_alias$((time_Proc_Inst)(struct time_Proc_Inst));
T_decl_O$(time_Proc_Inst);
T_decl_E$($set(time_E)(time_Proc));

T_alias$((time_Thrd)(struct time_Thrd));
T_alias$((time_Thrd_Inst)(struct time_Thrd_Inst));
T_decl_O$(time_Thrd_Inst);
T_decl_E$($set(time_E)(time_Thrd));

/* --- Clock Types --- */
T_alias$((time_Clock)(struct time_Clock));
T_decl_O$(time_Clock);
T_alias$((time_Clock_Inst)(struct time_Clock_Inst));
T_decl_O$(time_Clock_Inst);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_base__included */
