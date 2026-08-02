/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    fs.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-23 (date of creation)
 * @updated 2026-08-02 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  fs
 *
 * @brief   File system utilities
 * @details Provides utilities for file system operations, including directory traversal,
 *          file path manipulation, and file handling.
 */
#pragma once
#ifndef fs__included
#define fs__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "fs/base.h"
#include "fs/common.h"

#include "fs/Dir.h"
#include "fs/File.h"

#include "fs/path.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs__included */
