/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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
#ifndef fs_common__included
#define fs_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_File_Err)(
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_common__included */
