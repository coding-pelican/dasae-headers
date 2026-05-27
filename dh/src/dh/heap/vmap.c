#include "dh/heap/vmap.h"

#if plat_is_windows
#include "dh/os/windows/mem.h"
#elif plat_is_linux
#include "dh/os/linux/syscall.h"
#elif plat_based_unix
#include <sys/mman.h>
#endif

fn_((heap_vmap_geom(void))(heap_Geom)) {
#if plat_is_windows
    return (heap_Geom){
        .page_size = mem_page_size,
        .reserve_align = 64ull * 1024,
        .commit_align = mem_page_size,
        .map_align = 64ull * 1024,
    };
#else
    return heap_Geom_from(mem_page_size);
#endif
};

fn_((heap_vmap_map(P$raw addr_hint, usize len))(O$P$u8) $scope) {
    let aligned_len = heap_Geom_alignPageWith(heap_vmap_geom(), len);
#if plat_is_windows
    let addr = VirtualAlloc(addr_hint, aligned_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return_(expr_(ReturnType $scope)(
        addr == null ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#elif plat_is_linux
    let mapped = os_linux_mmap(addr_hint, aligned_len, os_linux_PROT_READ | os_linux_PROT_WRITE, os_linux_MAP_PRIVATE | os_linux_MAP_ANONYMOUS, -1, 0);
    return_(expr_(ReturnType $scope)(
        os_linux_syscall_isErr(mapped) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(mapped))))
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
    let aligned_len = heap_Geom_alignPageWith(heap_vmap_geom(), len);
#if plat_is_windows
    let_ignore = aligned_len;
    return VirtualFree(addr, 0, MEM_RELEASE);
#elif plat_is_linux
    return os_linux_munmap(addr, aligned_len) == 0;
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
    let geometry = heap_vmap_geom();
    let new_addr = os_linux_mremap(
        addr,
        heap_Geom_alignPageWith(geometry, old_len),
        heap_Geom_alignPageWith(geometry, new_len),
        os_linux_MREMAP_MAYMOVE,
        null
    );
    return_(expr_(ReturnType $scope)(
        os_linux_syscall_isErr(new_addr) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(new_addr))))
    ) $unscoped(expr));
#endif
    let_ignore = addr;
    let_ignore = old_len;
    let_ignore = new_len;
    return_none();
} $unscoped(fn);
