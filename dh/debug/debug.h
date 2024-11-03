#ifndef DEBUG_INCLUDED
#define DEBUG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "debug_cfg.h"

/*========== Macros and Definitions =========================================*/

#define debug_break() \
    /* Breakpoint. */ \
    RETURN_debug_break()
#define debug_only(...)                   \
    /* Used only when `DEBUG_ENABLED`. */ \
    RETURN_debug_only(__VA_ARGS__)

/*========== Macros Implementation ==========================================*/

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED

#if defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#define RETURN_debug_break() __builtin_trap()
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define RETURN_debug_break() __debugbreak()
#else
/* Fallback using signal */
#include <signal.h>
#define RETURN_debug_break() raise(SIGTRAP)
#endif
#define RETURN_debug_only(...) __VA_ARGS__

#else

#define RETURN_debug_break()
#define RETURN_debug_only(...)

#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_INCLUDED */
