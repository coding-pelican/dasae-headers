/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    stream.h
 * @author  Gyeongtae Kim (@dev-dasae) <codingpelican@gmail.com>
 * @date    2025-08-09 (date of creation)
 * @updated 2025-08-09 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_stream
 */
#ifndef io_stream__included
#define io_stream__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include <stdarg.h>

/*========== Macros and Declarations ========================================*/

#ifdef UNUSED_CODE
$extern fn_((io_stream_scan(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_scanVaArgs(S_const$u8 fmt, va_list va_args))(void));
#endif /* UNUSED_CODE */

$extern fn_((io_stream_nl(void))(void));
$extern fn_((io_stream_print(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printVaArgs(S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_println(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printlnVaArgs(S_const$u8 fmt, va_list va_args))(void));

$extern fn_((io_stream_enl(void))(void));
$extern fn_((io_stream_eprint(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintVaArgs(S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_eprintln(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintlnVaArgs(S_const$u8 fmt, va_list va_args))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_stream__included */
