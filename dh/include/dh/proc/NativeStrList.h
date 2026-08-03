#pragma once
#ifndef proc_NativeStrList__included
#define proc_NativeStrList__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"
#include "../ArrList.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_NativeStrList)(ArrList));
T_use_E$($set(mem_E)(proc_NativeStrList));

$attr($must_check)
$extern fn_((proc_NativeStrList_from(mem_Alctr gpa, S$S_const$u8 items))(mem_E$proc_NativeStrList));
$attr($must_check)
$extern fn_((proc_NativeStrList_fromEnv(mem_Alctr gpa, proc_Env inherited, O$proc_cmd_Env override))(mem_E$proc_NativeStrList));
$extern fn_((proc_NativeStrList_init(void))(proc_NativeStrList));
$extern fn_((proc_NativeStrList_fini(proc_NativeStrList* self, mem_Alctr gpa))(void));

$extern fn_((proc_NativeStrList_raw(proc_NativeStrList self))(char**));
$extern fn_((proc_NativeStrList_len(proc_NativeStrList self))(usize));
$extern fn_((proc_NativeStrList_at(proc_NativeStrList self, usize idx))(S_const$u8));

/// Add an item to the end of the list.
$attr($must_check)
$extern fn_((proc_NativeStrList_append(proc_NativeStrList* self, mem_Alctr gpa, S_const$u8 item))(mem_E$void));
/// Add a null terminator to the end of the list.
$attr($must_check)
$extern fn_((proc_NativeStrList_finish(proc_NativeStrList* self, mem_Alctr gpa))(mem_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_NativeStrList__included */
