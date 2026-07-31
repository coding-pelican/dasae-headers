#pragma once
#ifndef proc_common__included
#define proc_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_AccessDenied_E)(proc_AccessDenied));
errset_((proc_FileNotFound_E)(proc_FileNotFound));
errset_((proc_InvalidExe_E)(proc_InvalidExe));
errset_((proc_InvalidName_E)(proc_InvalidName));
errset_((proc_NotDir_E)(proc_NotDir));
errset_((proc_OperationUnsupported_E)(proc_OperationUnsupported));
errset_((proc_PermissionDenied_E)(proc_PermissionDenied));
errset_((proc_ProcessAlreadyExited_E)(proc_ProcessAlreadyExited));
errset_((proc_ResourceLimitReached_E)(proc_ResourceLimitReached));
errset_((proc_SystemResources_E)(proc_SystemResources));

errset_((proc_Path_E)() $union_errset_(
    proc_AccessDenied_E,
    proc_FileNotFound_E,
    proc_InvalidName_E,
    proc_NotDir_E,
    proc_OperationUnsupported_E,
    proc_ResourceLimitReached_E,
    proc_SystemResources_E
));
T_use_E$($set(proc_Path_E)(S$u8));

errset_((proc_Spawn_E)() $union_errset_(
    proc_Path_E,
    proc_InvalidExe_E,
    proc_PermissionDenied_E
));

errset_((proc_Child_Wait_E)() $union_errset_(
    proc_AccessDenied_E,
    proc_OperationUnsupported_E,
    proc_ProcessAlreadyExited_E,
    proc_SystemResources_E
));

errset_((proc_Args_E)() $union_errset_(proc_ResourceLimitReached_E));
T_useBy$(($spec(E, $set(proc_Args_E)))(O$S_const$u8, bool));

errset_((proc_Env_E)() $union_errset_(proc_ResourceLimitReached_E));
T_use_E$($set(proc_Env_E)(O$S_const$u8));

errset_((proc_Direct_E)(proc_Direct_Unsupported));

errset_((proc_MemLock_E)(
    proc_MemLock_Unsupported,
    proc_MemLock_PermissionDenied,
    proc_MemLock_LimitExceeded,
    proc_MemLock_SystemResources
));

errset_((proc_MemUnlock_E)(
    proc_MemUnlock_PermissionDenied,
    proc_MemUnlock_OutOfMemory,
    proc_MemUnlock_SystemResources
));

errset_((proc_E)() $union_errset_(
    proc_Path_E,
    proc_Spawn_E,
    proc_Child_Wait_E,
    proc_Args_E,
    proc_Env_E,
    proc_Direct_E,
    proc_MemLock_E,
    proc_MemUnlock_E
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_common__included */
