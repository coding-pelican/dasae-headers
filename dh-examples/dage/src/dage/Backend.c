#include "dage/Backend.h"

/*========== Standard Wrappers ==========*/

fn_((dage_Backend_createTarget(dage_Backend self, dage_Target_Cfg cfg))(E$dage_TargetId) $scope) {
    self = dage_Backend_ensureValid(self);
    return self.vt->createTarget(self.ptr, cfg);
} $unscoped_(fn);

fn_((dage_Backend_destroyTarget(dage_Backend self, dage_TargetId id))(void)) {
    self = dage_Backend_ensureValid(self);
    claim_assert(id != dage_TargetId_invalid);
    self.vt->destroyTarget(self.ptr, id);
};

fn_((dage_Backend_pumpEvents(dage_Backend self, S$dage_RawEvent out_events))(S$dage_RawEvent)) {
    self = dage_Backend_ensureValid(self);
    claim_assert_nonnullS(out_events);
    /* Empty slice is valid - just returns 0 */
    if (out_events.len == 0) {
        return S_slice((out_events)$r(0, 0));
    }
    return self.vt->pumpEvents(self.ptr, out_events);
};

fn_((dage_Backend_present(dage_Backend self, dage_TargetId id, S_const$u32 pixels, u32 w, u32 h))(void)) {
    self = dage_Backend_ensureValid(self);
    claim_assert(id != dage_TargetId_invalid);
    claim_assert_nonnullS(pixels);
    debug_assert_fmt(
        pixels.len == ((as$(usize)(w)) * h),
        "Pixel buffer size mismatch: %zu != %u * %u",
        pixels.len, w, h
    );
    self.vt->present(self.ptr, id, pixels, w, h);
};

fn_((dage_Backend_getTargetMetrics(dage_Backend self, dage_TargetId id))(dage_TargetMetrics)) {
    self = dage_Backend_ensureValid(self);
    claim_assert(id != dage_TargetId_invalid);
    return self.vt->getTargetMetrics(self.ptr, id);
};

/*========== Convenience Wrappers ==========*/

fn_((dage_Backend_pumpEventsAlloc(dage_Backend self, mem_Allocator gpa, usize capacity_hint))(E$dage_Backend_PumpedEvents) $scope) {
    self = dage_Backend_ensureValid(self);
    let cap = capacity_hint == 0 ? 128 : capacity_hint;
    /* Allocate buffer - error boundary */
    let capped = u_castS$((S$dage_RawEvent)(try_(mem_Allocator_alloc(gpa, typeInfo$(dage_RawEvent), cap))));
    /* Pump events - never fails */
    let items = dage_Backend_pumpEvents(self, capped);
    return_ok((dage_Backend_PumpedEvents){
        .items = items,
        .cap = cap,
    });
} $unscoped_(fn);

fn_((dage_Backend_PumpedEvents_free(dage_Backend_PumpedEvents* self, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    mem_Allocator_free(gpa, u_anyS(self->items));
    asg_lit((self)(cleared()));
};
