#include "dh/ArrQue.h"
#include "dh/ArrDeq.h"

claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(ArrQue), packTypeInfo$(ArrDeq)));
claim_assert_static(offsetTo(ArrQue, buf) == offsetTo(ArrDeq, buf));
claim_assert_static(offsetTo(ArrQue, head) == offsetTo(ArrDeq, head));
claim_assert_static(offsetTo(ArrQue, len) == offsetTo(ArrDeq, len));
debug_assert_static(offsetTo(ArrQue, type) == offsetTo(ArrDeq, type));

#define queAsDeq(_p_que...) _Generic( \
    TypeOf(_p_que), \
    const ArrQue*: as$(const ArrDeq*)(_p_que), \
    ArrQue*: as$(ArrDeq*)(_p_que) \
)
#define deqAsQue(_p_deq...) _Generic( \
    TypeOf(_p_deq), \
    const ArrDeq*: as$(const ArrQue*)(_p_deq), \
    ArrDeq*: as$(ArrQue*)(_p_deq) \
)
$attr($inline_always)
$static fn_((queToDeq(ArrQue self))(ArrDeq)) { return *queAsDeq(&self); }
$attr($inline_always)
$static fn_((deqToQue(ArrDeq self))(ArrQue)) { return *deqAsQue(&self); }

fn_((ArrQue_empty(TypeInfo type))(ArrQue)) {
    return deqToQue(ArrDeq_empty(type));
}

fn_((ArrQue_fixed(u_S$raw buf))(ArrQue)) {
    return deqToQue(ArrDeq_fixed(buf));
}

fn_((ArrQue_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrQue) $scope) {
    return_ok(deqToQue(try_(ArrDeq_init(type, gpa, cap))));
} $unscoped_(fn);

fn_((ArrQue_fini(ArrQue* self, TypeInfo type, mem_Allocator gpa))(void)) {
    ArrDeq_fini(queAsDeq(self), type, gpa);
}

claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(ArrQue_Grip), packTypeInfo$(ArrDeq_Grip)));
claim_assert_static(offsetTo(ArrQue_Grip, buf) == offsetTo(ArrDeq_Grip, buf));
claim_assert_static(offsetTo(ArrQue_Grip, head) == offsetTo(ArrDeq_Grip, head));
claim_assert_static(offsetTo(ArrQue_Grip, len) == offsetTo(ArrDeq_Grip, len));
claim_assert_static(offsetTo(ArrQue_Grip, ctx) == offsetTo(ArrDeq_Grip, ctx));

#define deqGripAsQueGrip(_p_grip...) _Generic( \
    TypeOf(_p_grip), \
    const ArrDeq_Grip*: as$(const ArrQue_Grip*)(_p_grip), \
    ArrDeq_Grip*: as$(ArrQue_Grip*)(_p_grip) \
)
#define queGripAsDeqGrip(_p_grip...) _Generic( \
    TypeOf(_p_grip), \
    const ArrQue_Grip*: as$(const ArrDeq_Grip*)(_p_grip), \
    ArrQue_Grip*: as$(ArrDeq_Grip*)(_p_grip) \
)
$attr($inline_always)
$static fn_((deqGripToQueGrip(ArrDeq_Grip self))(ArrQue_Grip)) { return *deqGripAsQueGrip(&self); }

fn_((ArrQue_grip(u_S$raw buf, usize* head, usize* len))(ArrQue_Grip)) {
    return deqGripToQueGrip(ArrDeq_grip(buf, head, len));
}

fn_((ArrQue_Grip_release(ArrQue_Grip* self, TypeInfo type))(void)) {
    return ArrDeq_Grip_release(queGripAsDeqGrip(self), type);
}

fn_((ArrQue_len(ArrQue self))(usize)) {
    return ArrDeq_len(queToDeq(self));
}

fn_((ArrQue_cap(ArrQue self))(usize)) {
    return ArrDeq_cap(queToDeq(self));
}

fn_((ArrQue_isEmpty(ArrQue self))(bool)) {
    return ArrDeq_isEmpty(queToDeq(self));
}

fn_((ArrQue_isFull(ArrQue self))(bool)) {
    return ArrDeq_isFull(queToDeq(self));
}

fn_((ArrQue_head(ArrQue self))(usize)) {
    return ArrDeq_head(queToDeq(self));
}

