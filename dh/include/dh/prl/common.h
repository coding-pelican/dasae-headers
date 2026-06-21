/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 *
 * @brief   Common prelude for dh/prl
 * @details Provides common prelude for dh/prl.
 */
#pragma once
#ifndef prl_common__included
#define prl_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "simd.h"
#include "Limit.h"
#include "Range.h"

#include "Co.h"
#include "Clsr.h"
#include "va.h"

#include "CompHash.h"
#include "Err.h"
#include "ErrTrace.h"

/*========== Macros and Declarations ========================================*/

#define T_decl_prl$(_T...) \
    T_decl_prl_refs$(_T); \
    T_decl_prl_vals$(_T); \
    T_decl_prl_exts$(_T)
#define T_impl_prl$(_T...) \
    T_impl_prl_refs$(_T); \
    T_impl_prl_vals$(_T); \
    T_impl_prl_exts$(_T)
#define T_use_prl$(_T...) \
    T_use_prl_refs$(_T); \
    T_use_prl_vals$(_T); \
    T_use_prl_exts$(_T)

T_use$((TypeInfo)(P, S));
#define typeInfos$(_T...) ____typeInfos$(_T)
#define typeInfosFrom(_type_infos...) ____typeInfosFrom(_type_infos)

/*========== Macros and Definitions =========================================*/

#define ____typeInfos$(_T...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfos$__each, ~, _T) }))
#define ____typeInfos$__each(_$ignored, _T...) \
    typeInfo$(_T),
#define ____typeInfosFrom(_type_infos...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfosFrom__each, ~, _type_infos) }))
#define ____typeInfosFrom__each(_$ignored, _type_info...) \
    (_type_info),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_common__included */
