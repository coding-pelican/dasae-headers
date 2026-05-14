/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    src_loc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-02-09 (date of creation)
 * @updated 2026-02-09 (date of last update)
 * @ingroup dal-project/da-dsl/core/debug
 * @prefix  (none)
 *
 * @brief   Source location information utilities for debugging
 * @details Provides structures and functions for tracking source code locations for debugging.
 */
#ifndef da_dsl_core_debug_src_loc__included
#define da_dsl_core_debug_src_loc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../src_loc.h"

/*========== Macros and Declarations ========================================*/

typedef struct debug_SrcLoc debug_SrcLoc;
#define debug_srcLoc() __val__debug_srcLoc()

#define $traced __syn__debug_trace__param
#define $traced_(_ident...) __syn__debug_trace__paramNamed(_ident)
#define $trace __expr__debug_trace__arg
#define $tracing __ident__debug_trace__capt
#define $tracing_(_ident...) __ident__debug_trace__captNamed(_ident)

/*========== Macros and Definitions =========================================*/

struct debug_SrcLoc { /* clang-format off */
    var_(impl, pp_if_(debug_comp_enabled)(
        pp_then_(SrcLoc),
        pp_else_(Void)
    ));
};
#define __val__debug_srcLoc() l$((debug_SrcLoc){ \
    .impl = pp_if_(debug_comp_enabled)( \
        pp_then_(srcLoc()), \
        pp_else_({}) \
    ), \
}) /* clang-format on */

#define __syn__debug_trace__param \
    $maybe_unused debug_SrcLoc __traced,
#define __syn__debug_trace__paramNamed(_ident...) \
    $maybe_unused debug_SrcLoc pp_join(_, __traced, _ident),
#define __expr__debug_trace__arg \
    debug_srcLoc(),
#define __ident__debug_trace__capt \
    __traced,
#define __ident__debug_trace__captNamed(_ident...) \
    pp_join(_, __traced, _ident),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_core_debug_src_loc__included */
