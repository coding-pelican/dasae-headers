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

#include "pri.h"

/*========== Macros and Definitions =========================================*/

/// Structure for storing source code location information
typedef struct SrcLoc {
    var_(file_path, const u8*); /**< Full file path */
    var_(file_name, const u8*); /**< Just the file name */
    var_(fn_name, const u8*); /**< Function name */
    var_(line, u32); /**< Line number */
    var_(column, u32); /**< Column number */
} SrcLoc;
#define srcLoc() \
    /* Creates a source location at compile time */ \
    __val__srcLoc()

/*========== Macros and Implementations =====================================*/

#define __val__srcLoc() l$((SrcLoc){ \
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
