#include "dh/heap/VMem.h"

#if plat_is_windows
#include "dh/sys/api/windows/mem.h"
$static fn_((heap_VMem__windows_protect(heap_VMem_Protcn protect))(DWORD));
#elif plat_is_linux
#include "dh/sys/call/linux.h"
$static fn_((heap_VMem__linux_protect(heap_VMem_Protcn protect))(sys_call_linux_word));
#elif plat_is_darwin
#include "dh/sys/libc/darwin/mem.h"
$static fn_((heap_VMem__darwin_protect(heap_VMem_Protcn protect))(i32));
#endif

/*========== Internal Declarations ==========================================*/

$static fn_((heap_VMem_system__reserve(const heap_VMem_Ctx* ctx, O$P$raw addr_hint, usize len))(O$P$u8));
$static fn_((heap_VMem_system__commit(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));
$static fn_((heap_VMem_system__decommit(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));
$static fn_((heap_VMem_system__protect(const heap_VMem_Ctx* ctx, P$raw addr, usize len, heap_VMem_Protcn protect))(bool));
$static fn_((heap_VMem_system__release(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool));

/*========== External Definitions ===========================================*/

fn_((heap_VMem_system(void))(heap_VMem_E$heap_VMem) $scope) {
#if plat_is_windows || plat_is_linux || plat_is_darwin
    $static var_(inner, Void) $undefined_static;
    $static var_(ctx, heap_VMem_Ctx) $undefined_static;
    $static let_(vtbl, heap_VMem_VTbl) = {
        .reserveFn = heap_VMem_system__reserve,
        .commitFn = heap_VMem_system__commit,
        .decommitFn = heap_VMem_system__decommit,
        .protectFn = heap_VMem_system__protect,
        .releaseFn = heap_VMem_system__release,
    };
    let geom = catch_((heap_Geom_system())($ignore, return_err(E_cause$heap_VMem_Unsupported())));
    asg_l((&ctx)({ .inner = &inner, .geom = geom }));
    return_ok(heap_VMem_ensureValid((heap_VMem){
        .ctx = &ctx,
        .vtbl = &vtbl,
    }));
#else
    return_err(E_cause$heap_VMem_Unsupported());
#endif
} $unscoped(fn);

fn_((heap_VMem_geom(heap_VMem self))(heap_Geom)) {
    self = heap_VMem_ensureValid(self);
    return self.ctx->geom;
};

fn_((heap_VMem_reserve(heap_VMem self, O$P$raw addr_hint, usize len))(O$P$u8)) {
    self = heap_VMem_ensureValid(self);
    return self.vtbl->reserveFn(self.ctx, addr_hint, len);
};

fn_((heap_VMem_commit(heap_VMem self, P$raw addr, usize len))(bool)) {
    self = heap_VMem_ensureValid(self);
    return self.vtbl->commitFn(self.ctx, addr, len);
};

fn_((heap_VMem_decommit(heap_VMem self, P$raw addr, usize len))(bool)) {
    self = heap_VMem_ensureValid(self);
    return self.vtbl->decommitFn(self.ctx, addr, len);
};

fn_((heap_VMem_protect(heap_VMem self, P$raw addr, usize len, heap_VMem_Protcn protect))(bool)) {
    self = heap_VMem_ensureValid(self);
    return self.vtbl->protectFn(self.ctx, addr, len, protect);
};

fn_((heap_VMem_release(heap_VMem self, P$raw addr, usize len))(bool)) {
    self = heap_VMem_ensureValid(self);
    return self.vtbl->releaseFn(self.ctx, addr, len);
};

/*========== Internal Definitions ===========================================*/

#if plat_is_windows
fn_((heap_VMem__windows_protect(heap_VMem_Protcn protect))(DWORD)) {
    switch (protect) {
    case heap_VMem_Protcn_none: return PAGE_NOACCESS;
    case heap_VMem_Protcn_read_write: return PAGE_READWRITE;
    case heap_VMem_Protcn_read_write_guard: return PAGE_READWRITE | PAGE_GUARD;
    }
};
#elif plat_is_linux
fn_((heap_VMem__linux_protect(heap_VMem_Protcn protect))(sys_call_linux_word)) {
    switch (protect) {
    case heap_VMem_Protcn_none: return sys_call_linux_PROT_NONE;
    case heap_VMem_Protcn_read_write: return sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE;
    case heap_VMem_Protcn_read_write_guard: return sys_call_linux_PROT_NONE;
    }
};
#elif plat_is_darwin
fn_((heap_VMem__darwin_protect(heap_VMem_Protcn protect))(i32)) {
    switch (protect) {
    case heap_VMem_Protcn_none: return sys_libc_darwin_PROT_NONE;
    case heap_VMem_Protcn_read_write: return sys_libc_darwin_PROT_READ | sys_libc_darwin_PROT_WRITE;
    case heap_VMem_Protcn_read_write_guard: return sys_libc_darwin_PROT_NONE;
    }
};
#endif

fn_((heap_VMem_system__reserve(const heap_VMem_Ctx* ctx, O$P$raw addr_hint, usize len))(O$P$u8) $scope) {
    let hint = orelse_((addr_hint)(null));
    let aligned_len = heap_Geom_alignReserveWith(ctx->geom, len);
#if plat_is_windows
    let addr = VirtualAlloc(hint, aligned_len, MEM_RESERVE, PAGE_NOACCESS);
    return_(expr_(ReturnType $scope)(
        addr == null ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#elif plat_is_linux
    let mapped = sys_call_linux_mmap(hint, aligned_len, sys_call_linux_PROT_NONE, sys_call_linux_MAP_PRIVATE | sys_call_linux_MAP_ANONYMOUS, -1, 0);
    return_(expr_(ReturnType $scope)(
        sys_call_linux_syscall_isErr(mapped) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(mapped))))
    ) $unscoped(expr));
#elif plat_is_darwin
    let mapped = sys_libc_darwin_mmap(
        hint,
        aligned_len,
        sys_libc_darwin_PROT_NONE,
        sys_libc_darwin_MAP_PRIVATE | sys_libc_darwin_MAP_ANONYMOUS,
        -1,
        0
    );
    return_(expr_(ReturnType $scope)(
        mapped == sys_libc_darwin_MAP_FAILED ? $break_(none()) : $break_(some(mapped))
    ) $unscoped(expr));
#else
    let_ignore = hint;
    let_ignore = aligned_len;
    return_none();
#endif
} $unscoped(fn);

fn_((heap_VMem_system__commit(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(ctx->geom, len);
#if plat_is_windows
    return VirtualAlloc(addr, aligned_len, MEM_COMMIT, PAGE_READWRITE) != null;
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE) == 0;
#elif plat_is_darwin
    return sys_libc_darwin_mprotect(addr, aligned_len, sys_libc_darwin_PROT_READ | sys_libc_darwin_PROT_WRITE) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};

fn_((heap_VMem_system__decommit(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(ctx->geom, len);
#if plat_is_windows
    return VirtualFree(addr, aligned_len, MEM_DECOMMIT);
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, sys_call_linux_PROT_NONE) == 0;
#elif plat_is_darwin
    return sys_libc_darwin_mprotect(addr, aligned_len, sys_libc_darwin_PROT_NONE) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};

fn_((heap_VMem_system__protect(const heap_VMem_Ctx* ctx, P$raw addr, usize len, heap_VMem_Protcn protect))(bool)) {
    let aligned_len = heap_Geom_alignCommitWith(ctx->geom, len);
#if plat_is_windows
    DWORD old_protect = 0;
    return VirtualProtect(addr, aligned_len, heap_VMem__windows_protect(protect), &old_protect);
#elif plat_is_linux
    return sys_call_linux_mprotect(addr, aligned_len, heap_VMem__linux_protect(protect)) == 0;
#elif plat_is_darwin
    return sys_libc_darwin_mprotect(addr, aligned_len, heap_VMem__darwin_protect(protect)) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    let_ignore = protect;
    return false;
#endif
};

fn_((heap_VMem_system__release(const heap_VMem_Ctx* ctx, P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignReserveWith(ctx->geom, len);
#if plat_is_windows
    let_ignore = aligned_len;
    return VirtualFree(addr, 0, MEM_RELEASE);
#elif plat_is_linux
    return sys_call_linux_munmap(addr, aligned_len) == 0;
#elif plat_is_darwin
    return sys_libc_darwin_munmap(addr, aligned_len) == 0;
#else
    let_ignore = addr;
    let_ignore = aligned_len;
    return false;
#endif
};
