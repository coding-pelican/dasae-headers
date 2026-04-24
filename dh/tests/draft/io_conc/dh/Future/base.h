#ifndef Future_base__included
#define Future_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl.h"

/*========== Macros and Declarations ========================================*/

T_alias$((FutureAny)(Opaq));
T_use_P$(FutureAny);
T_use_O$(P$FutureAny);

T_alias$((Future$raw)(struct Future$raw));
T_use_P$(Future$raw);
T_use_O$(P$Future$raw);
T_alias$((V$Future$raw)(P$Future$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Future_base__included */
