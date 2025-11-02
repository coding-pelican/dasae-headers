/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
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

/*========== Macros and Declarations ========================================*/

#define debug__comp_enabled_default __comp_flag__debug__default_enabled
#define debug_comp_enabled          __comp_bool__debug_comp_enabled

/*========== Macros and Definitions =========================================*/

/* Default values */

#define __comp_flag__debug__default_enabled 1
#define __comp_bool__debug_comp_enabled     debug__comp_enabled_default

/* Override values */

#if defined(NDEBUG)
#undef __comp_flag__debug__default_enabled
#define __comp_flag__debug__default_enabled 0
#endif /* !defined(NDEBUG) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_cfg__included */
