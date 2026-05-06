/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    share.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-22 (date of creation)
 * @updated 2025-12-22 (date of last update)
 * @ingroup dasae-headers(dh)/fmt/private
 * @prefix  fmt
 *
 * @brief   Internal shared definitions for `fmt` module
 * @details This header contains internal structures and functions shared
 *          between `fmt/common.c` and `fmt/private/impl/flt.c`. Not part of public API.
 */
#ifndef fmt_private_share__included
#define fmt_private_share__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/fmt/common.h"
#include "dh/mem/common.h"
#include "dh/ascii.h"
#include "dh/utf8.h"

/*========== Macros and Declarations ========================================*/

/// Write content with padding according to format spec
$attr($must_check)
$extern fn_((fmt__writePadded(io_Writer writer, S_const$u8 content, fmt_Spec spec))(E$void));

/// Format f64 as a floating-point number
$attr($must_check)
$extern fn_((fmt__formatFltImpl(io_Writer writer, f64 val, fmt_Spec spec))(E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_private_share__included */
