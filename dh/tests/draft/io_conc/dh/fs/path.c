#include "path.h"

$static fn_((fs_path__isSep(u8 c))(bool)) {
    return c == '/' || c == '\\';
}

#if UNUSED_CODE
$static fn_((fs_path__copy(S$u8 out_buf, S_const$u8 src))(E$S$u8) $scope) {
    if (out_buf.len < src.len) return_err(fs_path_E_BufferTooSmall());
    mem_copyBytes(S_slice((out_buf)$r(0, src.len)), src);
    return_ok(S_slice((out_buf)$r(0, src.len)));
} $unscoped(fn);
#endif /* UNUSED_CODE */

$static fn_((fs_path__alloc(mem_Alctr gpa, usize len))(E$S$u8) $scope) {
    let mem = try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), len));
    return_ok(u_castS$((S$u8)(mem)));
} $unscoped(fn);

fn_((fs_path_isAbs(S_const$u8 path))(bool)) {
    if (path.len == 0) return false;
    if (fs_path__isSep(path.ptr[0])) return true;
    if (2 <= path.len && path.ptr[1] == ':') return true;
    return false;
}

fn_((fs_path_basename(S_const$u8 path))(S_const$u8)) {
    var_(end, usize) = path.len;
    while (0 < end && fs_path__isSep(path.ptr[end - 1])) end -= 1;
    var_(start, usize) = end;
    while (0 < start && !fs_path__isSep(path.ptr[start - 1])) start -= 1;
    return S_slice((path)$r(start, end));
}

fn_((fs_path_dirname(S_const$u8 path))(S_const$u8)) {
    var_(end, usize) = path.len;
    while (0 < end && fs_path__isSep(path.ptr[end - 1])) end -= 1;
    while (0 < end && !fs_path__isSep(path.ptr[end - 1])) end -= 1;
    while (1 < end && fs_path__isSep(path.ptr[end - 1])) end -= 1;
    return S_slice((path)$r(0, end));
}

fn_((fs_path_extension(S_const$u8 path))(S_const$u8)) {
    let base = fs_path_basename(path);
    var_(idx, usize) = base.len;
    while (0 < idx) {
        idx -= 1;
        if (base.ptr[idx] == '.') {
            if (idx == 0) return S_slice((base)$r(base.len, base.len));
            return S_slice((base)$r(idx, base.len));
        }
    }
    return S_slice((base)$r(base.len, base.len));
}

fn_((fs_path_stem(S_const$u8 path))(S_const$u8)) {
    let base = fs_path_basename(path);
    let ext = fs_path_extension(base);
    return S_slice((base)$r(0, base.len - ext.len));
}

fn_((fs_path_join2(S_const$u8 lhs, S_const$u8 rhs, S$u8 out_buf))(E$S$u8) $scope) {
    if (fs_path_isAbs(rhs)) return fs_path_normalize(rhs, out_buf);
    let need_sep = lhs.len != 0 && rhs.len != 0 && !fs_path__isSep(lhs.ptr[lhs.len - 1]);
    let len = lhs.len + rhs.len + (need_sep ? 1 : 0);
    if (out_buf.len < len) return_err(fs_path_E_BufferTooSmall());
    var_(pos, usize) = 0;
    mem_copyBytes(S_slice((out_buf)$r(pos, pos + lhs.len)), lhs);
    pos += lhs.len;
    if (need_sep) {
        out_buf.ptr[pos] = fs_path_sep;
        pos += 1;
    }
    mem_copyBytes(S_slice((out_buf)$r(pos, pos + rhs.len)), rhs);
    return_ok(S_slice((out_buf)$r(0, len)));
} $unscoped(fn);

fn_((fs_path_join2Alloc(S_const$u8 lhs, S_const$u8 rhs, mem_Alctr gpa))(E$S$u8) $scope) {
    let len = lhs.len + rhs.len + 1;
    let buf = try_(fs_path__alloc(gpa, len));
    return fs_path_join2(lhs, rhs, buf);
} $unscoped(fn);

fn_((fs_path_normalize(S_const$u8 path, S$u8 out_buf))(E$S$u8) $scope) {
    if (out_buf.len < path.len) return_err(fs_path_E_BufferTooSmall());
    var_(pos, usize) = 0;
    var_(prev_sep, bool) = false;
    for (usize i = 0; i < path.len; i += 1) {
        let c = path.ptr[i];
        let is_sep = fs_path__isSep(c);
        if (is_sep && prev_sep) continue;
        out_buf.ptr[pos] = is_sep ? fs_path_sep : c;
        pos += 1;
        prev_sep = is_sep;
    }
    return_ok(S_slice((out_buf)$r(0, pos)));
} $unscoped(fn);

fn_((fs_path_normalizeAlloc(S_const$u8 path, mem_Alctr gpa))(E$S$u8) $scope) {
    let buf = try_(fs_path__alloc(gpa, path.len));
    return fs_path_normalize(path, buf);
} $unscoped(fn);

fn_((fs_path_resolve(S_const$u8 base, S_const$u8 sub_path, S$u8 out_buf))(E$S$u8)) {
    if (fs_path_isAbs(sub_path)) return fs_path_normalize(sub_path, out_buf);
    return fs_path_join2(base, sub_path, out_buf);
}

fn_((fs_path_resolveAlloc(S_const$u8 base, S_const$u8 sub_path, mem_Alctr gpa))(E$S$u8) $scope) {
    let len = base.len + sub_path.len + 1;
    let buf = try_(fs_path__alloc(gpa, len));
    return fs_path_resolve(base, sub_path, buf);
} $unscoped(fn);
