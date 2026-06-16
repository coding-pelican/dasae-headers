/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2026-06-06 (date of last update)
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
#pragma once
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

#define __comp_inline__debug_break() pp_if_(debug_break_enabled)( \
    pp_then_(local_({ \
        local_return_(if (debug_isDebuggerPresent()) ____debug_break__impl()); \
    })), \
    pp_else_($unused(0)) \
)

#define __comp_attr__$debug_point debug_break(),

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
#endif /* arch_family_type == arch_family_type_x86 \
|| arch_type == arch_type_aarch64 \
|| arch_type == arch_type_arm \
|| arch_family_type == arch_family_type_riscv \
|| arch_family_type == arch_family_type_wasm \
|| others */
#endif /* comp_type == comp_type_clang \
|| comp_type == comp_type_msvc \
|| others */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_common__included */
