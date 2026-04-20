/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    core.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   "Make C Great Again"
 * @details Provides core utilities and definitions for the project.
 */
#ifndef core__included
#define core__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core/pri.h"
#include "core/fn.h"
#if DEPRECATED_CODE
#include "core/Callable.h"
#endif /* DEPRECATED_CODE */
#include "core/claim.h"
#include "core/debug.h"
#include "core/range.h"
#include "core/op.h"
#include "core/cmp.h"
#include "core/pipe.h"
#include "core/chain.h"
#include "core/scope.h"
#include "core/src_loc.h"
#include "core/type_info.h"
#include "core/struct_layout.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core__included */
