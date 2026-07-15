#include "dh/ArrStk.h"
#include "dh/ArrList.h"

claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(ArrStk), packTypeInfo$(ArrList)));
claim_assert_static(offsetTo(ArrStk, items) == offsetTo(ArrList, items));
claim_assert_static(offsetTo(ArrStk, cap) == offsetTo(ArrList, cap));
debug_assert_static(offsetTo(ArrStk, type) == offsetTo(ArrList, type));

#define ArrStk__stkAsList(_p_stk...) T_switch$((TypeOf(_p_stk))( \
    T_qual$((const ArrStk*))(ptrQualCast$((const ArrList*)(_p_stk))), \
    T_qual$((ArrStk*))(ptrQualCast$((ArrList*)(_p_stk))) \
))
#define ArrStk__listAsStk(_p_list...) T_switch$((TypeOf(_p_list))( \
    T_qual$((const ArrList*))(ptrQualCast$((const ArrStk*)(_p_list))), \
    T_qual$((ArrList*))(ptrQualCast$((ArrStk*)(_p_list))) \
))
$attr($inline_always)
$static fn_((ArrStk__stkToList(ArrStk self))(ArrList)) { return *ArrStk__stkAsList(&self); }
$attr($inline_always)
$static fn_((ArrStk__listToStk(ArrList self))(ArrStk)) { return *ArrStk__listAsStk(&self); }

fn_((ArrStk_empty(TypeInfo type))(ArrStk)) {
    return ArrStk__listToStk(ArrList_empty(type));
}

fn_((ArrStk_fixed(u_S$raw buf))(ArrStk)) {
    return ArrStk__listToStk(ArrList_fixed(buf));
}

fn_((ArrStk_init(TypeInfo type, mem_Alctr gpa, usize cap))(mem_E$ArrStk) $scope) {
    return_ok(ArrStk__listToStk(try_(ArrList_init(type, gpa, cap))));
} $unscoped(fn);

fn_((ArrStk_fini(ArrStk* self, TypeInfo type, mem_Alctr gpa))(void)) {
    ArrList_fini(ArrStk__stkAsList(self), type, gpa);
}

fn_((ArrStk_clone(ArrStk self, TypeInfo type, mem_Alctr gpa))(mem_E$ArrStk) $scope) {
    return_ok(ArrStk__listToStk(try_(ArrList_clone(ArrStk__stkToList(self), type, gpa))));
} $unscoped(fn);

claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(ArrStk_Grip), packTypeInfo$(ArrList_Grip)));
claim_assert_static(offsetTo(ArrStk_Grip, buf) == offsetTo(ArrList_Grip, buf));
claim_assert_static(offsetTo(ArrStk_Grip, len) == offsetTo(ArrList_Grip, len));
claim_assert_static(offsetTo(ArrStk_Grip, ctx) == offsetTo(ArrList_Grip, ctx));

#define listGripAsStkGrip(_p_grip...) T_switch$((TypeOf(_p_grip))( \
    T_qual$((const ArrList_Grip*))(ptrQualCast$((const ArrStk_Grip*)(_p_grip))), \
    T_qual$((ArrList_Grip*))(ptrQualCast$((ArrStk_Grip*)(_p_grip))) \
))
#define stkGripAsListGrip(_p_grip...) T_switch$((TypeOf(_p_grip))( \
    T_qual$((const ArrStk_Grip*))(ptrQualCast$((const ArrList_Grip*)(_p_grip))), \
    T_qual$((ArrStk_Grip*))(ptrQualCast$((ArrList_Grip*)(_p_grip))) \
))
$attr($inline_always)
$static fn_((ArrStk__listGripToStkGrip(ArrList_Grip self))(ArrStk_Grip)) { return *listGripAsStkGrip(&self); }

fn_((ArrStk_grip(u_S$raw buf, usize* len))(ArrStk_Grip)) {
    return ArrStk__listGripToStkGrip(ArrList_grip(buf, len));
}

fn_((ArrStk_Grip_release(ArrStk_Grip* self, TypeInfo type))(void)) {
    return ArrList_Grip_release(stkGripAsListGrip(self), type);
}

fn_((ArrStk_len(ArrStk self))(usize)) {
    return ArrList_len(ArrStk__stkToList(self));
}

fn_((ArrStk_cap(ArrStk self))(usize)) {
    return ArrList_cap(ArrStk__stkToList(self));
}

fn_((ArrStk_isEmpty(ArrStk self))(bool)) {
    return ArrList_isEmpty(ArrStk__stkToList(self));
}

fn_((ArrStk_isFull(ArrStk self))(bool)) {
    return ArrList_isFull(ArrStk__stkToList(self));
}

fn_((ArrStk_at(ArrStk self, TypeInfo type, usize idx))(u_P_const$raw)) {
    return ArrList_at(ArrStk__stkToList(self), type, idx);
}

fn_((ArrStk_atMut(ArrStk self, TypeInfo type, usize idx))(u_P$raw)) {
    return ArrList_atMut(ArrStk__stkToList(self), type, idx);
}

