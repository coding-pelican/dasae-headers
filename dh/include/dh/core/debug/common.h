/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug
 *
 * @brief   Common debug utilities and macros
 * @details Provides common debug utilities and macros for:
 *          - Debug breakpoints
 *          - Debug-only code execution
 *          - Debugger detection
 *          - Debug configuration
 */
#ifndef core_debug_common__included
#define core_debug_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../fn.h"

/*========== Macros and Declarations ========================================*/

/// Function to detect if debugger is attached
$extern fn_((debug_isDebuggerPresent(void))(bool));

#define debug_break() \
    /* Breakpoint. trigger debug break if debugger is attached. */ \
    __comp_inline__debug_break()
#define $debug_point \
    /* breakpoint attribute. trigger debug break if debugger is attached. */ \
    __comp_attr__$debug_point

/*========== Macros and Definitions =========================================*/

#if !debug_enabled
#define __comp_inline__debug_break() $unused(0)
#else /* debug_enabled */

#define __comp_inline__debug_break() ({ \
    if (debug_isDebuggerPresent()) { ____debug_break__impl(); } \
})

#if comp_type == comp_type_clang
#define ____debug_break__impl() __builtin_debugtrap()
#elif comp_type == comp_type_msvc
#define ____debug_break__impl() __debugbreak()
#else /* others */

#if arch_family_type == arch_family_type_x86
#define ____debug_break__impl() asm_volatile("int $0x03")
#elif arch_type == arch_type_aarch64
#define ____debug_break__impl() asm_volatile("brk #0")
#elif arch_type == arch_type_arm
#define ____debug_break__impl() asm_volatile("bkpt #0")
#elif arch_family_type == arch_family_type_riscv
#define ____debug_break__impl() asm_volatile("ebreak")
#elif arch_family_type == arch_family_type_wasm
#include <emscripten.h>
#define ____debug_break__impl() emscripten_debugger()
#else
#include <signal.h>
#define ____debug_break__impl() raise(SIGTRAP)
#endif

#endif

#endif /* debug_enabled */

#define __comp_attr__$debug_point debug_break(),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_common__included */