fn_((ArrQue_tail(ArrQue self))(usize)) {
    return ArrDeq_tail(queToDeq(self));
}

fn_((ArrQue_at(ArrQue self, TypeInfo type, usize idx))(u_P_const$raw)) {
    return ArrDeq_at(queToDeq(self), type, idx);
}

fn_((ArrQue_atMut(ArrQue self, TypeInfo type, usize idx))(u_P$raw)) {
    return ArrDeq_atMut(queToDeq(self), type, idx);
}

fn_((ArrQue_front(ArrQue self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_front(queToDeq(self), type);
}

fn_((ArrQue_frontMut(ArrQue self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_frontMut(queToDeq(self), type);
}

fn_((ArrQue_back(ArrQue self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_back(queToDeq(self), type);
}

fn_((ArrQue_backMut(ArrQue self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_backMut(queToDeq(self), type);
}

fn_((ArrQue_ensureCap(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) {
    return ArrDeq_ensureCap(queAsDeq(self), type, gpa, new_cap);
}

fn_((ArrQue_ensureCapPrecise(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) {
    return ArrDeq_ensureCapPrecise(queAsDeq(self), type, gpa, new_cap);
}

fn_((ArrQue_ensureUnusedCap(ArrQue* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) {
    return ArrDeq_ensureUnusedCap(queAsDeq(self), type, gpa, additional);
}

fn_((ArrQue_clearRetainingCap(ArrQue* self))(void)) {
    return ArrDeq_clearRetainingCap(queAsDeq(self));
}

fn_((ArrQue_clearAndFree(ArrQue* self, TypeInfo type, mem_Allocator gpa))(void)) {
    return ArrDeq_clearAndFree(queAsDeq(self), type, gpa);
}

fn_((ArrQue_enque(ArrQue* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) {
    return ArrDeq_append(queAsDeq(self), gpa, item);
}

fn_((ArrQue_enqueFixed(ArrQue* self, u_V$raw item))(mem_Err$void)) {
    return ArrDeq_appendFixed(queAsDeq(self), item);
}

fn_((ArrQue_enqueWithin(ArrQue* self, u_V$raw item))(void)) {
    return ArrDeq_appendWithin(queAsDeq(self), item);
}

fn_((ArrQue_deque(ArrQue* self, u_V$raw ret_mem))(O$u_V$raw)) {
    return ArrDeq_shift(queAsDeq(self), ret_mem);
}

claim_assert_static(TypeInfoPacked_eq(packTypeInfo$(ArrQue_Iter), packTypeInfo$(ArrDeq_Iter)));
claim_assert_static(offsetTo(ArrQue_Iter, que) == offsetTo(ArrDeq_Iter, deq));
claim_assert_static(offsetTo(ArrQue_Iter, idx) == offsetTo(ArrDeq_Iter, idx));
debug_assert_static(offsetTo(ArrQue_Iter, type) == offsetTo(ArrDeq_Iter, type));

#define queIterAsDeqIter(_p_que_iter...) _Generic( \
    TypeOf(_p_que_iter), \
    const ArrQue_Iter*: as$(const ArrDeq_Iter*)(_p_que_iter), \
    ArrQue_Iter*: as$(ArrDeq_Iter*)(_p_que_iter) \
)
#define deqIterAsQueIter(_p_deq_iter...) _Generic( \
    TypeOf(_p_deq_iter), \
    const ArrDeq_Iter*: as$(const ArrQue_Iter*)(_p_deq_iter), \
    ArrDeq_Iter*: as$(ArrQue_Iter*)(_p_deq_iter) \
)
$attr($inline_always)
$static fn_((deqIterToQueIter(ArrDeq_Iter self))(ArrQue_Iter)) { return *deqIterAsQueIter(&self); }

fn_((ArrQue_iter(const ArrQue* self, TypeInfo type))(ArrQue_Iter)) {
    return deqIterToQueIter(ArrDeq_iter(queAsDeq(self), type));
}

fn_((ArrQue_Iter_next(ArrQue_Iter* self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_Iter_next(queIterAsDeqIter(self), type);
}

fn_((ArrQue_Iter_nextMut(ArrQue_Iter* self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_Iter_nextMut(queIterAsDeqIter(self), type);
}
