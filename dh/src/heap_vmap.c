#include "dh/heap/vmap.h"

#if plat_is_windows
#include "dh/os/windows/mem.h"
#elif plat_based_unix
#include <sys/mman.h>
#endif

fn_((heap_vmap_map(P$raw addr_hint, usize len))(O$P$u8) $scope) {
    let aligned_len = heap_alignPage(len);
#if plat_is_windows
    let addr = VirtualAlloc(addr_hint, aligned_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return_(expr_(ReturnType $scope)(
        addr == null ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#elif plat_based_unix
    let addr = mmap(addr_hint, aligned_len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return_(expr_(ReturnType $scope)(
        addr == MAP_FAILED ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#else
    let_ignore = addr_hint;
    let_ignore = aligned_len;
    return_none();
#endif
} $unscoped(fn);

fn_((heap_vmap_release(P$raw addr, usize len))(bool)) {
    let aligned_len = heap_alignPage(len);
#if plat_is_windows
    let_ignore = aligned_len;
    return VirtualFree(addr, 0, MEM_RELEASE);
#elif plat_based_unix
    return munmap(addr, aligned_len) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};

fn_((heap_vmap_remap(P$raw addr, usize old_len, usize new_len))(O$P$u8) $scope) {
#if plat_is_linux
    let new_addr = mremap(addr, heap_alignPage(old_len), heap_alignPage(new_len), MREMAP_MAYMOVE);
    return new_addr == MAP_FAILED ? none() : some(new_addr);
#endif
    let_ignore = addr;
    let_ignore = old_len;
    let_ignore = new_len;
    return_none();
} $unscoped(fn);
