#include "dh/heap/common.h"

/*========== External Definitions ===========================================*/

fn_((heap_Geom_system(void))(heap_Geom_E$heap_Geom) $scope) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_windows)(
            return_ok(heap_Geom_ensureValid((heap_Geom){
                .page_size = mem_page_size,
                .reserve_align = 64ull * 1024,
                .commit_align = mem_page_size,
                .map_align = 64ull * 1024,
            }))
        )),
        pp_cases_((plat_type_linux, plat_type_darwin, plat_type_wasi)(
            return_ok(heap_Geom_from(mem_page_size))
        )),
        pp_default_(()(
            return_err(E_cause$heap_Geom_Unsupported())
        ))
    ));
} $unscoped(fn);
