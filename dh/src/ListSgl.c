#include "dh/ListSgl.h"
#include "dh/mem/common.h"
#include "dh/meta.h"

/* $static fn_((alignUp(TypeInfo type))(usize)) {
    return (type.size + (1ull << type.align) - 1) & ~((1ull << type.align) - 1);
} */

fn_((ListSgl_Link_empty(TypeInfo type))(ListSgl_Link) $scope) {
    let_ignore = type;
    return_({ .next = none(), debug_only(.type = type) });
} $unscoped(fn);

fn_((ListSgl_Link_insertNext(P$ListSgl_Link self, P$ListSgl_Link after))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(after);
    debug_assert_eqBy(self->type, after->type, TypeInfo_eql);
    after->next = self->next;
    asg_l((&self->next)(some(after)));
};

fn_((ListSgl_Link_removeNext(P$ListSgl_Link self))(O$P$ListSgl_Link) $scope) {
    claim_assert_nonnull(self);
    let after = orelse_((self->next)(return_none()));
    debug_assert_eqBy(self->type, after->type, TypeInfo_eql);
    self->next = after->next;
    return_some(after);
} $unscoped(fn);

fn_((ListSgl_Link_findLast(P$ListSgl_Link self))(P$ListSgl_Link)) {
    claim_assert_nonnull(self);
    var curr = self;
    while_some(curr->next, next) {
        debug_assert_eqBy(curr->type, next->type, TypeInfo_eql);
        curr = next;
    }
    return curr;
};

fn_((ListSgl_Link_countChildren(P_const$ListSgl_Link self))(usize)) {
    claim_assert_nonnull(self);
    var_(count, usize) = 0;
    var curr = self;
    while_some(curr->next, next) {
        debug_assert_eqBy(curr->type, next->type, TypeInfo_eql);
        count++;
        curr = next;
    }
    return count;
};

fn_((ListSgl_Link_reverse(O$P$ListSgl_Link* indirect))(void)) {
    claim_assert_nonnull(indirect);
    if_none(*indirect) return;
    var_(prev, O$P$ListSgl_Link) = none();
    var_(curr, O$P$ListSgl_Link) = *indirect;
    while_some(curr, link) {
        let next = link->next;
        link->next = prev;
        prev = curr;
        curr = next;
    }
    *indirect = prev;
};

fn_((ListSgl_Link_adp(P_const$ListSgl_Link self))(const ListSgl_Adp$raw*)) {
    return claim_assert_nonnull(self), as$(const ListSgl_Adp$raw*)(self);
};

fn_((ListSgl_Link_adpMut(P$ListSgl_Link self))(ListSgl_Adp$raw*)) {
    return claim_assert_nonnull(self), as$(ListSgl_Adp$raw*)(self);
};

fn_((ListSgl_Link_data(P_const$ListSgl_Link self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    return ListSgl_Adp_data(ListSgl_Link_adp(self), type);
};

fn_((ListSgl_Link_dataMut(P$ListSgl_Link self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    return ListSgl_Adp_dataMut(ListSgl_Link_adpMut(self), type);
};

fn_((ListSgl_Adp_empty(TypeInfo type, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    {
        ret_mem->link = ListSgl_Link_empty(type);
        mem_set0P(ListSgl_Link_dataMut(&ret_mem->link, type));
        debug_only(ret_mem->type = type;)
    }
    return ret_mem;
};

fn_((ListSgl_Adp_init(u_V$raw data, ListSgl_Adp$raw* ret_mem))(ListSgl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    let type = data.type;
    {
        ret_mem->link = ListSgl_Link_empty(type);
        mem_setP(ListSgl_Link_dataMut(&ret_mem->link, type), data);
        debug_only(ret_mem->type = type;)
    }
    return ret_mem;
};

fn_((ListSgl_Adp_link(const ListSgl_Adp$raw* self))(P_const$ListSgl_Link)) {
    return claim_assert_nonnull(self), &self->link;
};

fn_((ListSgl_Adp_linkMut(ListSgl_Adp$raw* self))(P$ListSgl_Link)) {
    return claim_assert_nonnull(self), &self->link;
};

fn_((ListSgl_Adp_data(const ListSgl_Adp$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, 1);
};

fn_((ListSgl_Adp_dataMut(ListSgl_Adp$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, 1);
};

fn_((ListSgl_empty(TypeInfo type))(ListSgl) $scope) {
    let_ignore = type;
    return_({ .first = none(), debug_only(.type = type) });
} $unscoped(fn);

fn_((ListSgl_prepend(ListSgl* self, P$ListSgl_Link before))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(before);
    debug_assert_eqBy(self->type, before->type, TypeInfo_eql);
    before->next = self->first;
    asg_l((&self->first)(some(before)));
};

fn_((ListSgl_remove(ListSgl* self, P$ListSgl_Link link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(link);
    debug_assert_eqBy(self->type, link->type, TypeInfo_eql);
    if_some((self->first)(first)) {
        if (first == link) {
            debug_assert_eqBy(first->type, link->type, TypeInfo_eql);
            self->first = link->next;
            asg_l((&link->next)(none()));
            return;
        }
        var current = first;
        while_some(current->next, next) {
            if (next == link) {
                debug_assert_eqBy(current->type, next->type, TypeInfo_eql);
                current->next = link->next;
                asg_l((&link->next)(none()));
                return;
            }
            current = next;
        }
    }
};

fn_((ListSgl_shift(ListSgl* self))(O$P$ListSgl_Link) $scope) {
    claim_assert_nonnull(self);
    if_none(self->first) return_none();
    let first = unwrap_(self->first);
    debug_assert_eqBy(self->type, first->type, TypeInfo_eql);
    self->first = first->next;
    asg_l((&first->next)(none()));
    return_some(first);
} $unscoped(fn);

fn_((ListSgl_len(const ListSgl* self))(usize)) {
    claim_assert_nonnull(self);
    var_(len, usize) = 0;
    if_some((self->first)(first)) {
        debug_assert_eqBy(self->type, first->type, TypeInfo_eql);
        len = 1 + ListSgl_Link_countChildren(first);
    }
    return len;
};
