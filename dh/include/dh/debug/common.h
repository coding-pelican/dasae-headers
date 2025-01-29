#ifndef DEBUG_COMMON_INCLUDED
#define DEBUG_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define debug_break() \
    /* Breakpoint. */ \
    IMPL_debug_break()
#define debug_only(...)                        \
    /* Used only when `debug_comp_enabled`. */ \
    IMPL_debug_only(__VA_ARGS__)

/*========== Macros Implementation ==========================================*/

#if defined(debug_comp_enabled) && debug_comp_enabled

#if defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#define IMPL_debug_break() __builtin_trap()
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define IMPL_debug_break() __debugbreak()
#else
/* Fallback using signal */
#include <signal.h>
#define IMPL_debug_break() raise(SIGTRAP)
#endif
#define IMPL_debug_only(...) __VA_ARGS__

#else

#define IMPL_debug_break()   unused(0)
#define IMPL_debug_only(...) unused(0)

#endif /* defined(debug_comp_enabled) && debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_COMMON_INCLUDED */
