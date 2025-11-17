#include "dh/mem/common.h"

fn_((mem_copyFwds(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    // claim_assert_fmt(len$u_S(dst) == len$u_S(src), "Length mismatch: dst(%zu) != src(%zu)", len$u_S(dst), len$u_S(src));

    for_(($us(u_slice$S(dst, $r(0, src.len))), $us(src))(d, s) { u_copy$P(d, s); });
    return dst;
}

fn_((mem_copyBwds(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_fmt(dst.type.size == src.type.size, "Type size mismatch: dst(%zu) != src(%zu)", dst.type.size, src.type.size);
    claim_assert_fmt(dst.type.align == src.type.align, "Type alignment mismatch: dst(%zu) != src(%zu)", dst.type.align, src.type.align);
    // claim_assert_fmt(len$u_S(dst) == len$u_S(src), "Length mismatch: dst(%zu) != src(%zu)", len$u_S(dst), len$u_S(src));

    for_($bwd($us(u_slice$S(dst, $r(0, src.len))), $us(src))(d, s) { u_copy$P(d, s); });
    return dst;
}

fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);

    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) { return true; }
    return prim_memcmp(lhs.ptr, rhs.ptr, lhs.len) == 0;
}

fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) {
    return !mem_eqBytes(lhs, rhs);
}

fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_nonnull(lhs.ptr);
    claim_assert_nonnull(rhs.ptr);

    if (lhs.len != rhs.len) { return false; }
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) { return true; }
    for_(($us(lhs), $us(rhs))(l, r) {
        if (!u_eqP(l, r)) { return false; }
    });
    return true;
}

fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    return !mem_eq(lhs, rhs);
}

fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnull(haystack.ptr);
    claim_assert_nonnull(needle.ptr);

    return expr_(bool $scope) if (needle.len > haystack.len) {
        $break_(false);
    } else {
        $break_(mem_eq(u_prefix$S(haystack, needle.len), needle));
    } $unscoped_(expr);
}

fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnull(haystack.ptr);
    claim_assert_nonnull(needle.ptr);

    return expr_(bool $scope) if (needle.len > haystack.len) {
        $break_(false);
    } else {
        $break_(mem_eq(u_suffix$S(haystack, haystack.len - needle.len), needle));
    } $unscoped_(expr);
}

fn_((mem_tokenizeValue(u_S_const$raw buf, u_V$raw value, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_tag = mem_delimType_value;
    u_copy$P((u_P$raw){ .raw = ret_mem->delim.value.inner, .type = value.inner_type }, value.ref.as_const);
    debug_only(ret_mem->type = value.inner_type;)
    return ret_mem;
}

fn_((mem_tokenizePattern(u_S_const$raw buf, u_S_const$raw pattern, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_tag = mem_delimType_pattern;
    ret_mem->delim.pattern = pattern.raw;
    debug_only(ret_mem->type = pattern.type;)
    return ret_mem;
}

fn_((mem_tokenizeChoice(u_S_const$raw buf, u_S_const$raw choice, P$mem_TokenIter ret_mem))(P$mem_TokenIter)) {
    claim_assert_nonnull(ret_mem);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_tag = mem_delimType_choice;
    ret_mem->delim.choice = choice.raw;
    debug_only(ret_mem->type = choice.type;)
    return ret_mem;
}

$static fn_((mem_TokenIter__buf(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->buf, .type = type };
}

$static fn_((mem_TokenIter__value(mem_TokenIter* self, TypeInfo type))(u_V$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_V$raw){ .inner = self->delim.value.inner, .inner_type = type };
}

$static fn_((mem_TokenIter__pattern(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->delim.pattern, .type = type };
}

$static fn_((mem_TokenIter__choice(mem_TokenIter* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (u_S_const$raw){ .raw = self->delim.choice, .type = type };
}

$static fn_((mem_TokenIter__isDelim(mem_TokenIter* self, TypeInfo type, usize index))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let buf = mem_TokenIter__buf(self, type);
    switch (self->delim_tag) {
    case mem_delimType_value:{
        let delim = mem_TokenIter__value(self, type);
        return u_eqP(u_at$S(buf, index), delim.ref.as_const);
    }
    case mem_delimType_pattern:{
        let delim = mem_TokenIter__pattern(self, type);
        return mem_startsWith(u_suffix$S(buf, index), delim);
    }
    case mem_delimType_choice: {
        let delims = mem_TokenIter__choice(self, type);
        for_(($us(delims))(delim) {
            if (u_eqP(u_at$S(buf, index), delim)) { return true; }
        });
        return false;
    }
    default:
        claim_unreachable;
    }
}

fn_((mem_TokenIter_reset(mem_TokenIter* self))(void)) { claim_assert_nonnull(self), self->idx = 0; }

fn_((mem_TokenIter_next(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let token = orelse_((mem_TokenIter_peek(self, type))(return_none()));
    self->idx += token.len;
    return_some(token);
} $unscoped_(fn);

fn_((mem_TokenIter_peek(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    while (self->idx < self->buf.len && mem_TokenIter__isDelim(self, type, self->idx)) {
        self->idx += expr_(usize $scope) switch (self->delim_tag) {
            case mem_delimType_value: $break_(1);
            case mem_delimType_pattern: $break_(self->delim.pattern.len);
            case mem_delimType_choice: $break_(1);
            default: claim_unreachable;
        } $unscoped_(expr);
    }
    let begin = self->idx;
    if (begin == self->buf.len) { return_none(); }
    var end = begin;
    while (end < self->buf.len && !mem_TokenIter__isDelim(self, type, end)) { end++; }
    return_some(u_slice$S(mem_TokenIter__buf(self, type), $r(begin, end)));
} $unscoped_(fn);

fn_((mem_TokenIter_rest(mem_TokenIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eq);
    var idx = self->idx;
    while (idx < self->buf.len && mem_TokenIter__isDelim(self, type, idx)) {
        idx += expr_(usize $scope) switch (self->delim_tag) {
            case mem_delimType_value: $break_(1);
            case mem_delimType_pattern: $break_(self->delim.pattern.len);
            case mem_delimType_choice: $break_(1);
            default: claim_unreachable;
        } $unscoped_(expr);
    }
    return_some(u_suffix$S(mem_TokenIter__buf(self, type), idx));
} $unscoped_(fn);
