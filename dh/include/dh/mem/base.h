/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2026-05-19 (date of last update)
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Memory base types
 * @details Defines base types for memory operations and utilities.
 */
#ifndef mem_base__included
#define mem_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

T_alias$((mem_Log2Align)(u8));
T_decl_O$(mem_Log2Align);
T_alias$((mem_Align)(mem_Log2Align));
T_decl_O$(mem_Align);

errset_((mem_E)(OutOfMemory));
T_useBy$(($spec(E, $set(mem_E)))(
    P_const$u8, P$u8,
    S_const$u8, S$u8,
    u_P_const$raw, u_P$raw,
    u_S_const$raw, u_S$raw
));

T_alias$((mem_Alctr)(struct mem_Alctr));
T_decl_O$(mem_Alctr);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_base__included */
