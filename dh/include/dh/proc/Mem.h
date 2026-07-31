#pragma once
#ifndef proc_Mem__included
#define proc_Mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((proc_MemLock_Opts)(struct proc_MemLock_Opts {
    var_(on_fault, bool);
}));
$static const proc_MemLock_Opts proc_MemLock_Opts_default = {
    .on_fault = false,
};

T_alias$((proc_MemLockAll_Opts)(struct proc_MemLockAll_Opts {
    var_(current, bool);
    var_(future, bool);
    var_(on_fault, bool);
}));
$static const proc_MemLockAll_Opts proc_MemLockAll_Opts_default = {
    .current = false,
    .future = false,
    .on_fault = false,
};

$attr($must_check)
$extern fn_((proc_lockMemory(S_const$u8 memory, proc_MemLock_Opts opts))(proc_MemLock_E$void));
$attr($must_check)
$extern fn_((proc_unlockMemory(S_const$u8 memory))(proc_MemUnlock_E$void));
$attr($must_check)
$extern fn_((proc_lockMemoryAll(proc_MemLockAll_Opts opts))(proc_MemLock_E$void));
$attr($must_check)
$extern fn_((proc_unlockMemoryAll(void))(proc_MemUnlock_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Mem__included */
