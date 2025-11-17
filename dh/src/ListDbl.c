#if UNUSED_CODE
#include "dh/ListDbl.h"

$static fn_((alignUp(TypeInfo type))(usize)) {
    return (type.size + (1ull << type.align) - 1) & ~((1ull << type.align) - 1);
}

fn_((ListDbl_Node_init(TypeInfo type))(ListDbl_Node)) {
    return (ListDbl_Node){
        .type = type,
        .prev = none(),
        .next = none(),
    };
}

fn_((ListDbl_Node_data(ListDbl_Node* node))(u_P$raw)) {
    debug_assert_nonnull(node);
    return (u_P$raw){
        .type = node->type,
        .inner = as$(u8*)(node) - alignUp(node->type),
    };
}

fn_((ListDbl_init(TypeInfo type))(ListDbl$raw)) {
    return (ListDbl$raw){
        .type = type,
        .first = none(),
        .last = none(),
        .len = 0,
    };
}

fn_((ListDbl_insertNext(ListDbl$raw* self, ListDbl_Node* node, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);
    debug_assert(TypeInfo_eq(self->type, node->type));
    debug_assert(TypeInfo_eq(node->type, new_node->type));

    asg$O((&new_node->prev)(some(node)));
    new_node->next = node->next;
    if_some((node->next)(next)) {
        asg$O((&next->prev)(some(new_node)));
    } else {
        asg$O((&self->last)(some(new_node)));
    }
    asg$O((&node->next)(some(new_node)));
    self->len++;
}

fn_((ListDbl_insertPrev(ListDbl$raw* self, ListDbl_Node* node, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert_nonnull(new_node);
    debug_assert(TypeInfo_eq(self->type, node->type));
    debug_assert(TypeInfo_eq(node->type, new_node->type));

    new_node->next = some$((P$ListDbl_Node)(node));
    new_node->prev = node->prev;
    if_some((node->prev)(prev)) {
        asg$O((&prev->next)(some(new_node)));
    } else {
        asg$O((&self->first)(some(new_node)));
    }
    asg$O((&node->prev)(some(new_node)));
    self->len++;
}

fn_((ListDbl_concatByMoving(ListDbl$raw* dst, ListDbl$raw* src))(void)) {
    debug_assert_nonnull(dst);
    debug_assert_nonnull(src);
    debug_assert(TypeInfo_eq(dst->type, src->type));

    if_some((dst->last)(last)) {
        if_some((src->first)(first)) {
            last->next = src->first;
            asg$O((&first->prev)(some(last)));
            dst->last = src->last;
            dst->len += src->len;
        }
    } else {
        dst->first = src->first;
        dst->last = src->last;
        dst->len = src->len;
    }
    asg$O((&src->first)(none()));
    asg$O((&src->last)(none()));
    src->len = 0;
}

fn_((ListDbl_append(ListDbl$raw* self, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);
    debug_assert(TypeInfo_eq(self->type, new_node->type));

    if_some((self->last)(last)) {
        ListDbl_insertNext(self, last, new_node);
    } else {
        asg$O((&new_node->prev)(none()));
        asg$O((&new_node->next)(none()));
        asg$O((&self->first)(some(new_node)));
        asg$O((&self->last)(some(new_node)));
        self->len = 1;
    }
}

fn_((ListDbl_prepend(ListDbl$raw* self, ListDbl_Node* new_node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(new_node);
    debug_assert(TypeInfo_eq(self->type, new_node->type));

    if_some((self->first)(first)) {
        ListDbl_insertPrev(self, first, new_node);
    } else {
        asg$O((&new_node->prev)(none()));
        asg$O((&new_node->next)(none()));
        asg$O((&self->first)(some(new_node)));
        asg$O((&self->last)(some(new_node)));
        self->len = 1;
    }
}

fn_((ListDbl_remove(ListDbl$raw* self, ListDbl_Node* node))(void)) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(node);
    debug_assert(TypeInfo_eq(self->type, node->type));

    if_some((node->prev)(prev)) {
        prev->next = node->next;
    } else {
        self->first = node->next;
    }
    if_some((node->next)(next)) {
        next->prev = node->prev;
    } else {
        self->last = node->prev;
    }
    asg$O((&node->prev)(none()));
    asg$O((&node->next)(none()));
    self->len--;
}

fn_((ListDbl_pop(ListDbl$raw* self))(O$P$ListDbl_Node) $scope) {
    debug_assert_nonnull(self);
    if_none(self->last) {
        return_none();
    }
    var last = unwrap_(self->last);
    ListDbl_remove(self, last);
    return_some(last);
} $unscoped_(fn);

fn_((ListDbl_shift(ListDbl$raw* self))(O$P$ListDbl_Node) $scope) {
    debug_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first = unwrap_(self->first);
    ListDbl_remove(self, first);
    return_some(first);
} $unscoped_(fn);

fn_((ListDbl_len(const ListDbl$raw* self))(usize)) {
    debug_assert_nonnull(self);
    return self->len;
}

#endif /* UNUSED_CODE */
