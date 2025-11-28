/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    builtin.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   Builtin configurations and compiler-specific macros
 * @details Includes architecture, compiler, language and platform specific
 *          configurations. Serves as a central point for builtin definitions
 *          and compiler-related functionality used throughout the project.
 */
#ifndef builtin__included
#define builtin__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "builtin/pp.h"
#include "builtin/lang_cfg.h"
#include "builtin/arch_cfg.h"
#include "builtin/plat_cfg.h"
#include "builtin/comp_cfg.h"
#include "builtin/comp.h"
#include "builtin/src_loc.h"
#include "builtin/static_assert.h"
#include "builtin/auto.h"
#include "builtin/lambda.h"
#include "builtin/type_info.h"
#include "builtin/container_info.h"
#include "builtin/mem.h"
#include "builtin/atom.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin__included */
