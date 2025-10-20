/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-09 (date of creation)
 * @updated 2025-06-15 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/fmt
 * @prefix  fmt
 */

#ifndef FMT_COMMON_INCLUDED
#define FMT_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/io/Writer.h"
#include <stdarg.h>

/*========== Macros and Declarations ========================================*/

config_ErrSet(fmt_Err,
    IndexOutOfBounds,
    InvalidIndexFormat,
    InvalidIntegerFormat,
    InvalidFloatFormat,
    InvalidBoolFormat
);

extern fn_((fmt_format(io_Writer writer, Sli_const$u8 fmt, ...))(Err$void)) $must_check;
extern fn_((fmt_formatVaArgs(io_Writer writer, Sli_const$u8 fmt, va_list va_args))(Err$void)) $must_check;

extern fn_((fmt_parseInt_usize(Sli_const$u8 str, u8 base))(Err$usize)) $must_check;
extern fn_((fmt_parseInt_u64(Sli_const$u8 str, u8 base))(Err$u64)) $must_check;
extern fn_((fmt_parseInt_u32(Sli_const$u8 str, u8 base))(Err$u32)) $must_check;

extern fn_((fmt_parseInt_isize(Sli_const$u8 str, u8 base))(Err$isize)) $must_check;
extern fn_((fmt_parseInt_i64(Sli_const$u8 str, u8 base))(Err$i64)) $must_check;
extern fn_((fmt_parseInt_i32(Sli_const$u8 str, u8 base))(Err$i32)) $must_check;

extern fn_((fmt_parseFlt_f64(Sli_const$u8 str))(Err$f64)) $must_check;
extern fn_((fmt_parseFlt_f32(Sli_const$u8 str))(Err$f32)) $must_check;

extern fn_((fmt_parseBool(Sli_const$u8 str))(Err$bool)) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* IO_COMMON_INCLUDED */
