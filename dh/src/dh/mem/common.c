#include "dh/mem/common.h"
#include "dh/mem/Alctr.h"
#include "dh/meta.h"

#if !comp_libc_linked
$extern fn_((memset(P$raw dst, int val, usize len))(P$raw));
$extern fn_((memcpy(P$raw dst, P_const$raw src, usize len))(P$raw));
$extern fn_((memmove(P$raw dst, P_const$raw src, usize len))(P$raw));
$extern fn_((memcmp(P_const$raw lhs, P_const$raw rhs, usize len))(int));
$extern fn_((strlen(const char* str))(usize));

fn_((memset(P$raw dst, int val, usize len))(P$raw)) {
    let out = P_prefix((ptrCast$((u8*)(dst)))(len));
    let byte_val = as$(u8)(val);
    for_(($s(out))(byte)) { *byte = byte_val; } $end(for);
    return out.ptr;
};
fn_((memcpy(P$raw dst, P_const$raw src, usize len))(P$raw)) {
    let out = P_prefix((ptrCast$((u8*)(dst)))(len));
    let in = P_prefix((ptrCast$((const u8*)(src)))(len));
    for_(($s(out), $s(in))(o, i)) { *o = *i; } $end(for);
    return out.ptr;
};
fn_((memmove(P$raw dst, P_const$raw src, usize len))(P$raw)) {
    let out = P_prefix((ptrCast$((u8*)(dst)))(len));
    let in = P_prefix((ptrCast$((const u8*)(src)))(len));
    if (out.ptr == in.ptr || len == 0) return dst;
    if (out.ptr < in.ptr) {
        for_(($s(out), $s(in))(o, i)) { *o = *i; } $end(for);
    } else {
        for_($rev($s(out), $s(in))(o, i)) { *o = *i; } $end(for);
    }
    return out.ptr;
};
fn_((memcmp(P_const$raw lhs, P_const$raw rhs, usize len))(int)) {
    let out = P_prefix((ptrCast$((const u8*)(lhs)))(len));
    let in = P_prefix((ptrCast$((const u8*)(rhs)))(len));
    for_(($s(out), $s(in))(o, i)) {
        if (*o != *i) return intCast$((int)(*o - *i));
    } $end(for);
    return 0;
};
fn_((strlen(const char* str))(usize)) {
    return mem_lenZ0$u8(as$(const u8*)(str));
};
#endif /* !comp_libc_linked */

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
    let buf = S_prefix((tmp)(lhs.len));
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

