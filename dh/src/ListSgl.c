#include "dh/ListSgl.h"

/* $static fn_((alignUp(TypeInfo type))(usize)) {
    return (type.size + (1ull << type.align) - 1) & ~((1ull << type.align) - 1);
} */

fn_((ListSgl_Node_init(P$ListSgl_Node$raw self, TypeInfo type))(P$ListSgl_Node$raw)) {
    asg_lit((&self->next)(none()));
    debug_only(self->type = type;)
    let_ignore = type;
    return self;
}

fn_((ListSgl_Node_data(P_const$ListSgl_Node$raw self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P_const$raw){
        // .raw = as$(u8*)(node) - alignUp(type),
        .raw = self->data,
        .type = type
    };
}

fn_((ListSgl_Node_dataMut(P$ListSgl_Node$raw self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P$raw){
        // .raw = as$(u8*)(node) - alignUp(type),
        .raw = self->data,
        .type = type
    };
}

fn_((ListSgl_Node_insertNext(P$ListSgl_Node$raw self, P$ListSgl_Node$raw after))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(after);
    debug_assert_eqBy(self->type, after->type, TypeInfo_eq);
    after->next = self->next;
    asg_lit((&self->next)(some(after)));
}

fn_((ListSgl_Node_removeNext(P$ListSgl_Node$raw self))(O$P$ListSgl_Node$raw) $scope) {
    claim_assert_nonnull(self);
    let after = orelse_((self->next)(return_none()));
    debug_assert_eqBy(self->type, after->type, TypeInfo_eq);
    self->next = after->next;
    return_some(after);
} $unscoped_(fn);

fn_((ListSgl_Node_findLast(ListSgl_Node$raw* self))(ListSgl_Node$raw*)) {
    claim_assert_nonnull(self);
    var current = self;
    while_some(current->next, next) {
        debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
        current = next;
    }
    return current;
}

fn_((ListSgl_Node_countChildren(P_const$ListSgl_Node$raw self))(usize)) {
    claim_assert_nonnull(self);
    usize count = 0;
    var current = self;
    while_some(current->next, next) {
        debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
        count++;
        current = next;
    }
    return count;
}

fn_((ListSgl_Node_reverse(O$P$ListSgl_Node$raw* indirect))(void)) {
    claim_assert_nonnull(indirect);
    if_none(*indirect) {
        return;
    }
    var_(prev, O$P$ListSgl_Node$raw) = none();
    var_(current, O$P$ListSgl_Node$raw) = *indirect;
    while_some(current, curr) {
        let next = curr->next;
        curr->next = prev;
        prev = current;
        current = next;
    }
    *indirect = prev;
}

fn_((ListSgl_init(TypeInfo type))(ListSgl$raw)) {
    let_ignore = type;
    return (ListSgl$raw){
        .first = none(),
        debug_only(.type = type)
    };
}

fn_((ListSgl_prepend(ListSgl$raw* self, P$ListSgl_Node$raw before))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(before);
    debug_assert_eqBy(self->type, before->type, TypeInfo_eq);

    before->next = self->first;
    asg_lit((&self->first)(some(before)));
}

fn_((ListSgl_remove(ListSgl$raw* self, P$ListSgl_Node$raw node))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(node);
    debug_assert_eqBy(self->type, node->type, TypeInfo_eq);

    if_some((self->first)(first)) {
        if (first == node) {
            debug_assert_eqBy(first->type, node->type, TypeInfo_eq);
            self->first = node->next;
            asg_lit((&node->next)(none()));
            return;
        }
        var current = first;
        while_some(current->next, next) {
            if (next == node) {
                debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
                current->next = node->next;
                asg_lit((&node->next)(none()));
                return;
            }
            current = next;
        }
    }
}

fn_((ListSgl_shift(ListSgl$raw* self))(O$P$ListSgl_Node$raw) $scope) {
    claim_assert_nonnull(self);
    if_none(self->first) {
        return_none();
    }
    var first = unwrap_(self->first);
    debug_assert_eqBy(self->type, first->type, TypeInfo_eq);
    self->first = first->next;
    asg_lit((&first->next)(none()));
    return_some(first);
} $unscoped_(fn);

fn_((ListSgl_len(const ListSgl$raw* self))(usize)) {
    claim_assert_nonnull(self);
    usize len = 0;
    if_some((self->first)(first)) {
        debug_assert_eqBy(self->type, first->type, TypeInfo_eq);
        len = 1 + ListSgl_Node_countChildren(first);
    }
    return len;
}
