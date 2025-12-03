#include "dh/ListSgl.h"
#include "dh/mem/common.h"

/* $static fn_((alignUp(TypeInfo type))(usize)) {
    return (type.size + (1ull << type.align) - 1) & ~((1ull << type.align) - 1);
} */

fn_((ListSgl_Link_empty(TypeInfo type))(ListSgl_Link)) {
    let_ignore = type;
    return (ListSgl_Link){
        .next = none(),
        debug_only(.type = type)
    };
};

fn_((ListSgl_Link_insertNext(P$ListSgl_Link self, P$ListSgl_Link after))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(after);
    debug_assert_eqBy(self->type, after->type, TypeInfo_eq);
    after->next = self->next;
    asg_lit((&self->next)(some(after)));
};

fn_((ListSgl_Link_removeNext(P$ListSgl_Link self))(O$P$ListSgl_Link) $scope) {
    claim_assert_nonnull(self);
    let after = orelse_((self->next)(return_none()));
    debug_assert_eqBy(self->type, after->type, TypeInfo_eq);
    self->next = after->next;
    return_some(after);
} $unscoped_(fn);

fn_((ListSgl_Link_findLast(P$ListSgl_Link self))(P$ListSgl_Link)) {
    claim_assert_nonnull(self);
    var current = self;
    while_some(current->next, next) {
        debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
        current = next;
    }
    return current;
};

fn_((ListSgl_Link_countChildren(P_const$ListSgl_Link self))(usize)) {
    claim_assert_nonnull(self);
    usize count = 0;
    var current = self;
    while_some(current->next, next) {
        debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
        count++;
        current = next;
    }
    return count;
};

fn_((ListSgl_Link_reverse(O$P$ListSgl_Link* indirect))(void)) {
    claim_assert_nonnull(indirect);
    if_none(*indirect) {
        return;
    }
    var_(prev, O$P$ListSgl_Link) = none();
    var_(current, O$P$ListSgl_Link) = *indirect;
    while_some(current, curr) {
        let next = curr->next;
        curr->next = prev;
        prev = current;
        current = next;
    }
    *indirect = prev;
};

fn_((ListSgl_Link_adp(P_const$ListSgl_Link self))(const ListSgl_Adp$raw*)) {
    claim_assert_nonnull(self);
    return as$(const ListSgl_Adp$raw*)(self);
};

fn_((ListSgl_Link_adpMut(P$ListSgl_Link self))(ListSgl_Adp$raw*)) {
    claim_assert_nonnull(self);
    return as$(ListSgl_Adp$raw*)(self);
};

fn_((ListSgl_Link_data(P_const$ListSgl_Link self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P_const$raw){ .raw = ListSgl_Link_adp(self)->data, .type = type };
};

fn_((ListSgl_Link_dataMut(P$ListSgl_Link self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P$raw){ .raw = ListSgl_Link_adpMut(self)->data, .type = type };
};

fn_((ListSgl_Adp_empty(TypeInfo type, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->link = ListSgl_Link_empty(type);
    mem_set0((u_S$raw){ .ptr = ret_mem->data, .len = 1, .type = type });
    debug_only(ret_mem -> type = type;)
    return ret_mem;
};

fn_((ListSgl_Adp_init(u_V$raw data, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    let type = data.inner_type;
    ret_mem->link = ListSgl_Link_empty(type);
    mem_set((u_S$raw){ .ptr = ret_mem->data, .len = 1, .type = type }, data);
    debug_only(ret_mem -> type = type;)
    return ret_mem;
};

fn_((ListSgl_Adp_link(const ListSgl_Adp$raw* self))(P_const$ListSgl_Link)) {
    claim_assert_nonnull(self);
    return &self->link;
};

fn_((ListSgl_Adp_linkMut(ListSgl_Adp$raw* self))(P$ListSgl_Link)) {
    claim_assert_nonnull(self);
    return &self->link;
};

fn_((ListSgl_Adp_data(const ListSgl_Adp$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self->data, .type = type };
};

fn_((ListSgl_Adp_dataMut(ListSgl_Adp$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_P$raw){ .raw = self->data, .type = type };
};

fn_((ListSgl_empty(TypeInfo type))(ListSgl)) {
    let_ignore = type;
    return (ListSgl){
        .first = none(),
        debug_only(.type = type)
    };
};

fn_((ListSgl_prepend(ListSgl* self, P$ListSgl_Link before))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(before);
    debug_assert_eqBy(self->type, before->type, TypeInfo_eq);

    before->next = self->first;
    asg_lit((&self->first)(some(before)));
};

fn_((ListSgl_remove(ListSgl* self, P$ListSgl_Link link))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(link);
    debug_assert_eqBy(self->type, link->type, TypeInfo_eq);

    if_some((self->first)(first)) {
        if (first == link) {
            debug_assert_eqBy(first->type, link->type, TypeInfo_eq);
            self->first = link->next;
            asg_lit((&link->next)(none()));
            return;
        }
        var current = first;
        while_some(current->next, next) {
            if (next == link) {
                debug_assert_eqBy(current->type, next->type, TypeInfo_eq);
                current->next = link->next;
                asg_lit((&link->next)(none()));
                return;
            }
            current = next;
        }
    }
};

fn_((ListSgl_shift(ListSgl* self))(O$P$ListSgl_Link) $scope) {
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

fn_((ListSgl_len(const ListSgl* self))(usize)) {
    claim_assert_nonnull(self);
    usize len = 0;
    if_some((self->first)(first)) {
        debug_assert_eqBy(self->type, first->type, TypeInfo_eq);
        len = 1 + ListSgl_Link_countChildren(first);
    }
    return len;
};
