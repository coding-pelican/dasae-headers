#include "dh/ptr.h"
#include "dh/fn.h"

#if bti_plat_windows
#include "dh/os/windows/mem.h"
#else /* posix */
#include <sys/mman.h>
#include <unistd.h>
#endif /* posix */

fn_(Ptr_constCastable(anyptr_const ptr), bool) {
    debug_assert_nonnull(ptr);
#if bti_plat_windows
    MEMORY_BASIC_INFORMATION mbi = cleared();
    if (!VirtualQuery(ptr, &mbi, sizeOf(mbi))) { return false; }
    return (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)) != 0;
#else  /* posix */
    return !mprotect(
        intToRawptr$(anyptr, rawptrToInt(ptr) & ~(sysconf(_SC_PAGESIZE) - 1)),
        1,
        PROT_READ | PROT_WRITE
    );
#endif /* posix */
}
