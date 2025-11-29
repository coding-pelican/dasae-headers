/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    src_loc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Source location information utilities
 * @details Provides structures and functions for tracking source code locations.
 */
#ifndef core_src_loc__included
#define core_src_loc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Definitions =========================================*/

/// Structure for storing source code location information
typedef struct SrcLoc {
    const char* file_path; /**< Full file path */
    const char* file_name; /**< Just the file name */
    const char* fn_name;   /**< Function name */
    u32 line;              /**< Line number */
    u32 column;            /**< Column number */
} SrcLoc;
#define srcLoc() \
    /* Creates a source location at compile time */ \
    FUNC__srcLoc()

/*========== Macros and Implementations =====================================*/

#define FUNC__srcLoc() \
    ((SrcLoc){ \
        .file_path = src_loc_filePath(), \
        .file_name = src_loc_fileName(), \
        .fn_name = src_loc_fnName(), \
        .line = src_loc_line(), \
        .column = src_loc_column(), \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_src_loc__included */
