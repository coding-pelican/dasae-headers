/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2026-05-24 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 *
 * @brief   Base types for IO utilities
 * @details Defines base types for IO utilities.
 */
#pragma once
#ifndef io_base__included
#define io_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

errset_((io_ReadE)(UnexpectedEOF));
errset_((io_WriteE)(TooSmallBuffer));
errset_((io_E)() $union_errset_(io_ReadE, io_WriteE));

T_alias$((io_Reader)(struct io_Reader));
T_alias$((io_Writer)(struct io_Writer));

#define io_nl __str__io_nl
#define io_nl_byte __uint__io_nl_byte
#define io_crlf __str__io_crlf

#define io_nl_native __str__io_nl_native
#define io_nl_windows __str__io_nl_windows
#define io_nl_unix __str__io_nl_unix

/*========== Macros and Definitions =========================================*/

#define __str__io_nl "\n"
#define __uint__io_nl_byte u8_c('\n')
#define __str__io_crlf "\r\n"

#define __str__io_nl_native pp_if_(plat_is_windows)( \
    pp_then_(io_nl_windows), \
    pp_else_(io_nl_unix) \
)
#define __str__io_nl_windows io_crlf
#define __str__io_nl_unix io_nl

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_base__included */
