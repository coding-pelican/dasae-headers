#include "dh/list.h"
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/meta/common.h"
#include "dh/debug.h"

/*========== Singly Linked List ============================================*/

fn_((ListSgl_Node_init())(ListSgl_Node)) {
    return (ListSgl_Node){
        .next = none()
    };
}

fn_((ListSgl_Node_insertAfter(ListSgl_Node* node, ListSgl_Node* new_node))(void)) {
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    new_node->next = node->next;
    toSome(&node->next, new_node);
}

fn_((ListSgl_Node_removeNext(ListSgl_Node* node))(Opt$Ptr$ListSgl_Node) $scope) {
    debug_assert_nonnull(node);
    let next_node = orelse_((node->next)(return_none()));
    node->next    = next_node->next;
    return_some(next_node);
} $unscoped_(fn);

fn_((ListSgl_Node_findLast(ListSgl_Node* node))(ListSgl_Node*)) {
    debug_assert_nonnull(node);
    var current = node;
    while_some(current->next, next) {
        current = next;
    }
    return current;
}

fn_((ListSgl_Node_countChildren(const ListSgl_Node* node))(usize)) {
    debug_assert_nonnull(node);
    usize count   = 0;
    var   current = node;
    while_some(current->next, next) {
        count++;
        current = next;
    }
    return count;
}

fn_((ListSgl_Node_reverse(Opt$Ptr$ListSgl_Node* indirect))(void)) {
    debug_assert_nonnull(indirect);
    if_none(*indirect) {
        return;
    }
    var_(prev, Opt$Ptr$ListSgl_Node)    = none();
    var_(current, Opt$Ptr$ListSgl_Node) = *indirect;
    while_some(current, curr) {
        let next   = curr->next;
        curr->next = prev;
        prev       = current;
        current    = next;
    }
    *indirect = prev;
}

fn_((ListSgl_init(void))(ListSgl)) {
    return (ListSgl){
        .first = none()
    };
}

fn_((ListSgl_prepend(ListSgl* self, ListSgl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    new_node->next = self->first;
    toSome(&self->first, new_node);
}

fn_((ListSgl_remove(ListSgl* self, ListSgl_Node* node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);

    if_some(self->first, first) {
        if (first == node) {
            self->first = node->next;
            toNone(&node->next);
            return;
        }
        var current = first;
        while_some(current->next, next) {
            if (next == node) {
                current->next = node->next;
                toNone(&node->next);
                return;
            }
            current = next;
        }
    }
}

fn_((ListSgl_popFirst(ListSgl* self))(Opt$Ptr$ListSgl_Node) $scope) {
    debug_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first   = unwrap(self->first);
    self->first = first->next;
    toNone(&first->next);
    return_some(first);
} $unscoped_(fn);

fn_((ListSgl_len(const ListSgl* self))(usize)) {
    debug_assert_nonnull(self);
    usize len = 0;
    if_some(self->first, first) {
        len = 1 + ListSgl_Node_countChildren(first);
    }
    return len;
}

/*========== Doubly Linked List ============================================*/

fn_((ListDbl_Node_init(void))(ListDbl_Node)) {
    return (ListDbl_Node){
        .prev = none(),
        .next = none(),
    };
}

fn_((ListDbl_init(void))(ListDbl)) {
    return (ListDbl){
        .first = none(),
        .last  = none(),
        .len   = 0
    };
}

fn_((ListDbl_insertAfter(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    toSome(&new_node->prev, node);
    new_node->next = node->next;
    if_some(node->next, next) {
        toSome(&next->prev, new_node);
    } else {
        toSome(&self->last, new_node);
    }
    toSome(&node->next, new_node);
    self->len++;
}

fn_((ListDbl_insertBefore(ListDbl* self, ListDbl_Node* node, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);

    new_node->next = some$(Opt$Ptr$ListDbl_Node, node);
    new_node->prev = node->prev;
    if_some(node->prev, prev) {
        toSome(&prev->next, new_node);
    } else {
        toSome(&self->first, new_node);
    }
    toSome(&node->prev, new_node);
    self->len++;
}

fn_((ListDbl_concatByMoving(ListDbl* dst, ListDbl* src))(void)) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);

    if_some(dst->last, last) {
        if_some(src->first, first) {
            last->next = src->first;
            toSome(&first->prev, last);
            dst->last = src->last;
            dst->len += src->len;
        }
    } else {
        dst->first = src->first;
        dst->last  = src->last;
        dst->len   = src->len;
    }
    toNone(&src->first);
    toNone(&src->last);
    src->len = 0;
}

fn_((ListDbl_append(ListDbl* self, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    if_some(self->last, last) {
        ListDbl_insertAfter(self, last, new_node);
    } else {
        toNone(&new_node->prev);
        toNone(&new_node->next);
        toSome(&self->first, new_node);
        toSome(&self->last, new_node);
        self->len = 1;
    }
}

fn_((ListDbl_prepend(ListDbl* self, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);

    if_some(self->first, first) {
        ListDbl_insertBefore(self, first, new_node);
    } else {
        toNone(&new_node->prev);
        toNone(&new_node->next);
        toSome(&self->first, new_node);
        toSome(&self->last, new_node);
        self->len = 1;
    }
}

fn_((ListDbl_remove(ListDbl* self, ListDbl_Node* node))(void)) {
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
    toNone(&node->prev);
    toNone(&node->next);
    self->len--;
}

fn_((ListDbl_pop(ListDbl* self))(Opt$Ptr$ListDbl_Node) $scope) {
    debug_assert_nonnull(self);
    if_none(self->last) {
        return_none();
    }
    var last = unwrap(self->last);
    ListDbl_remove(self, last);
    return_some(last);
} $unscoped_(fn);

fn_((ListDbl_popFirst(ListDbl* self))(Opt$Ptr$ListDbl_Node) $scope) {
    debug_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first = unwrap(self->first);
    ListDbl_remove(self, first);
    return_some(first);
} $unscoped_(fn);

fn_((ListDbl_len(const ListDbl* self))(usize)) {
    debug_assert_nonnull(self);
    return self->len;
}
