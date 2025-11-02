#include "dh/ascii.h"

fn_((ascii_toUppers(S$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(ascii_str.ptr);
    for (usize i = 0; i < ascii_str.len; ++i) {
        ascii_str.ptr[i] = ascii_toUpper(ascii_str.ptr[i]);
    }
    return ascii_str;
}

fn_((ascii_toLowers(S$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(ascii_str.ptr);
    for (usize i = 0; i < ascii_str.len; ++i) {
        ascii_str.ptr[i] = ascii_toLower(ascii_str.ptr[i]);
    }
    return ascii_str;
}

fn_((ascii_toggleCases(S$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(ascii_str.ptr);
    for (usize i = 0; i < ascii_str.len; ++i) {
        ascii_str.ptr[i] = ascii_toggleCase(ascii_str.ptr[i]);
    }
    return ascii_str;
}

fn_((ascii_makeUppers(S$u8 buf, S_const$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(buf.ptr);
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert(ascii_str.len <= buf.len);

    for (usize i = 0; i < ascii_str.len; ++i) {
        buf.ptr[i] = ascii_toUpper(ascii_str.ptr[i]);
    }
    return slice$S(buf, $r(0, ascii_str.len));
}

fn_((ascii_makeLowers(S$u8 buf, S_const$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(buf.ptr);
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert(ascii_str.len <= buf.len);

    for (usize i = 0; i < ascii_str.len; ++i) {
        buf.ptr[i] = ascii_toLower(ascii_str.ptr[i]);
    }
    return slice$S(buf, $r(0, ascii_str.len));
}

fn_((ascii_makeToggledCases(S$u8 buf, S_const$u8 ascii_str))(S$u8)) {
    debug_assert_nonnull(buf.ptr);
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert(ascii_str.len <= buf.len);

    for (usize i = 0; i < ascii_str.len; ++i) {
        buf.ptr[i] = ascii_toggleCase(ascii_str.ptr[i]);
    }
    return slice$S(buf, $r(0, ascii_str.len));
}

fn_((ascii_allocUppers(mem_Allocator gpa, S_const$u8 ascii_str))(E$S$u8) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    let result = try_(mem_Allocator_alloc(gpa, typeInfo$(u8), ascii_str.len));
    return_ok(ascii_makeUppers(u_castS((S$u8)(result)), ascii_str));
} $unscoped_(fn);

fn_((ascii_allocLowers(mem_Allocator gpa, S_const$u8 ascii_str))(E$S$u8) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    let result = try_(mem_Allocator_alloc(gpa, typeInfo$(u8), ascii_str.len));
    return_ok(ascii_makeLowers(u_castS((S$u8)(result)), ascii_str));
} $unscoped_(fn);

fn_((ascii_allocToggledCases(mem_Allocator gpa, S_const$u8 ascii_str))(E$S$u8) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    let result = try_(mem_Allocator_alloc(gpa, typeInfo$(u8), ascii_str.len));
    return_ok(ascii_makeToggledCases(u_castS((S$u8)(result)), ascii_str));
} $unscoped_(fn);

fn_((ascii_idxOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr))(O$usize) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert_nonnull(ascii_substr.ptr);

    if (ascii_substr.len == 0) { return_some(0); }
    if (ascii_str.len < ascii_substr.len) { return_none(); }
    for (usize i = 0; i <= ascii_str.len - ascii_substr.len; ++i) {
        bool matches = true;
        for (usize j = 0; j < ascii_substr.len; ++j) {
            if (ascii_toLower(ascii_str.ptr[i + j]) != ascii_toLower(ascii_substr.ptr[j])) {
                matches = false;
                break;
            }
        }
        if (matches) { return_some(i); }
    }
    return_none();
} $unscoped_(fn);

fn_((ascii_idxFirstOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_front))(O$usize) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert_nonnull(ascii_substr.ptr);
    debug_assert(start_front <= ascii_str.len);

    if (ascii_substr.len == 0) { return_some(start_front); }
    if (ascii_str.len < start_front + ascii_substr.len) { return_none(); }
    let search_slice = slice$S(ascii_str, $r(start_front, ascii_str.len));
    if_some((ascii_idxOfIgnoreCase(search_slice, ascii_substr))(idx)) {
        return_some(start_front + idx);
    }
    return_none();
} $unscoped_(fn);

fn_((ascii_idxLastOfIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_substr, usize start_back))(O$usize) $scope) {
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert_nonnull(ascii_substr.ptr);
    debug_assert(start_back <= ascii_str.len);

    if (ascii_substr.len == 0) { return_some(start_back); }
    if (start_back + 1 < ascii_substr.len) { return_none(); }

    let max_start = start_back + 1 - ascii_substr.len;
    for (usize i = max_start; i != usize_limit_max; --i) {
        bool matches = true;
        for (usize j = 0; j < ascii_substr.len; ++j) {
            if (ascii_toLower(ascii_str.ptr[i + j]) != ascii_toLower(ascii_substr.ptr[j])) {
                matches = false;
                break;
            }
        }
        if (matches) { return_some(i); }
    }
    return_none();
} $unscoped_(fn);

fn_((ascii_startsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_prefix))(bool)) {
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert_nonnull(ascii_prefix.ptr);

    if (ascii_str.len < ascii_prefix.len) { return false; }
    for (usize i = 0; i < ascii_prefix.len; ++i) {
        if (ascii_toLower(ascii_str.ptr[i]) == ascii_toLower(ascii_prefix.ptr[i])) { continue; }
        return false;
    }
    return true;
}

fn_((ascii_endsWithIgnoreCase(S_const$u8 ascii_str, S_const$u8 ascii_suffix))(bool)) {
    debug_assert_nonnull(ascii_str.ptr);
    debug_assert_nonnull(ascii_suffix.ptr);

    if (ascii_str.len < ascii_suffix.len) { return false; }
    let start_offset = ascii_str.len - ascii_suffix.len;
    for (usize i = 0; i < ascii_suffix.len; ++i) {
        if (ascii_toLower(ascii_str.ptr[start_offset + i]) == ascii_toLower(ascii_suffix.ptr[i])) { continue; }
        return false;
    }
    return true;
}

fn_((ascii_cmp(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(cmp_Ord)) {
    let cmpFn = ignores_case ? ascii_cmpIgnoreCase : ascii_cmpSenseCase;
    return cmpFn(ascii_lhs, ascii_rhs);
}

fn_((ascii_cmpSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord)) {
    debug_assert_nonnull(ascii_lhs.ptr);
    debug_assert_nonnull(ascii_rhs.ptr);

    let min_len = prim_min(ascii_lhs.len, ascii_rhs.len);
    for (usize i = 0; i < min_len; ++i) {
        switch (prim_cmp(ascii_lhs.ptr[i], ascii_rhs.ptr[i])) {
            case_(cmp_Ord_lt, return cmp_Ord_lt);
            case_(cmp_Ord_gt, return cmp_Ord_gt);
            case_(cmp_Ord_eq, continue);
            default_(claim_unreachable);
        }
    }
    return prim_cmp(ascii_lhs.len, ascii_rhs.len);
}

fn_((ascii_cmpIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(cmp_Ord)) {
    debug_assert_nonnull(ascii_lhs.ptr);
    debug_assert_nonnull(ascii_rhs.ptr);

    let min_len = prim_min(ascii_lhs.len, ascii_rhs.len);
    for (usize i = 0; i < min_len; ++i) {
        switch (prim_cmp(ascii_toLower(ascii_lhs.ptr[i]), ascii_toLower(ascii_rhs.ptr[i]))) {
            case_(cmp_Ord_lt, return cmp_Ord_lt);
            case_(cmp_Ord_gt, return cmp_Ord_gt);
            case_(cmp_Ord_eq, continue);
            default_(claim_unreachable);
        }
    }
    return prim_cmp(ascii_lhs.len, ascii_rhs.len);
}

fn_((ascii_eql(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs, bool ignores_case))(bool)) {
    let eqlFn = ignores_case ? ascii_eqlIgnoreCase : ascii_eqlSenseCase;
    return eqlFn(ascii_lhs, ascii_rhs);
}

fn_((ascii_eqlSenseCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool)) {
    debug_assert_nonnull(ascii_lhs.ptr);
    debug_assert_nonnull(ascii_rhs.ptr);

    if (ascii_lhs.len != ascii_rhs.len) { return false; }
    for (usize i = 0; i < ascii_lhs.len; ++i) {
        if (ascii_lhs.ptr[i] == ascii_rhs.ptr[i]) { continue; }
        return false;
    }
    return true;
}

fn_((ascii_eqlIgnoreCase(S_const$u8 ascii_lhs, S_const$u8 ascii_rhs))(bool)) {
    debug_assert_nonnull(ascii_lhs.ptr);
    debug_assert_nonnull(ascii_rhs.ptr);

    if (ascii_lhs.len != ascii_rhs.len) { return false; }
    for (usize i = 0; i < ascii_lhs.len; ++i) {
        if (ascii_toLower(ascii_lhs.ptr[i]) == ascii_toLower(ascii_rhs.ptr[i])) { continue; }
        return false;
    }
    return true;
}
