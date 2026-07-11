/**
 * @file    conc/Group.h
 * @ingroup dasae-headers(dh)/conc
 * @prefix  conc_Group
 */
#pragma once
#ifndef conc_Group__included
#define conc_Group__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "base.h"
#include "../Future.h"
#include "../Sched.h"
#include "../ArrList.h"
#include "../mem/Alctr.h"
#include "../clsr.h"

T_alias$((conc_Group)(struct conc_Group));
errset_((conc_Group_SpawnE)() $union_errset_(mem_E, Sched_ConcE));
T_use_ArrList$(P$Future$raw);

struct conc_Group {
    var_(futs, ArrList$P$Future$raw);
    var_(cancel_requested, atom_V$bool);
    var_(closed, bool);
};
T_use_E$($set(mem_E)(conc_Group));

$extern fn_((conc_Group_init(mem_Alctr gpa, usize cap))(E$($set(mem_E)(conc_Group))));
$extern fn_((conc_Group_empty(void))(conc_Group));
$extern fn_((conc_Group_fini(conc_Group* self, mem_Alctr gpa))(void));
$extern fn_((conc_Group_add(conc_Group* self, mem_Alctr gpa, V$Future$raw fut))(mem_E$void));
$extern fn_((conc_Group_async(conc_Group* self, mem_Alctr gpa, Sched sched, Clsr$raw* clsr))(mem_E$void));
$attr($must_check)
$extern fn_((conc_Group_spawn(conc_Group* self, mem_Alctr gpa, Sched sched, Clsr$raw* clsr))(E$($set(conc_Group_SpawnE)(void))));
$attr($must_check)
$extern fn_((conc_Group_await(conc_Group* self, Sched sched))(Sched_Cancelable$void));
$extern fn_((conc_Group_cancel(conc_Group* self, Sched sched))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* conc_Group__included */
