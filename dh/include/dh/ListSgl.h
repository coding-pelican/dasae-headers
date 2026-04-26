/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ListSgl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-12-03 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ListSgl
 *
 * @brief   Singly linked list data structure
 * @details Memory-efficient forward traversal with basic insertion,
 *          removal, and traversal operations.
 */
#ifndef ListSgl__included
#define ListSgl__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"

/*========== Macros and Declarations ========================================*/

#define ListSgl_Link$$(_T...) __comp_anon__ListSgl_Link$$(_T)
#define ListSgl_Link$(_T...) __comp_alias__ListSgl_Link$(_T)
#define T_decl_ListSgl_Link$(_T...) __comp_gen__T_decl_ListSgl_Link$(_T)
#define T_impl_ListSgl_Link$(_T...) __comp_gen__T_impl_ListSgl_Link$(_T)
#define T_use_ListSgl_Link$(_T...) __comp_gen__T_use_ListSgl_Link$(_T)

typedef struct ListSgl_Link ListSgl_Link;
T_use_P$(ListSgl_Link);
T_use_O$(P$ListSgl_Link);
struct ListSgl_Link {
    var_(next, O$P$ListSgl_Link);
    debug_only(var_(type, TypeInfo);)
};

$extern fn_((ListSgl_Link_empty(TypeInfo type))(ListSgl_Link));
$extern fn_((ListSgl_Link_insertNext(P$ListSgl_Link self, P$ListSgl_Link after))(void));
$extern fn_((ListSgl_Link_removeNext(P$ListSgl_Link self))(O$P$ListSgl_Link));
$extern fn_((ListSgl_Link_findLast(P$ListSgl_Link self))(P$ListSgl_Link));
$extern fn_((ListSgl_Link_countChildren(P_const$ListSgl_Link self))(usize));
$extern fn_((ListSgl_Link_reverse(O$P$ListSgl_Link* indirect))(void));

typedef struct ListSgl_Adp$raw ListSgl_Adp$raw;
$extern fn_((ListSgl_Link_adp(P_const$ListSgl_Link self))(const ListSgl_Adp$raw*));
$extern fn_((ListSgl_Link_adpMut(P$ListSgl_Link self))(ListSgl_Adp$raw*));
$extern fn_((ListSgl_Link_data(P_const$ListSgl_Link self, TypeInfo type))(u_P_const$raw));
$extern fn_((ListSgl_Link_dataMut(P$ListSgl_Link self, TypeInfo type))(u_P$raw));


#define ListSgl_Adp$$(_T...) __comp_anon__ListSgl_Adp$$(_T)
#define ListSgl_Adp$(_T...) __comp_alias__ListSgl_Adp$(_T)
#define T_decl_ListSgl_Adp$(_T...) __comp_gen__T_decl_ListSgl_Adp$(_T)
#define T_impl_ListSgl_Adp$(_T...) __comp_gen__T_impl_ListSgl_Adp$(_T)
#define T_use_ListSgl_Adp$(_T...) __comp_gen__T_use_ListSgl_Adp$(_T)

struct ListSgl_Adp$raw {
    struct {
        var_(link, ListSgl_Link);
        debug_only(var_(type, TypeInfo);)
    };
    var_(data, u8) $flexible;
};

