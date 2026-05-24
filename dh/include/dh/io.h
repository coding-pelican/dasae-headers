
/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    io.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2026-05-24 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  io
 *
 * @brief   Input/output utilities
 * @details Provides utilities for input/output operations, including file handling,
 *          string formatting, and console input/output.
 */
#ifndef io__included
#define io__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "io/cfg.h"
#include "io/base.h"

#include "io/Reader.h"
#include "io/Writer.h"
#include "io/Fixed.h"
#include "io/Buf.h"

#include "io/common.h"
#include "io/stream.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io__included */
