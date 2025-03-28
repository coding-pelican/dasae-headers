#include "dh/list.h"
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/meta/common.h"
#include "dh/debug.h"

/*========== Singly Linked List ============================================*/

pub fn_(ListSgl_Node_init(meta_Ptr data), ListSgl_Node) {
    return (ListSgl_Node){
        .next = none$(Opt$Ptr$ListSgl_Node),
        .data = data
    };
}

pub fn_(ListSgl_Node_insertAfter(ListSgl_Node* node, ListSgl_Node* new_node), void) {
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    new_node->next = node->next;
    node->next     = some$(Opt$Ptr$ListSgl_Node, new_node);
}

pub fn_ext_scope(ListSgl_Node_removeNext(ListSgl_Node* node), Opt$Ptr$ListSgl_Node) {
    debug_assert_nonnull(node);
    if_none(node->next) {
        return_none();
    }
    let next_node   = unwrap(node->next);
    node->next      = next_node->next;
    next_node->next = none$(Opt$Ptr$ListSgl_Node);
    return_some(next_node);
} ext_unscoped;

pub fn_(ListSgl_Node_findLast(ListSgl_Node* node), ListSgl_Node*) {
    debug_assert_nonnull(node);
    var current = node;
    while_some(current->next, next) {
        current = next;
    }
    return current;
}

pub fn_(ListSgl_Node_countChildren(const ListSgl_Node* node), usize) {
    debug_assert_nonnull(node);
    usize count   = 0;
    var   current = node;
    while_some(current->next, next) {
        count++;
        current = next;
    }
    return count;
}

pub fn_(ListSgl_Node_reverse(Opt$Ptr$ListSgl_Node* indirect), void) {
    debug_assert_nonnull(indirect);
    if_none(*indirect) {
        return;
    }
    var prev    = none$(Opt$Ptr$ListSgl_Node);
    var current = *indirect;
    while_some(current, curr) {
        let next   = curr->next;
        curr->next = prev;
        prev       = current;
        current    = next;
    }
    *indirect = prev;
}

pub fn_(ListSgl_init(void), ListSgl) {
    return (ListSgl){
        .first = none$(Opt$Ptr$ListSgl_Node)
    };
}

