/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Writer.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-13 (date of creation)
 * @updated 2025-05-13 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Writer
 */
#ifndef io_Writer__included
#define io_Writer__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include <stdarg.h>

/*========== Macros and Declarations ========================================*/

typedef struct io_Writer {
    P$raw ctx;
    fn_(((*write)(P$raw ctx, S_const$u8 bytes))(E$usize)) $must_check;
} io_Writer;

$extern fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(E$usize)) $must_check;
$extern fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(E$void)) $must_check;
$extern fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(E$void)) $must_check;
$extern fn_((io_Writer_writeByte(io_Writer self, u8 byte))(E$void)) $must_check;
$extern fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(E$void)) $must_check;

$extern fn_((io_Writer_nl(io_Writer self))(E$void)) $must_check;
$extern fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(E$void)) $must_check;
$extern fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void)) $must_check;
$extern fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(E$void)) $must_check;
$extern fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(E$void)) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Writer__included */
