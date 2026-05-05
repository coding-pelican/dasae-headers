/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    exts.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-03 (date of creation)
 * @updated 2026-05-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#ifndef prl_base_exts__included
#define prl_base_exts__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "refs.h"
#include "vals.h"

/*========== Macros and Declarations ========================================*/

#define T_use_prl_exts$(_T...) \
    T_use$((P_const$(_T))(O, E)); \
    T_use$((P$(_T))(O, E)); \
    T_use$((S_const$(_T))(O, E)); \
    T_use$((S$(_T))(O, E)); \
    T_use$((O$(_T))(P, S)); \
    T_use$((E$(_T))(P, S));

/*========== Macros and Definitions =========================================*/

// clang-format off
T_use_prl_exts$(u8); T_use_prl_exts$(u16); T_use_prl_exts$(u32); T_use_prl_exts$(u64); T_use_prl_exts$(usize);
T_use_prl_exts$(i8); T_use_prl_exts$(i16); T_use_prl_exts$(i32); T_use_prl_exts$(i64); T_use_prl_exts$(isize);
T_use_prl_exts$(f32); T_use_prl_exts$(f64);
T_use_prl_exts$(bool); T_use_prl_exts$(Void); T_use_prl_exts$(Opaq);
T_use_prl_exts$(P_const$raw); T_use_prl_exts$(S_const$raw);
T_use_prl_exts$(P$raw); T_use_prl_exts$(S$raw);
T_use_prl_exts$(V$raw); T_use_prl_exts$(A$raw);
T_use_prl_exts$(u_P_const$raw); T_use_prl_exts$(u_S_const$raw);
T_use_prl_exts$(u_P$raw); T_use_prl_exts$(u_S$raw);
T_use_prl_exts$(u_V$raw); T_use_prl_exts$(u_A$raw);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_base_exts__included */
