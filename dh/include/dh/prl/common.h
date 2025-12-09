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
#ifndef prl_common__included
#define prl_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "types.h"

/*========== Macros and Declarations ========================================*/

#define T_use_prl$(_T...) \
    T_use$((_T)(P, S, O, E)); \
    T_use$((P$(const _T))(P, S, O, E)); \
    T_use$((P$(_T))(P, S, O, E)); \
    T_use$((S$(const _T))(P, S, O, E)); \
    T_use$((S$(_T))(P, S, O, E)); \
    T_use$((O$(_T))(P, S, O, E)); \
    T_use$((E$(_T))(P, S, O, E))

T_use$((TypeInfo)(P, S));
#define typeInfos$(_T...) ____typeInfos$(_T)
#define typeInfosFrom(_type_infos...) ____typeInfosFrom(_type_infos)

/*========== Macros and Definitions =========================================*/

// clang-format off
T_use_prl$(u8); T_use_prl$(u16); T_use_prl$(u32); T_use_prl$(u64); T_use_prl$(usize);
T_use_prl$(i8); T_use_prl$(i16); T_use_prl$(i32); T_use_prl$(i64); T_use_prl$(isize);
T_use_prl$(f32); T_use_prl$(f64);
T_use_prl$(bool);
T_use_prl$(u_P_const$raw); T_use_prl$(u_S_const$raw);
T_use_prl$(u_P$raw); T_use_prl$(u_S$raw);
T_use_prl$(u_V$raw); T_use_prl$(u_A$raw);
// clang-format on

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
