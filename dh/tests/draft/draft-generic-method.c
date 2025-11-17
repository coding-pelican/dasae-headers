#include "dh/core/pp/common.h"
#include "dh/main.h"
#include "dh/ArrList.h"
#include "dh/heap/Page.h"

#define ArrList_init$(T...) \
    lam_((mem_Allocator allocator), ArrList$(T)) { \
        return type$(ArrList$(T), ArrList_init(typeInfo$(T), allocator)); \
    }
#define ArrList_append$(T...) \
    lam_((ArrList$(T) * self, T item), mem_Err$void) { \
        return ArrList_append(self->base, meta_refPtr(&item)); \
    }

#include "dh/io/stream.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    let_ignore = args;

    let page = heap_Page_allocator(create$(heap_Page));

    use_ArrList$(i32);
    var arr_list = ArrList_init$(i32)(page);
    defer_(ArrList_fini(arr_list.base));

    let append = ArrList_append$(i32);
    try_(append(&arr_list, 1));
    try_(append(&arr_list, 2));
    try_(append(&arr_list, 3));
    try_(append(&arr_list, 4));
    try_(append(&arr_list, 5));
    try_(append(&arr_list, 6));
    try_(append(&arr_list, 7));
    try_(append(&arr_list, 8));

    for_slice(arr_list.items, item) {
        io_stream_print(u8_l("{:d} "), *item);
    }
    io_stream_print(u8_l("\n"));

    return_ok({});
} $unguarded_(fn);


#define tpl_id$T(_id, _T)                                        pp_join($, _id, _T)
#define tpl_id$1T$2U(_id, _T, _U)                                pp_join($, _id, pp_cat(pp_cat($1, _T), pp_cat($2, _U)))
#define tpl_id$2T$2U$3V(_id, _T, _U, _V)                         pp_join($, _id, pp_cat(pp_cat(pp_cat($1, _T), pp_cat($2, _U)), pp_cat($3, _V)))
#define tpl_fn$T(_id_w_T_and_Params, _ReturnT...)                fn_((tpl_id$T _id_w_T_and_Params)(_ReturnT))
#define tpl_fn$1T$2U(_id_w_T_w_U_and_Params, _ReturnT...)        fn_((tpl_id$1T$2U _id_w_T_w_U_and_Params)(_ReturnT))
#define tpl_fn$2T$2U$3V(_id_w_T_w_U_w_V_and_Params, _ReturnT...) fn_((tpl_id$2T$2U$3V _id_w_T_w_U_w_V_and_Params)(_ReturnT))

#define swap$T(_T) \
    static tpl_fn$T((swap, _T)(P$$(_T) lhs, P$$(_T) rhs), void) { \
        swap_raw(typeInfo$(_T), lhs, rhs); \
    }
static $inline fn_((swap_raw(TypeInfo type, void* lhs, void* rhs))(void)) {
    let tmp = as$(void*)(alloca(type.size));
    prim_memcpy(tmp, lhs, type.size);
    prim_memcpy(lhs, rhs, type.size);
    prim_memcpy(rhs, tmp, type.size);
}

// #define ArrList_atRo$T(_T) \
//     static tpl_fn$T((ArrList_atRo, _T)(P_const$$(ArrList$(_T)) self, usize index), P_const$$(_T)) { \
//         return as$(P_const$$(_T)()(ArrList_atRo_raw(self->base, index))); \
//     }
// static $inline fn_(ArrList_atRo_raw(const ArrList* self, usize index), const void*) {
//     return self->items.addr + (index * self->items.type.size);
// }
// #define ArrList_atRw$T(_T) \
//     static tpl_fn$T((ArrList_atRw, _T)(P$$(ArrList$(_T)) self, usize index), P$$(_T)) { \
//         return as$(P$$(_T)()(ArrList_atRw_raw(self->base, index))); \
//     }
// static $inline fn_(ArrList_atRw_raw(ArrList* self, usize index), void*) {
//     return self->items.addr + (index * self->items.type.size);
// }
//
// use_ArrList$(i32);
// ArrList_atRo$T(i32);
// ArrList_atRw$T(i32);
