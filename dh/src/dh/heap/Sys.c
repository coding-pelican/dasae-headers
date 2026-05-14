#include "dh/heap/Sys.h"

/*========== External Definitions ===========================================*/

fn_((heap_Sys_init(void))(heap_Sys)) {
#if plat_is_wasi
    var_(self, heap_Sys) = {
        .impl = {
            .local = cleared(),
            .sys_wasm = heap_Sbrk_Sys_Wasm_init(),
            .ctx = cleared(),
            .sbrk = cleared(),
        },
    };
    self.impl.ctx = heap_Sbrk_Sys_Wasm_ctx(&self.impl.sys_wasm);
    self.impl.sbrk = heap_Sbrk_from(heap_Sbrk_LocalLarge_ref(&self.impl.local), &self.impl.ctx);
    return self;
#else
    return (heap_Sys){
        .impl = {
            .page = heap_Page_default,
        },
    };
#endif /* plat_is_wasi */
};

fn_((heap_Sys_fini(heap_Sys* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_wasi
    heap_Sbrk_Sys_Wasm_fini(&self->impl.sys_wasm);
#endif /* plat_is_wasi */
    asg_l((self)(cleared()));
};

fn_((heap_Sys_alctr(heap_Sys* self))(mem_Alctr)) {
    claim_assert_nonnull(self);
#if plat_is_wasi
    return heap_Sbrk_alctr(&self->impl.sbrk);
#else
    return heap_Page_alctr(&self->impl.page);
#endif /* plat_is_wasi */
};
