/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    refs.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-03 (date of creation)
 * @updated 2026-05-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#ifndef prl_base_refs__included
#define prl_base_refs__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../raw.h"
#include "../tpl.h"
#include "../meta.h"

/*========== Macros and Declarations ========================================*/

#define T_use_prl_refs$(_T...) \
    T_use$((_T)(P, S)); \
    T_use$((P_const$(_T))(P, S)); \
    T_use$((P$(_T))(P, S)); \
    T_use$((S_const$(_T))(P, S)); \
    T_use$((S$(_T))(P, S));

/*========== Macros and Definitions =========================================*/

// clang-format off
T_use_prl_refs$(u8); T_use_prl_refs$(u16); T_use_prl_refs$(u32); T_use_prl_refs$(u64); T_use_prl_refs$(usize);
T_use_prl_refs$(i8); T_use_prl_refs$(i16); T_use_prl_refs$(i32); T_use_prl_refs$(i64); T_use_prl_refs$(isize);
T_use_prl_refs$(f32); T_use_prl_refs$(f64);
T_use_prl_refs$(bool); T_use_prl_refs$(Void); T_use_prl_refs$(Opaq);
T_use_prl_refs$(P_const$raw); T_use_prl_refs$(S_const$raw);
T_use_prl_refs$(P$raw); T_use_prl_refs$(S$raw);
T_use_prl_refs$(V$raw); T_use_prl_refs$(A$raw);
T_use_prl_refs$(u_P_const$raw); T_use_prl_refs$(u_S_const$raw);
T_use_prl_refs$(u_P$raw); T_use_prl_refs$(u_S$raw);
T_use_prl_refs$(u_V$raw); T_use_prl_refs$(u_A$raw);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_base_refs__included */
