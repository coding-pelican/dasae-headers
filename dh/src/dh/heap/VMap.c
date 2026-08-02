#include "dh/heap/VMap.h"

#if plat_is_windows
#include "dh/sys/api/windows/mem.h"
#elif plat_is_linux
#include "dh/sys/call/linux.h"
#elif plat_is_darwin
#include "dh/sys/libc/darwin/mem.h"
#endif

/*========== Internal Declarations ==========================================*/

$static fn_((heap_VMap__system_map(const heap_VMap_Ctx* ctx, O$P$raw addr_hint, usize len))(O$P$u8));
$static fn_((heap_VMap__system_release(const heap_VMap_Ctx* ctx, P$raw addr, usize len))(bool));
$static fn_((heap_VMap__system_remap(const heap_VMap_Ctx* ctx, P$raw addr, usize old_len, usize new_len))(O$P$u8));

/*========== External Definitions ===========================================*/

fn_((heap_VMap_system(void))(heap_VMap_E$heap_VMap) $scope) {
#if plat_is_windows || plat_is_linux || plat_is_darwin
    $static var_(inner, Void) $undefined_static;
    $static var_(ctx, O$$(heap_VMap_Ctx)) = none();
    $static let_(vtbl, heap_VMap_VTbl) = {
        .mapFn = heap_VMap__system_map,
        .releaseFn = heap_VMap__system_release,
        .remapFn = heap_VMap__system_remap,
    };
    let ctx_initialized = orelse_((O_ref(&ctx))(local_({
        let geom = catch_((heap_Geom_system())(err, switch (E_tag$heap_Geom_E(err)) {
            case_((E_Tag$heap_Geom_Unsupported)) return_err(E_cause$heap_VMap_Unsupported()) $end(case);
            case_((E_Tag$Any)) claim_unreachable $end(case);
        }));
        asg_l((&ctx)(some({ .inner = &inner, .geom = geom })));
        local_return_(unwrap_(O_ref(&ctx)));
    })));
    return_ok(heap_VMap_ensureValid((heap_VMap){
        .ctx = ctx_initialized,
        .vtbl = &vtbl,
    }));
#else
    return_err(E_cause$heap_VMap_Unsupported());
#endif
} $unscoped(fn);

fn_((heap_VMap_geom(heap_VMap self))(heap_Geom)) {
    self = heap_VMap_ensureValid(self);
    return self.ctx->geom;
};

fn_((heap_VMap_map(heap_VMap self, O$P$raw addr_hint, usize len))(O$P$u8)) {
    self = heap_VMap_ensureValid(self);
    return self.vtbl->mapFn(self.ctx, addr_hint, len);
};

fn_((heap_VMap_release(heap_VMap self, P$raw addr, usize len))(bool)) {
    self = heap_VMap_ensureValid(self);
    return self.vtbl->releaseFn(self.ctx, addr, len);
};

fn_((heap_VMap_remap(heap_VMap self, P$raw addr, usize old_len, usize new_len))(O$P$u8)) {
    self = heap_VMap_ensureValid(self);
    return self.vtbl->remapFn(self.ctx, addr, old_len, new_len);
};

/*========== Internal Definitions ===========================================*/

fn_((heap_VMap__system_map(const heap_VMap_Ctx* ctx, O$P$raw addr_hint, usize len))(O$P$u8) $scope) {
    let hint = orelse_((addr_hint)(null));
    let aligned_len = heap_Geom_alignPageWith(ctx->geom, len);
#if plat_is_windows
    let addr = VirtualAlloc(hint, aligned_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return_(expr_(ReturnType $scope)(
        addr == null ? $break_(none()) : $break_(some(addr))
    ) $unscoped(expr));
#elif plat_is_linux
    let mapped = sys_call_linux_mmap(hint, aligned_len, sys_call_linux_PROT_READ | sys_call_linux_PROT_WRITE, sys_call_linux_MAP_PRIVATE | sys_call_linux_MAP_ANONYMOUS, -1, 0);
    return_(expr_(ReturnType $scope)(
        sys_call_linux_syscall_isErr(mapped) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(mapped))))
    ) $unscoped(expr));
#elif plat_is_darwin
    let mapped = sys_libc_darwin_mmap(
        hint,
        aligned_len,
        sys_libc_darwin_PROT_READ | sys_libc_darwin_PROT_WRITE,
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

fn_((heap_VMap__system_release(const heap_VMap_Ctx* ctx, P$raw addr, usize len))(bool)) {
    let aligned_len = heap_Geom_alignPageWith(ctx->geom, len);
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

fn_((heap_VMap__system_remap(const heap_VMap_Ctx* ctx, P$raw addr, usize old_len, usize new_len))(O$P$u8) $scope) {
#if plat_is_linux
    let new_addr = sys_call_linux_mremap(
        addr,
        heap_Geom_alignPageWith(ctx->geom, old_len),
        heap_Geom_alignPageWith(ctx->geom, new_len),
        sys_call_linux_MREMAP_MAYMOVE,
        null
    );
    return_(expr_(ReturnType $scope)(
        sys_call_linux_syscall_isErr(new_addr) ? $break_(none()) : $break_(some(intToPtr$((P$raw)(new_addr))))
    ) $unscoped(expr));
#else
    let_ignore = ctx;
#endif
    let_ignore = addr;
    let_ignore = old_len;
    let_ignore = new_len;
    return_none();
} $unscoped(fn);
