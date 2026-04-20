/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    stream.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-08-09 (date of creation)
 * @updated 2026-02-06 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_stream
 */
#ifndef io_stream__included
#define io_stream__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"

/*========== Macros and Declarations ========================================*/

#ifdef UNUSED_CODE
$extern fn_((io_stream_scan(io_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_scanVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void));
#endif /* UNUSED_CODE */

$extern fn_((io_stream_nl(io_Self self))(void));
$extern fn_((io_stream_print(io_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_println(io_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printlnVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void));

$extern fn_((io_stream_enl(io_Self self))(void));
$extern fn_((io_stream_eprint(io_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_eprintln(io_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintlnVaArgs(io_Self self, S_const$u8 fmt, va_list va_args))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_stream__included */
