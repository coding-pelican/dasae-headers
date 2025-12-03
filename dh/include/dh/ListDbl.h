/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ListDbl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-12-03 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ListDbl
 *
 * @brief   Doubly linked list data structure
 * @details Bidirectional traversal with O(1) operations
 *          with basic insertion, removal, and traversal operations.
 */
#ifndef ListDbl__included
#define ListDbl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

#define ListDbl_Link$$(_T...) __comp_anon__ListDbl_Link$$(_T)
#define ListDbl_Link$(_T...) __comp_alias__ListDbl_Link$(_T)
#define T_decl_ListDbl_Link$(_T...) __comp_gen__T_decl_ListDbl_Link$(_T)
#define T_impl_ListDbl_Link$(_T...) __comp_gen__T_impl_ListDbl_Link$(_T)
#define T_use_ListDbl_Link$(_T...) __comp_gen__T_use_ListDbl_Link$(_T)

typedef struct ListDbl_Link ListDbl_Link;
T_use_P$(ListDbl_Link);
T_use_O$(P$ListDbl_Link);
struct ListDbl_Link {
    var_(prev, O$P$ListDbl_Link);
    var_(next, O$P$ListDbl_Link);
    debug_only(var_(type, TypeInfo);)
};

$extern fn_((ListDbl_Link_empty(TypeInfo type))(ListDbl_Link));
typedef struct ListDbl_Adp$raw ListDbl_Adp$raw;
$extern fn_((ListDbl_Link_adp(P_const$ListDbl_Link self))(const ListDbl_Adp$raw*));
$extern fn_((ListDbl_Link_adpMut(P$ListDbl_Link self))(ListDbl_Adp$raw*));
$extern fn_((ListDbl_Link_data(P_const$ListDbl_Link self, TypeInfo type))(u_P_const$raw));
$extern fn_((ListDbl_Link_dataMut(P$ListDbl_Link self, TypeInfo type))(u_P$raw));


#define ListDbl_Adp$$(_T...) __comp_anon__ListDbl_Adp$$(_T)
#define ListDbl_Adp$(_T...) __comp_alias__ListDbl_Adp$(_T)
#define T_decl_ListDbl_Adp$(_T...) __comp_gen__T_decl_ListDbl_Adp$(_T)
#define T_impl_ListDbl_Adp$(_T...) __comp_gen__T_impl_ListDbl_Adp$(_T)
#define T_use_ListDbl_Adp$(_T...) __comp_gen__T_use_ListDbl_Adp$(_T)

struct ListDbl_Adp$raw {
    struct {
        var_(link, ListDbl_Link);
        debug_only(var_(type, TypeInfo);)
    };
    var_(data, u8) $flexible;
};

$extern fn_((ListDbl_Adp_empty(TypeInfo type, ListDbl_Adp$raw* ret_mem))(ListDbl_Adp$raw*));
$extern fn_((ListDbl_Adp_init(u_V$raw data, ListDbl_Adp$raw* ret_mem))(ListDbl_Adp$raw*));
$extern fn_((ListDbl_Adp_link(const ListDbl_Adp$raw* self))(P_const$ListDbl_Link));
$extern fn_((ListDbl_Adp_linkMut(ListDbl_Adp$raw* self))(P$ListDbl_Link));
$extern fn_((ListDbl_Adp_data(const ListDbl_Adp$raw* self, TypeInfo type))(u_P_const$raw));
$extern fn_((ListDbl_Adp_dataMut(ListDbl_Adp$raw* self, TypeInfo type))(u_P$raw));


#define ListDbl$$(_T...) __comp_anon__ListDbl$$(_T)
#define ListDbl$(_T...) __comp_alias__ListDbl$(_T)
#define T_decl_ListDbl$(_T...) __comp_gen__T_decl_ListDbl$(_T)
#define T_impl_ListDbl$(_T...) __comp_gen__T_impl_ListDbl$(_T)
#define T_use_ListDbl$(_T...) __comp_gen__T_use_ListDbl$(_T)

typedef struct ListDbl {
    var_(first, O$P$ListDbl_Link);
    var_(last, O$P$ListDbl_Link);
    var_(len, usize);
    debug_only(var_(type, TypeInfo);)
} ListDbl;

