#include "dh/ArrStk.h"
#include "dh/ArrList.h"

claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(ArrStk), packTypeInfo$(ArrList)));
claim_assert_static(offsetTo(ArrStk, items) == offsetTo(ArrList, items));
claim_assert_static(offsetTo(ArrStk, cap) == offsetTo(ArrList, cap));
debug_assert_static(offsetTo(ArrStk, type) == offsetTo(ArrList, type));

#define stkAsList(_p_stk...) _Generic( \
    TypeOf(_p_stk), \
    const ArrStk*: as$(const ArrList*)(_p_stk), \
    ArrStk*: as$(ArrList*)(_p_stk) \
)
#define listAsStk(_p_list...) _Generic( \
    TypeOf(_p_list), \
    const ArrList*: as$(const ArrStk*)(_p_list), \
    ArrList*: as$(ArrStk*)(_p_list) \
)
$attr($inline_always)
$static fn_((stkToList(ArrStk self))(ArrList)) { return *stkAsList(&self); }
$attr($inline_always)
$static fn_((listToStk(ArrList self))(ArrStk)) { return *listAsStk(&self); }

fn_((ArrStk_empty(TypeInfo type))(ArrStk)) {
    return listToStk(ArrList_empty(type));
}

fn_((ArrStk_fixed(u_S$raw buf))(ArrStk)) {
    return listToStk(ArrList_fixed(buf));
}

fn_((ArrStk_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrStk) $scope) {
    return_ok(listToStk(try_(ArrList_init(type, gpa, cap))));
} $unscoped_(fn);

fn_((ArrStk_fini(ArrStk* self, TypeInfo type, mem_Allocator gpa))(void)) {
    ArrList_fini(stkAsList(self), type, gpa);
}

fn_((ArrStk_clone(ArrStk self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrStk) $scope) {
    return_ok(listToStk(try_(ArrList_clone(stkToList(self), type, gpa))));
} $unscoped_(fn);

claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(ArrStk_Grip), packTypeInfo$(ArrList_Grip)));
claim_assert_static(offsetTo(ArrStk_Grip, buf) == offsetTo(ArrList_Grip, buf));
claim_assert_static(offsetTo(ArrStk_Grip, len) == offsetTo(ArrList_Grip, len));
claim_assert_static(offsetTo(ArrStk_Grip, ctx) == offsetTo(ArrList_Grip, ctx));

#define listGripAsStkGrip(_p_grip...) _Generic( \
    TypeOf(_p_grip), \
    const ArrList_Grip*: as$(const ArrStk_Grip*)(_p_grip), \
    ArrList_Grip*: as$(ArrStk_Grip*)(_p_grip) \
)
#define stkGripAsListGrip(_p_grip...) _Generic( \
    TypeOf(_p_grip), \
    const ArrStk_Grip*: as$(const ArrList_Grip*)(_p_grip), \
    ArrStk_Grip*: as$(ArrList_Grip*)(_p_grip) \
)
$attr($inline_always)
$static fn_((listGripToStkGrip(ArrList_Grip self))(ArrStk_Grip)) { return *listGripAsStkGrip(&self); }

fn_((ArrStk_grip(u_S$raw buf, usize* len))(ArrStk_Grip)) {
    return listGripToStkGrip(ArrList_grip(buf, len));
}

fn_((ArrStk_Grip_release(ArrStk_Grip* self, TypeInfo type))(void)) {
    return ArrList_Grip_release(stkGripAsListGrip(self), type);
}

fn_((ArrStk_len(ArrStk self))(usize)) {
    return ArrList_len(stkToList(self));
}

fn_((ArrStk_cap(ArrStk self))(usize)) {
    return ArrList_cap(stkToList(self));
}

fn_((ArrStk_isEmpty(ArrStk self))(bool)) {
    return ArrList_isEmpty(stkToList(self));
}

fn_((ArrStk_isFull(ArrStk self))(bool)) {
    return ArrList_isFull(stkToList(self));
}

fn_((ArrStk_at(ArrStk self, TypeInfo type, usize idx))(u_P_const$raw)) {
    return ArrList_at(stkToList(self), type, idx);
}

fn_((ArrStk_atMut(ArrStk self, TypeInfo type, usize idx))(u_P$raw)) {
    return ArrList_atMut(stkToList(self), type, idx);
}

