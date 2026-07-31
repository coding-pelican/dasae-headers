/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    errors.h
 * @ingroup dasae-headers(dh)/fmt
 * @prefix  fmt
 *
 * @brief   Formatting error sets independent of writer implementations
 */
#pragma once
#ifndef fmt_errors__included
#define fmt_errors__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

errset_((fmt_E)(
    fmt_MissingClosingBrace,
    fmt_UnexpectedEndFormat,
    fmt_InvalidIdx,
    fmt_IdxOutOfBounds,
    fmt_TooFewArgs,
    fmt_TooManyArgs,
    fmt_InvalidAlignSpec,
    fmt_InvalidWidthSpec,
    fmt_InvalidTypeSpec,
    fmt_InvalidSizeSpec,
    fmt_InvalidBool,
    fmt_InvalidIInt,
    fmt_InvalidUInt,
    fmt_InvalidPrecisionSpec,
    fmt_InvalidFlt,
    fmt_FltDisabled
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fmt_errors__included */
