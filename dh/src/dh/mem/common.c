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

fn_((mem_trimStartBytes(S_const$u8 haystack, S_const$u8 values_to_strip))(S_const$u8)) {
    claim_assert_nonnullS(haystack), claim_assert_nonnullS(values_to_strip);
    for_(($s(haystack), $rf(0))(item, idx)) {
        if (!mem_containsUnitBytes(values_to_strip, *item)) return S_suffix((haystack)(idx));
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
        if (!mem_containsUnitBytes(values_to_strip, *item)) return S_prefix((haystack)(idx + 1));
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

$static fn_((mem__catSliceAt(u_S_const$S_const$raw slices, TypeInfo type, usize idx))(u_S_const$raw)) {
    let raw = *u_castP$((const S_const$raw*)(u_atS(slices.as_raw, idx)));
    return S_meta((type)(raw));
};
$static fn_((mem__catLenBytes(S_const$S_const$u8 slices))(O$usize $scope)) {
    claim_assert_nonnullS(slices);
    var_(required_len, usize) = 0;
    for_(($s(slices))(slice)) {
        required_len = orelse_((usize_addChkd(required_len, slice->len))(return_none()));
    } $end(for);
    return_some(required_len);
} $unscoped(fn);
$static fn_((mem__catLen(TypeInfo type, u_S_const$S_const$raw slices))(O$usize $scope)) {
    claim_assert_nonnullS(slices);
    var_(required_len, usize) = 0;
    for_(($r(0, slices.len))(i)) {
        let slice = mem__catSliceAt(slices, type, i);
        required_len = orelse_((usize_addChkd(required_len, slice.len))(return_none()));
    } $end(for);
    return_some(required_len);
} $unscoped(fn);
$static fn_((mem__joinLenBytes(S_const$u8 sep, S_const$S_const$u8 slices))(O$usize $scope)) {
    let cat_len = orelse_((mem__catLenBytes(slices))(return_none()));
    if (slices.len <= 1) return_some(cat_len);
    let sep_count = slices.len - 1;
    let sep_len = orelse_((usize_mulChkd(sep.len, sep_count))(return_none()));
    return usize_addChkd(cat_len, sep_len);
} $unscoped(fn);
$static fn_((mem__joinLen(TypeInfo type, u_S_const$raw sep, u_S_const$S_const$raw slices))(O$usize $scope)) {
    let cat_len = orelse_((mem__catLen(type, slices))(return_none()));
    if (slices.len <= 1) return_some(cat_len);
    let sep_count = slices.len - 1;
    let sep_len = orelse_((usize_mulChkd(sep.len, sep_count))(return_none()));
    return usize_addChkd(cat_len, sep_len);
} $unscoped(fn);

fn_((mem_catBytes(S_const$S_const$u8 slices, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = orelse_((mem__catLenBytes(slices))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_catWithinBytes(slices, out));
} $unscoped(fn);
fn_((mem_cat(u_S_const$S_const$raw slices, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = orelse_((mem__catLen(out.type, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_catWithin(slices, out));
} $unscoped(fn);
fn_((mem_catWithinBytes(S_const$S_const$u8 slices, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = unwrap_(mem__catLenBytes(slices));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    for_(($s(slices))(slice)) {
        mem_copyBytes(S_slice((out)$r(written, written + slice->len)), *slice);
        written += slice->len;
    } $end(for);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_catWithin(u_S_const$S_const$raw slices, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = unwrap_(mem__catLen(out.type, slices));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    for_(($r(0, slices.len))(i)) {
        let slice = mem__catSliceAt(slices, out.type, i);
        mem_copy(u_sliceS(out, $r(written, written + slice.len)), slice);
        written += slice.len;
    } $end(for);
    return u_prefixS(out, required_len);
};
fn_((mem_catAllocBytes(S_const$S_const$u8 slices, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(slices);
    let required_len = orelse_((mem__catLenBytes(slices))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_catWithinBytes(slices, out));
} $unscoped(fn);
fn_((mem_catAlloc(TypeInfo type, u_S_const$S_const$raw slices, mem_Alctr gpa))(mem_E$u_S$raw) $scope) {
    claim_assert_nonnullS(slices);
    let required_len = orelse_((mem__catLen(type, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_alloc($trace gpa, type, required_len));
    return_ok(mem_catWithin(slices, out));
} $unscoped(fn);

fn_((mem_joinBytes(S_const$u8 sep, S_const$S_const$u8 slices, S$u8 out))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = orelse_((mem__joinLenBytes(sep, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_joinWithinBytes(sep, slices, out));
} $unscoped(fn);
fn_((mem_join(u_S_const$raw sep, u_S_const$S_const$raw slices, u_S$raw out))(mem_E$u_S$raw $scope)) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(sep.type, out.type));
    let required_len = orelse_((mem__joinLen(out.type, sep, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    if (required_len > out.len) return_err(E_cause$OutOfMemory());
    return_ok(mem_joinWithin(sep, slices, out));
} $unscoped(fn);
fn_((mem_joinWithinBytes(S_const$u8 sep, S_const$S_const$u8 slices, S$u8 out))(S$u8)) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    let required_len = unwrap_(mem__joinLenBytes(sep, slices));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    for_(($s(slices), $rf(0))(slice, idx)) {
        if (idx != 0) {
            mem_copyBytes(S_slice((out)$r(written, written + sep.len)), sep);
            written += sep.len;
        }
        mem_copyBytes(S_slice((out)$r(written, written + slice->len)), *slice);
        written += slice->len;
    } $end(for);
    return S_slice((out)$r(0, required_len));
};
fn_((mem_joinWithin(u_S_const$raw sep, u_S_const$S_const$raw slices, u_S$raw out))(u_S$raw)) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices), claim_assert_nonnullS(out);
    claim_assert(TypeInfo_eql(sep.type, out.type));
    let required_len = unwrap_(mem__joinLen(out.type, sep, slices));
    claim_assert(required_len <= out.len);
    var_(written, usize) = 0;
    for_(($r(0, slices.len))(i)) {
        if (i != 0) {
            mem_copy(u_sliceS(out, $r(written, written + sep.len)), sep);
            written += sep.len;
        }
        let slice = mem__catSliceAt(slices, out.type, i);
        mem_copy(u_sliceS(out, $r(written, written + slice.len)), slice);
        written += slice.len;
    } $end(for);
    return u_prefixS(out, required_len);
};
fn_((mem_joinAllocBytes(S_const$u8 sep, S_const$S_const$u8 slices, mem_Alctr gpa))(mem_E$S$u8 $scope)) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices);
    let required_len = orelse_((mem__joinLenBytes(sep, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_allocBytes($trace gpa, required_len));
    return_ok(mem_joinWithinBytes(sep, slices, out));
} $unscoped(fn);
fn_((mem_joinAlloc(u_S_const$raw sep, u_S_const$S_const$raw slices, mem_Alctr gpa))(mem_E$u_S$raw) $scope) {
    claim_assert_nonnullS(sep), claim_assert_nonnullS(slices);
    let required_len = orelse_((mem__joinLen(sep.type, sep, slices))(
        return_err(E_cause$OutOfMemory())
    ));
    let out = try_(mem_Alctr_alloc($trace gpa, sep.type, required_len));
    return_ok(mem_joinWithin(sep, slices, out));
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
        case mem_Delim_any: $break_(mem_containsUnitBytes(mem_Delim__anyBytes(&self->delim), *S_at((self->buf)[index])));
    }) $unscoped(expr);
};
$static fn_((mem_TokzIter__isDelim(mem_TokzIter$raw* self, TypeInfo type, usize index))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let buf = mem_TokzIter__buf(self, type);
    return expr_(bool $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(u_memeql(u_atS(buf, index), mem_Delim__unit(self->delim_, type).ref.as_const));
        case mem_Delim_seq: $break_(mem_startsWith(u_suffixS(buf, index), mem_Delim__seq(self->delim_, type).as_const));
        case mem_Delim_any: {
            $break_(mem_containsUnit(mem_Delim__any(self->delim_, type).as_const, u_deref(u_atS(buf, index))));
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

fn_((mem_tokzBwdUnitBytes(S_const$u8 buf, u8 unit))(mem_TokzBwdIter_Bytes)) {
    claim_assert_nonnullS(buf);
    return (mem_TokzBwdIter_Bytes){
        .buf = buf,
        .idx = buf.len,
        .delim = union_of((mem_Delim_unit)(unit)),
    };
};
fn_((mem_tokzBwdUnit(u_S_const$raw buf, u_V$raw unit, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(unit.inner), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->type = $typing(unit.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = buf.len;
    ret_mem->delim_[0].tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(ret_mem->delim_, unit.type).ref, unit.ref.as_const);
    return ret_mem;
};
fn_((mem_tokzBwdSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_TokzBwdIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(seq), claim_assert(seq.len > 0);
    return (mem_TokzBwdIter_Bytes){
        .buf = buf,
        .idx = buf.len,
        .delim = union_of((mem_Delim_seq)(seq)),
    };
};
fn_((mem_tokzBwdSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type)), claim_assert(seq.len > 0);
    ret_mem->type = $typing(seq.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = buf.len;
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_seq)(seq.raw))));
    return ret_mem;
};
fn_((mem_tokzBwdAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_TokzBwdIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(any);
    return (mem_TokzBwdIter_Bytes){
        .buf = buf,
        .idx = buf.len,
        .delim = union_of((mem_Delim_any)(any)),
    };
};
fn_((mem_tokzBwdAny(u_S_const$raw buf, u_S_const$raw any, V$mem_TokzBwdIter$raw ret_mem))(V$mem_TokzBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(any.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, any.type));
    ret_mem->type = $typing(any.type);
    ret_mem->buf = buf.raw;
    ret_mem->idx = buf.len;
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_any)(any.raw))));
    return ret_mem;
};

$static fn_((mem_TokzBwdIter__buf(mem_TokzBwdIter$raw* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return S_meta((type)(self->buf));
};
$static fn_((mem_TokzBwdIter__delimLenBytes(mem_TokzBwdIter_Bytes* self))(usize)) {
    claim_assert_nonnull(self);
    return expr_(usize $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seqBytes(&self->delim).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_TokzBwdIter__delimLen(mem_TokzBwdIter$raw* self, TypeInfo type))(usize)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return expr_(usize $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seq(self->delim_, type).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_TokzBwdIter__isDelimEndingBytes(mem_TokzBwdIter_Bytes* self, usize end))(bool)) {
    claim_assert_nonnull(self);
    if (end == 0) return false;
    return expr_(bool $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(*S_at((self->buf)[end - 1]) == mem_Delim__unitBytes(&self->delim));
        case mem_Delim_seq: {
            let seq = mem_Delim__seqBytes(&self->delim);
            $break_(end >= seq.len && mem_startsWithBytes(S_suffix((self->buf)(end - seq.len)), seq));
        };
        case mem_Delim_any: $break_(mem_containsUnitBytes(mem_Delim__anyBytes(&self->delim), *S_at((self->buf)[end - 1])));
    }) $unscoped(expr);
};
$static fn_((mem_TokzBwdIter__isDelimEnding(mem_TokzBwdIter$raw* self, TypeInfo type, usize end))(bool)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    if (end == 0) return false;
    let buf = mem_TokzBwdIter__buf(self, type);
    return expr_(bool $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(u_memeql(u_atS(buf, end - 1), mem_Delim__unit(self->delim_, type).ref.as_const));
        case mem_Delim_seq: {
            let seq = mem_Delim__seq(self->delim_, type).as_const;
            $break_(end >= seq.len && mem_startsWith(u_suffixS(buf, end - seq.len), seq));
        };
        case mem_Delim_any: $break_(mem_containsUnit(mem_Delim__any(self->delim_, type).as_const, u_deref(u_atS(buf, end - 1))));
    }) $unscoped(expr);
};

fn_((mem_TokzBwdIter_resetBytes(mem_TokzBwdIter_Bytes* self))(void)) { claim_assert_nonnull(self), self->idx = self->buf.len; };
fn_((mem_TokzBwdIter_reset(mem_TokzBwdIter$raw* self))(void)) { claim_assert_nonnull(self), self->idx = self->buf.len; };
fn_((mem_TokzBwdIter_nextBytes(mem_TokzBwdIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let token = orelse_((mem_TokzBwdIter_peekBytes(self))(return_none()));
    self->idx -= token.len;
    return_some(token);
} $unscoped(fn);
fn_((mem_TokzBwdIter_next(mem_TokzBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let token = orelse_((mem_TokzBwdIter_peek(self, type))(return_none()));
    self->idx -= token.len;
    return_some(token);
} $unscoped(fn);
fn_((mem_TokzBwdIter_peekBytes(mem_TokzBwdIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    while (self->idx > 0 && mem_TokzBwdIter__isDelimEndingBytes(self, self->idx)) {
        self->idx -= mem_TokzBwdIter__delimLenBytes(self);
    }
    let end = self->idx;
    if (end == 0) return_none();
    var begin = end;
    while (begin > 0 && !mem_TokzBwdIter__isDelimEndingBytes(self, begin)) begin--;
    return_some(S_slice((self->buf)$r(begin, end)));
} $unscoped(fn);
fn_((mem_TokzBwdIter_peek(mem_TokzBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    while (self->idx > 0 && mem_TokzBwdIter__isDelimEnding(self, type, self->idx)) {
        self->idx -= mem_TokzBwdIter__delimLen(self, type);
    }
    let end = self->idx;
    if (end == 0) return_none();
    var begin = end;
    while (begin > 0 && !mem_TokzBwdIter__isDelimEnding(self, type, begin)) begin--;
    return_some(u_sliceS(mem_TokzBwdIter__buf(self, type), $r(begin, end)));
} $unscoped(fn);
fn_((mem_TokzBwdIter_restBytes(mem_TokzBwdIter_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self);
    var idx = self->idx;
    while (idx > 0 && mem_TokzBwdIter__isDelimEndingBytes(self, idx)) {
        idx -= mem_TokzBwdIter__delimLenBytes(self);
    }
    return S_prefix((self->buf)(idx));
};
fn_((mem_TokzBwdIter_rest(mem_TokzBwdIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    var idx = self->idx;
    while (idx > 0 && mem_TokzBwdIter__isDelimEnding(self, type, idx)) {
        idx -= mem_TokzBwdIter__delimLen(self, type);
    }
    return u_prefixS(mem_TokzBwdIter__buf(self, type), idx);
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

fn_((mem_splitBwdUnitBytes(S_const$u8 buf, u8 unit))(mem_SplitBwdIter_Bytes)) {
    claim_assert_nonnullS(buf);
    return (mem_SplitBwdIter_Bytes){
        .buf = buf,
        .idx = some(buf.len),
        .delim = union_of((mem_Delim_unit)(unit)),
    };
};
fn_((mem_splitBwdUnit(u_S_const$raw buf, u_V$raw unit, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnull(unit.inner), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, unit.type));
    ret_mem->type = $typing(unit.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(buf.len)));
    ret_mem->delim_[0].tag = mem_Delim_unit;
    u_memcpy(mem_Delim__unit(ret_mem->delim_, unit.type).ref, unit.ref.as_const);
    return ret_mem;
};
fn_((mem_splitBwdSeqBytes(S_const$u8 buf, S_const$u8 seq))(mem_SplitBwdIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(seq), claim_assert(seq.len > 0);
    return (mem_SplitBwdIter_Bytes){
        .buf = buf,
        .idx = some(buf.len),
        .delim = union_of((mem_Delim_seq)(seq)),
    };
};
fn_((mem_splitBwdSeq(u_S_const$raw buf, u_S_const$raw seq, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(seq.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, seq.type)), claim_assert(seq.len > 0);
    ret_mem->type = $typing(seq.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(buf.len)));
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_seq)(seq.raw))));
    return ret_mem;
};
fn_((mem_splitBwdAnyBytes(S_const$u8 buf, S_const$u8 any))(mem_SplitBwdIter_Bytes)) {
    claim_assert_nonnullS(buf), claim_assert_nonnullS(any);
    return (mem_SplitBwdIter_Bytes){
        .buf = buf,
        .idx = some(buf.len),
        .delim = union_of((mem_Delim_any)(any)),
    };
};
fn_((mem_splitBwdAny(u_S_const$raw buf, u_S_const$raw any, V$mem_SplitBwdIter$raw ret_mem))(V$mem_SplitBwdIter$raw)) {
    claim_assert_nonnullS(buf.raw), claim_assert_nonnullS(any.raw), claim_assert_nonnull(ret_mem);
    claim_assert(TypeInfo_eql(buf.type, any.type));
    ret_mem->type = $typing(any.type);
    ret_mem->buf = buf.raw;
    asg_l((&ret_mem->idx)(some(buf.len)));
    asg_l((&ret_mem->delim_[0])(union_of((mem_Delim_any)(any.raw))));
    return ret_mem;
};

$static fn_((mem_SplitBwdIter__buf(mem_SplitBwdIter$raw* self, TypeInfo type))(u_S_const$raw)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return S_meta((type)(self->buf));
};
$static fn_((mem_SplitBwdIter__delimLenBytes(mem_SplitBwdIter_Bytes* self))(usize)) {
    claim_assert_nonnull(self);
    return expr_(usize $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seqBytes(&self->delim).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_SplitBwdIter__delimLen(mem_SplitBwdIter$raw* self, TypeInfo type))(usize)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    return expr_(usize $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: $break_(1);
        case mem_Delim_seq: $break_(mem_Delim__seq(self->delim_, type).len);
        case mem_Delim_any: $break_(1);
    }) $unscoped(expr);
};
$static fn_((mem_SplitBwdIter__findDelimBytes(mem_SplitBwdIter_Bytes* self, usize end))(O$usize $scope)) {
    claim_assert_nonnull(self);
    return expr_(ReturnType $scope)(switch (self->delim.tag) {
        case mem_Delim_unit: {
            var idx = end;
            while (idx != 0) {
                idx--;
                if (*S_at((self->buf)[idx]) == mem_Delim__unitBytes(&self->delim)) $break_(some(idx));
            }
            $break_(none());
        };
        case mem_Delim_seq: {
            let seq = mem_Delim__seqBytes(&self->delim);
            if (seq.len > end) $break_(none());
            var idx = end - seq.len + 1;
            while (idx != 0) {
                idx--;
                if (mem_startsWithBytes(S_suffix((self->buf)(idx)), seq)) $break_(some(idx));
            }
            $break_(none());
        };
        case mem_Delim_any: {
            let any = mem_Delim__anyBytes(&self->delim);
            var idx = end;
            while (idx != 0) {
                idx--;
                if (mem_containsUnitBytes(any, *S_at((self->buf)[idx]))) $break_(some(idx));
            }
            $break_(none());
        };
    }) $unscoped(expr);
} $unscoped(fn);
$static fn_((mem_SplitBwdIter__findDelim(mem_SplitBwdIter$raw* self, TypeInfo type, usize end))(O$usize $scope)) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let buf = mem_SplitBwdIter__buf(self, type);
    return expr_(ReturnType $scope)(switch (self->delim_[0].tag) {
        case mem_Delim_unit: {
            let unit = mem_Delim__unit(self->delim_, type);
            var idx = end;
            while (idx != 0) {
                idx--;
                if (u_memeql(u_atS(buf, idx), unit.ref.as_const)) $break_(some(idx));
            }
            $break_(none());
        };
        case mem_Delim_seq: {
            let seq = mem_Delim__seq(self->delim_, type).as_const;
            if (seq.len > end) $break_(none());
            var idx = end - seq.len + 1;
            while (idx != 0) {
                idx--;
                if (mem_startsWith(u_suffixS(buf, idx), seq)) $break_(some(idx));
            }
            $break_(none());
        };
        case mem_Delim_any: {
            let any = mem_Delim__any(self->delim_, type).as_const;
            var idx = end;
            while (idx != 0) {
                idx--;
                if (mem_containsUnit(any, u_deref(u_atS(buf, idx)))) $break_(some(idx));
            }
            $break_(none());
        };
    }) $unscoped(expr);
} $unscoped(fn);

fn_((mem_SplitBwdIter_resetBytes(mem_SplitBwdIter_Bytes* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(self->buf.len)));
};
fn_((mem_SplitBwdIter_reset(mem_SplitBwdIter$raw* self))(void)) {
    claim_assert_nonnull(self), $ignore_void asg_l((&self->idx)(some(self->buf.len)));
};
fn_((mem_SplitBwdIter_firstBytes(mem_SplitBwdIter_Bytes* self))(S_const$u8) $scope) {
    mem_SplitBwdIter_resetBytes(self);
    return_(unwrap_(mem_SplitBwdIter_nextBytes(self)));
} $unscoped(fn);
fn_((mem_SplitBwdIter_first(mem_SplitBwdIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    mem_SplitBwdIter_reset(self);
    return_(unwrap_(mem_SplitBwdIter_next(self, type)));
} $unscoped(fn);
fn_((mem_SplitBwdIter_nextBytes(mem_SplitBwdIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let segment = orelse_((mem_SplitBwdIter_peekBytes(self))(return_none()));
    let end = unwrap_(self->idx);
    let begin = end - segment.len;
    $ignore_void asg_l((&self->idx)(expr_(O$usize $scope)(
        begin != 0
            ? $break_(some(begin - mem_SplitBwdIter__delimLenBytes(self)))
            : $break_(none())
    ) $unscoped(expr)));
    return_some(segment);
} $unscoped(fn);
fn_((mem_SplitBwdIter_next(mem_SplitBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let segment = orelse_((mem_SplitBwdIter_peek(self, type))(return_none()));
    let end = unwrap_(self->idx);
    let begin = end - segment.len;
    $ignore_void asg_l((&self->idx)(expr_(O$usize $scope)(
        begin != 0
            ? $break_(some(begin - mem_SplitBwdIter__delimLen(self, type)))
            : $break_(none())
    ) $unscoped(expr)));
    return_some(segment);
} $unscoped(fn);
fn_((mem_SplitBwdIter_peekBytes(mem_SplitBwdIter_Bytes* self))(O$S_const$u8) $scope) {
    claim_assert_nonnull(self);
    let end = orelse_((self->idx)(return_none()));
    claim_assert(end <= self->buf.len);
    var_(begin, usize) = 0;
    if_some((mem_SplitBwdIter__findDelimBytes(self, end))(delim_idx)) {
        begin = delim_idx + mem_SplitBwdIter__delimLenBytes(self);
    }
    return_some(S_slice((self->buf)$r(begin, end)));
} $unscoped(fn);
fn_((mem_SplitBwdIter_peek(mem_SplitBwdIter$raw* self, TypeInfo type))(O$u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let end = orelse_((self->idx)(return_none()));
    let buf = mem_SplitBwdIter__buf(self, type);
    claim_assert(end <= buf.len);
    var_(begin, usize) = 0;
    if_some((mem_SplitBwdIter__findDelim(self, type, end))(delim_idx)) {
        begin = delim_idx + mem_SplitBwdIter__delimLen(self, type);
    }
    return_some(u_sliceS(buf, $r(begin, end)));
} $unscoped(fn);
fn_((mem_SplitBwdIter_restBytes(mem_SplitBwdIter_Bytes* self))(S_const$u8)) {
    claim_assert_nonnull(self);
    let end = orelse_((self->idx)(return S_prefix((self->buf)(0))));
    return S_prefix((self->buf)(end));
};
fn_((mem_SplitBwdIter_rest(mem_SplitBwdIter$raw* self, TypeInfo type))(u_S_const$raw) $scope) {
    claim_assert_nonnull(self), debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let buf = mem_SplitBwdIter__buf(self, type);
    let end = orelse_((self->idx)(return u_prefixS(buf, 0)));
    return u_prefixS(buf, end);
} $unscoped(fn);
