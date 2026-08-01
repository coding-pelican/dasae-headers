#pragma once
#ifndef proc_common__included
#define proc_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../mem/base.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_AccessDenied_E)(proc_AccessDenied));
errset_((proc_FileBusy_E)(proc_FileBusy));
errset_((proc_FileSystem_E)(proc_FileSystem));
errset_((proc_FileNotFound_E)(proc_FileNotFound));
errset_((proc_InvalidExe_E)(proc_InvalidExe));
errset_((proc_InvalidName_E)(proc_InvalidName));
errset_((proc_IsDir_E)(proc_IsDir));
errset_((proc_NameTooLong_E)(proc_NameTooLong));
errset_((proc_NoDevice_E)(proc_NoDevice));
errset_((proc_NotDir_E)(proc_NotDir));
errset_((proc_OperationUnsupported_E)(proc_OperationUnsupported));
errset_((proc_PermissionDenied_E)(proc_PermissionDenied));
errset_((proc_ProcessFdQuotaExceeded_E)(proc_ProcessFdQuotaExceeded));
errset_((proc_ResourceLimitReached_E)(proc_ResourceLimitReached));
errset_((proc_SymLinkLoop_E)(proc_SymLinkLoop));
errset_((proc_SystemFdQuotaExceeded_E)(proc_SystemFdQuotaExceeded));
errset_((proc_SystemResources_E)(proc_SystemResources));

errset_((proc_ExecutablePath_E)() $union_errset_(
    proc_AccessDenied_E,
    proc_FileSystem_E,
    proc_FileNotFound_E,
    proc_OperationUnsupported_E,
    proc_ResourceLimitReached_E,
    proc_SymLinkLoop_E,
    proc_SystemResources_E
));
T_use_E$($set(proc_ExecutablePath_E)(S$u8));
errset_((proc_ExecutablePathAlloc_E)() $union_errset_(
    proc_ExecutablePath_E,
    mem_E
));
T_use_E$($set(proc_ExecutablePathAlloc_E)(S$u8));

errset_((proc_CurrentPath_E)() $union_errset_(
    proc_AccessDenied_E,
    proc_FileNotFound_E,
    proc_OperationUnsupported_E,
    proc_ResourceLimitReached_E,
    proc_SystemResources_E
));
T_use_E$($set(proc_CurrentPath_E)(S$u8));
errset_((proc_CurrentPathAlloc_E)() $union_errset_(
    proc_CurrentPath_E,
    mem_E
));
T_use_E$($set(proc_CurrentPathAlloc_E)(S$u8));

errset_((proc_SetCurrentPath_E)() $union_errset_(
    mem_E,
    proc_AccessDenied_E,
    proc_FileNotFound_E,
    proc_FileSystem_E,
    proc_InvalidName_E,
    proc_NameTooLong_E,
    proc_NotDir_E,
    proc_OperationUnsupported_E,
    proc_ResourceLimitReached_E,
    proc_SymLinkLoop_E,
    proc_SystemResources_E
));

errset_((proc_SetCurrentDir_E)() $union_errset_(
    mem_E,
    proc_AccessDenied_E,
    proc_FileSystem_E,
    proc_FileNotFound_E,
    proc_NotDir_E,
    proc_OperationUnsupported_E,
    proc_ResourceLimitReached_E,
    proc_SystemResources_E
));

errset_((proc_Spawn_E)() $union_errset_(
    proc_SetCurrentPath_E,
    mem_E,
    proc_FileBusy_E,
    proc_FileSystem_E,
    proc_InvalidExe_E,
    proc_IsDir_E,
    proc_NoDevice_E,
    proc_PermissionDenied_E,
    proc_ProcessFdQuotaExceeded_E,
    proc_SystemFdQuotaExceeded_E
));

errset_((proc_Replace_E)() $union_errset_(
    mem_E,
    proc_AccessDenied_E,
    proc_FileBusy_E,
    proc_FileSystem_E,
    proc_FileNotFound_E,
    proc_InvalidExe_E,
    proc_InvalidName_E,
    proc_IsDir_E,
    proc_NameTooLong_E,
    proc_NotDir_E,
    proc_OperationUnsupported_E,
    proc_PermissionDenied_E,
    proc_ProcessFdQuotaExceeded_E,
    proc_ResourceLimitReached_E,
    proc_SystemFdQuotaExceeded_E,
    proc_SystemResources_E
));

errset_((proc_Child_Wait_E)() $union_errset_(
    proc_AccessDenied_E,
    proc_SystemResources_E
));

errset_((proc_Args_E)() $union_errset_(proc_ResourceLimitReached_E));
T_useBy$(($spec(E, $set(proc_Args_E)))(O$S_const$u8, bool));

errset_((proc_Env_E)() $union_errset_(proc_ResourceLimitReached_E));
T_use_E$($set(proc_Env_E)(O$S_const$u8));

errset_((proc_Preopens_direct_E)(proc_Preopens_direct_Unsupported));

errset_((proc_Mem_direct_E)(proc_Mem_direct_Unsupported));

errset_((proc_Mem_Lock_E)(
    proc_Mem_Lock_Unsupported,
    proc_Mem_Lock_PermissionDenied,
    proc_Mem_Lock_LimitExceeded,
    proc_Mem_Lock_SystemResources
));

errset_((proc_Mem_Unlock_E)(
    proc_Mem_Unlock_Unsupported,
    proc_Mem_Unlock_PermissionDenied,
    proc_Mem_Unlock_OutOfMemory,
    proc_Mem_Unlock_SystemResources
));

errset_((proc_E)() $union_errset_(
    proc_ExecutablePath_E,
    proc_ExecutablePathAlloc_E,
    proc_CurrentPath_E,
    proc_CurrentPathAlloc_E,
    proc_SetCurrentDir_E,
    proc_SetCurrentPath_E,
    proc_Spawn_E,
    proc_Replace_E,
    proc_Child_Wait_E,
    proc_Args_E,
    proc_Env_E,
    proc_Preopens_direct_E,
    proc_direct_E,
    proc_Mem_direct_E,
    proc_Mem_Lock_E,
    proc_Mem_Unlock_E
));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_common__included */
