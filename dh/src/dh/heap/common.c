#include "dh/heap/common.h"

/*========== External Definitions ===========================================*/

fn_((heap_Geom_system(void))(heap_Geom_E$heap_Geom) $scope) pp_if_(plat_is_windows)(
    pp_then_({
        return_ok(heap_Geom_ensureValid((heap_Geom){
            .page_size = mem_page_size,
            .reserve_align = 64ull * 1024,
            .commit_align = mem_page_size,
            .map_align = 64ull * 1024,
        }));
    }),
    pp_else_(pp_if_(pp_or(plat_is_linux, pp_or(plat_is_darwin, plat_is_wasi)))(
        pp_then_({
            return_ok(heap_Geom_from(mem_page_size));
        }),
        pp_else_({
            return_err(E_cause$heap_Geom_Unsupported());
        })
    ))) $unscoped(fn);
