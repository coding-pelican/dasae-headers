#if UNUSED_CODE
#include "dh/ptr.h"
#include "dh/fn.h"

#if plat_windows
#include "dh/os/windows/mem.h"
#else /* posix */
#include <sys/mman.h>
#include <unistd.h>
#endif /* posix */

fn_((constCastable$P(P_const$raw ptr))(bool)) {
    debug_assert_nonnull(ptr);
#if plat_windows
    MEMORY_BASIC_INFORMATION mbi = cleared();
    if (!VirtualQuery(ptr, &mbi, sizeOf$(mbi))) { return false; }
    return (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)) != 0;
#else  /* posix */
    return !mprotect(
        intToPtr$(P$raw, ptrToInt(ptr) & ~(sysconf(_SC_PAGESIZE) - 1)),
        1,
        PROT_READ | PROT_WRITE
    );
#endif /* posix */
}

#endif /* UNUSED_CODE */
