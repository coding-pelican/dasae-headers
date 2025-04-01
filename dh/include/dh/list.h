/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    list.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   List data structures
 * @details Provides comprehensive linked list implementations:
 *          - Singly linked list: Memory-efficient forward traversal
 *          - Doubly linked list: Bidirectional traversal with O(1) operations
 *
 *          Both implementations include standard operations like insertion,
 *          deletion, traversal, and utility functions for common list operations.
 */

#ifndef LIST_INCLUDED
#define LIST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/ptr.h"
#include "dh/opt.h"
#include "dh/meta/common.h"
#include "dh/TEST.h"

/*========== Macros and Declarations ========================================*/

/* Singly Linked List */

#define use_ListSgl_Node$(T)  comp_type_gen__use_ListSgl_Node$(T)
#define decl_ListSgl_Node$(T) comp_type_gen__decl_ListSgl_Node$(T)
#define impl_ListSgl_Node$(T) comp_type_gen__impl_ListSgl_Node$(T)
#define ListSgl_Node$(T)      comp_type_alias__ListSgl_Node$(T)

typedef struct ListSgl_Node ListSgl_Node;
use_Ptr$(ListSgl_Node);
use_Opt$(Ptr$ListSgl_Node);
struct ListSgl_Node {
    Opt$Ptr$ListSgl_Node next;
    meta_Ptr             data;
};

extern fn_(ListSgl_Node_init(meta_Ptr data), ListSgl_Node);
extern fn_(ListSgl_Node_insertAfter(ListSgl_Node* node, ListSgl_Node* new_node), void);
extern fn_(ListSgl_Node_removeNext(ListSgl_Node* node), Opt$Ptr$ListSgl_Node);
extern fn_(ListSgl_Node_findLast(ListSgl_Node* node), ListSgl_Node*);
extern fn_(ListSgl_Node_countChildren(const ListSgl_Node* node), usize);
extern fn_(ListSgl_Node_reverse(Opt$Ptr$ListSgl_Node* indirect), void);

#define use_ListSgl$(T)  comp_type_gen__use_ListSgl$(T)
#define decl_ListSgl$(T) comp_type_gen__decl_ListSgl$(T)
#define impl_ListSgl$(T) comp_type_gen__impl_ListSgl$(T)
#define ListSgl$(T)      comp_type_alias__ListSgl$(T)

typedef struct ListSgl {
    Opt$Ptr$ListSgl_Node first;
} ListSgl;

extern fn_(ListSgl_init(void), ListSgl);
extern fn_(ListSgl_prepend(ListSgl* self, ListSgl_Node* new_node), void);
extern fn_(ListSgl_remove(ListSgl* self, ListSgl_Node* node), void);
extern fn_(ListSgl_popFirst(ListSgl* self), Opt$Ptr$ListSgl_Node);
extern fn_(ListSgl_len(const ListSgl* self), usize);

/* Doubly Linked List */

#define use_ListDbl_Node$(T)  comp_type_gen__use_ListDbl_Node$(T)
#define decl_ListDbl_Node$(T) comp_type_gen__decl_ListDbl_Node$(T)
#define impl_ListDbl_Node$(T) comp_type_gen__impl_ListDbl_Node$(T)
#define ListDbl_Node$(T)      comp_type_alias__ListDbl_Node$(T)

typedef struct ListDbl_Node ListDbl_Node;
use_Ptr$(ListDbl_Node);
use_Opt$(Ptr$ListDbl_Node);
struct ListDbl_Node {
    Opt$Ptr$ListDbl_Node prev;
    Opt$Ptr$ListDbl_Node next;
    meta_Ptr             data;
};

extern fn_(ListDbl_Node_init(meta_Ptr data), ListDbl_Node);

#define use_ListDbl$(T)  comp_type_gen__use_ListDbl$(T)
#define decl_ListDbl$(T) comp_type_gen__decl_ListDbl$(T)
#define impl_ListDbl$(T) comp_type_gen__impl_ListDbl$(T)
#define ListDbl$(T)      comp_type_alias__ListDbl$(T)

typedef struct ListDbl {
    Opt$Ptr$ListDbl_Node first;
    Opt$Ptr$ListDbl_Node last;
    usize                len;
} ListDbl;

