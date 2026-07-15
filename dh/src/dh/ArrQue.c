#include "dh/ArrQue.h"
#include "dh/ArrDeq.h"

claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(ArrQue), packTypeInfo$(ArrDeq)));
claim_assert_static(offsetTo(ArrQue, buf) == offsetTo(ArrDeq, buf));
claim_assert_static(offsetTo(ArrQue, head) == offsetTo(ArrDeq, head));
claim_assert_static(offsetTo(ArrQue, len) == offsetTo(ArrDeq, len));
debug_assert_static(offsetTo(ArrQue, type) == offsetTo(ArrDeq, type));

#define ArrQue__queAsDeq(_p_que...) T_switch$((TypeOf(_p_que))( \
    T_qual$((const ArrQue*))(ptrQualCast$((const ArrDeq*)(_p_que))), \
    T_qual$((ArrQue*))(ptrQualCast$((ArrDeq*)(_p_que))) \
))
#define ArrQue__deqAsQue(_p_deq...) T_switch$((TypeOf(_p_deq))( \
    T_qual$((const ArrDeq*))(ptrQualCast$((const ArrQue*)(_p_deq))), \
    T_qual$((ArrDeq*))(ptrQualCast$((ArrQue*)(_p_deq))) \
))
$attr($inline_always)
$static fn_((ArrQue__queToDeq(ArrQue self))(ArrDeq)) { return *ArrQue__queAsDeq(&self); }
$attr($inline_always)
$static fn_((ArrQue__deqToQue(ArrDeq self))(ArrQue)) { return *ArrQue__deqAsQue(&self); }

fn_((ArrQue_empty(TypeInfo type))(ArrQue)) {
    return ArrQue__deqToQue(ArrDeq_empty(type));
}

fn_((ArrQue_fixed(u_S$raw buf))(ArrQue)) {
    return ArrQue__deqToQue(ArrDeq_fixed(buf));
}

fn_((ArrQue_init(TypeInfo type, mem_Alctr gpa, usize cap))(mem_E$ArrQue) $scope) {
    return_ok(ArrQue__deqToQue(try_(ArrDeq_init(type, gpa, cap))));
} $unscoped(fn);

fn_((ArrQue_fini(ArrQue* self, TypeInfo type, mem_Alctr gpa))(void)) {
    ArrDeq_fini(ArrQue__queAsDeq(self), type, gpa);
}

claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(ArrQue_Grip), packTypeInfo$(ArrDeq_Grip)));
claim_assert_static(offsetTo(ArrQue_Grip, buf) == offsetTo(ArrDeq_Grip, buf));
claim_assert_static(offsetTo(ArrQue_Grip, head) == offsetTo(ArrDeq_Grip, head));
claim_assert_static(offsetTo(ArrQue_Grip, len) == offsetTo(ArrDeq_Grip, len));
claim_assert_static(offsetTo(ArrQue_Grip, ctx) == offsetTo(ArrDeq_Grip, ctx));

#define deqGripAsQueGrip(_p_grip...) T_switch$((TypeOf(_p_grip))( \
    T_qual$((const ArrDeq_Grip*))(ptrQualCast$((const ArrQue_Grip*)(_p_grip))), \
    T_qual$((ArrDeq_Grip*))(ptrQualCast$((ArrQue_Grip*)(_p_grip))) \
))
#define queGripAsDeqGrip(_p_grip...) T_switch$((TypeOf(_p_grip))( \
    T_qual$((const ArrQue_Grip*))(ptrQualCast$((const ArrDeq_Grip*)(_p_grip))), \
    T_qual$((ArrQue_Grip*))(ptrQualCast$((ArrDeq_Grip*)(_p_grip))) \
))
$attr($inline_always)
$static fn_((ArrQue__deqGripToQueGrip(ArrDeq_Grip self))(ArrQue_Grip)) { return *deqGripAsQueGrip(&self); }

fn_((ArrQue_grip(u_S$raw buf, usize* head, usize* len))(ArrQue_Grip)) {
    return ArrQue__deqGripToQueGrip(ArrDeq_grip(buf, head, len));
}

fn_((ArrQue_Grip_release(ArrQue_Grip* self, TypeInfo type))(void)) {
    return ArrDeq_Grip_release(queGripAsDeqGrip(self), type);
}

fn_((ArrQue_len(ArrQue self))(usize)) {
    return ArrDeq_len(ArrQue__queToDeq(self));
}

fn_((ArrQue_cap(ArrQue self))(usize)) {
    return ArrDeq_cap(ArrQue__queToDeq(self));
}

fn_((ArrQue_isEmpty(ArrQue self))(bool)) {
    return ArrDeq_isEmpty(ArrQue__queToDeq(self));
}

fn_((ArrQue_isFull(ArrQue self))(bool)) {
    return ArrDeq_isFull(ArrQue__queToDeq(self));
}

fn_((ArrQue_head(ArrQue self))(usize)) {
    return ArrDeq_head(ArrQue__queToDeq(self));
}

fn_((ArrQue_tail(ArrQue self))(usize)) {
    return ArrDeq_tail(ArrQue__queToDeq(self));
}

