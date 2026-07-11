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

/* --- include level --- */

#define eval_include_level __comp_uint__eval_include_level

#define eval_include_level_0 __comp_uint__eval_include_level_0
#define eval_include_level_1 __comp_uint__eval_include_level_1
#define eval_include_level_2 __comp_uint__eval_include_level_2
#define eval_include_level_3 __comp_uint__eval_include_level_3
#define eval_include_level_4 __comp_uint__eval_include_level_4
#define eval_include_level_5 __comp_uint__eval_include_level_5
#define eval_include_level_6 __comp_uint__eval_include_level_6
#define eval_include_level_7 __comp_uint__eval_include_level_7
#define eval_include_level_8 __comp_uint__eval_include_level_8
#define eval_include_level_9 __comp_uint__eval_include_level_9
#define eval_include_level_10 __comp_uint__eval_include_level_10
#define eval_include_level_11 __comp_uint__eval_include_level_11
#define eval_include_level_12 __comp_uint__eval_include_level_12
#define eval_include_level_13 __comp_uint__eval_include_level_13
#define eval_include_level_14 __comp_uint__eval_include_level_14
#define eval_include_level_15 __comp_uint__eval_include_level_15

#define eval_include_level_is_0 __comp_bool__eval_include_level_is_0
#define eval_include_level_is_1 __comp_bool__eval_include_level_is_1
#define eval_include_level_is_2 __comp_bool__eval_include_level_is_2
#define eval_include_level_is_3 __comp_bool__eval_include_level_is_3
#define eval_include_level_is_4 __comp_bool__eval_include_level_is_4
#define eval_include_level_is_5 __comp_bool__eval_include_level_is_5
#define eval_include_level_is_6 __comp_bool__eval_include_level_is_6
#define eval_include_level_is_7 __comp_bool__eval_include_level_is_7
#define eval_include_level_is_8 __comp_bool__eval_include_level_is_8
#define eval_include_level_is_9 __comp_bool__eval_include_level_is_9
#define eval_include_level_is_10 __comp_bool__eval_include_level_is_10
#define eval_include_level_is_11 __comp_bool__eval_include_level_is_11
#define eval_include_level_is_12 __comp_bool__eval_include_level_is_12
#define eval_include_level_is_13 __comp_bool__eval_include_level_is_13
#define eval_include_level_is_14 __comp_bool__eval_include_level_is_14
#define eval_include_level_is_15 __comp_bool__eval_include_level_is_15

/* --- static evaluation --- */

#if !defined(eval_on_analysis)
#define eval_on_analysis __comp_bool__eval_on_analysis
#endif /* !defined(eval_on_analysis) */
#define eval_on_analysis__default __comp_flag__eval_on_analysis__default
#define eval_analysis_enabled __comp_bool__eval_analysis_enabled

#if !defined(eval_analysis_mode)
#define eval_analysis_mode __comp_enum__eval_analysis_mode
#endif /* !defined(eval_analysis_mode) */
#define eval_analysis_mode__default __comp_enum__eval_analysis_mode__default
#define eval_analysis_mode_active_only __comp_enum__eval_analysis_mode_active_only
#define eval_analysis_mode_full __comp_enum__eval_analysis_mode_full

#define eval_analysis_is_active_only __comp_bool__eval_analysis_is_active_only
#define eval_analysis_is_full __comp_bool__eval_analysis_is_full

#define eval_on_analysis_active_only __comp_bool__eval_on_analysis_active_only
#define eval_on_analysis_full __comp_bool__eval_on_analysis_full

#define eval_analysis_only(_$inner...) __comp_syn__eval_analysis_only(_$inner)
#define eval_analysis_unless(_$inner...) __comp_syn__eval_analysis_unless(_$inner)

#if !defined(eval_on_comptime)
#define eval_on_comptime __comp_bool__eval_on_comptime
#endif /* !defined(eval_on_comptime) */
#define eval_on_comptime__default __comp_flag__eval_on_comptime__default
#define eval_comptime_enabled __comp_bool__eval_comptime_enabled
#define eval_comptime_only(_$inner...) __comp_syn__eval_comptime_only(_$inner)
#define eval_comptime_unless(_$inner...) __comp_syn__eval_comptime_unless(_$inner)

#if !defined(eval_on_runtime)
#define eval_on_runtime __comp_bool__eval_on_runtime
#endif /* !defined(eval_on_runtime) */
#define eval_on_runtime__default __comp_flag__eval_on_runtime__default
#define eval_runtime_enabled __comp_bool__eval_runtime_enabled
#define eval_runtime_only(_$inner...) __comp_syn__eval_runtime_only(_$inner)
#define eval_runtime_unless(_$inner...) __comp_syn__eval_runtime_unless(_$inner)

/*========== Macros and Definitions =========================================*/

#define __comp_uint__eval_include_level __INCLUDE_LEVEL__

#define __comp_uint__eval_include_level_0 0
#define __comp_uint__eval_include_level_1 1
#define __comp_uint__eval_include_level_2 2
#define __comp_uint__eval_include_level_3 3
#define __comp_uint__eval_include_level_4 4
#define __comp_uint__eval_include_level_5 5
#define __comp_uint__eval_include_level_6 6
#define __comp_uint__eval_include_level_7 7
#define __comp_uint__eval_include_level_8 8
#define __comp_uint__eval_include_level_9 9
#define __comp_uint__eval_include_level_10 10
#define __comp_uint__eval_include_level_11 11
#define __comp_uint__eval_include_level_12 12
#define __comp_uint__eval_include_level_13 13
#define __comp_uint__eval_include_level_14 14
#define __comp_uint__eval_include_level_15 15

