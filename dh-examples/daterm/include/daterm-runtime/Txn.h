/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Txn.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-20 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup daterm-runtime
 * @prefix  daterm_Txn
 */
#pragma once
#ifndef daterm_Txn__included
#define daterm_Txn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/Writer.h>
#include <dh/time/Dur.h>
#include <dh/Sched.h>
#include <dansi-core/Seq.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((daterm_TxnMatch $fits($packed))(
    daterm_TxnMatch_no,
    daterm_TxnMatch_done
)) daterm_TxnMatch;
T_use_prl$(daterm_TxnMatch);

errset_((daterm_Txn_E)(
    daterm_Txn_PendingFull,
    daterm_Txn_Unsupported
) $union_errset_(Sched_TimeoutE));

typedef struct daterm_Txn {
    var_(timeout, time_Dur);
    var_(ctx, P$raw);
    var_(out, P$raw);
    $attr($must_check)
    fn_(((*requestWriteFn)(P$raw ctx, io_Writer out))(E$void));
    $attr($must_check)
    fn_(((*matchFn)(P$raw ctx, dansi_Seq seq, P$raw out))(E$daterm_TxnMatch));
} daterm_Txn;
T_use_prl$(daterm_Txn);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Txn__included */
