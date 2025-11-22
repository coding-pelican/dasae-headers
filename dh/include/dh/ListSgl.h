/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ListSgl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha
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

#define T_use_ListSgl_Node$(_T) __comp_gen__T_use_ListSgl_Node$(_T)
#define T_decl_ListSgl_Node$(_T) __comp_gen__T_decl_ListSgl_Node$(_T)
#define T_impl_ListSgl_Node$(_T) __comp_gen__T_impl_ListSgl_Node$(_T)
#define ListSgl_Node$(_T) __comp_alias__ListSgl_Node$(_T)

typedef struct ListSgl_Node$raw ListSgl_Node$raw;
T_use_P$(ListSgl_Node$raw);
T_use_O$(P$ListSgl_Node$raw);
struct ListSgl_Node$raw {
    struct {
        var_(next, O$P$ListSgl_Node$raw);
        debug_only(var_(type, TypeInfo);)
    };
    var_(data $flexible, u8);
};

extern fn_((ListSgl_Node_init(P$ListSgl_Node$raw self, TypeInfo type))(P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_data(P_const$ListSgl_Node$raw self, TypeInfo type))(u_P_const$raw));
extern fn_((ListSgl_Node_dataMut(P$ListSgl_Node$raw self, TypeInfo type))(u_P$raw));
extern fn_((ListSgl_Node_insertNext(P$ListSgl_Node$raw self, P$ListSgl_Node$raw after))(void));
extern fn_((ListSgl_Node_removeNext(P$ListSgl_Node$raw self))(O$P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_findLast(P$ListSgl_Node$raw self))(P$ListSgl_Node$raw));
extern fn_((ListSgl_Node_countChildren(P_const$ListSgl_Node$raw self))(usize));
extern fn_((ListSgl_Node_reverse(O$P$ListSgl_Node$raw* indirect))(void));

#define T_use_ListSgl$(_T) __comp_gen__T_use_ListSgl$(_T)
#define T_decl_ListSgl$(_T) __comp_gen__T_decl_ListSgl$(_T)
#define T_impl_ListSgl$(_T) __comp_gen__T_impl_ListSgl$(_T)
#define ListSgl$(_T) __comp_alias__ListSgl$(_T)

typedef struct ListSgl$raw {
    var_(first, O$P$ListSgl_Node$raw);
    debug_only(var_(type, TypeInfo);)
} ListSgl$raw;

extern fn_((ListSgl_init(TypeInfo type))(ListSgl$raw));
extern fn_((ListSgl_prepend(ListSgl$raw* self, P$ListSgl_Node$raw before))(void));
extern fn_((ListSgl_remove(ListSgl$raw* self, P$ListSgl_Node$raw node))(void));
extern fn_((ListSgl_shift(ListSgl$raw* self))(O$P$ListSgl_Node$raw));
extern fn_((ListSgl_len(const ListSgl$raw* self))(usize));

/*========== Macros and Definitions =========================================*/

#define __comp_gen__T_use_ListSgl_Node$(_T) \
    T_decl_ListSgl_Node$(_T); \
    T_impl_ListSgl_Node$(_T)
#define __comp_gen__T_decl_ListSgl_Node$(_T) \
    $maybe_unused typedef union ListSgl_Node$(_T) ListSgl_Node$(_T); \
    T_use_P$(ListSgl_Node$(_T)); \
    T_use_O$(P$(ListSgl_Node$(_T)))
#define __comp_gen__T_impl_ListSgl_Node$(_T) \
    union ListSgl_Node$(_T) { \
        struct { \
            struct { \
                var_(next, O$(P$(ListSgl_Node$(_T)))); \
                debug_only(var_(type, TypeInfo);) \
            }; \
            var_(data, _T); \
        }; \
        var_(as_raw $like_ref, ListSgl_Node$raw); \
    }
#define __comp_alias__ListSgl_Node$(_T) \
    pp_join($, ListSgl_Node, _T)

