/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    scope.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-14 (date of creation)
 * @updated 2025-01-13 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Scope-based resource management utilities
 * @details Provides macros and utilities for automatic resource cleanup,
 *          including defer statements and scope guards that ensure resources
 *          are properly released when execution leaves the current scope.
 */

#ifndef core_scope__included
#define core_scope__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "scope/common.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_scope__included */
