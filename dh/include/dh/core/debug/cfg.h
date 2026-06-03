/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug
 *
 * @brief   Configuration synthesis for debug system
 * @details Combines default and custom configurations for debug system
 */
#ifndef core_debug_cfg__included
#define core_debug_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#if !defined(debug_enabled)
#define debug_enabled __comp_bool__debug_enabled
#endif /* !defined(debug_enabled) */
#define debug__enabled_default __comp_flag__debug__enabled_default

#define debug_only(_inner...) \
    /* Used only when `debug_enabled`. */ \
    __comp_syn__debug_only(_inner)

/*========== Macros and Definitions =========================================*/

/* Default values */

#define __comp_bool__debug_enabled debug__enabled_default
#define __comp_flag__debug__enabled_default pp_true

/* Override values */

#if defined(NDEBUG)
#undef __comp_flag__debug__enabled_default
#define __comp_flag__debug__enabled_default pp_false
#endif /* !defined(NDEBUG) */

#define __comp_syn__debug_only(_inner...) pp_if_(debug_enabled)(pp_then_(_inner))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_cfg__included */
