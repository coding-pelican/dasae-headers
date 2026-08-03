#pragma once
#ifndef proc_private_share__included
#define proc_private_share__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/proc/Self.h"
#include "dh/proc/NativeStrList.h"

/*========== Macros and Declarations ========================================*/

$attr($must_check)
$extern fn_((proc__growScratch(S$u8* self, mem_Alctr gpa))(mem_E$void));

errset_((proc__cmdLineWTF16_E)() $union_errset_(
    mem_E,
    proc_InvalidNameE
));
T_use_E$($set(proc__cmdLineWTF16_E)(S$u16));
$attr($must_check)
$extern fn_((proc__cmdLineWTF16(mem_Alctr gpa, S$S_const$u8 argv))(proc__cmdLineWTF16_E$S$u16));

errset_((proc__envWTF16_E)() $union_errset_(
    mem_E,
    proc_InvalidNameE
));
T_use_E$($set(proc__envWTF16_E)(S$u16));
$attr($must_check)
$extern fn_((proc__envWTF16(mem_Alctr gpa, proc_Env inherited, O$proc_cmd_Env override))(proc__envWTF16_E$S$u16));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_private_share__included */
