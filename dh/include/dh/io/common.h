
/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2025-03-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io
 *
 * @brief   Common IO utilities
 * @details Provides common IO utilities and functions for:
 *          - File operations
 *          - Stream operations
 *          - Buffer operations
 *          - Error handling
 *          - Utility functions
 */

#ifndef IO_COMMON_INCLUDED
#define IO_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/Err.h"

/*========== Macros and Declarations ========================================*/

config_ErrSet(io_FileErr,
    NotFound,
    AccessDenied,
    OpenFailed,
    ReadFailed,
    WriteFailed
);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* IO_COMMON_INCLUDED */
