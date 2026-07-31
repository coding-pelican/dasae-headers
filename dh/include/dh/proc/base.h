#pragma once
#ifndef proc_base__included
#define proc_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_Handle)(usize));
T_use_prl$(proc_Handle);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_base__included */
