/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2026-02-06 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 *
 * @brief   Common IO utilities
 * @details Provides common IO utilities and functions for:
 *          - Stream operations
 *          - Buffer operations
 *          - Utility functions
 */
#ifndef io_common__included
#define io_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "../fs/File.h"

/*========== Macros and Declarations ========================================*/

errset_((io_E)(
    UnexpectedEof,
    BufferTooSmall
));

$extern fn_((io_getStdIn(void))(fs_File));
$extern fn_((io_getStdOut(void))(fs_File));
$extern fn_((io_getStdErr(void))(fs_File));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_common__included */
