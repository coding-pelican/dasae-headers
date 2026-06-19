/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    dansi-core.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_core
 */
#pragma once
#ifndef dansi_core__included
#define dansi_core__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/c0.h"
#include "dansi-core/c1.h"
#include "dansi-core/ctrl.h"
#include "dansi-core/Seq.h"
#include "dansi-core/esc.h"
#include "dansi-core/csi.h"
#include "dansi-core/osc.h"
#include "dansi-core/dcs.h"
#include "dansi-core/pm.h"
#include "dansi-core/apc.h"
#include "dansi-core/sos.h"

#include "dansi-core/cursor.h"
#include "dansi-core/erase.h"
#include "dansi-core/scroll.h"
#include "dansi-core/mode.h"

#include "dansi-core/sgr.h"
#include "dansi-core/style.h"
#include "dansi-core/color.h"

#include "dansi-core/device.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_core__included */
