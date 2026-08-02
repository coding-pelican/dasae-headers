#pragma once
#ifndef proc_private_share__included
#define proc_private_share__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/ArrList.h"
#include "dh/proc/Self.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc__NativeStrs)(ArrList));
T_use_E$($set(mem_E)(proc__NativeStrs));
T_use_E$($set(proc_Spawn_E)(proc__NativeStrs));

errset_((proc__envWTF16_E)() $union_errset_(
    mem_E,
    proc_InvalidName_E
));
T_use_E$($set(proc__envWTF16_E)(S$u16));

$attr($must_check)
$extern fn_((proc__growScratch(
    S$u8* self,
    mem_Alctr gpa
))(mem_E$void));
$extern fn_((proc__NativeStrs_fini(
    proc__NativeStrs* self,
    mem_Alctr gpa
))(void));
$attr($must_check)
$extern fn_((proc__NativeStrs_from(
    mem_Alctr gpa,
    S$S_const$u8 items
))(mem_E$proc__NativeStrs));
$attr($must_check)
$extern fn_((proc__NativeStrs_fromEnv(
    mem_Alctr gpa,
    proc_Env inherited,
    O$proc_Spawn_Env override
))(mem_E$proc__NativeStrs));
$extern fn_((proc__NativeStrs_raw(proc__NativeStrs self))(char**));
$extern fn_((proc__NativeStrs_at(
    proc__NativeStrs self,
    usize idx
))(S_const$u8));

$attr($must_check)
$extern fn_((proc__envWTF16(
    mem_Alctr gpa,
    proc_Env inherited,
    O$proc_Spawn_Env override
))(proc__envWTF16_E$S$u16));

$extern fn_((proc__child(
    proc_Child_Handle handle,
    proc_Child_Id id,
    proc_Child_IO io
))(proc_Child));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_private_share__included */
