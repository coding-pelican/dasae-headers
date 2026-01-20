#include "dh/heap/Sbrk.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_ctx(P$raw self))(heap_Sbrk_Ctx));
$attr($maybe_unused $inline_always)
$static fn_((heap_Sbrk_Sys__unsupported_sbrk(u_P$raw ctx, usize n))(usize));

/*========== External Definitions ===========================================*/

#if plat_is_windows
#include "dh/os/windows/mem.h"

fn_((heap_Sbrk_Sys_Windows_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Windows*)(ctx));
    let aligned_n = mem_alignFwd(n, heap_page_size);
    if (self->committed_size + aligned_n > self->reserved_size) {
        return 0; /* Out of reserved space */
    }

    let commit_addr = ptrToInt(self->base_addr) + self->committed_size;
    let result = VirtualAlloc(
        (intToPtr$((void*)(commit_addr))),
        aligned_n,
        MEM_COMMIT,
        PAGE_READWRITE
    );
    if (result == null) { return 0; /* Commit failed */ }

    let old_committed = self->committed_size;
    self->committed_size += aligned_n;
    return ptrToInt(self->base_addr) + old_committed;
};

fn_((heap_Sbrk_Sys_Windows_ctx(heap_Sbrk_Sys_Windows* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Windows_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Windows_init(usize reserve_size))(heap_Sbrk_Sys_Windows)) {
    claim_assert(reserve_size > 0);
    let aligned_size = mem_alignFwd(reserve_size, heap_page_size);
    let base = VirtualAlloc(null, aligned_size, MEM_RESERVE, PAGE_NOACCESS);
    if (base == null) { return lit0$((heap_Sbrk_Sys_Windows)); }
    return (heap_Sbrk_Sys_Windows){
        .base_addr = ptrToInt(base),
        .reserved_size = aligned_size,
        .committed_size = 0,
    };
};

fn_((heap_Sbrk_Sys_Windows_fini(heap_Sbrk_Sys_Windows* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        VirtualFree(intToPtr$((void*)(self->base_addr)), 0, MEM_RELEASE);
        self->base_addr = 0;
        self->committed_size = 0;
    }
    asg_lit((self)(cleared()));
};
#endif /* plat_is_windows */

#if plat_is_posix
#include <sys/mman.h>
#include <unistd.h>

fn_((heap_Sbrk_Sys_Posix_sbrk(u_P$raw ctx, usize n))(usize)) {
    let self = u_castP$((heap_Sbrk_Sys_Posix*)(ctx));
    let page_size = as$(usize)(sysconf(_SC_PAGESIZE));
    let aligned_n = mem_alignFwd(n, page_size);
    let result = mmap(
        null,
        aligned_n,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (result == MAP_FAILED) { return 0; /* mmap failed */ }

    let addr = ptrToInt(result);
    // Track first allocation as base
    if (self->base_addr == 0) { self->base_addr = addr; }
    self->mapped_size += aligned_n;
    return addr;
};

fn_((heap_Sbrk_Sys_Posix_ctx(heap_Sbrk_Sys_Posix* self))(heap_Sbrk_Ctx)) {
    claim_assert_nonnull(self);
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys_Posix_sbrk,
    };
};

fn_((heap_Sbrk_Sys_Posix_init(void))(heap_Sbrk_Sys_Posix)) {
    return (heap_Sbrk_Sys_Posix){
        .base_addr = 0,
        .mapped_size = 0,
    };
}

fn_((heap_Sbrk_Sys_Posix_fini(heap_Sbrk_Sys_Posix* self))(void)) {
    claim_assert_nonnull(self);
    if (self->base_addr != 0) {
        munmap(intToPtr$((void*)(self->base_addr)), self->mapped_size);
        self->base_addr = 0;
        self->mapped_size = 0;
    }
    asg_lit((self)(cleared()));
};
#endif /* plat_is_posix */

#if plat_is_wasi
fn_((heap_Sbrk_Sys_Wasm_sbrk(u_P$raw ctx, usize n))(usize)) {
    /* TODO: Implement */
    return heap_Sbrk_Sys__unsupported_sbrk(ctx, n);
};

fn_((heap_Sbrk_Sys_Wasm_ctx(heap_Sbrk_Sys_Wasm* self))(heap_Sbrk_Ctx)) {
    /* TODO: Implement */
    return heap_Sbrk_Sys__unsupported_ctx(self);
};

fn_((heap_Sbrk_Sys_Wasm_init(void))(heap_Sbrk_Sys_Wasm)) {
    /* TODO: Implement */
    return (heap_Sbrk_Sys_Wasm){
        .page_count = 0,
    };
};

fn_((heap_Sbrk_Sys_Wasm_fini(heap_Sbrk_Sys_Wasm* self))(void)) {
    asg_lit((self)(cleared()));
};
#endif /* plat_is_wasi */

/*========== Internal Definitions ===========================================*/

fn_((heap_Sbrk_Sys__unsupported_sbrk(u_P$raw ctx, usize n))(usize)) {
    let_ignore = ctx;
    let_ignore = n;
    return 0;
};

fn_((heap_Sbrk_Sys__unsupported_ctx(P$raw self))(heap_Sbrk_Ctx)) {
    return (heap_Sbrk_Ctx){
        .inner = u_anyP(self),
        .sbrkFn = heap_Sbrk_Sys__unsupported_sbrk,
    };
};