fn_((ArrStk_top(ArrStk self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrList_back(ArrStk__stkToList(self), type);
}

fn_((ArrStk_topMut(ArrStk self, TypeInfo type))(O$u_P$raw)) {
    return ArrList_backMut(ArrStk__stkToList(self), type);
}

fn_((ArrStk_items(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_items(ArrStk__stkToList(self), type);
}

fn_((ArrStk_itemsMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsMut(ArrStk__stkToList(self), type);
}

fn_((ArrStk_itemsCapped(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_itemsCapped(ArrStk__stkToList(self), type);
}

fn_((ArrStk_itemsCappedMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsCappedMut(ArrStk__stkToList(self), type);
}

fn_((ArrStk_itemsUnused(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_itemsUnused(ArrStk__stkToList(self), type);
}

fn_((ArrStk_itemsUnusedMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsUnusedMut(ArrStk__stkToList(self), type);
}

fn_((ArrStk_ensureCap(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize new_cap))(mem_E$void)) {
    return ArrList_ensureCap(ArrStk__stkAsList(self), type, gpa, new_cap);
}

fn_((ArrStk_ensureCapPrecise(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize new_cap))(mem_E$void)) {
    return ArrList_ensureCapPrecise(ArrStk__stkAsList(self), type, gpa, new_cap);
}

fn_((ArrStk_ensureUnusedCap(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize additional))(mem_E$void)) {
    return ArrList_ensureUnusedCap(ArrStk__stkAsList(self), type, gpa, additional);
}

fn_((ArrStk_expandToCap(ArrStk* self))(void)) {
    return ArrList_expandToCap(ArrStk__stkAsList(self));
}

fn_((ArrStk_resize(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize new_len))(mem_E$void)) {
    return ArrList_resize(ArrStk__stkAsList(self), type, gpa, new_len);
}

fn_((ArrStk_shrinkRetainingCap(ArrStk* self, usize new_len))(void)) {
    return ArrList_shrinkRetainingCap(ArrStk__stkAsList(self), new_len);
}

fn_((ArrStk_shrinkAndFree(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize new_len))(void)) {
    return ArrList_shrinkAndFree(ArrStk__stkAsList(self), type, gpa, new_len);
}

fn_((ArrStk_clearRetainingCap(ArrStk* self))(void)) {
    return ArrList_clearRetainingCap(ArrStk__stkAsList(self));
}

fn_((ArrStk_clearAndFree(ArrStk* self, TypeInfo type, mem_Alctr gpa))(void)) {
    return ArrList_clearAndFree(ArrStk__stkAsList(self), type, gpa);
}

fn_((ArrStk_add(ArrStk* self, TypeInfo type, mem_Alctr gpa))(mem_E$u_P$raw)) {
    return ArrList_addBack(ArrStk__stkAsList(self), type, gpa);
}

fn_((ArrStk_addFixed(ArrStk* self, TypeInfo type))(mem_E$u_P$raw)) {
    return ArrList_addBackFixed(ArrStk__stkAsList(self), type);
}

fn_((ArrStk_addWithin(ArrStk* self, TypeInfo type))(u_P$raw)) {
    return ArrList_addBackWithin(ArrStk__stkAsList(self), type);
}

fn_((ArrStk_addN(ArrStk* self, TypeInfo type, mem_Alctr gpa, usize n))(mem_E$u_S$raw)) {
    return ArrList_addBackN(ArrStk__stkAsList(self), type, gpa, n);
}

fn_((ArrStk_addNFixed(ArrStk* self, TypeInfo type, usize n))(mem_E$u_S$raw)) {
    return ArrList_addBackNFixed(ArrStk__stkAsList(self), type, n);
}

fn_((ArrStk_push(ArrStk* self, mem_Alctr gpa, u_V$raw item))(mem_E$void)) {
    return ArrList_append(ArrStk__stkAsList(self), gpa, item);
}

fn_((ArrStk_pushFixed(ArrStk* self, u_V$raw item))(mem_E$void)) {
    return ArrList_appendFixed(ArrStk__stkAsList(self), item);
}

fn_((ArrStk_pushWithin(ArrStk* self, u_V$raw item))(void)) {
    return ArrList_appendWithin(ArrStk__stkAsList(self), item);
}

fn_((ArrStk_pushS(ArrStk* self, mem_Alctr gpa, u_S_const$raw items))(mem_E$void)) {
    return ArrList_appendS(ArrStk__stkAsList(self), gpa, items);
}

fn_((ArrStk_pushSFixed(ArrStk* self, u_S_const$raw items))(mem_E$void)) {
    return ArrList_appendSFixed(ArrStk__stkAsList(self), items);
}

fn_((ArrStk_pushSWithin(ArrStk* self, u_S_const$raw items))(void)) {
    return ArrList_appendSWithin(ArrStk__stkAsList(self), items);
}

fn_((ArrStk_pushN(ArrStk* self, mem_Alctr gpa, u_V$raw item, usize n))(mem_E$void)) {
    return ArrList_appendN(ArrStk__stkAsList(self), gpa, item, n);
}

fn_((ArrStk_pushNFixed(ArrStk* self, u_V$raw item, usize n))(mem_E$void)) {
    return ArrList_appendNFixed(ArrStk__stkAsList(self), item, n);
}

fn_((ArrStk_pushNWithin(ArrStk* self, u_V$raw item, usize n))(void)) {
    return ArrList_appendNWithin(ArrStk__stkAsList(self), item, n);
}

fn_((ArrStk_pop(ArrStk* self, u_V$raw ret_mem))(O$u_V$raw)) {
    return ArrList_pop(ArrStk__stkAsList(self), ret_mem);
}
