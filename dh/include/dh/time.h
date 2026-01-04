/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    time.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2026-01-03 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  time
 *
 * @brief   Time management and measurement utilities
 * @details Provides functionality for:
 *          - Time measurement and duration tracking
 *          - High-precision timestamps and intervals
 *          - Time formatting and conversion utilities
 *          - Platform-independent time operations
 */
#ifndef time__included
#define time__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "time/cfg.h"
#include "time/common.h"
#include "time/Duration.h"
#include "time/Instant.h"
#include "time/SysTime.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time__included */
