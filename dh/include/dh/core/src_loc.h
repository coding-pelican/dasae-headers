/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    src_loc.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  NONE
 *
 * @brief   Source location information utilities
 * @details Provides structures and functions for tracking source code locations.
 */

#ifndef CORE_SRC_LOC_INCLUDED
#define CORE_SRC_LOC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/// Structure for storing source code location information
typedef struct SrcLoc {
    const char* file_path; /**< Full file path */
    const char* file_name; /**< Just the file name */
    const char* fn_name;   /**< Function name */
    u32         line;      /**< Line number */
    u32         column;    /**< Column number */
} SrcLoc;
#define srcLoc()                                    \
    /* Creates a source location at compile time */ \
    FUNC__srcLoc()

/*========== Macros and Implementations =====================================*/

#define FUNC__srcLoc()               \
    ((SrcLoc){                       \
        .file_path = bti_filePath(), \
        .file_name = bti_fileName(), \
        .fn_name   = bti_fnName(),   \
        .line      = bti_line(),     \
        .column    = bti_column(),   \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_SRC_LOC_INCLUDED */
