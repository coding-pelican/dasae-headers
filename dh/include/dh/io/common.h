/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2026-05-24 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 *
 * @brief   Common IO utilities
 * @details Provides common IO utilities and functions for:
 *          - Stream operations
 *          - Buffer operations
 *          - Utility functions
 */
#pragma once
#ifndef io_common__included
#define io_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

#define io_lf __str__io_lf
#define io_lf_byte __uint__io_lf_byte
#define io_cr __str__io_cr
#define io_cr_byte __uint__io_cr_byte
#define io_crlf __str__io_crlf

#define io_nl __str__alias__io_nl
#define io_nl_native __str__io_nl_native
#define io_nl_windows __str__io_nl_windows
#define io_nl_unix __str__io_nl_unix

/*========== Macros and Definitions =========================================*/

#define __str__io_lf "\n"
#define __uint__io_lf_byte u8_c('\n')
#define __str__io_cr "\r"
#define __uint__io_cr_byte u8_c('\r')
#define __str__io_crlf io_cr io_lf

#define __str__alias__io_nl io_nl_native
#define __str__io_nl_native pp_if_(plat_is_windows)( \
    pp_then_(io_nl_windows), \
    pp_else_(io_nl_unix) \
)
#define __str__io_nl_windows io_crlf
#define __str__io_nl_unix io_nl

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_common__included */