fn_((ArrQue_at(ArrQue self, TypeInfo type, usize idx))(u_P_const$raw)) {
    return ArrDeq_at(ArrQue__queToDeq(self), type, idx);
}

fn_((ArrQue_atMut(ArrQue self, TypeInfo type, usize idx))(u_P$raw)) {
    return ArrDeq_atMut(ArrQue__queToDeq(self), type, idx);
}

fn_((ArrQue_front(ArrQue self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_front(ArrQue__queToDeq(self), type);
}

fn_((ArrQue_frontMut(ArrQue self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_frontMut(ArrQue__queToDeq(self), type);
}

fn_((ArrQue_back(ArrQue self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_back(ArrQue__queToDeq(self), type);
}

fn_((ArrQue_backMut(ArrQue self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_backMut(ArrQue__queToDeq(self), type);
}

fn_((ArrQue_ensureCap(ArrQue* self, TypeInfo type, mem_Alctr gpa, usize new_cap))(mem_E$void)) {
    return ArrDeq_ensureCap(ArrQue__queAsDeq(self), type, gpa, new_cap);
}

fn_((ArrQue_ensureCapPrecise(ArrQue* self, TypeInfo type, mem_Alctr gpa, usize new_cap))(mem_E$void)) {
    return ArrDeq_ensureCapPrecise(ArrQue__queAsDeq(self), type, gpa, new_cap);
}

fn_((ArrQue_ensureUnusedCap(ArrQue* self, TypeInfo type, mem_Alctr gpa, usize additional))(mem_E$void)) {
    return ArrDeq_ensureUnusedCap(ArrQue__queAsDeq(self), type, gpa, additional);
}

fn_((ArrQue_clearRetainingCap(ArrQue* self))(void)) {
    return ArrDeq_clearRetainingCap(ArrQue__queAsDeq(self));
}

fn_((ArrQue_clearAndFree(ArrQue* self, TypeInfo type, mem_Alctr gpa))(void)) {
    return ArrDeq_clearAndFree(ArrQue__queAsDeq(self), type, gpa);
}

fn_((ArrQue_enque(ArrQue* self, mem_Alctr gpa, u_V$raw item))(mem_E$void)) {
    return ArrDeq_append(ArrQue__queAsDeq(self), gpa, item);
}

fn_((ArrQue_enqueFixed(ArrQue* self, u_V$raw item))(mem_E$void)) {
    return ArrDeq_appendFixed(ArrQue__queAsDeq(self), item);
}

fn_((ArrQue_enqueWithin(ArrQue* self, u_V$raw item))(void)) {
    return ArrDeq_appendWithin(ArrQue__queAsDeq(self), item);
}

fn_((ArrQue_deque(ArrQue* self, u_V$raw ret_mem))(O$u_V$raw)) {
    return ArrDeq_shift(ArrQue__queAsDeq(self), ret_mem);
}

claim_assert_static(TypeInfoPacked_eql(packTypeInfo$(ArrQue_Iter), packTypeInfo$(ArrDeq_Iter)));
claim_assert_static(offsetTo(ArrQue_Iter, que) == offsetTo(ArrDeq_Iter, deq));
claim_assert_static(offsetTo(ArrQue_Iter, idx) == offsetTo(ArrDeq_Iter, idx));
debug_assert_static(offsetTo(ArrQue_Iter, type) == offsetTo(ArrDeq_Iter, type));

#define ArrQue__queIterAsDeqIter(_p_que_iter...) T_switch$((TypeOf(_p_que_iter))( \
    T_qual$((const ArrQue_Iter*))(ptrQualCast$((const ArrDeq_Iter*)(_p_que_iter))), \
    T_qual$((ArrQue_Iter*))(ptrQualCast$((ArrDeq_Iter*)(_p_que_iter))) \
))
#define ArrQue__deqIterAsQueIter(_p_deq_iter...) T_switch$((TypeOf(_p_deq_iter))( \
    T_qual$((const ArrDeq_Iter*))(ptrQualCast$((const ArrQue_Iter*)(_p_deq_iter))), \
    T_qual$((ArrDeq_Iter*))(ptrQualCast$((ArrQue_Iter*)(_p_deq_iter))) \
))

$attr($inline_always)
$static fn_((ArrQue__deqIterToQueIter(ArrDeq_Iter self))(ArrQue_Iter)) { return *ArrQue__deqIterAsQueIter(&self); }

fn_((ArrQue_iter(const ArrQue* self, TypeInfo type))(ArrQue_Iter)) {
    return ArrQue__deqIterToQueIter(ArrDeq_iter(ArrQue__queAsDeq(self), type));
}

fn_((ArrQue_Iter_next(ArrQue_Iter* self, TypeInfo type))(O$u_P_const$raw)) {
    return ArrDeq_Iter_next(ArrQue__queIterAsDeqIter(self), type);
}

fn_((ArrQue_Iter_nextMut(ArrQue_Iter* self, TypeInfo type))(O$u_P$raw)) {
    return ArrDeq_Iter_nextMut(ArrQue__queIterAsDeqIter(self), type);
}
