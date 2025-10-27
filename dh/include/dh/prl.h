/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-21 (date of creation)
 * @updated 2025-06-21 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Prelude for dh
 * @details Provides a prelude for dh.
 */

#ifndef prl__included
#define prl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Range.h"
#include "dh/scope.h"
#include "dh/blk.h"
#include "dh/pipe.h"
#include "dh/callback.h"

#include "dh/types.h"
#include "dh/variant.h"
#include "dh/ErrTrace.h"

/*========== Macros and Declarations ========================================*/

#define prl_T_use$(_T...) \
    T_use$(_T, (P, S, O, E)); \
    T_use$(P$(const _T), (P, S, O, E)); \
    T_use$(P$(_T), (P, S, O, E)); \
    T_use$(S$(const _T), (P, S, O, E)); \
    T_use$(S$(_T), (P, S, O, E)); \
    T_use$(O$(_T), (P, S, O, E)); \
    T_use$(E$(_T), (P, S, O, E))

/*========== Macros and Definitions =========================================*/

// clang-format off
prl_T_use$(u8); prl_T_use$(u16); prl_T_use$(u32); prl_T_use$(u64); prl_T_use$(usize);
prl_T_use$(i8); prl_T_use$(i16); prl_T_use$(i32); prl_T_use$(i64); prl_T_use$(isize);
prl_T_use$(f32); prl_T_use$(f64);
prl_T_use$(bool);
prl_T_use$(meta_P$raw); prl_T_use$(meta_S$raw);
prl_T_use$(meta_V$raw); prl_T_use$(meta_A$raw);
// clang-format on

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl__included */