pub fn_(ListSgl_prepend(ListSgl* self, ListSgl_Node* new_node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    new_node->next = self->first;
    self->first    = some$(Opt$Ptr$ListSgl_Node, new_node);
}

pub fn_(ListSgl_remove(ListSgl* self, ListSgl_Node* node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);

    if_some(self->first, first) {
        if (first == node) {
            self->first = node->next;
            node->next  = none$(Opt$Ptr$ListSgl_Node);
            return;
        }
        var current = first;
        while_some(current->next, next) {
            if (next == node) {
                current->next = node->next;
                node->next    = none$(Opt$Ptr$ListSgl_Node);
                return;
            }
            current = next;
        }
    }
}

pub fn_ext_scope(ListSgl_popFirst(ListSgl* self), Opt$Ptr$ListSgl_Node) {
    debug_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first   = unwrap(self->first);
    self->first = first->next;
    first->next = none$(Opt$Ptr$ListSgl_Node);
    return_some(first);
} ext_unscoped;

pub fn_(ListSgl_len(const ListSgl* self), usize) {
    debug_assert_nonnull(self);
    usize len = 0;
    if_some(self->first, first) {
        len = 1 + ListSgl_Node_countChildren(first);
    }
    return len;
}

/*========== Doubly Linked List ============================================*/

pub fn_(ListDbl_Node_init(meta_Ptr data), ListDbl_Node) {
    return (ListDbl_Node){
        .prev = none$(Opt$Ptr$ListDbl_Node),
        .next = none$(Opt$Ptr$ListDbl_Node),
        .data = data
    };
}

pub fn_(ListDbl_init(void), ListDbl) {
    return (ListDbl){
        .first = none$(Opt$Ptr$ListDbl_Node),
        .last  = none$(Opt$Ptr$ListDbl_Node),
        .len   = 0
    };
}

pub fn_(ListDbl_insertAfter(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    new_node->prev = some$(Opt$Ptr$ListDbl_Node, node);
    new_node->next = node->next;
    if_some(node->next, next) {
        next->prev = some$(Opt$Ptr$ListDbl_Node, new_node);
    } else {
        self->last = some$(Opt$Ptr$ListDbl_Node, new_node);
    }
    node->next = some$(Opt$Ptr$ListDbl_Node, new_node);
    self->len++;
}

pub fn_(ListDbl_insertBefore(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    new_node->next = some$(Opt$Ptr$ListDbl_Node, node);
    new_node->prev = node->prev;
    if_some(node->prev, prev) {
        prev->next = some$(Opt$Ptr$ListDbl_Node, new_node);
    } else {
        self->first = some$(Opt$Ptr$ListDbl_Node, new_node);
    }
    node->prev = some$(Opt$Ptr$ListDbl_Node, new_node);
    self->len++;
}

pub fn_(ListDbl_concatByMoving(ListDbl* dst, ListDbl* src), void) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);

    if_some(dst->last, last) {
        if_some(src->first, first) {
            last->next  = src->first;
            first->prev = some$(Opt$Ptr$ListDbl_Node, last);
            dst->last   = src->last;
            dst->len += src->len;
        }
    } else {
        dst->first = src->first;
        dst->last  = src->last;
        dst->len   = src->len;
    }
    src->first = none$(Opt$Ptr$ListDbl_Node);
    src->last  = none$(Opt$Ptr$ListDbl_Node);
    src->len   = 0;
}

pub fn_(ListDbl_append(ListDbl* self, ListDbl_Node* new_node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    if_some(self->last, last) {
        ListDbl_insertAfter(self, last, new_node);
    } else {
        new_node->prev = none$(Opt$Ptr$ListDbl_Node);
        new_node->next = none$(Opt$Ptr$ListDbl_Node);
        self->first    = some$(Opt$Ptr$ListDbl_Node, new_node);
        self->last     = some$(Opt$Ptr$ListDbl_Node, new_node);
        self->len      = 1;
    }
}

pub fn_(ListDbl_prepend(ListDbl* self, ListDbl_Node* new_node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    if_some(self->first, first) {
        ListDbl_insertBefore(self, first, new_node);
    } else {
        new_node->prev = none$(Opt$Ptr$ListDbl_Node);
        new_node->next = none$(Opt$Ptr$ListDbl_Node);
        self->first    = some$(Opt$Ptr$ListDbl_Node, new_node);
        self->last     = some$(Opt$Ptr$ListDbl_Node, new_node);
        self->len      = 1;
    }
}

pub fn_(ListDbl_remove(ListDbl* self, ListDbl_Node* node), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);

    if_some(node->prev, prev) {
        prev->next = node->next;
    } else {
        self->first = node->next;
    }
    if_some(node->next, next) {
        next->prev = node->prev;
    } else {
        self->last = node->prev;
    }
    node->prev = none$(Opt$Ptr$ListDbl_Node);
    node->next = none$(Opt$Ptr$ListDbl_Node);
    self->len--;
}

pub fn_ext_scope(ListDbl_pop(ListDbl* self), Opt$Ptr$ListDbl_Node) {
    debug_assert_nonnull(self);
    if_none(self->last) {
        return_none();
    }
    var last = unwrap(self->last);
    ListDbl_remove(self, last);
    return_some(last);
} ext_unscoped;

pub fn_ext_scope(ListDbl_popFirst(ListDbl* self), Opt$Ptr$ListDbl_Node) {
    debug_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first = unwrap(self->first);
    ListDbl_remove(self, first);
    return_some(first);
} ext_unscoped;

pub fn_(ListDbl_len(const ListDbl* self), usize) {
    debug_assert_nonnull(self);
    return self->len;
}
