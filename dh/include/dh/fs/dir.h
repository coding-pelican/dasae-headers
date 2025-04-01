/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dir.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2025-02-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/fs
 * @prefix  fs_dir
 *
 * @brief   Directory utilities
 * @details Provides utilities for directory operations:
 *          - Directory creation
 *          - Directory listing
 *          - Directory traversal
 *          - Directory deletion
 *          - Directory utilities
 */

#ifndef FS_DIR_INCLUDED
#define FS_DIR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/err_res.h"
#include "dh/Str.h"

/*========== Function Prototypes ============================================*/

extern Err$void fs_dir_create(Str_const path) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* FS_DIR_INCLUDED */
