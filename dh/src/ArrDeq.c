#include "dh/ArrDeq.h"

T_use_E$($set(mem_Err)(usize));
$static fn_((addOrOom(usize lhs, usize rhs))(mem_Err$usize) $scope) {
    if_some((usize_addChkd(lhs, rhs))(result)) {
        return_ok(result);
    } else_none {
        return_err(mem_Err_OutOfMemory());
    }
    claim_unreachable;
} $unscoped_(fn);

$inline_always
$static fn_((calcInitCap(TypeInfo type))(usize)) {
    return as$(usize)(prim_max(1, arch_cache_line / type.size));
}

$static fn_((growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = calcInitCap(type);
    usize new = current;
    do { new = usize_addSat(new, new / 2 + init_cap); } while (new < minimum);
    return new;
}

$static fn_((bufIdx(const ArrDeq* self, usize idx))(usize)) {
    let head_len = self->buf.len - self->head;
    return idx < head_len
             ? self->head + idx
             : idx - head_len;
}

fn_((ArrDeq_empty(TypeInfo type))(ArrDeq)) {
    let_ignore = type;
    return (ArrDeq){
        .buf = zero$S(),
        .head = 0,
        .len = 0,
        debug_only(.type = type)
    };
}

fn_((ArrDeq_fromBuf(u_S$raw buf))(ArrDeq)) {
    return (ArrDeq){
        .buf = buf.raw,
        .head = 0,
        .len = 0,
        debug_only(.type = buf.type)
    };
}

fn_((ArrDeq_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrDeq) $scope) {
    var deq = ArrDeq_empty(type);
    try_(ArrDeq_ensureCapPrecise(&deq, type, gpa, cap));
    return_ok(deq);
} $unscoped_(fn);

fn_((ArrDeq_fini(ArrDeq* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrDeq_bufMut(*self, type));
    *self = ArrDeq_empty(type);
}

fn_((ArrDeq_len(ArrDeq self))(usize)) {
    return self.len;
}

fn_((ArrDeq_cap(ArrDeq self))(usize)) {
    return self.buf.len;
}

fn_((ArrDeq_isEmpty(ArrDeq self))(bool)) {
    return self.len == 0;
}

fn_((ArrDeq_isFull(ArrDeq self))(bool)) {
    return self.len == self.buf.len;
}

fn_((ArrDeq_head(ArrDeq self))(usize)) {
    return self.head;
}

fn_((ArrDeq_tail(ArrDeq self))(usize)) {
    return bufIdx(&self, self.len - 1);
}

fn_((ArrDeq_at(ArrDeq self, TypeInfo type, usize idx))(u_P_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrDeq_buf(self, type), bufIdx(&self, idx));
}

fn_((ArrDeq_atMut(ArrDeq self, TypeInfo type, usize idx))(u_P$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrDeq_bufMut(self, type), bufIdx(&self, idx));
}

fn_((ArrDeq_front(ArrDeq self, TypeInfo type))(O$u_P_const$raw) $scope) {
    if (self.len == 0) { return_none(); }
    return_some(u_atS(ArrDeq_buf(self, type), bufIdx(&self, 0)));
} $unscoped_(fn);

fn_((ArrDeq_frontMut(ArrDeq self, TypeInfo type))(O$u_P$raw) $scope) {
    if (self.len == 0) { return_none(); }
    return_some(u_atS(ArrDeq_bufMut(self, type), bufIdx(&self, 0)));
} $unscoped_(fn);

fn_((ArrDeq_back(ArrDeq self, TypeInfo type))(O$u_P_const$raw) $scope) {
    if (self.len == 0) { return_none(); }
    return_some(u_atS(ArrDeq_buf(self, type), bufIdx(&self, self.len - 1)));
} $unscoped_(fn);

fn_((ArrDeq_backMut(ArrDeq self, TypeInfo type))(O$u_P$raw) $scope) {
    if (self.len == 0) { return_none(); }
    return_some(u_atS(ArrDeq_bufMut(self, type), bufIdx(&self, self.len - 1)));
} $unscoped_(fn);

fn_((ArrDeq_buf(ArrDeq self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((const type)(self.buf.as_const));
}

fn_((ArrDeq_bufMut(ArrDeq self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((type)(self.buf));
}

fn_((ArrDeq_ensureCap(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->buf.len) { return_ok({}); }
    return ArrDeq_ensureCapPrecise(self, type, gpa, growCap(type, self->buf.len, new_cap));
} $unscoped_(fn);

fn_((ArrDeq_ensureCapPrecise(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->len) { return_ok({}); }

    let old_buf = ArrDeq_bufMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_buf, new_cap))(new_buf)) {
        if (self->head < old_buf.len - self->len) {
            let head = u_sliceS(new_buf, $r(self->head, old_buf.len));
            let tail = u_sliceS(new_buf, $r(0, self->len - head.len));
            if (head.len > tail.len && new_buf.len - old_buf.len > tail.len) {
                u_memcpyS(u_prefixS(u_suffixS(new_buf, old_buf.len), tail.len), tail.as_const);
            } else {
                self->head = new_buf.len - head.len;
                u_memmoveS(u_prefixS(u_suffixS(new_buf, self->head), head.len), head.as_const);
            }
        }
        self->buf = new_buf.raw;
    } else_none {
        let new_buf = try_(mem_Allocator_alloc(gpa, type, new_cap));
        if (self->head < old_buf.len - self->len) {
            u_memcpyS(u_prefixS(new_buf, self->len), u_prefixS(u_suffixS(old_buf, self->head), self->len).as_const);
        } else {
            let head = u_suffixS(old_buf, self->head);
            let tail = u_prefixS(old_buf, self->len - head.len);
            u_memcpyS(u_prefixS(new_buf, head.len), head.as_const);
            u_memcpyS(u_prefixS(u_suffixS(new_buf, head.len), tail.len), tail.as_const);
        }
        self->head = 0;
        self->buf = new_buf.raw;
        mem_Allocator_free(gpa, old_buf);
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrDeq_ensureUnusedCap(ArrDeq* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return ArrDeq_ensureCap(self, type, gpa, try_(addOrOom(self->len, additional)));
} $unscoped_(fn);

fn_((ArrDeq_clearRetainingCap(ArrDeq* self))(void)) {
    self->len = 0;
}

fn_((ArrDeq_clearAndFree(ArrDeq* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrDeq_bufMut(*self, type));
    *self = ArrDeq_empty(type);
}

fn_((ArrDeq_append(ArrDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrDeq_ensureUnusedCap(self, type, gpa, 1));
    return_ok_void(ArrDeq_appendWithin(self, item));
} $unscoped_(fn);

fn_((ArrDeq_appendFixed(ArrDeq* self, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, item.inner_type, TypeInfo_eq);
    if (usize_sub(self->buf.len, self->len) == 0) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrDeq_appendWithin(self, item));
} $unscoped_(fn);

fn_((ArrDeq_appendWithin(ArrDeq* self, u_V$raw item))(void)) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->len < self->buf.len);
    let idx = bufIdx(self, self->len);
    u_memcpy(u_atS(ArrDeq_bufMut(*self, type), idx), item.ref.as_const);
    self->len++;
}

fn_((ArrDeq_prepend(ArrDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrDeq_ensureUnusedCap(self, type, gpa, 1));
    return_ok_void(ArrDeq_prependWithin(self, item));
} $unscoped_(fn);

fn_((ArrDeq_prependFixed(ArrDeq* self, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    if (usize_sub(self->buf.len, self->len) == 0) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrDeq_prependWithin(self, item));
} $unscoped_(fn);

fn_((ArrDeq_prependWithin(ArrDeq* self, u_V$raw item))(void)) {
    claim_assert_nonnull(self);
    claim_assert(self->len < self->buf.len);
    if (self->head == 0) {
        self->head = self->buf.len;
    }
    self->head--;
    u_memcpy(u_atS(ArrDeq_bufMut(*self, item.inner_type), self->head), item.ref.as_const);
    self->len++;
}

fn_((ArrDeq_pop(ArrDeq* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->len == 0) { return_none(); }
    self->len--;
    let value = u_memcpy(u_allocV(type).ref, u_atS(ArrDeq_buf(*self, type), bufIdx(self, self->len)));
    return_some({ .inner = u_memcpy(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrDeq_shift(ArrDeq* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->len == 0) { return_none(); }
    let shift_idx = self->head;
    self->head = bufIdx(self, 1);
    self->len--;
    let value = u_memcpy(u_allocV(type).ref, u_atS(ArrDeq_buf(*self, type), shift_idx));
    return_some({ .inner = u_memcpy(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrDeq_iter(const ArrDeq* self, TypeInfo type))(ArrDeq_Iter)) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let_ignore = type;
    return (ArrDeq_Iter){
        .deq = self,
        .idx = 0,
        debug_only(.type = type)
    };
}

fn_((ArrDeq_Iter_next(ArrDeq_Iter* self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->idx < self->deq->len
            ? $break_(some(u_atS(ArrDeq_buf(*self->deq, type), bufIdx(self->deq, self->idx++))))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);

fn_((ArrDeq_Iter_nextMut(ArrDeq_Iter* self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->idx < self->deq->len
            ? $break_(some(u_atS(ArrDeq_bufMut(*self->deq, type), bufIdx(self->deq, self->idx++))))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);