fn_((ArrStk_top(ArrStk self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrList_back(stkToList(self), type);
}

fn_((ArrStk_topMut(ArrStk self, TypeInfo type))(O$u_P$raw)) {
    return ArrList_backMut(stkToList(self), type);
}

fn_((ArrStk_items(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_items(stkToList(self), type);
}

fn_((ArrStk_itemsMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsMut(stkToList(self), type);
}

fn_((ArrStk_itemsCapped(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_itemsCapped(stkToList(self), type);
}

fn_((ArrStk_itemsCappedMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsCappedMut(stkToList(self), type);
}

fn_((ArrStk_itemsUnused(ArrStk self, TypeInfo type))(u_S_const$raw)) {
    return ArrList_itemsUnused(stkToList(self), type);
}

fn_((ArrStk_itemsUnusedMut(ArrStk self, TypeInfo type))(u_S$raw)) {
    return ArrList_itemsUnusedMut(stkToList(self), type);
}

fn_((ArrStk_ensureCap(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) {
    return ArrList_ensureCap(stkAsList(self), type, gpa, new_cap);
}

fn_((ArrStk_ensureCapPrecise(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) {
    return ArrList_ensureCapPrecise(stkAsList(self), type, gpa, new_cap);
}

fn_((ArrStk_ensureUnusedCap(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) {
    return ArrList_ensureUnusedCap(stkAsList(self), type, gpa, additional);
}

fn_((ArrStk_expandToCap(ArrStk* self))(void)) {
    return ArrList_expandToCap(stkAsList(self));
}

fn_((ArrStk_resize(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_len))(mem_Err$void)) {
    return ArrList_resize(stkAsList(self), type, gpa, new_len);
}

fn_((ArrStk_shrinkRetainingCap(ArrStk* self, usize new_len))(void)) {
    return ArrList_shrinkRetainingCap(stkAsList(self), new_len);
}

fn_((ArrStk_shrinkAndFree(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void)) {
    return ArrList_shrinkAndFree(stkAsList(self), type, gpa, new_len);
}

fn_((ArrStk_clearRetainingCap(ArrStk* self))(void)) {
    return ArrList_clearRetainingCap(stkAsList(self));
}

fn_((ArrStk_clearAndFree(ArrStk* self, TypeInfo type, mem_Allocator gpa))(void)) {
    return ArrList_clearAndFree(stkAsList(self), type, gpa);
}

fn_((ArrStk_add(ArrStk* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw)) {
    return ArrList_addBack(stkAsList(self), type, gpa);
}

fn_((ArrStk_addFixed(ArrStk* self, TypeInfo type))(mem_Err$u_P$raw)) {
    return ArrList_addBackFixed(stkAsList(self), type);
}

fn_((ArrStk_addWithin(ArrStk* self, TypeInfo type))(u_P$raw)) {
    return ArrList_addBackWithin(stkAsList(self), type);
}

fn_((ArrStk_addN(ArrStk* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw)) {
    return ArrList_addBackN(stkAsList(self), type, gpa, n);
}

fn_((ArrStk_addFixedN(ArrStk* self, TypeInfo type, usize n))(mem_Err$u_S$raw)) {
    return ArrList_addBackFixedN(stkAsList(self), type, n);
}

fn_((ArrStk_push(ArrStk* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) {
    return ArrList_append(stkAsList(self), gpa, item);
}

fn_((ArrStk_pushFixed(ArrStk* self, u_V$raw item))(mem_Err$void)) {
    return ArrList_appendFixed(stkAsList(self), item);
}

fn_((ArrStk_pushWithin(ArrStk* self, u_V$raw item))(void)) {
    return ArrList_appendWithin(stkAsList(self), item);
}

fn_((ArrStk_pushS(ArrStk* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) {
    return ArrList_appendS(stkAsList(self), gpa, items);
}

fn_((ArrStk_pushFixedS(ArrStk* self, u_S_const$raw items))(mem_Err$void)) {
    return ArrList_appendFixedS(stkAsList(self), items);
}

fn_((ArrStk_pushWithinS(ArrStk* self, u_S_const$raw items))(void)) {
    return ArrList_appendWithinS(stkAsList(self), items);
}

fn_((ArrStk_pushN(ArrStk* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) {
    return ArrList_appendN(stkAsList(self), gpa, item, n);
}

fn_((ArrStk_pushFixedN(ArrStk* self, u_V$raw item, usize n))(mem_Err$void)) {
    return ArrList_appendFixedN(stkAsList(self), item, n);
}

fn_((ArrStk_pushWithinN(ArrStk* self, u_V$raw item, usize n))(void)) {
    return ArrList_appendWithinN(stkAsList(self), item, n);
}

fn_((ArrStk_pop(ArrStk* self, u_V$raw ret_mem))(O$u_V$raw)) {
    return ArrList_pop(stkAsList(self), ret_mem);
}
