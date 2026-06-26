#include "dh/sys/libc/darwin/mem.h"

#if plat_is_darwin
#include <sys/mman.h>

claim_assert_static(sys_libc_darwin_PROT_NONE == PROT_NONE);
claim_assert_static(sys_libc_darwin_PROT_READ == PROT_READ);
claim_assert_static(sys_libc_darwin_PROT_WRITE == PROT_WRITE);
claim_assert_static(sys_libc_darwin_MAP_PRIVATE == MAP_PRIVATE);
claim_assert_static(sys_libc_darwin_MAP_ANONYMOUS == MAP_ANON);
#endif /* plat_is_darwin */

fn_((sys_libc_darwin_mmap(P$raw addr, usize len, i32 prot, i32 flags, i32 fd, isize offset))(P$raw)) {
#if plat_is_darwin
    return mmap(addr, len, prot, flags, fd, offset);
#else
    let_ignore = addr;
    let_ignore = len;
    let_ignore = prot;
    let_ignore = flags;
    let_ignore = fd;
    let_ignore = offset;
    claim_unreachable_msg(nameOf(sys_libc_darwin_mmap) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_munmap(P$raw addr, usize len))(i32)) {
#if plat_is_darwin
    return munmap(addr, len);
#else
    let_ignore = addr;
    let_ignore = len;
    claim_unreachable_msg(nameOf(sys_libc_darwin_munmap) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_mprotect(P$raw addr, usize len, i32 prot))(i32)) {
#if plat_is_darwin
    return mprotect(addr, len, prot);
#else
    let_ignore = addr;
    let_ignore = len;
    let_ignore = prot;
    claim_unreachable_msg(nameOf(sys_libc_darwin_mprotect) "is not supported on this platform");
#endif /* plat_is_darwin */
};
