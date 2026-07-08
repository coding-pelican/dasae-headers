/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file Select.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-30 (date of creation)
 * @updated 2026-06-30 (date of last update)
 * @ingroup dasae-headers(dh)/thrd
 * @prefix  thrd_Select
 * @brief   Heterogeneous OS-thread blocking wait selection surface.
 */
#pragma once
#ifndef thrd_Select__included
#define thrd_Select__included 1
#if defined(__cplusplus)
extern "C" {
#endif

#include "base.h"
#include "CancelTok.h"
#include "wait.h"

typedef struct thrd_Select_Case {
    var_(field_idx, u32);
    var_(src, thrd_wait_Src);
    var_(result, u_P$raw);
    var_(link, thrd_wait_Link);
} thrd_Select_Case;
T_use_prl$(thrd_Select_Case);
typedef struct thrd_Select {
    var_(cases, S$thrd_Select_Case);
    var_(len, usize);
    var_(type, debug_TypeInfo);
} thrd_Select;

$extern fn_((thrd_Select_init(TypeInfo union_ty, S$thrd_Select_Case cases))(thrd_Select));
$extern fn_((thrd_Select_addSrc(thrd_Select* self, u32 field_idx, thrd_wait_Src src, u_P$raw result))(void));
$extern fn_((thrd_Select_poll(thrd_Select* self, u_V$raw ret_mem))(bool));
$attr($must_check)
$extern fn_((thrd_Select_wait(thrd_Select* self, u_V$raw ret_mem))(Sched_Cancelable$u_V$raw));
$attr($must_check)
$extern fn_((thrd_Select_waitFor(thrd_Select* self, time_Dur dur, u_V$raw ret_mem))(Sched_TimedE$V$raw));

#if defined(__cplusplus)
}
#endif
#endif
