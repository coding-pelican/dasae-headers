/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Writer.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-13 (date of creation)
 * @updated 2026-02-06 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_Writer
 */
#ifndef io_Writer__included
#define io_Writer__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

typedef struct io_Writer {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*write)(P$raw ctx, S_const$u8 bytes))(E$usize));
} io_Writer;

$attr($must_check)
$extern fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(E$usize));
$attr($must_check)
$extern fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(E$void));
$attr($must_check)
$extern fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(E$void));
$attr($must_check)
$extern fn_((io_Writer_writeByte(io_Writer self, u8 byte))(E$void));
$attr($must_check)
$extern fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(E$void));

$attr($must_check)
$extern fn_((io_Writer_nl(io_Writer self))(E$void));
$attr($must_check)
$extern fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(E$void));
$attr($must_check)
$extern fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void));
$attr($must_check)
$extern fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(E$void));
$attr($must_check)
$extern fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Writer__included */