$extern fn_((ListDbl_empty(TypeInfo type))(ListDbl));
$extern fn_((ListDbl_insertNext(ListDbl* self, P$ListDbl_Link link, P$ListDbl_Link new_link))(void));
$extern fn_((ListDbl_insertPrev(ListDbl* self, P$ListDbl_Link link, P$ListDbl_Link new_link))(void));
$extern fn_((ListDbl_concatByMoving(ListDbl* dst, ListDbl* src))(void));
$extern fn_((ListDbl_append(ListDbl* self, P$ListDbl_Link last))(void));
$extern fn_((ListDbl_prepend(ListDbl* self, P$ListDbl_Link first))(void));
$extern fn_((ListDbl_remove(ListDbl* self, P$ListDbl_Link link))(void));
$extern fn_((ListDbl_pop(ListDbl* self))(O$P$ListDbl_Link));
$extern fn_((ListDbl_shift(ListDbl* self))(O$P$ListDbl_Link));
$extern fn_((ListDbl_len(const ListDbl* self))(usize));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ListDbl_Link$$(_T...) \
    union { \
        struct { \
            var_(prev, O$$(P$$(ListDbl_Link$(_T)))); \
            var_(next, O$$(P$$(ListDbl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListDbl_Link) $like_ref; \
    }
#define __comp_alias__ListDbl_Link$(_T...) \
    pp_join($, ListDbl_Link, _T)
#define __comp_gen__T_decl_ListDbl_Link$(_T...) \
    $maybe_unused typedef union ListDbl_Link$(_T) ListDbl_Link$(_T); \
    T_use_P$(ListDbl_Link$(_T)); \
    T_decl_O$(P$(ListDbl_Link$(_T)))
#define __comp_gen__T_impl_ListDbl_Link$(_T...) \
    T_impl_O$(P$(ListDbl_Link$(_T))); \
    union ListDbl_Link$(_T) { \
        struct { \
            var_(prev, O$(P$(ListDbl_Link$(_T)))); \
            var_(next, O$(P$(ListDbl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
    }
#define __comp_gen__T_use_ListDbl_Link$(_T...) \
    T_decl_ListDbl_Link$(_T); \
    T_impl_ListDbl_Link$(_T)

/* clang-format off */
#define T_use_ListDbl_Link_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Link_empty, _T)(void))(ListDbl_Link$(_T))) { \
        return type$((ListDbl_Link$(_T))(ListDbl_Link_empty(typeInfo$(_T)))); \
    }
#define T_use_ListDbl_Link_adp$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Link_adp, _T)(P_const$(ListDbl_Link$(_T)) self))(const ListDbl_Adp$(_T)*)) { \
        return as$(const ListDbl_Adp$(_T)*)(ListDbl_Link_adp(self->as_raw)); \
    }
#define T_use_ListDbl_Link_adpMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Link_adpMut, _T)(P$(ListDbl_Link$(_T)) self))(ListDbl_Adp$(_T)*)) { \
        return as$(ListDbl_Adp$(_T)*)(ListDbl_Link_adpMut(self->as_raw)); \
    }
#define T_use_ListDbl_Link_data$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Link_data, _T)(P_const$(ListDbl_Link$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(ListDbl_Link_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListDbl_Link_dataMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Link_dataMut, _T)(P$(ListDbl_Link$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(ListDbl_Link_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListDbl_Adp$$(_T...) \
    union { \
        struct { \
            struct { \
                var_(link, ListDbl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListDbl_Adp$raw) $like_ref; \
    }
#define __comp_alias__ListDbl_Adp$(_T...) \
    pp_join($, ListDbl_Adp, _T)
#define __comp_gen__T_decl_ListDbl_Adp$(_T...) \
    $maybe_unused typedef union ListDbl_Adp$(_T) ListDbl_Adp$(_T)
#define __comp_gen__T_impl_ListDbl_Adp$(_T...) \
    union ListDbl_Adp$(_T) { \
        struct { \
            struct { \
                var_(link, ListDbl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListDbl_Adp$raw) $like_ref; \
    }
#define __comp_gen__T_use_ListDbl_Adp$(_T...) \
    T_decl_ListDbl_Adp$(_T); \
    T_impl_ListDbl_Adp$(_T)

/* clang-format off */
#define T_use_ListDbl_Adp_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_empty, _T)(void))(ListDbl_Adp$(_T)*)) { \
        return *as$(ListDbl_Adp$(_T)*)(ListDbl_Adp_empty(typeInfo$(_T), lit0$((ListDbl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListDbl_Adp_init$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_init, _T)(_T data))(ListDbl_Adp$(_T)*)) { \
        return *as$(ListDbl_Adp$(_T)*)(ListDbl_Adp_init(u_anyV(data), lit0$((ListDbl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListDbl_Adp_link$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_link, _T)(const ListDbl_Adp$(_T)* self))(P_const$(ListDbl_Link$(_T)))) { \
        return as$(P$(ListDbl_Link$(_T)))(ListDbl_Adp_link(self->as_raw)); \
    }
#define T_use_ListDbl_Adp_linkMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_linkMut, _T)(ListDbl_Adp$(_T)* self))(P$(ListDbl_Link$(_T)))) { \
        return as$(P$(ListDbl_Link$(_T)))(ListDbl_Adp_linkMut(self->as_raw)); \
    }
#define T_use_ListDbl_Adp_data$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_data, _T)(const ListDbl_Adp$(_T)* self))(const _T*)) { \
        return u_castP$((const _T*)(ListDbl_Adp_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListDbl_Adp_dataMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_Adp_dataMut, _T)(ListDbl_Adp$(_T)* self))(_T*)) { \
        return u_castP$((_T*)(ListDbl_Adp_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListDbl$$(_T...) \
    union { \
        struct { \
            var_(first, O$$(P$$(ListDbl_Link$(_T)))); \
            var_(last, O$$(P$$(ListDbl_Link$(_T)))); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListDbl) $like_ref; \
    }
#define __comp_alias__ListDbl$(_T...) \
    pp_join($, ListDbl, _T)
#define __comp_gen__T_decl_ListDbl$(_T...) \
    $maybe_unused typedef union ListDbl$(_T) ListDbl$(_T)
#define __comp_gen__T_impl_ListDbl$(_T...) \
    union ListDbl$(_T) { \
        struct { \
            var_(first, O$(P$(ListDbl_Link$(_T)))); \
            var_(last, O$(P$(ListDbl_Link$(_T)))); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListDbl) $like_ref; \
    }
#define __comp_gen__T_use_ListDbl$(_T...) \
    T_decl_ListDbl$(_T); \
    T_impl_ListDbl$(_T)

/* clang-format off */
#define T_use_ListDbl_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_empty, _T)(void))(ListDbl$(_T))) { \
        return type$((ListDbl$(_T))(ListDbl_empty(typeInfo$(_T)))); \
    }
#define T_use_ListDbl_insertNext$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_insertNext, _T)(ListDbl$(_T)* self, P$(ListDbl_Link$(_T)) link, P$(ListDbl_Link$(_T)) new_link))(void)) { \
        return ListDbl_insertNext(self->as_raw, link->as_raw, new_link->as_raw); \
    }
#define T_use_ListDbl_insertPrev$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_insertPrev, _T)(ListDbl$(_T)* self, P$(ListDbl_Link$(_T)) link, P$(ListDbl_Link$(_T)) new_link))(void)) { \
        return ListDbl_insertPrev(self->as_raw, link->as_raw, new_link->as_raw); \
    }
#define T_use_ListDbl_concatByMoving$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_concatByMoving, _T)(ListDbl$(_T)* dst, ListDbl$(_T)* src))(void)) { \
        return ListDbl_concatByMoving(dst->as_raw, src->as_raw); \
    }
#define T_use_ListDbl_append$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_append, _T)(ListDbl$(_T)* self, P$(ListDbl_Link$(_T)) last))(void)) { \
        return ListDbl_append(self->as_raw, last->as_raw); \
    }
#define T_use_ListDbl_prepend$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_prepend, _T)(ListDbl$(_T)* self, P$(ListDbl_Link$(_T)) first))(void)) { \
        return ListDbl_prepend(self->as_raw, first->as_raw); \
    }
#define T_use_ListDbl_remove$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_remove, _T)(ListDbl$(_T)* self, P$(ListDbl_Link$(_T)) link))(void)) { \
        return ListDbl_remove(self->as_raw, link->as_raw); \
    }
#define T_use_ListDbl_pop$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_pop, _T)(ListDbl$(_T)* self))(O$(P$(ListDbl_Link$(_T))))) { \
        return typeO$((O$(P$(ListDbl_Link$(_T))))(ListDbl_pop(self->as_raw))); \
    }
#define T_use_ListDbl_shift$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_shift, _T)(ListDbl$(_T)* self))(O$(P$(ListDbl_Link$(_T))))) { \
        return typeO$((O$(P$(ListDbl_Link$(_T))))(ListDbl_shift(self->as_raw))); \
    }
#define T_use_ListDbl_len$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl_id(ListDbl_len, _T)(const ListDbl$(_T)* self))(usize)) { \
        return ListDbl_len(self->as_raw); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ListDbl__included */
