/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2026-07-30 (date of last update)
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
#include "pri.h"
#include "simd.h"

#include "cmp.h"
#include "L-Limit.h"
#include "R-Range.h"

#include "chain.h"
#include "Co.h"
#include "Clsr.h"
#include "va.h"

#include "CompHash.h"
#include "E-Err.h"
#include "ETrace.h"

#include "claim.h"
#include "debug.h"

/*========== Macros and Declarations ========================================*/

#define T_decl_prl$(_$T...) \
    T_decl_prl_refs$(_$T); \
    T_decl_prl_vals$(_$T); \
    T_decl_prl_exts$(_$T)
#define T_impl_prl$(_$T...) \
    T_impl_prl_refs$(_$T); \
    T_impl_prl_vals$(_$T); \
    T_impl_prl_exts$(_$T)
#define T_use_prl$(_$T...) \
    T_use_prl_refs$(_$T); \
    T_use_prl_vals$(_$T); \
    T_use_prl_exts$(_$T)

#define typeInfos$(_$T...) ____typeInfos$(_$T)
#define typeInfosFrom(_$type_infos...) ____typeInfosFrom(_$type_infos)

/*========== Macros and Definitions =========================================*/

#define ____typeInfos$(_$T...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfos$__each, ~, _$T) }))
#define ____typeInfos$__each(_$ignored, _$T...) \
    typeInfo$(_$T),
#define ____typeInfosFrom(_$type_infos...) \
    A_ref$((S_const$TypeInfo)A_from$((TypeInfo){ pp_foreach(____typeInfosFrom__each, ~, _$type_infos) }))
#define ____typeInfosFrom__each(_$ignored, _$type_info...) \
    (_$type_info),

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_common__included */