extern fn_(ListDbl_init(void), ListDbl);
extern fn_(ListDbl_insertAfter(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node), void);
extern fn_(ListDbl_insertBefore(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node), void);
extern fn_(ListDbl_concatByMoving(ListDbl* dst, ListDbl* src), void);
extern fn_(ListDbl_append(ListDbl* self, ListDbl_Node* new_node), void);
extern fn_(ListDbl_prepend(ListDbl* self, ListDbl_Node* new_node), void);
extern fn_(ListDbl_remove(ListDbl* self, ListDbl_Node* node), void);
extern fn_(ListDbl_pop(ListDbl* self), Opt$Ptr$ListDbl_Node);
extern fn_(ListDbl_popFirst(ListDbl* self), Opt$Ptr$ListDbl_Node);
extern fn_(ListDbl_len(const ListDbl* self), usize);

/*========== Macros and Definitions =========================================*/

/* Singly Linked List */

#define comp_type_gen__use_ListSgl_Node$(T) \
    decl_ListSgl_Node$(T);                  \
    impl_ListSgl_Node$(T)
#define comp_type_gen__decl_ListSgl_Node$(T)         \
    typedef union ListSgl_Node$(T) ListSgl_Node$(T); \
    use_Ptr$(ListSgl_Node$(T));                      \
    use_Opt$(Ptr$(ListSgl_Node$(T)))
#define comp_type_gen__impl_ListSgl_Node$(T)   \
    union ListSgl_Node$(T) {                   \
        ListSgl_Node base[1];                  \
        struct {                               \
            Opt$(Ptr$(ListSgl_Node$(T))) next; \
            TypeInfo type;                     \
            rawptr$(T) data;                   \
        };                                     \
    }
#define comp_type_alias__ListSgl_Node$(T) \
    pp_join($, ListSgl_Node, T)

#define comp_type_gen__use_ListSgl$(T) \
    use_ListSgl_Node$(T);              \
    decl_ListSgl$(T);                  \
    impl_ListSgl$(T)
#define comp_type_gen__decl_ListSgl$(T) \
    typedef union ListSgl$(T) ListSgl$(T)
#define comp_type_gen__impl_ListSgl$(T)         \
    union ListSgl$(T) {                         \
        ListSgl base[1];                        \
        struct {                                \
            Opt$(Ptr$(ListSgl_Node$(T))) first; \
        };                                      \
    }
#define comp_type_alias__ListSgl$(T) \
    pp_join($, ListSgl, T)

/* Doubly Linked List */

#define comp_type_gen__use_ListDbl_Node$(T) \
    decl_ListDbl_Node$(T);                  \
    impl_ListDbl_Node$(T)
#define comp_type_gen__decl_ListDbl_Node$(T)         \
    typedef union ListDbl_Node$(T) ListDbl_Node$(T); \
    use_Ptr$(ListDbl_Node$(T));                      \
    use_Opt$(Ptr$(ListDbl_Node$(T)))
#define comp_type_gen__impl_ListDbl_Node$(T)   \
    union ListDbl_Node$(T) {                   \
        ListDbl_Node base[1];                  \
        struct {                               \
            Opt$(Ptr$(ListDbl_Node$(T))) prev; \
            Opt$(Ptr$(ListDbl_Node$(T))) next; \
            TypeInfo type;                     \
            rawptr$(T) data;                   \
        };                                     \
    }
#define comp_type_alias__ListDbl_Node$(T) \
    pp_join($, ListDbl_Node, T)

#define comp_type_gen__use_ListDbl$(T) \
    decl_ListDbl$(T);                  \
    impl_ListDbl$(T)
#define comp_type_gen__decl_ListDbl$(T) \
    typedef union ListDbl$(T) ListDbl$(T)
#define comp_type_gen__impl_ListDbl$(T)         \
    union ListDbl$(T) {                         \
        ListDbl base[1];                        \
        struct {                                \
            Opt$(Ptr$(ListDbl_Node$(T))) first; \
            Opt$(Ptr$(ListDbl_Node$(T))) last;  \
            usize len;                          \
        };                                      \
    }
#define comp_type_alias__ListDbl$(T) \
    pp_join($, ListDbl, T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* LIST_INCLUDED */