#define __comp_gen__T_use_ListSgl$(_T) \
    T_decl_ListSgl$(_T); \
    T_impl_ListSgl$(_T)
#define __comp_gen__T_decl_ListSgl$(_T) \
    $maybe_unused typedef union ListSgl$(_T) ListSgl$(_T)
#define __comp_gen__T_impl_ListSgl$(_T) \
    union ListSgl$(_T) { \
        struct { \
            var_(first, O$(P$(ListSgl_Node$(_T)))); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ListSgl$raw); \
    }
#define __comp_alias__ListSgl$(_T) \
    pp_join($, ListSgl, _T)

/* clang-format off */
#define T_use_ListSgl_Node_init$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_init, _T)(P$(ListSgl_Node$(_T)) self))(P$(ListSgl_Node$(_T)))) { \
        return as$(P$(ListSgl_Node$(_T)))(ListSgl_Node_init(self->as_raw, typeInfo$(_T))); \
    }
#define T_use_ListSgl_Node_data$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_data, _T)(P$(const ListSgl_Node$(_T)) self))(const _T*)) { \
        return u_castP$((const _T*)(ListSgl_Node_data(self->as_raw, typeInfo$(_T)))); \
    }
#define T_use_ListSgl_Node_dataMut$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_dataMut, _T)(P$(ListSgl_Node$(_T)) self))(P$(_T*))) { \
        return u_castP$((P$(_T*)(ListSgl_Node_dataMut(self->as_raw, typeInfo$(_T))))); \
    }
#define T_use_ListSgl_Node_insertNext$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_insertNext, _T)(P$(ListSgl_Node$(_T)) self, P$(ListSgl_Node$(_T)) after))(void)) { \
        return ListSgl_Node_insertNext(self->as_raw, after->as_raw); \
    }
#define T_use_ListSgl_Node_removeNext$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_removeNext, _T)(P$(ListSgl_Node$(_T)) self))(O$(P$(ListSgl_Node$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Node$(_T))))(ListSgl_Node_removeNext(self->as_raw))); \
    }
#define T_use_ListSgl_Node_findLast$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_findLast, _T)(P$(ListSgl_Node$(_T)) self))(P$(ListSgl_Node$(_T)))) { \
        return as$(P$(ListSgl_Node$(_T)))(ListSgl_Node_findLast(self->as_raw)); \
    }
#define T_use_ListSgl_Node_countChildren$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_countChildren, _T)(P$(const ListSgl_Node$(_T)) self))(usize)) { \
        return ListSgl_Node_countChildren(self->as_raw); \
    }
#define T_use_ListSgl_Node_reverse$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_Node_reverse, _T)(O$(P$(ListSgl_Node$(_T)))* self))(void)) { \
        return ListSgl_Node_reverse(as$(O$P$ListSgl_Node$raw*)(&self)); \
    }

#define T_use_ListSgl_init$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_init, _T)(void))(ListSgl$(_T))) { \
        return type$((ListSgl$(_T))(ListSgl_init(typeInfo$(_T)))); \
    }
#define T_use_ListSgl_prepend$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_prepend, _T)(ListSgl$(_T)* self, P$(ListSgl_Node$(_T)) before))(void)) { \
        return ListSgl_prepend(self->as_raw, before->as_raw); \
    }
#define T_use_ListSgl_remove$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_remove, _T)(ListSgl$(_T)* self, P$(ListSgl_Node$(_T)) node))(void)) { \
        return ListSgl_remove(self->as_raw, node->as_raw); \
    }
#define T_use_ListSgl_shift$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_shift, _T)(ListSgl$(_T)* self))(O$(P$(ListSgl_Node$(_T))))) { \
        return typeO$((O$(P$(ListSgl_Node$(_T))))(ListSgl_shift(self->as_raw))); \
    }
#define T_use_ListSgl_len$(_T...)\
    $inline_always \
    $static fn_((tpl_id(ListSgl_len, _T)(const ListSgl$(_T)* self))(usize)) { \
        return ListSgl_len(self->as_raw); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ListSgl__included */
