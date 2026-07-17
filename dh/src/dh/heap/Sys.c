#include "dh/heap/Sys.h"

/*========== External Definitions ===========================================*/

fn_((heap_Sys_init(void))(heap_Sys_E$heap_Sys) $scope) {
#if plat_is_wasi
    var_(self, heap_Sys) = {
        ._impl = {
            .local = cleared(),
            .sys_wasm = heap_Sbrk_Sys_Wasm_init(),
            .ctx = cleared(),
            .sbrk = cleared(),
        },
    };
    self._impl.ctx = heap_Sbrk_Sys_Wasm_ctx(&self._impl.sys_wasm);
    self._impl.sbrk = heap_Sbrk_from(heap_Sbrk_local_Large_ref(&self._impl.local), &self._impl.ctx);
    return_ok(self);
#else
    let vmap = try_(heap_VMap_system());
    return_ok((heap_Sys){
        ._impl = {
            .page = heap_Page_from(vmap),
        },
    });
#endif /* plat_is_wasi */
} $unscoped(fn);

fn_((heap_Sys_fini(heap_Sys* self))(void)) {
    claim_assert_nonnull(self);
#if plat_is_wasi
    heap_Sbrk_Sys_Wasm_fini(&self->_impl.sys_wasm);
#endif /* plat_is_wasi */
    asg_l((self)(cleared()));
};

fn_((heap_Sys_alctr(heap_Sys* self))(mem_Alctr)) {
    claim_assert_nonnull(self);
#if plat_is_wasi
    return heap_Sbrk_alctr(&self->_impl.sbrk);
#else
    return heap_Page_alctr(&self->_impl.page);
#endif /* plat_is_wasi */
};

fn_((heap_Sys_thrdSafeAlctr(heap_Sys* self))(heap_Sys_E$mem_Alctr) $scope) {
    claim_assert_nonnull(self);
#if plat_is_wasi
    return_err(E_cause$heap_Sys_Unsupported());
#else
    return_ok(heap_Page_thrdSafeAlctr(&self->_impl.page));
#endif /* plat_is_wasi */
} $unscoped(fn);
