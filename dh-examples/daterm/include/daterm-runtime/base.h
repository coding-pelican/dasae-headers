/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    base.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm
 */
#pragma once
#ifndef daterm_runtime_base__included
#define daterm_runtime_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <dh/prl.h>

typedef struct daterm_Pos {
    var_(x, u16);
    var_(y, u16);
} daterm_Pos;
T_use_prl$(daterm_Pos);

typedef struct daterm_CellSize {
    var_(cols, u16);
    var_(rows, u16);
} daterm_CellSize;
T_use_prl$(daterm_CellSize);

typedef struct daterm_PixelSize {
    var_(width, u16);
    var_(height, u16);
} daterm_PixelSize;
T_use_prl$(daterm_PixelSize);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_runtime_base__included */
