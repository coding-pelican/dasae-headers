#include "dh/ListDbl.h"
#include "dh/mem/common.h"
#include "dh/meta.h"

fn_((ListDbl_Link_empty(TypeInfo type))(ListDbl_Link) $scope) {
    let_ignore = type;
    return_({ .prev = none(), .next = none(), debug_only(.type = type) });
} $unscoped(fn);

fn_((ListDbl_Link_adp(P_const$ListDbl_Link self))(const ListDbl_Adp$raw*)) {
    return claim_assert_nonnull(self), as$(const ListDbl_Adp$raw*)(self);
};

fn_((ListDbl_Link_adpMut(P$ListDbl_Link self))(ListDbl_Adp$raw*)) {
    return claim_assert_nonnull(self), as$(ListDbl_Adp$raw*)(self);
};

fn_((ListDbl_Link_data(P_const$ListDbl_Link self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    return ListDbl_Adp_data(ListDbl_Link_adp(self), type);
};

fn_((ListDbl_Link_dataMut(P$ListDbl_Link self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    return ListDbl_Adp_dataMut(ListDbl_Link_adpMut(self), type);
};

fn_((ListDbl_Adp_empty(TypeInfo type, ListDbl_Adp$raw* ret_mem))(ListDbl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    {
        ret_mem->link = ListDbl_Link_empty(type);
        mem_set0P(ListDbl_Link_dataMut(&ret_mem->link, type));
        debug_only(ret_mem->type = type;)
    }
    return ret_mem;
};

fn_((ListDbl_Adp_init(u_V$raw data, ListDbl_Adp$raw* ret_mem))(ListDbl_Adp$raw*)) {
    claim_assert_nonnull(ret_mem);
    let type = data.type;
    {
        ret_mem->link = ListDbl_Link_empty(type);
        mem_setP(ListDbl_Link_dataMut(&ret_mem->link, type), data);
        debug_only(ret_mem->type = type;)
    }
    return ret_mem;
};

fn_((ListDbl_Adp_link(const ListDbl_Adp$raw* self))(P_const$ListDbl_Link)) {
    return claim_assert_nonnull(self), &self->link;
};

fn_((ListDbl_Adp_linkMut(ListDbl_Adp$raw* self))(P$ListDbl_Link)) {
    return claim_assert_nonnull(self), &self->link;
};

fn_((ListDbl_Adp_data(const ListDbl_Adp$raw* self, TypeInfo type))(u_P_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, 1);
};

fn_((ListDbl_Adp_dataMut(ListDbl_Adp$raw* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let ty_fields = typeInfosFrom(u_anyP(self).type, type);
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, 1);
};

fn_((ListDbl_empty(TypeInfo type))(ListDbl) $scope) {
    let_ignore = type;
    return_({ .first = none(), .last = none(), .len = 0, debug_only(.type = type) });
} $unscoped(fn);

fn_((ListDbl_insertNext(ListDbl* self, P$ListDbl_Link link, P$ListDbl_Link new_link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(link), claim_assert_nonnull(new_link);
    debug_assert_eqBy(self->type, link->type, TypeInfo_eql);
    debug_assert_eqBy(link->type, new_link->type, TypeInfo_eql);
    asg_l((&new_link->prev)(some(link)));
    new_link->next = link->next;
    if_some((link->next)(next)) {
        asg_l((&next->prev)(some(new_link)));
    } else {
        asg_l((&self->last)(some(new_link)));
    }
    asg_l((&link->next)(some(new_link)));
    self->len++;
};

fn_((ListDbl_insertPrev(ListDbl* self, P$ListDbl_Link link, P$ListDbl_Link new_link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(link), claim_assert_nonnull(new_link);
    debug_assert_eqBy(self->type, link->type, TypeInfo_eql);
    debug_assert_eqBy(link->type, new_link->type, TypeInfo_eql);
    asg_l((&new_link->next)(some(link)));
    new_link->prev = link->prev;
    if_some((link->prev)(prev)) {
        asg_l((&prev->next)(some(new_link)));
    } else {
        asg_l((&self->first)(some(new_link)));
    }
    asg_l((&link->prev)(some(new_link)));
    self->len++;
};

fn_((ListDbl_concatByMoving(ListDbl* dst, ListDbl* src))(void)) {
    claim_assert_nonnull(dst), claim_assert_nonnull(src);
    debug_assert_eqBy(dst->type, src->type, TypeInfo_eql);
    if_some((dst->last)(last)) {
        if_some((src->first)(first)) {
            last->next = src->first;
            asg_l((&first->prev)(some(last)));
            dst->last = src->last;
            dst->len += src->len;
        }
    } else {
        dst->first = src->first;
        dst->last = src->last;
        dst->len = src->len;
    }
    asg_l((&src->first)(none()));
    asg_l((&src->last)(none()));
    src->len = 0;
};

fn_((ListDbl_append(ListDbl* self, P$ListDbl_Link new_link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(new_link);
    debug_assert_eqBy(self->type, new_link->type, TypeInfo_eql);
    if_some((self->last)(last)) {
        ListDbl_insertNext(self, last, new_link);
    } else {
        asg_l((&new_link->prev)(none()));
        asg_l((&new_link->next)(none()));
        asg_l((&self->first)(some(new_link)));
        asg_l((&self->last)(some(new_link)));
        self->len = 1;
    }
};

fn_((ListDbl_prepend(ListDbl* self, P$ListDbl_Link new_link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(new_link);
    debug_assert_eqBy(self->type, new_link->type, TypeInfo_eql);
    if_some((self->first)(first)) {
        ListDbl_insertPrev(self, first, new_link);
    } else {
        asg_l((&new_link->prev)(none()));
        asg_l((&new_link->next)(none()));
        asg_l((&self->first)(some(new_link)));
        asg_l((&self->last)(some(new_link)));
        self->len = 1;
    }
};

fn_((ListDbl_remove(ListDbl* self, P$ListDbl_Link link))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(link);
    debug_assert_eqBy(self->type, link->type, TypeInfo_eql);
    if_some((link->prev)(prev)) {
        prev->next = link->next;
    } else {
        self->first = link->next;
    }
    if_some((link->next)(next)) {
        next->prev = link->prev;
    } else {
        self->last = link->prev;
    }
    asg_l((&link->prev)(none()));
    asg_l((&link->next)(none()));
    self->len--;
};

fn_((ListDbl_pop(ListDbl* self))(O$P$ListDbl_Link) $scope) {
    claim_assert_nonnull(self);
    let last = orelse_((self->last)(return_none()));
    ListDbl_remove(self, last);
    return_some(last);
} $unscoped(fn);

fn_((ListDbl_shift(ListDbl* self))(O$P$ListDbl_Link) $scope) {
    claim_assert_nonnull(self);
    let first = orelse_((self->first)(return_none()));
    ListDbl_remove(self, first);
    return_some(first);
} $unscoped(fn);

fn_((ListDbl_len(const ListDbl* self))(usize)) {
    return claim_assert_nonnull(self), self->len;
};
