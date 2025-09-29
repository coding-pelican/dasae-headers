/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2025-02-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs
 *
 * @brief   Common file system utilities
 * @details Provides common file system utilities and functions for:
 *          - File operations
 *          - Directory operations
 *          - Path operations
 *          - File system operations
 *          - File system utilities
 */

#ifndef FS_COMMON_INCLUDED
#define FS_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

config_ErrSet(fs_FileErr,
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* FS_COMMON_INCLUDED */
