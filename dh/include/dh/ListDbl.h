#if UNUSED_CODE
/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ListDbl.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha
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

#define T_use_ListDbl_Node$(T)  comp_type_gen__T_use_ListDbl_Node$(T)
#define T_decl_ListDbl_Node$(T) comp_type_gen__T_decl_ListDbl_Node$(T)
#define T_impl_ListDbl_Node$(T) comp_type_gen__T_impl_ListDbl_Node$(T)
#define ListDbl_Node$(T)        comp_type_alias__ListDbl_Node$(T)

typedef struct ListDbl_Node ListDbl_Node;
T_use_P$(ListDbl_Node);
T_use_O$(P$ListDbl_Node);
struct ListDbl_Node {
    var_(prev, O$P$ListDbl_Node);
    var_(next, O$P$ListDbl_Node);
    debug_only(var_(type, TypeInfo);)
};

extern fn_((ListDbl_Node_init(TypeInfo type))(ListDbl_Node));
extern fn_((ListDbl_Node_data(ListDbl_Node* node))(u_P_const$raw));
extern fn_((ListDbl_Node_dataMut(ListDbl_Node* node))(u_P$raw));

#define T_use_ListDbl$(T)  comp_type_gen__T_use_ListDbl$(T)
#define T_decl_ListDbl$(T) comp_type_gen__T_decl_ListDbl$(T)
#define T_impl_ListDbl$(T) comp_type_gen__T_impl_ListDbl$(T)
#define ListDbl$(T)        comp_type_alias__ListDbl$(T)

typedef struct ListDbl$raw {
    var_(first, O$P$ListDbl_Node);
    var_(last, O$P$ListDbl_Node);
    var_(len, usize);
    debug_only(var_(type, TypeInfo);)
} ListDbl$raw;

extern fn_((ListDbl_init(TypeInfo type))(ListDbl$raw));
extern fn_((ListDbl_insertNext(ListDbl$raw* self, ListDbl_Node* node, ListDbl_Node* new_node))(void));
extern fn_((ListDbl_insertPrev(ListDbl$raw* self, ListDbl_Node* node, ListDbl_Node* new_node))(void));
extern fn_((ListDbl_concatByMoving(ListDbl$raw* dst, ListDbl$raw* src))(void));
extern fn_((ListDbl_append(ListDbl$raw* self, ListDbl_Node* last))(void));
extern fn_((ListDbl_prepend(ListDbl$raw* self, ListDbl_Node* first))(void));
extern fn_((ListDbl_remove(ListDbl$raw* self, ListDbl_Node* node))(void));
extern fn_((ListDbl_pop(ListDbl$raw* self))(O$P$ListDbl_Node));
extern fn_((ListDbl_shift(ListDbl$raw* self))(O$P$ListDbl_Node));
extern fn_((ListDbl_len(const ListDbl$raw* self))(usize));

/*========== Macros and Definitions =========================================*/

#define comp_type_gen__T_use_ListDbl_Node$(T) \
    T_decl_ListDbl_Node$(T); \
    T_impl_ListDbl_Node$(T)
#define comp_type_gen__T_decl_ListDbl_Node$(T) \
    $maybe_unused typedef struct ListDbl_Node$(T) ListDbl_Node$(T); \
    T_use_P$(ListDbl_Node$(T)); \
    T_use_O$(P$(ListDbl_Node$(T)))
#define comp_type_gen__T_impl_ListDbl_Node$(T) \
    struct ListDbl_Node$(T) { \
        var_(data, T); \
        union { \
            struct { \
                var_(type, TypeInfo); \
                var_(prev, O$(P$(ListDbl_Node$(T)))); \
                var_(next, O$(P$(ListDbl_Node$(T)))); \
                debug_only(var_(type, TypeInfo);) \
            }; \
        }; \
    }
#define comp_type_alias__ListDbl_Node$(T) \
    pp_join($, ListDbl_Node, T)

#define comp_type_gen__T_use_ListDbl$(T) \
    T_decl_ListDbl$(T); \
    T_impl_ListDbl$(T)
#define comp_type_gen__T_decl_ListDbl$(T) \
    $maybe_unused typedef union ListDbl$(T) ListDbl$(T)
#define comp_type_gen__T_impl_ListDbl$(T) \
    union ListDbl$(T) { \
        struct { \
            var_(first, O$(P$(ListDbl_Node$(T)))); \
            var_(last, O$(P$(ListDbl_Node$(T)))); \
            var_(len, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ListDbl$raw); \
    }
#define comp_type_alias__ListDbl$(T) \
    pp_join($, ListDbl, T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ListDbl__included */

#endif /* UNUSED_CODE */
