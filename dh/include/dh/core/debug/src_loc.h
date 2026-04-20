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

#define $traced __attr__$traced
#define $traced_(_ident...) __attr__$traced_(_ident)
#define $trace __attr__$trace
#define $tracing __val__$tracing
#define $tracing_(_ident...) __val__$tracing_(_ident)

/*========== Macros and Definitions =========================================*/

struct debug_SrcLoc { /* clang-format off */
    var_(base, pp_if_(debug_comp_enabled)(
        pp_then_(SrcLoc),
        pp_else_(Void)
    ));
};
#define __val__debug_srcLoc() l$((debug_SrcLoc){ \
    .base = pp_if_(debug_comp_enabled)( \
        pp_then_(srcLoc()), \
        pp_else_({}) \
    ), \
}) /* clang-format on */

#define __attr__$traced \
    $maybe_unused debug_SrcLoc __traced,
#define __attr__$traced_(_ident...) \
    $maybe_unused debug_SrcLoc pp_join(_, __traced, _ident),
#define __attr__$trace \
    debug_srcLoc(),
#define __val__$tracing \
    __traced,
#define __val__$tracing_(_ident...) \
    pp_join(_, __traced, _ident),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_core_debug_src_loc__included */