$extern fn_((ListSgl_Adp_empty(TypeInfo type, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*));
$extern fn_((ListSgl_Adp_init(u_V$raw data, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*));
$extern fn_((ListSgl_Adp_link(const ListSgl_Adp$raw* self))(P_const$ListSgl_Link));
$extern fn_((ListSgl_Adp_linkMut(ListSgl_Adp$raw* self))(P$ListSgl_Link));
$extern fn_((ListSgl_Adp_data(const ListSgl_Adp$raw* self, TypeInfo type))(u_P_const$raw));
$extern fn_((ListSgl_Adp_dataMut(ListSgl_Adp$raw* self, TypeInfo type))(u_P$raw));


#define ListSgl$$(_T...) __comp_anon__ListSgl$$(_T)
#define ListSgl$(_T...) __comp_alias__ListSgl$(_T)
#define T_decl_ListSgl$(_T...) __comp_gen__T_decl_ListSgl$(_T)
#define T_impl_ListSgl$(_T...) __comp_gen__T_impl_ListSgl$(_T)
#define T_use_ListSgl$(_T...) __comp_gen__T_use_ListSgl$(_T)

typedef struct ListSgl {
    var_(first, O$P$ListSgl_Link);
    debug_only(var_(type, TypeInfo);)
} ListSgl;

$extern fn_((ListSgl_empty(TypeInfo type))(ListSgl));
$extern fn_((ListSgl_prepend(ListSgl* self, P$ListSgl_Link before))(void));
$extern fn_((ListSgl_remove(ListSgl* self, P$ListSgl_Link link))(void));
$extern fn_((ListSgl_shift(ListSgl* self))(O$P$ListSgl_Link));
$extern fn_((ListSgl_len(const ListSgl* self))(usize));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ListSgl_Link$$(_T...) \
    union { \
        struct { \
            var_(next, O$$(P$$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl_Link) $like_ref; \
    }
#define __comp_alias__ListSgl_Link$(_T...) \
    tpl$(ListSgl_Link, _T)
#define __comp_gen__T_decl_ListSgl_Link$(_T...) \
    $maybe_unused typedef union ListSgl_Link$(_T) ListSgl_Link$(_T); \
    T_use_P$(ListSgl_Link$(_T)); \
    T_decl_O$(P$(ListSgl_Link$(_T)))
#define __comp_gen__T_impl_ListSgl_Link$(_T...) \
    T_impl_O$(P$(ListSgl_Link$(_T))); \
    union ListSgl_Link$(_T) { \
        struct { \
            var_(next, O$(P$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl_Link) $like_ref; \
    }
#define __comp_gen__T_use_ListSgl_Link$(_T...) \
    T_decl_ListSgl_Link$(_T); \
    T_impl_ListSgl_Link$(_T)

/* clang-format off */
#define T_use_ListSgl_Link_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_empty, _T)(void))(ListSgl_Link$(_T))) { \
        return type$((ListSgl_Link$(_T))(ListSgl_Link_empty(typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Link_insertNext$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_insertNext, _T)(P$(ListSgl_Link$(_T)) self, P$(ListSgl_Link$(_T)) after))(void)) { \
        return ListSgl_Link_insertNext(self->as_raw, after->as_raw); \
    }
#define T_use_ListSgl_Link_removeNext$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_removeNext, _T)(P$(ListSgl_Link$(_T)) self))(O$(P$(ListSgl_Link$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Link$(_T))))(ListSgl_Link_removeNext(self->as_raw))); \
    }
#define T_use_ListSgl_Link_findLast$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_findLast, _T)(P$(ListSgl_Link$(_T)) self))(P$(ListSgl_Link$(_T)))) { \
        return as$(P$(ListSgl_Link$(_T)))(ListSgl_Link_findLast(self->as_raw)); \
    }
#define T_use_ListSgl_Link_countChildren$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_countChildren, _T)(P_const$(ListSgl_Link$(_T)) self))(usize)) { \
        return ListSgl_Link_countChildren(self->as_raw); \
    }
#define T_use_ListSgl_Link_reverse$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_reverse, _T)(O$(P$(ListSgl_Link$(_T)))* indirect))(void)) { \
        return ListSgl_Link_reverse(as$(O$P$ListSgl_Link*)(indirect)); \
    }

#define T_use_ListSgl_Link_adp$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_adp, _T)(P_const$(ListSgl_Link$(_T)) self))(const ListSgl_Adp$(_T)*)) { \
        return as$(const ListSgl_Adp$(_T)*)(ListSgl_Link_adp(self->as_raw)); \
    }
#define T_use_ListSgl_Link_adpMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_adpMut, _T)(P$(ListSgl_Link$(_T)) self))(ListSgl_Adp$(_T)*)) { \
        return as$(ListSgl_Adp$(_T)*)(ListSgl_Link_adpMut(self->as_raw)); \
    }
