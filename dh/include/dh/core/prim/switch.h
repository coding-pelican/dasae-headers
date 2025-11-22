/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    switch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2025-06-01 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Switch utilities
 * @details Provides switch utilities and functions for:
 *          - Fallthrough
 *          - Case return
 *          - Default return
 */

#ifndef core_prim_switch__included
#define core_prim_switch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#define switch_(/*(_Init)(_Cond...)*/...) __syn__switch_(__syn__switch___parseInit __VA_ARGS__)

#define case_(/*(_Enum)({...})*/...) __syn__case_(__syn__case___parseEnum __VA_ARGS__)

#define default_(/*({...})*/...) __syn__default_(__VA_ARGS__)

#define $fallthrough __attr__$fallthrough

/*========== Macros and Definitions =========================================*/

#define __syn__switch_(...) __syn__switch___emit(__VA_ARGS__)
#define __syn__switch___parseInit(_Init...) pp_uniqTok(run_once), (_Init), __syn__switch___parseCond
#define __syn__switch___parseCond(_Cond...) (_Cond)
#define __syn__switch___emit(__run_once, _Init, _Cond...) \
    for (bool __run_once = false; !__run_once;) \
        for (__syn__switch___expand _Init; !__run_once; __run_once = true) switch (__syn__switch___expand _Cond)
#define __syn__switch___expand(...) __VA_ARGS__

#define __syn__case_(...) __syn__case___emit(__VA_ARGS__)
#define __syn__case___parseEnum(_Enum...) _Enum,
#define __syn__case___emit(_Enum, _Body...) \
    case _Enum: \
        __syn__case___expand _Body
#define __syn__case___expand(...) __VA_ARGS__

#define __syn__default_(...) __syn__default___emit(__VA_ARGS__)
#define __syn__default___emit(_Body...) \
    default: \
        __syn__default___expand _Body
#define __syn__default___expand(...) __VA_ARGS__

#define __attr__$fallthrough __attribute__((fallthrough))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_switch__included */
