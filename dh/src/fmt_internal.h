/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    fmt_internal.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2025-12-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fmt/internal
 * @prefix  fmt
 *
 * @brief   Internal shared definitions for `fmt` module
 * @details This header contains internal structures and functions shared
 *          between `fmt_common.c` and `fmt_internal_flt.c`. Not part of public API.
 */
#ifndef fmt_internal__included
#define fmt_internal__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/fmt/common.h"
#include "dh/mem/common.h"
#include "dh/ascii.h"

/*========== Macros and Declarations ========================================*/

/// Write content with padding according to format spec
$attr($must_check)
$extern fn_((fmt__writePadded(io_Writer writer, S_const$u8 content, fmt_Spec spec))(E$void));

/// Format f64 as a floating-point number
$attr($must_check)
$extern fn_((fmt__formatFlt(io_Writer writer, f64 val, fmt_Spec spec))(E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_internal__included */
