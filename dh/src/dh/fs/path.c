#include "dh/fs/path.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_trimEnd,
    mem_findLastUnit,
    mem_findLastAny,
    mem_catWithin,
    mem_joinWithin,
    mem_Cutted,
    mem_Cutted_before,
    mem_cutLastAny
));

$static fn_((fs_path__isSep(u8 c))(bool)) {
    return c == '/' || c == '\\';
}

$static fn_((fs_path__seps(void))(S_const$u8)) {
    return u8_l("/\\");
}

$static fn_((fs_path__sepS(void))(S_const$u8)) {
    return P_prefix$((S_const$u8)(&fs_path_sep)(1));
}

$static fn_((fs_path__joinLen(S_const$u8 lhs, S_const$u8 rhs))(O$usize $scope)) {
    let need_sep = lhs.len != 0 && rhs.len != 0 && !fs_path__isSep(*S_at((lhs)[lhs.len - 1]));
    let lhs_rhs_len = orelse_((usize_addChkd(lhs.len, rhs.len))(return_none()));
    return usize_addChkd(lhs_rhs_len, need_sep ? usize_(1) : usize_(0));
} $unscoped(fn);

$static fn_((fs_path__alloc(mem_Alctr gpa, usize len))(E$S$u8) $scope) {
    let mem = try_(mem_Alctr_alloc($trace gpa, typeInfo$(u8), len));
    return_ok(u_castS$((S$u8)(mem)));
} $unscoped(fn);

fn_((fs_path_isAbs(S_const$u8 path))(bool)) {
    if (path.len == 0) return false;
    if (fs_path__isSep(*S_at((path)[0]))) return true;
    if (2 <= path.len && *S_at((path)[1]) == ':') return true;
    return false;
}

fn_((fs_path_basename(S_const$u8 path))(S_const$u8)) {
    let trimmed = mem_trimEnd$u8(path, fs_path__seps());
    if_some((mem_findLastAny$u8(trimmed, fs_path__seps()))(idx)) {
        return S_suffix((trimmed)(idx + 1));
    }
    return trimmed;
}

fn_((fs_path_dirname(S_const$u8 path))(S_const$u8)) {
    let trimmed = mem_trimEnd$u8(path, fs_path__seps());
    let cut = orelse_((mem_cutLastAny$u8(trimmed, fs_path__seps()))(
        return S_prefix((path)(0))
    ));
    let before = mem_Cutted_before$u8(cut);
    let dirname = mem_trimEnd$u8(before, fs_path__seps());
    if (dirname.len != 0) return dirname;
    if (before.len != 0 && fs_path__isSep(*S_at((before)[0]))) return S_prefix((before)(1));
    return dirname;
}

fn_((fs_path_extension(S_const$u8 path))(S_const$u8)) {
    let base = fs_path_basename(path);
    if_some((mem_findLastUnit$u8(base, u8_c('.')))(idx)) {
        if (idx != 0) return S_suffix((base)(idx));
    }
    return S_suffix((base)(base.len));
}

fn_((fs_path_stem(S_const$u8 path))(S_const$u8)) {
    let base = fs_path_basename(path);
    let ext = fs_path_extension(base);
    return S_slice((base)$r(0, base.len - ext.len));
}

fn_((fs_path_join2(S_const$u8 lhs, S_const$u8 rhs, S$u8 out_buf))(E$S$u8) $scope) {
    if (fs_path_isAbs(rhs)) return fs_path_normalize(rhs, out_buf);
    let need_sep = lhs.len != 0 && rhs.len != 0 && !fs_path__isSep(*S_at((lhs)[lhs.len - 1]));
    let len = orelse_((fs_path__joinLen(lhs, rhs))(return_err(E_cause$PathBufferTooSmall())));
    if (out_buf.len < len) return_err(E_cause$PathBufferTooSmall());
    need_sep ? return_ok(mem_joinWithin$u8(fs_path__sepS(), lhs, rhs, out_buf))
             : return_ok(mem_catWithin$u8(lhs, rhs, out_buf));
} $unscoped(fn);

fn_((fs_path_join2Alloc(S_const$u8 lhs, S_const$u8 rhs, mem_Alctr gpa))(E$S$u8) $scope) {
    let len = fs_path_isAbs(rhs) ? rhs.len
                                 : orelse_((fs_path__joinLen(lhs, rhs))(return_err(E_cause$PathBufferTooSmall())));
    let buf = try_(fs_path__alloc(gpa, len));
    return fs_path_join2(lhs, rhs, buf);
} $unscoped(fn);

fn_((fs_path_normalize(S_const$u8 path, S$u8 out_buf))(E$S$u8) $scope) {
    if (out_buf.len < path.len) return_err(E_cause$PathBufferTooSmall());
    var_(pos, usize) = 0;
    var_(prev_sep, bool) = false;
    for_(($s(path))(c)) {
        let is_sep = fs_path__isSep(*c);
        if (is_sep && prev_sep) continue;
        *S_at((out_buf)[pos]) = is_sep ? fs_path_sep : *c;
        pos += 1;
        prev_sep = is_sep;
    } $end(for);
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
    let len = fs_path_isAbs(sub_path) ? sub_path.len
                                      : orelse_((fs_path__joinLen(base, sub_path))(return_err(E_cause$PathBufferTooSmall())));
    let buf = try_(fs_path__alloc(gpa, len));
    return fs_path_resolve(base, sub_path, buf);
} $unscoped(fn);
