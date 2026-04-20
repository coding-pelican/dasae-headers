#include "dh/mem/common.h"
#include "dh/meta.h"

fn_((mem_setBytes(S$u8 dst, u8 val))(S$u8)) {
    claim_assert_nonnullS(dst);
    return u_memsetS(u_anyS(dst), u_anyV(val)), dst;
};
fn_((mem_setP(u_P$raw dst, u_V$raw val))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(val.inner);
    claim_assert(TypeInfo_eql(dst.type, val.inner_type));
    return u_memset(dst, val);
};
fn_((mem_set(u_S$raw dst, u_V$raw val))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnull(val.inner);
    claim_assert(TypeInfo_eql(dst.type, val.inner_type));
    for_(($us(dst))(elem)) { u_memset(elem, val); } $end(for);
    return dst;
};

fn_((mem_set0Bytes(S$u8 dst))(S$u8)) {
    claim_assert_nonnullS(dst);
    return u_memset0S(u_anyS(dst)), dst;
};
fn_((mem_set0P(u_P$raw dst))(u_P$raw)) {
    claim_assert_nonnull(dst.raw);
    return u_memset0(dst);
};
fn_((mem_set0(u_S$raw dst))(u_S$raw)) {
    claim_assert_nonnullS(dst);
    return u_memset0S(dst);
};

