#pragma once
#ifndef Future_base__included
#define Future_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((FutureAny)(Opaq));
T_use$((FutureAny)(P, (O, P)));

T_alias$((Future$raw)(struct Future$raw));
T_use$((Future$raw)(P, (O, P), u_V));
T_alias$((V$Future$raw)(P$Future$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Future_base__included */
