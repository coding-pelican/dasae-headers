
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

#ifndef IO_STREAM_INCLUDED
#define IO_STREAM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include <stdarg.h>

/*========== Macros and Declarations ========================================*/

extern fn_((io_stream_nl(void))(void));
extern fn_((io_stream_print(Sli_const$u8 fmt, ...))(void));
extern fn_((io_stream_printVaArgs(Sli_const$u8 fmt, va_list va_args))(void));
extern fn_((io_stream_println(Sli_const$u8 fmt, ...))(void));
extern fn_((io_stream_printlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void));

extern fn_((io_stream_enl(void))(void));
extern fn_((io_stream_eprint(Sli_const$u8 fmt, ...))(void));
extern fn_((io_stream_eprintVaArgs(Sli_const$u8 fmt, va_list va_args))(void));
extern fn_((io_stream_eprintln(Sli_const$u8 fmt, ...))(void));
extern fn_((io_stream_eprintlnVaArgs(Sli_const$u8 fmt, va_list va_args))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* IO_STREAM_INCLUDED */
