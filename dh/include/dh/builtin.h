/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    builtin.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2026-05-25 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   Builtin configurations and compiler-specific macros
 * @details Includes architecture, compiler, language and platform specific
 *          configurations. Serves as a central point for builtin definitions
 *          and compiler-related functionality used throughout the project.
 */
#pragma once
#ifndef builtin__included
#define builtin__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "builtin/pp.h"
#include "builtin/cfg.h"

#include "builtin/auto.h" /* no prefix */
#include "builtin/comp.h" /* no prefix */
#include "builtin/type_info.h" /* no prefix */
#include "builtin/container_info.h" /* no prefix */

#include "builtin/asm.h"
#include "builtin/mem.h" /* no prefix */
#include "builtin/atom.h"
#include "builtin/prefetch.h"

#include "builtin/src_loc.h"
#include "builtin/static_assert.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin__included */