#define T_use_ListSgl_Link_data$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_data, _T)(P_const$(ListSgl_Link$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(ListSgl_Link_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Link_dataMut$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Link_dataMut, _T)(P$(ListSgl_Link$(_T)) self))(_T*)) { \
        return u_castP$((_T*)(ListSgl_Link_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListSgl_Adp$$(_T...) \
    union { \
        struct { \
            struct { \
                var_(link, ListSgl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListSgl_Adp$raw) $like_ref; \
    }
#define __comp_alias__ListSgl_Adp$(_T) \
    tpl$(ListSgl_Adp, _T)
#define __comp_gen__T_decl_ListSgl_Adp$(_T) \
    $maybe_unused typedef union ListSgl_Adp$(_T) ListSgl_Adp$(_T)
#define __comp_gen__T_impl_ListSgl_Adp$(_T) \
    union ListSgl_Adp$(_T) { \
        struct { \
            struct { \
                var_(link, ListSgl_Link$(_T)); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw, ListSgl_Adp$raw) $like_ref; \
    }
#define __comp_gen__T_use_ListSgl_Adp$(_T) \
    T_decl_ListSgl_Adp$(_T); \
    T_impl_ListSgl_Adp$(_T)

/* clang-format off */
#define T_use_ListSgl_Adp_empty$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Adp_empty, _T)(void))(ListSgl_Adp$(_T))) { \
        return *as$(ListSgl_Adp$(_T)*)(ListSgl_Adp_empty(typeInfo$(_T), l0$((ListSgl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListSgl_Adp_init$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Adp_init, _T)(_T data))(ListSgl_Adp$(_T))) { \
        return *as$(ListSgl_Adp$(_T)*)(ListSgl_Adp_init(u_anyV(data), l0$((ListSgl_Adp$(_T))).as_raw)); \
    }
#define T_use_ListSgl_Adp_data$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Adp_data, _T)(const ListSgl_Adp$(_T)* self))(const _T*)) { \
        return u_castP$((const _T*)(ListSgl_Adp_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Adp_dataMut$(_T...) \
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_Adp_dataMut, _T)(ListSgl_Adp$(_T)* self))(_T*)) { \
        return u_castP$((_T*)(ListSgl_Adp_dataMut(self->as_raw, typeInfo$(_T)))); \
    }
/* clang-format on */

#define __comp_anon__ListSgl$$(_T...) \
    union { \
        struct { \
            var_(first, O$$(P$$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl) $like_ref; \
    }
#define __comp_alias__ListSgl$(_T...) \
    tpl$(ListSgl, _T)
#define __comp_gen__T_decl_ListSgl$(_T...) \
    $maybe_unused typedef union ListSgl$(_T) ListSgl$(_T)
#define __comp_gen__T_impl_ListSgl$(_T) \
    union ListSgl$(_T) { \
        struct { \
            var_(first, O$(P$(ListSgl_Link$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw, ListSgl) $like_ref; \
    }
#define __comp_gen__T_use_ListSgl$(_T...) \
    T_decl_ListSgl$(_T); \
    T_impl_ListSgl$(_T)

/* clang-format off */
#define T_use_ListSgl_empty$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_empty, _T)(void))(ListSgl$(_T))) { \
        return type$((ListSgl$(_T))(ListSgl_empty(typeInfo$(_T)))); \
    }
#define T_use_ListSgl_prepend$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_prepend, _T)(ListSgl$(_T)* self, P$(ListSgl_Link$(_T)) before))(void)) { \
        return ListSgl_prepend(self->as_raw, before->as_raw); \
    }
#define T_use_ListSgl_remove$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_remove, _T)(ListSgl$(_T)* self, P$(ListSgl_Link$(_T)) link))(void)) { \
        return ListSgl_remove(self->as_raw, link->as_raw); \
    }
#define T_use_ListSgl_shift$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_shift, _T)(ListSgl$(_T)* self))(O$(P$(ListSgl_Link$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Link$(_T))))(ListSgl_shift(self->as_raw))); \
    }
#define T_use_ListSgl_len$(_T...)\
    $attr($inline_always) \
    $static fn_((tpl$(ListSgl_len, _T)(const ListSgl$(_T)* self))(usize)) { \
        return ListSgl_len(self->as_raw); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ListSgl__included */
