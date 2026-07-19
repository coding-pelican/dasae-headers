/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    time.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2026-07-19 (date of last update)
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
#pragma once
#ifndef time__included
#define time__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "time/cfg.h"
#include "time/base.h"
#include "time/common.h"
#include "time/epoch.h"

#include "time/Dur.h"
#include "time/Inst.h"
#include "time/self.h"
#include "time/Clock.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time__included */
