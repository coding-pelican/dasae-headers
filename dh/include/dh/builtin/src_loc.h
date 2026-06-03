/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    src_loc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  src_loc
 *
 * @brief   Source location information utilities
 * @details Provides structures and functions for tracking source code locations.
 */
#ifndef builtin_src_loc__included
#define builtin_src_loc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Definitions =========================================*/

#define src_loc_filePath(/*void) -> (PZ0_const(u8)*/) __comp_inline__src_loc_filePath()
#define src_loc_fileName(/*void) -> (PZ0_const(u8)*/) __comp_inline__src_loc_fileName()
#define src_loc_fnName(/*void) -> (PZ0_const(u8)*/) __comp_inline__src_loc_fnName()
#define src_loc_line(/*void) -> (u32*/) __comp_inline__src_loc_line()
#define src_loc_column(/*void) -> (u32*/) __comp_inline__src_loc_column()

/*========== Macros and Implementations =====================================*/

#define __comp_inline__src_loc_filePath() (as$(const u8*)(__builtin_FILE()))
#define __comp_inline__src_loc_fileName() (as$(const u8*)(__builtin_FILE_NAME()))
#define __comp_inline__src_loc_fnName() (as$(const u8*)(__builtin_FUNCTION()))
#define __comp_inline__src_loc_line() (as$(u32)(__builtin_LINE()))
#define __comp_inline__src_loc_column() (as$(u32)(__builtin_COLUMN()))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_src_loc__included */
