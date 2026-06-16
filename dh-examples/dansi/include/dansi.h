/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    dansi.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-28 (date of last update)
 * @version v0.1.0
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi
 *
 * @brief   Pure ANSI protocol library.
 * @details Provides ANSI escape sequence generation, raw sequence extraction,
 *          and standard ANSI event parsing without OS terminal management.
 */
#pragma once
#ifndef dansi__included
#define dansi__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core.h"
#include "dansi-xterm.h"
#include "dansi-kitty.h"
#include "dansi-sixel.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi__included */
