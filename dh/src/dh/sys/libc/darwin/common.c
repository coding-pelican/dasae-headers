#include "dh/sys/libc/darwin/common.h"

#if plat_is_darwin
#include <sys/types.h>

claim_assert_static(sizeOf$(sys_libc_darwin_fd_t) == sizeof(int));
claim_assert_static(alignOf$(sys_libc_darwin_fd_t) == __alignof__(int));
claim_assert_static(sizeOf$(sys_libc_darwin_mode_t) == sizeof(mode_t));
claim_assert_static(alignOf$(sys_libc_darwin_mode_t) == __alignof__(mode_t));
claim_assert_static(sizeOf$(sys_libc_darwin_signal_t) == sizeof(int));
claim_assert_static(alignOf$(sys_libc_darwin_signal_t) == __alignof__(int));
claim_assert_static(sizeOf$(sys_libc_darwin_ioctl_req_t) == sizeof(unsigned long));
claim_assert_static(alignOf$(sys_libc_darwin_ioctl_req_t) == __alignof__(unsigned long));
#endif /* plat_is_darwin */
