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
#pragma once
#ifndef io_stream__included
#define io_stream__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

#define io_stream_using_libc __comp_bool__io_stream_using_libc

#ifdef UNUSED_CODE
$extern fn_((io_stream_scan(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_scanVaArgs(S_const$u8 fmt, va_list va_args))(void));
#endif /* UNUSED_CODE */

$extern fn_((io_stream_nl(void))(void));
$extern fn_((io_stream_crlf(void))(void));
$extern fn_((io_stream_print(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printVaArgs(S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_println(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_printlnVaArgs(S_const$u8 fmt, va_list va_args))(void));

$extern fn_((io_stream_enl(void))(void));
$extern fn_((io_stream_ecrlf(void))(void));
$extern fn_((io_stream_eprint(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintVaArgs(S_const$u8 fmt, va_list va_args))(void));
$extern fn_((io_stream_eprintln(S_const$u8 fmt, ...))(void));
$extern fn_((io_stream_eprintlnVaArgs(S_const$u8 fmt, va_list va_args))(void));

/*========== Macros and Definitions =========================================*/

#define __comp_bool__io_stream_using_libc comp_libc_linked

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_stream__included */
