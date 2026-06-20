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
 * @prefix  daterm_LocalQuery
 */
#pragma once
#ifndef daterm_LocalQuery__included
#define daterm_LocalQuery__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((daterm_LocalQueryKind $fits($packed))(
    daterm_LocalQueryKind_native_screen_cells,
    daterm_LocalQueryKind_cached_screen_cells,
    daterm_LocalQueryKind_native_cursor_pos
)) daterm_LocalQueryKind;
T_use_prl$(daterm_LocalQueryKind);

typedef struct daterm_LocalQuery {
    var_(kind, daterm_LocalQueryKind);
} daterm_LocalQuery;
T_use_prl$(daterm_LocalQuery);

typedef variant_((daterm_LocalQueryResult $fits($packed))(
    (daterm_LocalQueryResult_size, daterm_Size),
    (daterm_LocalQueryResult_pos, daterm_Pos)
)) daterm_LocalQueryResult;
T_use_prl$(daterm_LocalQueryResult);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_LocalQuery__included */
