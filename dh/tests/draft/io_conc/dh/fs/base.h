#ifndef fs_base__included
#define fs_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

T_alias$((fs_Handle)(posix_fd_t));
T_alias$((fs_INode)(u64));
T_alias$((fs_NLink)(u64));
T_alias$((fs_Mode)(posix_mode_t));
T_alias$((fs_BlockSize)(u32));

errset_((fs_direct_E)(Unsupported));
T_alias$((fs_Self)(struct fs_Self));
T_decl_E$($set(fs_direct_E)(fs_Self));

T_alias$((fs_Dir)(struct fs_Dir));
T_decl_E$(fs_Dir);

T_alias$((fs_File)(struct fs_File));
T_decl_E$(fs_File);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_base__included */
