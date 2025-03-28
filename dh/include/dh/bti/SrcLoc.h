/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    bti_SrcLoc.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_SrcLoc
 *
 * @brief   Source location utilities
 * @details Provides source code location tracking utilities.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_SRC_LOC_INCLUDED
#define BTI_SRC_LOC_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#define bti_SrcLoc_filePath()                                   \
    /**                                                         \
     * @brief Get the full path of the current source file      \
     * @return const char* Full path of the current source file \
     */                                                         \
    comp_impl__bti_SrcLoc_filePath()
#define bti_SrcLoc_fileName()                              \
    /**                                                    \
     * @brief Get the name of the current source file      \
     * @return const char* Name of the current source file \
     */                                                    \
    comp_impl__bti_SrcLoc_fileName()
#define bti_SrcLoc_fnName()                             \
    /**                                                 \
     * @brief Get the name of the current function      \
     * @return const char* Name of the current function \
     */                                                 \
    comp_impl__bti_SrcLoc_fnName()
#define bti_SrcLoc_line()                                    \
    /**                                                      \
     * @brief Get the current line number in the source file \
     * @return int Current line number                       \
     */                                                      \
    comp_impl__bti_SrcLoc_line()
#define bti_SrcLoc_column()                                    \
    /**                                                        \
     * @brief Get the current column number in the source file \
     * @return int Current column number                       \
     */                                                        \
    comp_impl__bti_SrcLoc_column()

/*========== Macros and Definitions =========================================*/

#define comp_impl__bti_SrcLoc_filePath() __builtin_FILE()
#define comp_impl__bti_SrcLoc_fileName() __builtin_FILE_NAME()
#define comp_impl__bti_SrcLoc_fnName()   __builtin_FUNCTION()
#define comp_impl__bti_SrcLoc_line()     __builtin_LINE()
#define comp_impl__bti_SrcLoc_column()   __builtin_COLUMN()

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_SRCLOC_INCLUDED */
