#ifndef time_base__included
#define time_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

/* --- Time Self Type --- */
typedef struct time_Self time_Self;

/* --- Data Types --- */
typedef struct time_Dur time_Dur;
T_decl_O$(time_Dur);
typedef struct time_Inst time_Inst;
T_decl_O$(time_Inst);
typedef struct time_Clock time_Clock;
T_decl_O$(time_Clock);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_base__included */
