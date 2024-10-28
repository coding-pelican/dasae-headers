#ifndef DEBUG_INCLUDED
#define DEBUG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "debug_cfg.h"


/* MACROS DECLARATION */
#define debug_break() \
    /* Breakpoint. */ \
    RETURN_debug_break()
#define debug_only(...)                   \
    /* Used only when `DEBUG_ENABLED`. */ \
    RETURN_debug_only(__VA_ARGS__)


/* MACROS IMPLEMENTATION */
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED

#define RETURN_debug_only(...) __VA_ARGS__
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
#endif /* defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)  */

#else

#define RETURN_debug_only(...)
#define RETURN_debug_break()

#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_INCLUDED */