#define __comp_bool__eval_include_level_is_0 pp_Tok_eql(eval_include_level, 0)
#define __comp_bool__eval_include_level_is_1 pp_Tok_eql(eval_include_level, 1)
#define __comp_bool__eval_include_level_is_2 pp_Tok_eql(eval_include_level, 2)
#define __comp_bool__eval_include_level_is_3 pp_Tok_eql(eval_include_level, 3)
#define __comp_bool__eval_include_level_is_4 pp_Tok_eql(eval_include_level, 4)
#define __comp_bool__eval_include_level_is_5 pp_Tok_eql(eval_include_level, 5)
#define __comp_bool__eval_include_level_is_6 pp_Tok_eql(eval_include_level, 6)
#define __comp_bool__eval_include_level_is_7 pp_Tok_eql(eval_include_level, 7)
#define __comp_bool__eval_include_level_is_8 pp_Tok_eql(eval_include_level, 8)
#define __comp_bool__eval_include_level_is_9 pp_Tok_eql(eval_include_level, 9)
#define __comp_bool__eval_include_level_is_10 pp_Tok_eql(eval_include_level, 10)
#define __comp_bool__eval_include_level_is_11 pp_Tok_eql(eval_include_level, 11)
#define __comp_bool__eval_include_level_is_12 pp_Tok_eql(eval_include_level, 12)
#define __comp_bool__eval_include_level_is_13 pp_Tok_eql(eval_include_level, 13)
#define __comp_bool__eval_include_level_is_14 pp_Tok_eql(eval_include_level, 14)
#define __comp_bool__eval_include_level_is_15 pp_Tok_eql(eval_include_level, 15)

#define __comp_bool__eval_on_analysis eval_on_analysis__default
#define __comp_flag__eval_on_analysis__default pp_false
#define __comp_bool__eval_analysis_enabled eval_on_analysis

#define __comp_enum__eval_analysis_mode eval_analysis_mode__default
#define __comp_enum__eval_analysis_mode__default eval_analysis_mode_active_only
#define __comp_enum__eval_analysis_mode_active_only 0
#define __comp_enum__eval_analysis_mode_full 1

#define __comp_bool__eval_analysis_is_active_only \
    pp_Tok_eql(eval_analysis_mode, eval_analysis_mode_active_only)
#define __comp_bool__eval_analysis_is_full \
    pp_Tok_eql(eval_analysis_mode, eval_analysis_mode_full)

#define __comp_bool__eval_on_analysis_active_only \
    pp_and(eval_on_analysis, pp_and(eval_analysis_is_active_only, eval_include_level_is_0))
#define __comp_bool__eval_on_analysis_full \
    pp_and(eval_on_analysis, eval_analysis_is_full)

#define __comp_syn__eval_analysis_only(_$inner...) pp_if_(pp_and( \
    eval_on_analysis, \
    pp_switch_ pp_begin(eval_analysis_mode)( \
        pp_case_(eval_analysis_mode_active_only)(eval_include_level_is_0), \
        pp_case_(eval_analysis_mode_full)(pp_true) pp_end \
    ) \
))(pp_then_(_$inner))
#define __comp_syn__eval_analysis_unless(_$inner...) pp_if_(pp_not(eval_on_analysis))(pp_then_(_$inner))

#if defined(ANALYSIS)
#undef __comp_flag__eval_on_analysis__default
#define __comp_flag__eval_on_analysis__default pp_true
#endif /* defined(ANALYSIS) */

#if defined(ANALYSIS_ACTIVE_ONLY)
#undef __comp_enum__eval_analysis_mode__default
#define __comp_enum__eval_analysis_mode__default eval_analysis_mode_active_only
#endif /* defined(ANALYSIS_ACTIVE) */
#if defined(ANALYSIS_FULL)
#undef __comp_enum__eval_analysis_mode__default
#define __comp_enum__eval_analysis_mode__default eval_analysis_mode_full
#endif /* defined(ANALYSIS_FULL) */

#define __comp_bool__eval_on_comptime eval_on_comptime__default
#define __comp_flag__eval_on_comptime__default pp_false
#define __comp_bool__eval_comptime_enabled eval_on_comptime
#define __comp_syn__eval_comptime_only(_$inner...) pp_if_(eval_on_comptime)(pp_then_(_$inner))
#define __comp_syn__eval_comptime_unless(_$inner...) pp_if_(pp_not(eval_on_comptime))(pp_then_(_$inner))

#if defined(COMP)
#undef __comp_flag__eval_on_comptime__default
#define __comp_flag__eval_on_comptime__default pp_true
#endif /* defined(COMP) */

#define __comp_bool__eval_on_runtime eval_on_runtime__default
#define __comp_flag__eval_on_runtime__default pp_not(pp_or(eval_on_analysis, eval_on_comptime))
#define __comp_bool__eval_runtime_enabled eval_on_runtime
#define __comp_syn__eval_runtime_only(_$inner...) pp_if_(eval_on_runtime)(pp_then_(_$inner))
#define __comp_syn__eval_runtime_unless(_$inner...) pp_if_(pp_not(eval_on_runtime))(pp_then_(_$inner))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_eval__included */
