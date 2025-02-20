#ifndef DEBUG_COMMON_INCLUDED
#define DEBUG_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define debug_point            \
    /* breakpoint attribute */ \
    ATTR__debug_point
#define debug_break() \
    /* Breakpoint. */ \
    FUNC__debug_break()
#define debug_only(...)                        \
    /* Used only when `debug_comp_enabled`. */ \
    SYN__debug_only(__VA_ARGS__)

/*========== Macros Implementation ==========================================*/

#define ATTR__debug_point debug_break(),

#if !debug_comp_enabled

#define FUNC__debug_break()  unused(0)
#define SYN__debug_only(...) unused(0)

#else /* debug_comp_enabled */

#if defined(__GNUC__) || defined(__clang__)
/* GCC or Clang */
#define FUNC__debug_break() __builtin_debugtrap()
#elif defined(_MSC_VER)
/* Microsoft Visual Studio */
#define FUNC__debug_break() __debugbreak()
#else
/* Fallback using signal */
#include <signal.h>
#define FUNC__debug_break() raise(SIGTRAP)
#endif
#define SYN__debug_only(...) __VA_ARGS__

#endif /* debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_COMMON_INCLUDED */