fn_((mem_windowBytes(S_const$u8 buf, usize size, usize advance))(mem_WindowIter_Bytes)) {
    claim_assert_nonnullS(buf);
    claim_assert(size > 0);
    claim_assert(advance > 0);
    return (mem_WindowIter_Bytes){
        .buf = buf,
        .idx = expr_(O$usize $scope)(
            buf.len < size
                ? $break_(none())
                : $break_(some(0))
        ) $unscoped(expr),
        .size = size,
        .advance = advance,
    };
};
fn_((mem_window(u_S_const$raw buf, usize size, usize advance))(mem_WindowIter)) {
    claim_assert_nonnullS(buf);
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
        .type = $typing(buf.type),
    };
};
fn_((mem_WindowIter_resetBytes(mem_WindowIter_Bytes* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(0)));
};
fn_((mem_WindowIter_reset(mem_WindowIter* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(0)));
};
fn_((mem_WindowIter_nextBytes(mem_WindowIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
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
    return_some(S_slice((self->buf)$r(begin, end)));
} $unscoped(fn);
fn_((mem_WindowIter_next(mem_WindowIter* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
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

$static fn_((mem__byteIn(u8 value, S_const$u8 set))(bool)) {
    claim_assert_nonnullS(set);
    for_(($s(set))(item)) {
        if (*item == value) return true;
    } $end(for);
    return false;
};

fn_((mem_trimStartBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(values_to_strip);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (!mem__byteIn(*item, values_to_strip)) return S_suffix((haystack)(idx));
    } $end(for);
    return S_suffix((haystack)(haystack.len));
};
fn_((mem_trimStart(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(values_to_strip);
    claim_assert(TypeInfo_eql(haystack.type, values_to_strip.type));
    if_some((mem_findFirstNone(haystack, values_to_strip))(idx)) return u_suffixS(haystack, idx);
    return u_suffixS(haystack, haystack.len);
};
fn_((mem_trimEndBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(values_to_strip);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (!mem__byteIn(*item, values_to_strip)) return S_prefix((haystack)(idx + 1));
    } $end(for);
    return S_prefix((haystack)(0));
};
fn_((mem_trimEnd(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(values_to_strip);
    claim_assert(TypeInfo_eql(haystack.type, values_to_strip.type));
    if_some((mem_findLastNone(haystack, values_to_strip))(idx)) return u_prefixS(haystack, idx + 1);
    return u_prefixS(haystack, 0);
};
fn_((mem_trimBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8)) {
    return mem_trimEndBytes(mem_trimStartBytes(haystack, values_to_strip), values_to_strip);
};
fn_((mem_trim(u_S_const$raw haystack, u_S_const$raw values_to_strip))(u_S_const$raw)) {
    return mem_trimEnd(mem_trimStart(haystack, values_to_strip), values_to_strip);
};

fn_((mem_catBytes(S_const$u8 lhs, S_const$u8 rhs, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    let required_len = orelse_((usize_addChkd(lhs.len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_catWithinBytes(lhs, rhs, out));
} $unscoped(fn);
fn_((mem_cat(u_S_const$raw lhs, u_S_const$raw rhs, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, out.type));
    let required_len = orelse_((usize_addChkd(lhs.len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_catWithin(lhs, rhs, out));
} $unscoped(fn);
fn_((mem_catWithinBytes(S_const$u8 lhs, S_const$u8 rhs, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    let required_len = unwrap_(usize_addChkd(lhs.len, rhs.len));
    claim_assert(required_len <= out.len);
    mem_copyBytes(S_prefix((out)(lhs.len)), lhs);
    mem_copyBytes(S_slice((out)$r(lhs.len, required_len)), rhs);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_catWithin(u_S_const$raw lhs, u_S_const$raw rhs, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, out.type));
    let required_len = unwrap_(usize_addChkd(lhs.len, rhs.len));
    claim_assert(required_len <= out.len);
    mem_copy(u_prefixS(out, lhs.len), lhs);
    mem_copy(u_sliceS(out, $r(lhs.len, required_len)), rhs);
    return u_prefixS(out, required_len);
};
fn_((mem_catAllocBytes(S_const$u8 lhs, S_const$u8 rhs, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let required_len = orelse_((usize_addChkd(lhs.len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_catWithinBytes(lhs, rhs, out));
} $unscoped(fn);
fn_((mem_catAlloc(u_S_const$raw lhs, u_S_const$raw rhs, mem_Alctr gpa))(mem_E$u_S$raw) $scope) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let required_len = orelse_((usize_addChkd(lhs.len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_alloc($trace gpa, lhs.type, required_len));
    return_ok(mem_catWithin(lhs, rhs, out));
} $unscoped(fn);

fn_((mem_joinBytes(S_const$u8 sep, S_const$u8 lhs, S_const$u8 rhs, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    let lhs_sep_len = orelse_((usize_addChkd(lhs.len, sep.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let required_len = orelse_((usize_addChkd(lhs_sep_len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_joinWithinBytes(sep, lhs, rhs, out));
} $unscoped(fn);
fn_((mem_join(u_S_const$raw sep, u_S_const$raw lhs, u_S_const$raw rhs, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(lhs.type, sep.type)), claim_assert(TypeInfo_eql(sep.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, out.type));
    let required_len = orelse_((usize_addChkd(
        orelse_((usize_addChkd(lhs.len, sep.len))(
            return_err(E_cause$OutOfMemory())
        )),
        rhs.len
    ))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_joinWithin(sep, lhs, rhs, out));
} $unscoped(fn);
fn_((mem_joinWithinBytes(S_const$u8 sep, S_const$u8 lhs, S_const$u8 rhs, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    let required_len = unwrap_(usize_addChkd(unwrap_(usize_addChkd(lhs.len, sep.len)), rhs.len));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    mem_copyBytes(S_slice((out)$r(written, written + lhs.len)), lhs);
    written += lhs.len;
    mem_copyBytes(S_slice((out)$r(written, written + sep.len)), sep);
    written += sep.len;
    mem_copyBytes(S_slice((out)$r(written, written + rhs.len)), rhs);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_joinWithin(u_S_const$raw sep, u_S_const$raw lhs, u_S_const$raw rhs, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(lhs.type, sep.type)), claim_assert(TypeInfo_eql(sep.type, rhs.type)), claim_assert(TypeInfo_eql(rhs.type, out.type));
    let required_len = unwrap_(usize_addChkd(unwrap_(usize_addChkd(lhs.len, sep.len)), rhs.len));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    mem_copy(u_sliceS(out, $r(written, written + lhs.len)), lhs);
    written += lhs.len;
    mem_copy(u_sliceS(out, $r(written, written + sep.len)), sep);
    written += sep.len;
    mem_copy(u_sliceS(out, $r(written, written + rhs.len)), rhs);
    return u_prefixS(out, required_len);
};
fn_((mem_joinAllocBytes(S_const$u8 sep, S_const$u8 lhs, S_const$u8 rhs, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs);
    let lhs_sep_len = orelse_((usize_addChkd(lhs.len, sep.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let required_len = orelse_((usize_addChkd(lhs_sep_len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_joinWithinBytes(sep, lhs, rhs, out));
} $unscoped(fn);
fn_((mem_joinAlloc(u_S_const$raw sep, u_S_const$raw lhs, u_S_const$raw rhs, mem_Alctr gpa))(mem_E$u_S$raw) $scope) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(sep), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, sep.type)), claim_assert(TypeInfo_eql(sep.type, rhs.type));
    let lhs_sep_len = orelse_((usize_addChkd(lhs.len, sep.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let required_len = orelse_((usize_addChkd(lhs_sep_len, rhs.len))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_alloc($trace gpa, lhs.type, required_len));
    return_ok(mem_joinWithin(sep, lhs, rhs, out));
} $unscoped(fn);

fn_((mem_padLeftBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padLeftWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padLeft(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padLeftWithin(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padLeftWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    let pad_len = required_len - src.len;
    mem_setBytes(S_prefix((out)(pad_len)), fill);
    mem_copyBytes(S_slice((out)$r(pad_len, required_len)), src);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_padLeftWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    let pad_len = required_len - src.len;
    mem_set(u_prefixS(out, pad_len), fill);
    mem_copy(u_sliceS(out, $r(pad_len, required_len)), src);
    return u_prefixS(out, required_len);
};
fn_((mem_padLeftAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src);
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_padLeftWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padLeftAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner);
    claim_assert(TypeInfo_eql(src.type, fill.type));
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_alloc($trace gpa, src.type, required_len));
    return_ok(mem_padLeftWithin(src, width, fill, out));
} $unscoped(fn);

fn_((mem_padRightBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padRightWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padRight(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padRightWithin(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padRightWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    mem_copyBytes(S_prefix((out)(src.len)), src);
    mem_setBytes(S_slice((out)$r(src.len, required_len)), fill);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_padRightWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    mem_copy(u_prefixS(out, src.len), src);
    mem_set(u_sliceS(out, $r(src.len, required_len)), fill);
    return u_prefixS(out, required_len);
};
fn_((mem_padRightAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src);
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_padRightWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padRightAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner);
    claim_assert(TypeInfo_eql(src.type, fill.type));
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_alloc($trace gpa, src.type, required_len));
    return_ok(mem_padRightWithin(src, width, fill, out));
} $unscoped(fn);

fn_((mem_padCenterBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padCenterWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padCenter(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_padCenterWithin(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padCenterWithinBytes(S_const$u8 src, usize width, u8 fill, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(src), claim_assert_nonnullS(out);
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    let pad_len = required_len - src.len;
    let left_pad = pad_len / 2;
    let right_pad = pad_len - left_pad;
    mem_setBytes(S_prefix((out)(left_pad)), fill);
    mem_copyBytes(S_slice((out)$r(left_pad, left_pad + src.len)), src);
    mem_setBytes(S_slice((out)$r(left_pad + src.len, left_pad + src.len + right_pad)), fill);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_padCenterWithin(u_S_const$raw src, usize width, u_V$raw fill, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(src.type, fill.type)), claim_assert(TypeInfo_eql(fill.type, out.type));
    let required_len = pri_max(src.len, width);
    claim_assert(required_len <= out.len);
    let pad_len = required_len - src.len;
    let left_pad = pad_len / 2;
    let right_pad = pad_len - left_pad;
    mem_set(u_prefixS(out, left_pad), fill);
    mem_copy(u_sliceS(out, $r(left_pad, left_pad + src.len)), src);
    mem_set(u_sliceS(out, $r(left_pad + src.len, left_pad + src.len + right_pad)), fill);
    return u_prefixS(out, required_len);
};
fn_((mem_padCenterAllocBytes(S_const$u8 src, usize width, u8 fill, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(src);
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_padCenterWithinBytes(src, width, fill, out));
} $unscoped(fn);
fn_((mem_padCenterAlloc(u_S_const$raw src, usize width, u_V$raw fill, mem_Alctr gpa))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(src), claim_assert_nonnull(fill.inner);
    claim_assert(TypeInfo_eql(src.type, fill.type));
    let required_len = pri_max(src.len, width);
    let out = try_(mem_Alctr_alloc($trace gpa, src.type, required_len));
    return_ok(mem_padCenterWithin(src, width, fill, out));
} $unscoped(fn);

fn_((mem_findFirstUnitBytes(S_const$u8 haystack, u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (*item == needle) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    for_(($us(haystack), $rf(0))(item, idx)) {
        if (u_memeql(item, needle.ref.as_const)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastUnitBytes(S_const$u8 haystack, u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (*item == needle) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastUnit(u_S_const$raw haystack, u_V$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnull(needle.inner);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    for_($rev($us(haystack), $rt(haystack.len))(item, idx)) {
        if (u_memeql(item, needle.ref.as_const)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_(($rt($incl(end)))(idx)) {
        if (mem_eqlBytes(S_prefix((S_suffix((haystack)(idx)))(needle.len)), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_(($rt($incl(end)))(idx)) {
        if (mem_eql(u_prefixS(u_suffixS(haystack, idx), needle.len), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_($rev($rt($incl(end)))(idx)) {
        if (mem_eqlBytes(S_prefix((S_suffix((haystack)(idx)))(needle.len)), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needle);
    claim_assert(TypeInfo_eql(haystack.type, needle.type));
    if (needle.len > haystack.len) return_none();
    let end = haystack.len - needle.len;
    for_($rev($rt($incl(end)))(idx)) {
        if (mem_eql(u_prefixS(u_suffixS(haystack, idx), needle.len), needle)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (mem__byteIn(*item, needles)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_(($us(haystack), $rf(0))(item, idx)) {
        for_(($us(needles))(needle)) {
            if (u_memeql(item, needle)) return_some(idx);
        } $end(for);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (mem__byteIn(*item, needles)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastAny(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    for_($rev($us(haystack), $rt(haystack.len))(item, idx)) {
        for_(($us(needles))(needle)) {
            if (u_memeql(item, needle)) return_some(idx);
        } $end(for);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (!mem__byteIn(*item, needles)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    loop_labeled(outer, for_(($us(haystack), $rf(0))(item, idx))) {
        for_(($us(needles))(needle)) {
            if (u_memeql(item, needle)) loop_continue_(outer);
        } $end(for);
        return_some(idx);
    } $end(loop_labeled) $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastNoneBytes(S_const$u8 haystack, S_const$u8 needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    for_($rev($s(haystack), $rt(haystack.len))(item, idx)) {
        if (!mem__byteIn(*item, needles)) return_some(idx);
    } $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findLastNone(u_S_const$raw haystack, u_S_const$raw needles))(O$usize $scope)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(needles);
    claim_assert(TypeInfo_eql(haystack.type, needles.type));
    loop_labeled(outer, for_($rev($us(haystack), $rt(haystack.len))(item, idx))) {
        for_(($us(needles))(needle)) {
            if (u_memeql(item, needle)) loop_continue_(outer);
        } $end(for);
        return_some(idx);
    } $end(loop_labeled) $end(for);
    return_none();
} $unscoped(fn);
fn_((mem_findFirstDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_(($rt(shortest))(idx)) {
        if (*S_at((lhs)[idx]) != *S_at((rhs)[idx])) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findFirstDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_(($rt(shortest))(idx)) {
        if (!u_memeql(u_atS(lhs, idx), u_atS(rhs, idx))) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findLastDiffBytes(S_const$u8 lhs, S_const$u8 rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_($rev($rt(shortest))(idx)) {
        if (*S_at((lhs)[idx]) != *S_at((rhs)[idx])) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);
fn_((mem_findLastDiff(u_S_const$raw lhs, u_S_const$raw rhs))(O$usize $scope)) {
    claim_assert_nonnullS(lhs), claim_assert_nonnullS(rhs);
    claim_assert(TypeInfo_eql(lhs.type, rhs.type));
    let shortest = pri_min(lhs.len, rhs.len);
    if (lhs.ptr == rhs.ptr) return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
    for_($rev($rt(shortest))(idx)) {
        if (!u_memeql(u_atS(lhs, idx), u_atS(rhs, idx))) return_some(idx);
    } $end(for);
    return expr_(ReturnType $scope)(
        if (lhs.len == rhs.len) $break_(none());
        else $break_(some(shortest))
    ) $unscoped(expr);
} $unscoped(fn);

$static fn_((mem__cutAtBytes(S_const$u8 haystack, usize index, usize needle_len))(O$mem_Cutted_Bytes $scope)) {
    return_some({
        .before = S_prefix((haystack)(index)),
        .after = S_suffix((haystack)(index + needle_len)),
    });
} $unscoped(fn);
$static fn_((mem__cutAt(u_S_const$raw haystack, usize index, usize needle_len))(O$mem_Cutted $scope)) {
    return_some({
        .before = u_prefixS(haystack, index).raw,
        .after = u_suffixS(haystack, index + needle_len).raw,
        .type = $typing(haystack.type),
    });
} $unscoped(fn);
fn_((mem_cutFirstUnitBytes(S_const$u8 haystack, u8 needle))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findFirstUnitBytes(haystack, needle))(idx)) return_(mem__cutAtBytes(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutFirstUnit(u_S_const$raw haystack, u_V$raw needle))(O$mem_Cutted $scope)) {
    if_some((mem_findFirstUnit(haystack, needle))(idx)) return_(mem__cutAt(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastUnitBytes(S_const$u8 haystack, u8 needle))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findLastUnitBytes(haystack, needle))(idx)) return_(mem__cutAtBytes(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastUnit(u_S_const$raw haystack, u_V$raw needle))(O$mem_Cutted $scope)) {
    if_some((mem_findLastUnit(haystack, needle))(idx)) return_(mem__cutAt(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutFirstSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findFirstSeqBytes(haystack, needle))(idx)) return_(mem__cutAtBytes(haystack, idx, needle.len));
    return_none();
} $unscoped(fn);
fn_((mem_cutFirstSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$mem_Cutted $scope)) {
    if_some((mem_findFirstSeq(haystack, needle))(idx)) return_(mem__cutAt(haystack, idx, needle.len));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastSeqBytes(S_const$u8 haystack, S_const$u8 needle))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findLastSeqBytes(haystack, needle))(idx)) return_(mem__cutAtBytes(haystack, idx, needle.len));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastSeq(u_S_const$raw haystack, u_S_const$raw needle))(O$mem_Cutted $scope)) {
    if_some((mem_findLastSeq(haystack, needle))(idx)) return_(mem__cutAt(haystack, idx, needle.len));
    return_none();
} $unscoped(fn);
fn_((mem_cutFirstAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findFirstAnyBytes(haystack, needles))(idx)) return_(mem__cutAtBytes(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutFirstAny(u_S_const$raw haystack, u_S_const$raw needles))(O$mem_Cutted $scope)) {
    if_some((mem_findFirstAny(haystack, needles))(idx)) return_(mem__cutAt(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastAnyBytes(S_const$u8 haystack, S_const$u8 needles))(O$mem_Cutted_Bytes $scope)) {
    if_some((mem_findLastAnyBytes(haystack, needles))(idx)) return_(mem__cutAtBytes(haystack, idx, 1));
    return_none();
} $unscoped(fn);
fn_((mem_cutLastAny(u_S_const$raw haystack, u_S_const$raw needles))(O$mem_Cutted $scope)) {
    if_some((mem_findLastAny(haystack, needles))(idx)) return_(mem__cutAt(haystack, idx, 1));
    return_none();
} $unscoped(fn);

$static fn_((mem_Delim__unitBytes(mem_Delim_Bytes* self))(u8)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_unit);
    return union_to((*self)(mem_Delim_unit));
};
$static fn_((mem_Delim__unit(mem_Delim$raw* self, TypeInfo type))(u_V$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_unit);
    let fields = typeInfosFrom(typeInfo$(mem_Delim), type);
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return u_deref(u_fieldPtrMut(record, fields, 1));
};
$static fn_((mem_Delim__seqBytes(mem_Delim_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_seq);
    return union_to((*self)(mem_Delim_seq));
};
$static fn_((mem_Delim__seq(mem_Delim$raw* self, TypeInfo type))(u_S$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_seq);
    let fields = typeInfosFrom(typeInfo$(mem_Delim), typeInfo$(S$raw));
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return S_meta((type)(*u_castP$((S$raw*)(u_fieldPtrMut(record, fields, 1)))));
};
$static fn_((mem_Delim__anyBytes(mem_Delim_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_any);
    return union_to((*self)(mem_Delim_any));
};
$static fn_((mem_Delim__any(mem_Delim$raw* self, TypeInfo type))(u_S$raw)) {
    claim_assert_nonnull(self), claim_assert(self->tag == mem_Delim_any);
    let fields = typeInfosFrom(typeInfo$(mem_Delim), typeInfo$(S$raw));
    let record = P_meta((u_typeInfoRecord(fields))(as$(P$raw)(self)));
    return S_meta((type)(*u_castP$((S$raw*)(u_fieldPtrMut(record, fields, 1)))));
};

fn_((mem_tokzUnitBytes(S_const$u8 buf, u8 unit))(mem_TokzIter_Bytes)) {
    claim_assert_nonnullS(buf);
    return (mem_TokzIter_Bytes){
        .buf = buf,
        .idx = 0,
        .delim = union_of((mem_Delim_unit)(unit)),
    };
};
fn_((mem_tokzUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(unit.inner), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->type = $typing(unit.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    ret_mem->delim_[0].tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(ret_mem->delim_, unit.type).ref, unit.ref.as_const);
    return ret_mem;
};
fn_((mem_tokzSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_TokzIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(seq), claim_assert(seq.len > 0);
    return (mem_TokzIter_Bytes){
        .buf = buf,
        .idx = 0,
        .delim = union_of((mem_Delim_seq)(seq)),
    };
};
fn_((mem_tokzSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type));
    ret_mem->type = $typing(seq.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_seq)(seq.raw))));
    return ret_mem;
};
fn_((mem_tokzAnyBytes(S_const$u8 buf, S_const$u8 set))(mem_TokzIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(set);
    return (mem_TokzIter_Bytes){
        .buf = buf,
        .idx = 0,
        .delim = union_of((mem_Delim_any)(set)),
    };
};
fn_((mem_tokzAny(u_S_const$raw buf, u_S_const$raw set, V$mem_TokzIter$raw ret_mem))(V$mem_TokzIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(set.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, set.type));
    ret_mem->type = $typing(set.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = 0;
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_any)(set.raw))));
    return ret_mem;
};

$static fn_((mem_TokzIter__buf(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return S_meta((type)(self->buf));
};
$static fn_((mem_TokzIter__isDelimBytes(mem_TokzIter_Bytes* self, usize index))(bool)) {
    claim_assert_nonnull(self);
    return expr_(bool $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(*S_at((self->buf)[index]) == mem_Delim__unitBytes(&self->delim));
        case mem_Delim_seq: $break_(mem_startsWithBytes(S_suffix((self->buf)(index)), mem_Delim__seqBytes(&self->delim)));
        case mem_Delim_any: $break_(mem__byteIn(*S_at((self->buf)[index]), mem_Delim__anyBytes(&self->delim)));
    }) $unscoped(expr);
};
$static fn_((mem_TokzIter__isDelim(mem_TokzIter$raw* self, TypeInfo type, usize index))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let buf = mem_TokzIter__buf(self, type);
    return expr_(bool $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(u_memeql(u_atS(buf, index), mem_Delim__unit(self->delim_, type).ref.as_const));
        case mem_Delim_seq: $break_(mem_startsWith(u_suffixS(buf, index), mem_Delim__seq(self->delim_, type).as_const));
        case mem_Delim_any: {
            $break_(eval_(bool $scope)(for_(($us(mem_Delim__any(self->delim_, type)))(delim)) {
                if (!u_memeql(u_atS(buf, index), delim.as_const)) continue;
                $break_(true);
            } $end(for)) eval_(else)({
                $break_(false);
            }) $unscoped(eval));
        };
    }) $unscoped(expr);
};
$static fn_((mem_TokzIter__delimLenBytes(mem_TokzIter_Bytes* self))(usize)) {
    claim_assert_nonnull(self);
    return expr_(usize $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seqBytes(&self->delim).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_TokzIter__delimLen(mem_TokzIter$raw* self, TypeInfo type))(usize)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return expr_(usize $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seq(self->delim_, type).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};

fn_((mem_TokzIter_resetBytes(mem_TokzIter_Bytes* self))(void)) { claim_assert_nonnull(self), self->idx = 0; };
fn_((mem_TokzIter_reset(mem_TokzIter$raw* self))(void)) { claim_assert_nonnull(self), self->idx = 0; };
fn_((mem_TokzIter_nextBytes(mem_TokzIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let token = orelse_((mem_TokzIter_peekBytes(self))(return_none()));
    self->idx += token.len;
    return_some(token);
} $unscoped(fn);
fn_((mem_TokzIter_next(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let token = orelse_((mem_TokzIter_peek(self, type))(return_none()));
    self->idx += token.len;
    return_some(token);
} $unscoped(fn);
fn_((mem_TokzIter_peekBytes(mem_TokzIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    while (self->idx < self->buf.len && mem_TokzIter__isDelimBytes(self, self->idx)) {
        self->idx += mem_TokzIter__delimLenBytes(self);
    }
    let begin = self->idx;
    if (begin == self->buf.len) return_none();
    var end = begin;
    while (end < self->buf.len && !mem_TokzIter__isDelimBytes(self, end)) end++;
    return_some(S_slice((self->buf)$r(begin, end)));
} $unscoped(fn);
fn_((mem_TokzIter_peek(mem_TokzIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    while (self->idx < self->buf.len && mem_TokzIter__isDelim(self, type, self->idx)) {
        self->idx += mem_TokzIter__delimLen(self, type);
    }
    let begin = self->idx;
    if (begin == self->buf.len) return_none();
    var end = begin;
    while (end < self->buf.len && !mem_TokzIter__isDelim(self, type, end)) end++;
    return_some(u_sliceS(mem_TokzIter__buf(self, type), $r(begin, end)));
} $unscoped(fn);
fn_((mem_TokzIter_restBytes(mem_TokzIter_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self);
    var idx = self->idx;
    while (idx < self->buf.len && mem_TokzIter__isDelimBytes(self, idx)) {
        idx += mem_TokzIter__delimLenBytes(self);
    }
    return S_suffix((self->buf)(idx));
};
fn_((mem_TokzIter_rest(mem_TokzIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    var idx = self->idx;
    while (idx < self->buf.len && mem_TokzIter__isDelim(self, type, idx)) {
        idx += mem_TokzIter__delimLen(self, type);
    }
    return u_suffixS(mem_TokzIter__buf(self, type), idx);
} $unscoped(fn);

fn_((mem_splitUnitBytes(S_const$u8 buf, u8 unit))(mem_SplitIter_Bytes)) {
    claim_assert_nonnullS(buf);
    return (mem_SplitIter_Bytes){
        .buf = buf,
        .idx = some(0),
        .delim = union_of((mem_Delim_unit)(unit)),
    };
};
fn_((mem_splitUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(unit.inner), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->type = $typing(unit.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(0)));
    ret_mem->delim_[0].tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(ret_mem->delim_, unit.type).ref, unit.ref.as_const);
    return ret_mem;
};
fn_((mem_splitSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_SplitIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(seq), claim_assert(seq.len > 0);
    return (mem_SplitIter_Bytes){
        .buf = buf,
        .idx = some(0),
        .delim = union_of((mem_Delim_seq)(seq)),
    };
};
fn_((mem_splitSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type)), claim_assert(seq.len > 0);
    ret_mem->type = $typing(seq.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(0)));
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_seq)(seq.raw))));
    return ret_mem;
};
fn_((mem_splitAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_SplitIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(any);
    return (mem_SplitIter_Bytes){
        .buf = buf,
        .idx = some(0),
        .delim = union_of((mem_Delim_any)(any)),
    };
};
fn_((mem_splitAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitIter$raw ret_mem))(V$mem_SplitIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(any.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, any.type));
    ret_mem->type = $typing(any.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(0)));
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_any)(any.raw))));
    return ret_mem;
};

$static fn_((mem_SplitIter__buf(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return S_meta((type)(self->buf));
};
$static fn_((mem_SplitIter__delimLenBytes(mem_SplitIter_Bytes* self))(usize)) {
    claim_assert_nonnull(self);
    return expr_(usize $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seqBytes(&self->delim).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_SplitIter__delimLen(mem_SplitIter$raw* self, TypeInfo type))(usize)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return expr_(usize $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seq(self->delim_, type).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_SplitIter__findDelimBytes(mem_SplitIter_Bytes* self, usize begin))(O$usize $scope)) {
    claim_assert_nonnull(self);
    let rest = S_suffix((self->buf)(begin));
    return expr_(ReturnType $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: {
            if_some((mem_findFirstUnitBytes(rest, mem_Delim__unitBytes(&self->delim)))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
        case mem_Delim_seq: {
            if_some((mem_findFirstSeqBytes(rest, mem_Delim__seqBytes(&self->delim)))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
        case mem_Delim_any: {
            if_some((mem_findFirstAnyBytes(rest, mem_Delim__anyBytes(&self->delim)))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
    }) $unscoped(expr);
} $unscoped(fn);
$static fn_((mem_SplitIter__findDelim(mem_SplitIter$raw* self, TypeInfo type, usize begin))(O$usize $scope)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let rest = u_suffixS(mem_SplitIter__buf(self, type), begin);
    return expr_(ReturnType $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: {
            if_some((mem_findFirstUnit(rest, mem_Delim__unit(self->delim_, type)))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
        case mem_Delim_seq: {
            if_some((mem_findFirstSeq(rest, mem_Delim__seq(self->delim_, type).as_const))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
        case mem_Delim_any: {
            if_some((mem_findFirstAny(rest, mem_Delim__any(self->delim_, type).as_const))(idx)) {
                $break_(some(begin + idx));
            }
            $break_(none());
        };
    }) $unscoped(expr);
} $unscoped(fn);

fn_((mem_SplitIter_resetBytes(mem_SplitIter_Bytes* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(0)));
};
fn_((mem_SplitIter_reset(mem_SplitIter$raw* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(0)));
};
fn_((mem_SplitIter_firstBytes(mem_SplitIter_Bytes* self))(S_const$u8) $scope) {
    mem_SplitIter_resetBytes(self);
    return_(unwrap_(mem_SplitIter_nextBytes(self)));
} $unscoped(fn);
fn_((mem_SplitIter_first(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    mem_SplitIter_reset(self);
    return_(unwrap_(mem_SplitIter_next(self, type)));
} $unscoped(fn);
fn_((mem_SplitIter_nextBytes(mem_SplitIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let segment = orelse_((mem_SplitIter_peekBytes(self))(return_none()));
    let begin = unwrap_(self->idx);
    let end = begin + segment.len;
    $ignore_void asg_l((&self->idx)(expr_(O$usize $scope)(
        end < self->buf.len
            ? $break_(some(end + mem_SplitIter__delimLenBytes(self)))
            : $break_(none())
    ) $unscoped(expr)));
    return_some(segment);
} $unscoped(fn);
fn_((mem_SplitIter_next(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let segment = orelse_((mem_SplitIter_peek(self, type))(return_none()));
    let begin = unwrap_(self->idx);
    let end = begin + segment.len;
    $ignore_void asg_l((&self->idx)(expr_(O$usize $scope)(
        end < self->buf.len
            ? $break_(some(end + mem_SplitIter__delimLen(self, type)))
            : $break_(none())
    ) $unscoped(expr)));
    return_some(segment);
} $unscoped(fn);
fn_((mem_SplitIter_peekBytes(mem_SplitIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let begin = orelse_((self->idx)(return_none()));
    claim_assert(begin <= self->buf.len);
    let end = orelse_((mem_SplitIter__findDelimBytes(self, begin))(self->buf.len));
    return_some(S_slice((self->buf)$r(begin, end)));
} $unscoped(fn);
fn_((mem_SplitIter_peek(mem_SplitIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let begin = orelse_((self->idx)(return_none()));
    let buf = mem_SplitIter__buf(self, type);
    claim_assert(begin <= buf.len);
    let end = orelse_((mem_SplitIter__findDelim(self, type, begin))(buf.len));
    return_some(u_sliceS(buf, $r(begin, end)));
} $unscoped(fn);
fn_((mem_SplitIter_restBytes(mem_SplitIter_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self);
    let begin = orelse_((self->idx)(return S_suffix((self->buf)(self->buf.len))));
    return S_suffix((self->buf)(begin));
};
fn_((mem_SplitIter_rest(mem_SplitIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let buf = mem_SplitIter__buf(self, type);
    let begin = orelse_((self->idx)(return u_suffixS(buf, buf.len)));
    return u_suffixS(buf, begin);
} $unscoped(fn);
