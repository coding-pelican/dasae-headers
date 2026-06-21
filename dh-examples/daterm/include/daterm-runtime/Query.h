/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Query.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-20 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_Query
 */
#pragma once
#ifndef daterm_Query__included
#define daterm_Query__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((daterm_Query $fits($packed))(
    daterm_Query_native_screen_cells,
    daterm_Query_cached_screen_cells,
    daterm_Query_native_cursor_pos
)) daterm_Query;
T_use_prl$(daterm_Query);

typedef variant_((daterm_Query_Result $fits($packed))(
    (daterm_Query_Result_size, daterm_CellSize),
    (daterm_Query_Result_pos, daterm_Pos)
)) daterm_Query_Result;
T_use_prl$(daterm_Query_Result);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Query__included */
