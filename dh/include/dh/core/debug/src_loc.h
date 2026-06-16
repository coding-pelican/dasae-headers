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
#pragma once
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

#define $traced __syn__param__$traced
#define $traced_(_ident...) __syn__paramNamed__$traced_(_ident)
#define $trace __syn__arg__$trace
#define $tracing __ident__capt__$tracing
#define $tracing_(_ident...) __ident__captNamed__$tracing_(_ident)

/*========== Macros and Definitions =========================================*/

struct debug_SrcLoc { /* clang-format off */
    var_(impl, pp_if_(debug_enabled)(
        pp_then_(SrcLoc),
        pp_else_(Void)
    ));
};
#define __val__debug_srcLoc() l$((debug_SrcLoc){ \
    .impl = pp_if_(debug_enabled)( \
        pp_then_(srcLoc()), \
        pp_else_({}) \
    ), \
}) /* clang-format on */

#define __syn__param__$traced \
    $maybe_unused debug_SrcLoc __traced,
#define __syn__paramNamed__$traced_(_ident...) \
    $maybe_unused debug_SrcLoc pp_join(_, __traced, _ident),
#define __syn__arg__$trace \
    debug_srcLoc(),
#define __ident__capt__$tracing \
    __traced,
#define __ident__captNamed__$tracing_(_ident...) \
    pp_join(_, __traced, _ident),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* da_dsl_core_debug_src_loc__included */
