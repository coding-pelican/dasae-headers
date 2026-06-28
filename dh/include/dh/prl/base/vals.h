/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    vals.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-03 (date of creation)
 * @updated 2026-05-03 (date of last update)
 * @ingroup dasae-headers(dh)/prl
 * @prefix  (none)
 */
#pragma once
#ifndef prl_base_vals__included
#define prl_base_vals__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../raw.h"
#include "../tpl.h"
#include "../meta.h"

/*========== Macros and Declarations ========================================*/

#define T_decl_prl_vals$(_T...) \
    T_decl$((_T)(O, E)); \
    T_decl$((O$(_T))(O, E)); \
    T_decl$((E$(_T))(O, E))
#define T_impl_prl_vals$(_T...) \
    T_impl$((_T)(O, E)); \
    T_impl$((O$(_T))(O, E)); \
    T_impl$((E$(_T))(O, E))
#define T_use_prl_vals$(_T...) \
    T_use$((_T)(O, E)); \
    T_use$((O$(_T))(O, E)); \
    T_use$((E$(_T))(O, E))

/* clang-format off */
#define T_decl_prl_u_vals$(_T...) \
    T_decl$((_T)(u_V)); T_decl$((O$(_T))(u_V)); T_decl$((E$(_T))(u_V)); \
    T_decl$((O$(O$(_T)))(u_V)); T_decl$((O$(E$(_T)))(u_V)); \
    T_decl$((E$(O$(_T)))(u_V)); T_decl$((E$(E$(_T)))(u_V))
#define T_impl_prl_u_vals$(_T...) \
    T_impl$((_T)(u_V)); T_impl$((O$(_T))(u_V)); T_impl$((E$(_T))(u_V)); \
    T_impl$((O$(O$(_T)))(u_V)); T_impl$((O$(E$(_T)))(u_V)); \
    T_impl$((E$(O$(_T)))(u_V)); T_impl$((E$(E$(_T)))(u_V))
#define T_use_prl_u_vals$(_T...) \
    T_use$((_T)(u_V)); T_use$((O$(_T))(u_V)); T_use$((E$(_T))(u_V)); \
    T_use$((O$(O$(_T)))(u_V)); T_use$((O$(E$(_T)))(u_V)); \
    T_use$((E$(O$(_T)))(u_V)); T_use$((E$(E$(_T)))(u_V))
/* clang-format on */

/*========== Macros and Definitions =========================================*/

/* clang-format off */
T_use_prl_vals$(bool);
T_use_prl_vals$(u8); T_use_prl_vals$(u16); T_use_prl_vals$(u32);
T_use_prl_vals$(ulong); T_use_prl_vals$(u64); T_use_prl_vals$(usize);
T_use_prl_vals$(i8); T_use_prl_vals$(i16); T_use_prl_vals$(i32);
T_use_prl_vals$(ilong); T_use_prl_vals$(i64); T_use_prl_vals$(isize);
T_use_prl_vals$(f32); T_use_prl_vals$(f64);

T_use_prl_vals$(Opaq); T_use_prl_vals$(EAny);
T_use$((O$Void)(O, E)); T_use$((O$void)(O, E));
T_use$((E$Void)(O, E)); T_use$((E$void)(O, E));
T_use_prl_vals$(P_const$raw); T_use_prl_vals$(S_const$raw);
T_use_prl_vals$(P$raw); T_use_prl_vals$(S$raw);
T_use_prl_vals$(V$raw); T_use_prl_vals$(A$raw);

T_use_prl_vals$(u_P_const$raw); T_use_prl_vals$(u_S_const$raw);
T_use_prl_vals$(u_P$raw); T_use_prl_vals$(u_S$raw);
T_use_prl_vals$(u_V$raw); T_use_prl_vals$(u_A$raw);

T_use_prl_u_vals$(Opaq); T_use_prl_u_vals$(EAny);
T_use_prl_u_vals$(Void);
T_use_prl_u_vals$(P_const$raw); T_use_prl_u_vals$(S_const$raw);
T_use_prl_u_vals$(P$raw); T_use_prl_u_vals$(S$raw);
T_use_prl_u_vals$(V$raw); T_use_prl_u_vals$(A$raw);
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_base_vals__included */
