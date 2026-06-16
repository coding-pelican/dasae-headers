/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    eval.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  eval
 */
#pragma once
#ifndef foundation_cfg_eval__included
#define foundation_cfg_eval__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../pp.h"

/*========== Macros and Declarations ========================================*/

/* --- static evaluation --- */

#if !defined(eval_on_comptime)
#define eval_on_comptime __comp_bool__eval_on_comptime
#endif /* !defined(eval_on_comptime) */
#define eval_on_comptime__default __comp_flag__eval_on_comptime__default
#define eval_comptime_comp_enabled __comp_bool__eval_comptime_comp_enabled
#define eval_comptime_only(_inner...) __comp_syn__eval_comptime_only(_inner)

#if !defined(eval_on_runtime)
#define eval_on_runtime __comp_bool__eval_on_runtime
#endif /* !defined(eval_on_runtime) */
#define eval_on_runtime__default __comp_flag__eval_on_runtime__default
#define eval_runtime_comp_enabled __comp_bool__eval_runtime_comp_enabled
#define eval_runtime_only(_inner...) __comp_syn__eval_runtime_only(_inner)

/*========== Macros and Definitions =========================================*/

#define __comp_bool__eval_on_comptime eval_on_comptime__default
#define __comp_flag__eval_on_comptime__default pp_false
#define __comp_bool__eval_comptime_comp_enabled eval_on_comptime
#define __comp_syn__eval_comptime_only(_inner...) pp_if_(eval_on_comptime)(pp_then_(_inner))

#define __comp_flag__eval_on_runtime__default pp_true
#define __comp_bool__eval_runtime_comp_enabled eval_on_runtime
#define __comp_syn__eval_runtime_only(_inner...) pp_if_(eval_on_runtime)(pp_then_(_inner))
#define __comp_syn__eval_runtime_only(_inner...) pp_if_(eval_on_runtime)(pp_then_(_inner))

#if defined(COMP)
#undef __comp_flag__eval_on_comptime__default
#define __comp_flag__eval_on_comptime__default pp_true
#endif /* defined(COMP) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_eval__included */
