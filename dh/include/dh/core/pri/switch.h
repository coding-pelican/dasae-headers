/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    switch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2025-06-01 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/pri
 * @prefix  (none)
 *
 * @brief   Switch utilities
 * @details Provides switch utilities and functions for:
 *          - Fallthrough
 *          - Case return
 *          - Default return
 */
#pragma once
#ifndef core_pri_switch__included
#define core_pri_switch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#undef default_
#undef case_
#define switch_(/*(_$Init)(_$Cond...)*/...) __syn__switch_(__syn__switch___parseInit __VA_ARGS__)
// #define case_(/*(_$Enum)({...})*/...) __syn__case_(__syn__case___parseEnum __VA_ARGS__)
// #define default_(/*({...})*/...) __syn__default_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __syn__switch_(...) __syn__switch___emit(__VA_ARGS__)
#define __syn__switch___parseInit(_$Init...) pp_uniqTok(run_once), (_$Init), __syn__switch___parseCond
#define __syn__switch___parseCond(_$Cond...) (_$Cond)
#define __syn__switch___emit(__run_once, _$Init, _$Cond...) \
    for (bool __run_once = false; !__run_once;) \
        for (__syn__switch___expand _$Init; !__run_once; __run_once = true) switch (__syn__switch___expand _$Cond)
#define __syn__switch___expand(...) __VA_ARGS__

#define __syn__case_(...) __syn__case___emit(__VA_ARGS__)
#define __syn__case___parseEnum(_$Enum...) _$Enum,
#define __syn__case___emit(_$Enum, _$Body...) \
    case _$Enum: \
        __syn__case___expand _$Body
#define __syn__case___expand(...) __VA_ARGS__

#define __syn__default_(...) __syn__default___emit(__VA_ARGS__)
#define __syn__default___emit(_$Body...) \
    default: \
        __syn__default___expand _$Body
#define __syn__default___expand(...) __VA_ARGS__

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_pri_switch__included */
