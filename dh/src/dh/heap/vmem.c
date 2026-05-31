#include "dh/heap/vmem.h"

#if plat_is_windows
#include "dh/sys/api/windows/mem.h"
$static fn_((heap_vmem__windowsProtect(heap_vmem_Protn protect))(DWORD));
#elif plat_is_linux
#include "dh/sys/call/linux.h"
$static fn_((heap_vmem__linuxProtect(heap_vmem_Protn protect))(sys_call_linux_word));
#endif

fn_((heap_vmem_geom(void))(heap_Geom)) {
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

#if plat_is_windows
fn_((heap_vmem__windowsProtect(heap_vmem_Protn protect))(DWORD)) {
    switch (protect) {
    case heap_vmem_Protn_none: return PAGE_NOACCESS;
    case heap_vmem_Protn_read_write: return PAGE_READWRITE;
    case heap_vmem_Protn_read_write_guard: return PAGE_READWRITE | PAGE_GUARD;
    }
};
#elif plat_is_linux
fn_((heap_vmem__linuxProtect(heap_vmem_Protn protect))(sys_call_linux_word)) {
    switch (protect) {
    case heap_vmem_Protn_none: return sys_call_linux_PROT_NONE;
    case heap_vmem_Protn_read_write: return sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE;
    case heap_vmem_Protn_read_write_guard: return sys_call_linux_PROT_NONE;
    }
};
#endif

fn_((heap_vmem_reserve(P$raw addr_hint, usize len))(O$P$u8) $scope) {
    let aligned_len = heap_Geom_alignReserveWith(heap_vmem_geom(), len);
#if plat_is_windows
    let addr = VirtualAlloc(addr_hint, aligned_len, MEM_RESERVE, PAGE_NOACCESS);
    return_(expr_(ReturnType $scope)(
        addr == null ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#elif plat_is_linux
    let mapped = sys_call_linux_mmap(addr_hint, aligned_len, sys_call_linux_PROT_NONE, sys_call_linux_MAP_PRIVATE | sys_call_linux_MAP_ANONYMOUS, -1, 0);
    return_(expr_(ReturnType $scope)(
        sys_call_linux_syscall_isErr(mapped) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(mapped))))
    ) $unscoped(expr));
#else
    let_ignore = addr_hint;
    let_ignore = aligned_len;
    return_none();
#endif
} $unscoped(fn);

fn_((heap_vmem_commit(P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(heap_vmem_geom(), len);
#if plat_is_windows
    return VirtualAlloc(addr, aligned_len, MEM_COMMIT, PAGE_READWRITE) != null;
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};

fn_((heap_vmem_decommit(P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(heap_vmem_geom(), len);
#if plat_is_windows
    return VirtualFree(addr, aligned_len, MEM_DECOMMIT);
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, sys_call_linux_PROT_NONE) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};

fn_((heap_vmem_protect(P$raw addr, usize len, heap_vmem_Protn protect))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(heap_vmem_geom(), len);
#if plat_is_windows
    DWORD old_protect = 0;
    return VirtualProtect(addr, aligned_len, heap_vmem__windowsProtect(protect), &old_protect);
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, heap_vmem__linuxProtect(protect)) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    let_ignore = protect;
    return false;
#endif
};

fn_((heap_vmem_release(P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignReserveWith(heap_vmem_geom(), len);
#if plat_is_windows
    let_ignore = aligned_len;
    return VirtualFree(addr, 0, MEM_RELEASE);
#elif plat_is_linux
    return sys_call_linux_munmap(addr, aligned_len) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};
