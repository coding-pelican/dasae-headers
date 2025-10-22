/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    src_loc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti
 *
 * @brief   Source location information utilities
 * @details Provides structures and functions for tracking source code locations.
 */

#ifndef BTI_SRC_LOC_INCLUDED
#define BTI_SRC_LOC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

#define bti_filePath() FUNC__bti_filePath()
#define bti_fileName() FUNC__bti_fileName()
#define bti_fnName()   FUNC__bti_fnName()
#define bti_line()     FUNC__bti_line()
#define bti_column()   FUNC__bti_column()

/*========== Macros and Implementations =====================================*/

#define FUNC__bti_filePath() __builtin_FILE()
#define FUNC__bti_fileName() __builtin_FILE_NAME()
#define FUNC__bti_fnName()   __builtin_FUNCTION()
#define FUNC__bti_line()     __builtin_LINE()
#define FUNC__bti_column()   __builtin_COLUMN()

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_SRC_LOC_INCLUDED */
