/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    core.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   "Make C Great Again"
 * @details Provides core utilities and definitions for the project.
 */
#pragma once
#ifndef core__included
#define core__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/debug.h"
#include "core/claim.h"

#include "core/pri.h"
#include "core/fn.h"
#include "core/op.h"
#include "core/cmp.h"
#include "core/range.h"

#include "core/scope.h"
#include "core/pipe.h"
#include "core/chain.h"

#include "core/src_loc.h"
#include "core/type_info.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core__included */
