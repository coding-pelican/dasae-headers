
/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/io
 * @prefix  io
 *
 * @brief   Common IO utilities
 * @details Provides common IO utilities and functions for:
 *          - Stream operations
 *          - Buffer operations
 *          - Utility functions
 */

#ifndef IO_COMMON_INCLUDED
#define IO_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/fs/File.h"

/*========== Macros and Declarations ========================================*/

extern fn_(io_getStdIn(void), fs_File);
extern fn_(io_getStdOut(void), fs_File);
extern fn_(io_getStdErr(void), fs_File);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* IO_COMMON_INCLUDED */
