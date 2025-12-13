#include "dh/mem/common.h"

fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnull(dst.ptr);
    claim_assert_nonnull(src.ptr);
    u_memcpyS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src));
    return dst;
};
fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnull(dst.ptr);
    claim_assert_nonnull(src.ptr);
    claim_assert(TypeInfo_eq(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s) { u_memcpy(d, s); });
    return dst;
};

fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnull(dst.ptr);
    claim_assert_nonnull(src.ptr);
    u_memmoveS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src));
    return dst;
};
fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnull(dst.ptr);
    claim_assert_nonnull(src.ptr);
    claim_assert(TypeInfo_eq(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s) { u_memmove(d, s); });
    return dst;
};

fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8)) {
    claim_assert_nonnull(dst.ptr);
    u_memsetS(u_anyS(dst), u_anyV(val));
    return dst;
};
fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw)) {
    claim_assert_nonnull(dst.ptr);
    claim_assert_nonnull(val.inner);
    claim_assert(TypeInfo_eq(dst.type, val.inner_type));
    for_(($us(dst))(elem) { u_memset(elem, val); });
    return dst;
};

fn_((mem_setBytes0(S$u8 dst))(S$u8)) {
    claim_assert_nonnull(dst.ptr);
    let val = u_anyV(u8_n(0x00));
    u_memsetS(u_anyS(dst), val);
    return dst;
};
fn_((mem_set0(u_S$raw dst))(u_S$raw)) {
    claim_assert_nonnull(dst.ptr);
    let val = u_allocV(dst.type);
    for_(($us(dst))(elem) { u_memset(elem, val); });
    return dst;
};

fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) { return true; }
    return u_memeqlS(u_anyS(lhs), u_anyS(rhs));
};
fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return !mem_eqlBytes(lhs, rhs); };

fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert(TypeInfo_eq(lhs.type, rhs.type));
    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) { return true; }
    for_(($us(lhs), $us(rhs))(l, r) {
        if (!u_memeql(l, r)) { return false; }
    });
    return true;
};

fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return !mem_eql(lhs, rhs); };

fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    let len = int_min(lhs.len, rhs.len);
    let result = u_memordS(u_sliceS(u_anyS(lhs), $r(0, len)), u_sliceS(u_anyS(rhs), $r(0, len)));
    return result != cmp_Ord_eq ? result : prim_ord(lhs.len, rhs.len);
};
fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };
fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };
fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };
fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };
fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };
fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return mem_ordBytes(lhs, rhs); };

fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert(TypeInfo_eq(lhs.type, rhs.type));
    let len = int_min(lhs.len, rhs.len);
    for_(($us(u_sliceS(lhs, $r(0, len))), $us(u_sliceS(rhs, $r(0, len))))(l, r) {
        switch (u_memord(l, r)) {
            case_((cmp_Ord_lt)) return cmp_Ord_lt $end(case);
            case_((cmp_Ord_gt)) return cmp_Ord_gt $end(case);
            case_((cmp_Ord_eq)) continue $end(case);
        }
    });
    return prim_ord(lhs.len, rhs.len);
};

fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };
fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };
fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };
fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };
fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };
fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return mem_ord(lhs, rhs); };

fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert_true(lhs.len == rhs.len);
    let tmp = u_castS$((S$u8)(u_allocA(lhs.len, typeInfo$(u8)).ref));
    mem_swapBytesTmp(lhs, rhs, tmp);
};
fn_((mem_swap(u_S$raw lhs, u_S$raw rhs))(void)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert_true(lhs.len == rhs.len);
    claim_assert(TypeInfo_eq(lhs.type, rhs.type));
    let tmp = u_allocA(lhs.len, typeInfo$(u8)).ref;
    mem_swapTmp(lhs, rhs, tmp);
};
fn_((mem_swapBytesTmp(S$u8 lhs, S$u8 rhs, S$u8 tmp))(S$u8)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert_nonnull(tmp.ptr);
    claim_assert_true(lhs.len == rhs.len);
    claim_assert_true(rhs.len <= tmp.len);
    let buf = prefixS(tmp, lhs.len);
    u_memcpyS(u_anyS(buf), u_anyS(lhs).as_const);
    u_memcpyS(u_anyS(lhs), u_anyS(rhs).as_const);
    u_memcpyS(u_anyS(rhs), u_anyS(buf).as_const);
    return buf;
};
fn_((mem_swapTmp(u_S$raw lhs, u_S$raw rhs, u_S$raw tmp))(u_S$raw)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);
    claim_assert_nonnull(tmp.ptr);
    claim_assert_true(lhs.len == rhs.len);
    claim_assert_true(rhs.len <= tmp.len);
    claim_assert(TypeInfo_eq(lhs.type, rhs.type));
    claim_assert(TypeInfo_eq(rhs.type, tmp.type));
    let buf = u_prefixS(tmp, lhs.len);
    for_(($us(lhs), $us(rhs), $us(buf))(l, r, t) {
        u_memcpy(l, r.as_const);
        u_memcpy(r, t.as_const);
        u_memcpy(t, l.as_const);
    });
    return buf;
};

fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnull(haystack.ptr);
    claim_assert_nonnull(needle.ptr);
    claim_assert(TypeInfo_eq(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_prefixS(haystack, needle.len), needle));
    }) $unscoped_(expr);
};
fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnull(haystack.ptr);
    claim_assert_nonnull(needle.ptr);
    claim_assert(TypeInfo_eq(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_suffixS(haystack, haystack.len - needle.len), needle));
    }) $unscoped_(expr);
};

fn_((mem_tokenizeValue(u_S_const$raw buf, u_V$raw value, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_type = mem_delimType_value;
    u_memcpy((u_P$raw){ .raw = ret_mem->delim.value.inner, .type = value.inner_type }, value.ref.as_const);
    debug_only(ret_mem->type = value.inner_type;) return ret_mem;
};

fn_((mem_tokenizePattern(u_S_const$raw buf, u_S_const$raw pattern, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_type = mem_delimType_pattern;
    ret_mem->delim.pattern = pattern.raw;
    debug_only(ret_mem->type = pattern.type;) return ret_mem;
};

fn_((mem_tokenizeChoice(u_S_const$raw buf, u_S_const$raw choice, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_type = mem_delimType_choice;
    ret_mem->delim.choice = choice.raw;
    debug_only(ret_mem->type = choice.type;) return ret_mem;
};

$static fn_((mem_TokenIter__buf(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->buf, .type = type };
};

$static fn_((mem_TokenIter__value(mem_TokenIter* self, TypeInfo type))(u_V$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_V$raw){ .inner = self->delim.value.inner, .inner_type = type };
};

$static fn_((mem_TokenIter__pattern(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->delim.pattern, .type = type };
};

$static fn_((mem_TokenIter__choice(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->delim.choice, .type = type };
};

$static fn_((mem_TokenIter__isDelim(mem_TokenIter* self, TypeInfo type, usize index))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let buf = mem_TokenIter__buf(self, type);
    return expr_(bool $scope)(switch (self->delim_type) {
        case_((mem_delimType_value)) {
            let delim = mem_TokenIter__value(self, type);
            $break_(u_memeql(u_atS(buf, index), delim.ref.as_const));
        } $end(case);
        case_((mem_delimType_pattern)) {
            let delim = mem_TokenIter__pattern(self, type);
            $break_(mem_startsWith(u_suffixS(buf, index), delim));
        } $end(case);
        case_((mem_delimType_choice)) {
            let delims = mem_TokenIter__choice(self, type);
            $break_(eval_(bool $scope)(for_(($us(delims))(delim) {
                if (!u_memeql(u_atS(buf, index), delim)) { continue; }
                $break_(true);
            })) eval_(else)({
                $break_(false);
            }) $unscoped_(eval));
        } $end(case);
    }) $unscoped_(expr);
};

fn_((mem_TokenIter_reset(mem_TokenIter* self))(void)) { claim_assert_nonnull(self), self->idx = 0; };

fn_((mem_TokenIter_next(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let token = orelse_((mem_TokenIter_peek(self, type))(return_none()));
    self->idx += token.len;
    return_some(token);
} $unscoped_(fn);

fn_((mem_TokenIter_peek(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    while (self->idx < self->buf.len && mem_TokenIter__isDelim(self, type, self->idx)) {
        self->idx += expr_(usize $scope)(switch (self->delim_type) {
            case_((mem_delimType_value)) $break_(1) $end(case);
            case_((mem_delimType_pattern)) $break_(self->delim.pattern.len) $end(case);
            case_((mem_delimType_choice)) $break_(1) $end(case);
        }) $unscoped_(expr);
    }
    let begin = self->idx;
    if (begin == self->buf.len) { return_none(); }
    var end = begin;
    while (end < self->buf.len && !mem_TokenIter__isDelim(self, type, end)) { end++; }
    return_some(u_sliceS(mem_TokenIter__buf(self, type), $r(begin, end)));
} $unscoped_(fn);

fn_((mem_TokenIter_rest(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    var idx = self->idx;
    while (idx < self->buf.len && mem_TokenIter__isDelim(self, type, idx)) {
        idx += expr_(usize $scope)(switch (self->delim_type) {
            case_((mem_delimType_value)) $break_(1) $end(case);
            case_((mem_delimType_pattern)) $break_(self->delim.pattern.len) $end(case);
            case_((mem_delimType_choice)) $break_(1) $end(case);
        }) $unscoped_(expr);
    }
    return_some(u_suffixS(mem_TokenIter__buf(self, type), idx));
} $unscoped_(fn);
