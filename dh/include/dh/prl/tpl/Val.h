#pragma once
#ifndef prl_tpl_Val__included
#define prl_tpl_Val__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
/*========== Includes =======================================================*/

#include "dh/core.h"

/*========== Macros and Declarations ========================================*/

#define V_ref(_v /*: T*/... /*P$$(_T)*/) ref(_v)
#define refV V_ref

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_tpl_Val__included */
