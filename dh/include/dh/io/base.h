/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-22 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 *
 * @brief   Base types for IO utilities
 * @details Defines base types for IO utilities.
 */
#pragma once
#ifndef io_base__included
#define io_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

errset_((io_ReadFailedE)(io_ReadFailed));
errset_((io_ReadE)() $union_errset_(io_ReadFailedE));
T_useBy$(($spec(E, $set(io_ReadE)))(usize, S_const$u8, S$u8));

errset_((io_UnexpectedEOFE)(UnexpectedEOF));
errset_((io_ReadExactE)() $union_errset_(
    io_ReadE,
    io_UnexpectedEOFE
));
T_useBy$(($spec(E, $set(io_ReadExactE)))(u8));

errset_((io_WriteFailedE)(io_WriteFailed));
errset_((io_TooSmallBufferE)(TooSmallBuffer));
errset_((io_WriteE)() $union_errset_(
    io_WriteFailedE,
    io_TooSmallBufferE
));
T_use_E$($set(io_WriteE)(usize));

errset_((io_CopyE)() $union_errset_(io_ReadE, io_WriteE));
T_use_E$($set(io_CopyE)(usize));
errset_((io_CopyExactE)() $union_errset_(
    io_CopyE,
    io_UnexpectedEOFE
));

T_alias$((io_Reader)(struct io_Reader));
T_alias$((io_Writer)(struct io_Writer));

T_alias$((io_std_Self)(struct io_std_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_base__included */
