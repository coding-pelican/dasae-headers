/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-02-06 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 *
 * @brief   Configuration settings for IO utilities
 * @details Defines compile-time options and configuration parameters
 *          for IO utilities.
 */
#pragma once
#ifndef io_cfg__included
#define io_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#if !defined(io_pre_ensured_utf8_env_enabled)
#define io_pre_ensured_utf8_env_enabled __comp_bool__io_pre_ensured_utf8_env_enabled
#endif /* !defined(io_pre_ensured_utf8_env_enabled) */
#define io_pre_ensured_utf8_env__enabled_default __comp_flag__io_pre_ensured_utf8_env__enabled_default
#define io_using_libc __comp_bool__io_using_libc

/*========== Macros and Definitions =========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#define __comp_bool__io_pre_ensured_utf8_env_enabled io_pre_ensured_utf8_env__enabled_default
#define __comp_flag__io_pre_ensured_utf8_env__enabled_default pp_true
#define __comp_bool__io_using_libc env_libc_linked
#if defined(IO_NO_PRE_ENSURED_UTF8_ENV)
#undef __comp_flag__io_pre_ensured_utf8_env__enabled_default
#define __comp_flag__io_pre_ensured_utf8_env__enabled_default pp_false
#endif /* defined(IO_NO_PRE_ENSURED_UTF8_ENV) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_cfg__included */