fn_((mem_copyBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    return u_memcpyS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src)), dst;
};
fn_((mem_copyP(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(src.raw);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    return u_memcpy(dst, src);
};
fn_((mem_copy(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s)) { u_memcpy(d, s); } $end(for);
    return dst;
};

fn_((mem_moveBytes(S$u8 dst, S_const$u8 src))(S$u8)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    return u_memmoveS(u_sliceS(u_anyS(dst), $r(0, src.len)), u_anyS(src)), dst;
};
fn_((mem_moveP(u_P$raw dst, u_P_const$raw src))(u_P$raw)) {
    claim_assert_nonnull(dst.raw), claim_assert_nonnull(src.raw);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    return u_memmove(dst, src);
};
fn_((mem_move(u_S$raw dst, u_S_const$raw src))(u_S$raw)) {
    claim_assert_nonnullS(dst), claim_assert_nonnullS(src);
    claim_assert(TypeInfo_eql(dst.type, src.type));
    for_(($us(u_sliceS(dst, $r(0, src.len))), $us(src))(d, s)) { u_memmove(d, s); } $end(for);
    return dst;
};

fn_((mem_eqlBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    if (lhs.len != rhs.len) return false;
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) return true;
    return u_memeqlS(u_anyS(lhs), u_anyS(rhs));
};
fn_((mem_eqlP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    if (lhs.raw == rhs.raw) return true;
    return u_memeql(lhs, rhs);
};
fn_((mem_eql(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    if (lhs.len != rhs.len) return false;
    if (lhs.len == 0 || lhs.ptr == rhs.ptr) return true;
    for_(($us(lhs), $us(rhs))(l, r)) {
        if (!u_memeql(l, r)) return false;
    } $end(for);
    return true;
};

fn_((mem_neqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return !mem_eqlBytes(lhs, rhs); };
fn_((mem_neqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return !mem_eqlP(lhs, rhs); };
fn_((mem_neq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return !mem_eql(lhs, rhs); };

fn_((mem_ordBytes(S_const$u8 lhs, S_const$u8 rhs))(cmp_Ord)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let len = int_min(lhs.len, rhs.len);
    let result = u_memordS(u_sliceS(u_anyS(lhs), $r(0, len)), u_sliceS(u_anyS(rhs), $r(0, len)));
    return result != cmp_Ord_eq ? result : pri_ord(lhs.len, rhs.len);
};
fn_((mem_ordP(u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    return u_memord(lhs, rhs);
};
fn_((mem_ord(u_S_const$raw lhs, u_S_const$raw rhs))(cmp_Ord)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let len = int_min(lhs.len, rhs.len);
    for_(($us(u_sliceS(lhs, $r(0, len))), $us(u_sliceS(rhs, $r(0, len))))(l, r)) {
        switch (u_memord(l, r)) {
        case cmp_Ord_lt: return cmp_Ord_lt;
        case cmp_Ord_gt: return cmp_Ord_gt;
        case cmp_Ord_eq: continue;
        }
    } $end(for);
    return pri_ord(lhs.len, rhs.len);
};

fn_((mem_eqBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isEq(mem_ordBytes(lhs, rhs)); };
fn_((mem_eqP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isEq(mem_ordP(lhs, rhs)); };
fn_((mem_eq(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isEq(mem_ord(lhs, rhs)); };

fn_((mem_neBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isNe(mem_ordBytes(lhs, rhs)); };
fn_((mem_neP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isNe(mem_ordP(lhs, rhs)); };
fn_((mem_ne(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isNe(mem_ord(lhs, rhs)); };

fn_((mem_ltBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isLt(mem_ordBytes(lhs, rhs)); };
fn_((mem_ltP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isLt(mem_ordP(lhs, rhs)); };
fn_((mem_lt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isLt(mem_ord(lhs, rhs)); };

fn_((mem_gtBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isGt(mem_ordBytes(lhs, rhs)); };
fn_((mem_gtP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isGt(mem_ordP(lhs, rhs)); };
fn_((mem_gt(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isGt(mem_ord(lhs, rhs)); };

fn_((mem_leBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isLe(mem_ordBytes(lhs, rhs)); };
fn_((mem_leP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isLe(mem_ordP(lhs, rhs)); };
fn_((mem_le(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isLe(mem_ord(lhs, rhs)); };

fn_((mem_geBytes(S_const$u8 lhs, S_const$u8 rhs))(bool)) { return cmp_Ord_isGe(mem_ordBytes(lhs, rhs)); };
fn_((mem_geP(u_P_const$raw lhs, u_P_const$raw rhs))(bool)) { return cmp_Ord_isGe(mem_ordP(lhs, rhs)); };
fn_((mem_ge(u_S_const$raw lhs, u_S_const$raw rhs))(bool)) { return cmp_Ord_isGe(mem_ord(lhs, rhs)); };

$static fn_((mem__swapTmpBytes(S$u8 lhs, S$u8 rhs, S$u8 tmp))(S$u8)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(tmp);
    claim_assert(lhs.len == rhs.len);
    claim_assert(rhs.len <= tmp.len);
    let buf = prefixS(tmp, lhs.len);
    u_memcpyS(u_anyS(buf), u_anyS(lhs).as_const);
    u_memcpyS(u_anyS(lhs), u_anyS(rhs).as_const);
    u_memcpyS(u_anyS(rhs), u_anyS(buf).as_const);
    return buf;
};
$static fn_((mem__swapTmpP(u_P$raw lhs, u_P$raw rhs, u_P$raw tmp))(u_P$raw)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw), claim_assert_nonnull(tmp.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, tmp.type));
    u_memcpy(tmp, lhs.as_const);
    u_memcpy(lhs, rhs.as_const);
    u_memcpy(rhs, tmp.as_const);
    return tmp;
};
$static fn_((mem__swapTmp(u_S$raw lhs, u_S$raw rhs, u_S$raw tmp))(u_S$raw)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(tmp);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, tmp.type));
    claim_assert(lhs.len == rhs.len);
    claim_assert(rhs.len <= tmp.len);
    let buf = u_prefixS(tmp, lhs.len);
    for_(($us(lhs), $us(rhs), $us(buf))(l, r, t)) {
        u_memcpy(t, l.as_const);
        u_memcpy(l, r.as_const);
        u_memcpy(r, t.as_const);
    } $end(for);
    return buf;
};

fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(lhs.len == rhs.len);
    let tmp = u_castS$((S$u8)(u_allocA(lhs.len, typeInfo$(u8)).ref));
    mem__swapTmpBytes(lhs, rhs, tmp);
};
fn_((mem_swapP(u_P$raw lhs, u_P$raw rhs))(void)) {
    claim_assert_nonnull(lhs.raw), claim_assert_nonnull(rhs.raw);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let tmp = u_allocV(lhs.type).ref;
    mem__swapTmpP(lhs, rhs, tmp);
};
fn_((mem_swap(u_S$raw lhs, u_S$raw rhs))(void)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    claim_assert(lhs.len == rhs.len);
    let tmp = u_allocA(lhs.len, typeInfo$(u8)).ref;
    mem__swapTmp(lhs, rhs, tmp);
};
fn_((mem_reverseBytes(S$u8 seq))(void)) {
    claim_assert_nonnullS(seq);
    let_(l, usize) = 0;
    let r = seq.len / 2;
    for_(($r(l, r))(p)) {
        mem_swapP(u_anyP(S_at((seq)[p])), u_anyP(S_at((seq)[seq.len - p - 1])));
    } $end(for);
};
fn_((mem_reverse(u_S$raw seq))(void)) {
    claim_assert_nonnullS(seq);
    let_(l, usize) = 0;
    let r = seq.len / 2;
    for_(($r(l, r))(p)) {
        mem_swapP(u_atS(seq, p), u_atS(seq, seq.len - p - 1));
    } $end(for);
};
fn_((mem_rotateBytes(S$u8 seq, usize amount))(void)) {
    claim_assert_nonnullS(seq);
    mem_reverseBytes(S_prefix((seq)(amount)));
    mem_reverseBytes(S_suffix((seq)(amount)));
    mem_reverseBytes(seq);
};
fn_((mem_rotate(u_S$raw seq, usize amount))(void)) {
    claim_assert_nonnullS(seq);
    mem_reverse(u_prefixS(seq, amount));
    mem_reverse(u_suffixS(seq, amount));
    mem_reverse(seq);
};

fn_((mem_window(TypeInfo type, u_S_const$raw buf, usize size, usize advance))(mem_WindowIter)) {
    claim_assert_nonnullS(buf.raw);
    claim_assert(size > 0);
    claim_assert(advance > 0);
    return (mem_WindowIter){
        .buf = buf.raw,
        .idx = expr_(O$usize $scope)(
            buf.len < size
                ? $break_(none())
                : $break_(some(0))
        ) $unscoped(expr),
        .size = size,
        .advance = advance,
        debug_only(.type = type)
    };
};
fn_((mem_WindowIter_reset(mem_WindowIter* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(0)));
};
fn_((mem_WindowIter_next(mem_WindowIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let begin = orelse_((self->idx)(return_none()));
    let next_idx = begin + self->advance;
    let end = expr_(usize $scope)(if (begin + self->size < self->buf.len) {
        asg_l((&self->idx)(expr_(O$usize $scope)(
            next_idx < self->buf.len
                ? $break_(some(next_idx))
                : $break_(none())
        ) $unscoped(expr)));
        $break_(begin + self->size);
    } else {
        asg_l((&self->idx)(none()));
        $break_(self->buf.len);
    }) $unscoped(expr);
    return_some(u_sliceS(S_meta((type)(self->buf)), $r(begin, end)));
} $unscoped(fn);

fn_((mem_startsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eqlBytes(S_prefix((haystack)(needle.len)), needle));
    }) $unscoped(expr);
};
fn_((mem_startsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_prefixS(haystack, needle.len), needle));
    }) $unscoped(expr);
};
fn_((mem_endsWithBytes(S_const$u8 haystack, S_const$u8 needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eqlBytes(S_suffix((haystack)(haystack.len - needle.len)), needle));
    }) $unscoped(expr);
};
fn_((mem_endsWith(u_S_const$raw haystack, u_S_const$raw needle))(bool)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    return expr_(bool $scope)(if (needle.len > haystack.len) {
        $break_(false);
    }) expr_(else)({
        $break_(mem_eql(u_suffixS(haystack, haystack.len - needle.len), needle));
    }) $unscoped(expr);
};

$static fn_((mem_Delim__unit(mem_Delim$raw* self, TypeInfo type))(u_V$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_unit);
    let fields = typeInfosFrom(typeInfo$(mem_DelimType), type);
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return u_deref(u_fieldPtrMut(record, fields, 1));
};
$static fn_((mem_Delim__seq(mem_Delim$raw* self, TypeInfo type))(u_S$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_seq);
    let fields = typeInfosFrom(typeInfo$(mem_DelimType), typeInfo$(S$raw));
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return S_meta((type)(*u_castP$((S$raw*)(u_fieldPtrMut(record, fields, 1)))));
};
$static fn_((mem_Delim__any(mem_Delim$raw* self, TypeInfo type))(u_S$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_any);
    let fields = typeInfosFrom(typeInfo$(mem_DelimType), typeInfo$(S$raw));
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return S_meta((type)(*u_castP$((S$raw*)(u_fieldPtrMut(record, fields, 1)))));
};

fn_((mem_tokzUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(unit.inner), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim.tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(&ret_mem->delim, unit.type).ref, unit.ref.as_const);
    debug_only(ret_mem->type = unit.type;) return ret_mem;
};
fn_((mem_tokzSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type));
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    asg_l((&ret_mem->delim)(union_of((mem_Delim_seq)(seq.raw))));
    debug_only(ret_mem->type = seq.type;) return ret_mem;
};
fn_((mem_tokzAny(u_S_const$raw buf, u_S_const$raw set, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(set.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, set.type));
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    asg_l((&ret_mem->delim)(union_of((mem_Delim_any)(set.raw))));
    debug_only(ret_mem->type = set.type;) return ret_mem;
};

$static fn_((mem_TokzIter__buf(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    return S_meta((type)(self->buf));
};
$static fn_((mem_TokzIter__isDelim(mem_TokzIter$raw* self, TypeInfo type, usize index))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let buf = mem_TokzIter__buf(self, type);
    return expr_(bool $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(u_memeql(u_atS(buf, index), mem_Delim__unit(&self->delim, type).ref.as_const));
        case mem_Delim_seq: $break_(mem_startsWith(u_suffixS(buf, index), mem_Delim__seq(&self->delim, type).as_const));
        case mem_Delim_any: {
            $break_(eval_(bool $scope)(for_(($us(mem_Delim__any(&self->delim, type)))(delim)) {
                if (!u_memeql(u_atS(buf, index), delim.as_const)) continue;
                $break_(true);
            } $end(for)) eval_(else)({
                $break_(false);
            }) $unscoped(eval));
        };
    }) $unscoped(expr);
};

fn_((mem_TokzIter_reset(mem_TokzIter$raw* self))(void)) { claim_assert_nonnull(self), self->idx = 0; };
fn_((mem_TokzIter_next(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    let token = orelse_((mem_TokzIter_peek(self, type))(return_none()));
    self->idx += token.len;
    return_some(token);
} $unscoped(fn);
fn_((mem_TokzIter_peek(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    while (self->idx < self->buf.len && mem_TokzIter__isDelim(self, type, self->idx)) {
        self->idx += expr_(usize $scope)(switch (self->delim.tag) {
            case mem_Delim_unit: $break_(1);
            case mem_Delim_seq: $break_(mem_Delim__seq(&self->delim, type).len);
            case mem_Delim_any: $break_(1);
        }) $unscoped(expr);
    }
    let begin = self->idx;
    if (begin == self->buf.len) return_none();
    var end = begin;
    while (end < self->buf.len && !mem_TokzIter__isDelim(self, type, end)) end++;
    return_some(u_sliceS(mem_TokzIter__buf(self, type), $r(begin, end)));
} $unscoped(fn);
fn_((mem_TokzIter_rest(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy(self->type, type, TypeInfo_eql);
    var idx = self->idx;
    while (idx < self->buf.len && mem_TokzIter__isDelim(self, type, idx)) {
        idx += expr_(usize $scope)(switch (self->delim.tag) {
            case mem_Delim_unit: $break_(1);
            case mem_Delim_seq: $break_(mem_Delim__seq(&self->delim, type).len);
            case mem_Delim_any: $break_(1);
        }) $unscoped(expr);
    }
    return u_suffixS(mem_TokzIter__buf(self, type), idx);
} $unscoped(fn);

fn_((mem_splitUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(none()));
    ret_mem->delim.tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(&ret_mem->delim, unit.type).ref, unit.ref.as_const);
    debug_only(ret_mem->type = unit.type;) return ret_mem;
};
fn_((mem_splitSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type));
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(none()));
    asg_l((&ret_mem->delim)(union_of((mem_Delim_seq)(seq.raw))));
    debug_only(ret_mem->type = seq.type;) return ret_mem;
};
fn_((mem_splitAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(any.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, any.type));
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(none()));
    asg_l((&ret_mem->delim)(union_of((mem_Delim_any)(any.raw))));
    debug_only(ret_mem->type = any.type;) return ret_mem;
};
