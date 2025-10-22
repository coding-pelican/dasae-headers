#include "dh/core.h"
#include "dh/fn.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/err_res.h"
#include "dh/mem.h"
#include "dh/mem/Allocator.h"

#define CASE 1
#if CASE == 1
#undef mem_asBytes_const
#undef mem_asBytes
#undef mem_toBytes_const$
#undef mem_toBytes_const
#undef mem_toBytes$
#undef mem_toBytes

#define mem_asBytes_const$(_T, /* const T* */ _val... /* Sli_const$u8 */) tpl_id(mem_asBytes_const, _T)(_val)
#define mem_asBytes$(_T, /* T* */ _val... /* Sli$u8 */)                   tpl_id(mem_asBytes, _T)(_val) z
#define mem_asBytes_useT$(_T...) \
    static fn_((tpl_id(mem_asBytes_const, _T)(const _T* val))(Sli_const$u8)) { \
        return Sli_from$(Sli_const$u8, as$((const u8*)(val)), sizeOf(*val)); \
    } \
    static fn_((tpl_id(mem_asBytes, _T)(_T * val))(Sli$u8)) { \
        return Sli_from$(Sli$u8, as$((u8*)(val)), sizeOf(*val)); \
    }

#define mem_toBytes$(_T, /* T */ _val... /* Arr$$(sizeOf(_T), _T) */) tpl_id(mem_toBytes, _T)(_val)
#define mem_toBytes_useT$(_T...) \
    typedef Arr$$(sizeOf(_T), _T) tpl_Ret(mem_toBytes, _T); \
    static fn_((tpl_id(mem_toBytes, _T)(_T val))(tpl_Ret(mem_toBytes, _T))) { \
        return Sli_deref$(tpl_Ret(mem_toBytes, _T), tpl_id(mem_asBytes, _T)(&val)); \
    }

// EQUALITY AND COMPARISON
//   Returns true if slices are equal, false otherwise
#define mem_eql$(_T, /* Sli_const$T */ _lhs, /* Sli_const$T */ _rhs... /* bool */) tpl_id(mem_eql, _T)(_lhs, _rhs)
#define mem_eql_useT$(_T...) \
    static fn_((tpl_id(mem_eql, _T)(Sli_const$(_T) lhs, Sli_const$(_T) rhs))(bool)) { \
        return mem_bytes_eql( \
            typeInfo$(_T), \
            Sli_from$(Sli_const$u8, as$((const u8*)(lhs.ptr)), lhs.len), \
            Sli_from$(Sli_const$u8, as$((const u8*)(rhs.ptr)), rhs.len) \
        ); \
    }

// PREFIX/SUFFIX OPERATIONS
//   Returns true if haystack starts with prefix
#define mem_startsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _prefix... /* Opt$usize */) tpl_id(mem_startsWith, _T)(_haystack, _prefix)
#define mem_startsWith_useT$(_T...) \
    static fn_((tpl_id(mem_startsWith, _T)(Sli_const$(_T) haystack, Sli_const$(_T) prefix))(Opt$usize)) { \
        return mem_bytes_startsWith( \
            typeInfo$(_T), \
            Sli_from$(Sli_const$u8, as$((const u8*)(haystack.ptr)), haystack.len), \
            Sli_from$(Sli_const$u8, as$((const u8*)(prefix.ptr)), prefix.len) \
        ); \
    } \
//   Returns true if haystack ends with suffix
#define mem_endsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _suffix... /* Opt$usize */) tpl_id(mem_endsWith, _T)(_haystack, _suffix)
#define mem_endsWith_useT$(_T...) \
    static fn_((tpl_id(mem_endsWith, _T)(Sli_const$(_T) haystack, Sli_const$(_T) suffix))(Opt$usize)) { \
        return mem_bytes_endsWith( \
            typeInfo$(_T), \
            Sli_from$(Sli_const$u8, as$((const u8*)(haystack.ptr)), haystack.len), \
            Sli_from$(Sli_const$u8, as$((const u8*)(suffix.ptr)), suffix.len) \
        ); \
    }

static $inline_always fn_((mem_bytes_eql(TypeInfo type, Sli_const$u8 lhs, Sli_const$u8 rhs))(bool)) {
    debug_assert_nonnull(lhs.ptr);
    debug_assert_nonnull(rhs.ptr);
    if (lhs.len != rhs.len) { return false; }
    return mem_cmp(lhs.ptr, rhs.ptr, lhs.len * type.size) == 0;
}
static $inline_always fn_((mem_bytes_startsWith(TypeInfo type, Sli_const$u8 haystack, Sli_const$u8 prefix))(Opt$usize) $scope) {
    debug_assert_nonnull(haystack.ptr);
    debug_assert_nonnull(prefix.ptr);
    let haystack_bytes = haystack.len * type.size;
    let prefix_bytes   = prefix.len * type.size;
    if (haystack_bytes < prefix_bytes) { return_none(); }
    if (!mem_eqlBytes(haystack.ptr, prefix.ptr, prefix_bytes)) { return_none(); }
    return_some(prefix.len);
}
$unscoped;
static $inline_always fn_((mem_bytes_endsWith(TypeInfo type, Sli_const$u8 haystack, Sli_const$u8 suffix))(Opt$usize) $scope) {
    debug_assert_nonnull(haystack.ptr);
    debug_assert_nonnull(suffix.ptr);
    let haystack_bytes = haystack.len * type.size;
    let suffix_bytes   = suffix.len * type.size;
    if (haystack_bytes < suffix_bytes) { return_none(); }
    if (!mem_eqlBytes(haystack.ptr + haystack_bytes - suffix_bytes, suffix.ptr, suffix_bytes)) { return_none(); }
    return_some(haystack.len - suffix.len);
}
$unscoped;
#elif CASE == 2
// PREFIX/SUFFIX OPERATIONS
//   Returns true if haystack starts with prefix
#define mem_startsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _prefix... /* Opt$usize */) \
    tpl_id(mem_startsWith, _T)(_haystack, _prefix)
#define mem_startsWith_useT$(_T...) \
    static fn_((tpl_id(mem_startsWith, _T)(Sli_const$(_T) haystack, Sli_const$(_T) prefix))(Opt$usize)$scope) { \
        debug_assert_nonnull(haystack.ptr); \
        debug_assert_nonnull(prefix.ptr); \
        if (haystack.len < prefix.len) { return_none(); } \
        for (usize i = 0; i < prefix.len; ++i) { \
            if (Sli_getAt(haystack, i) == Sli_getAt(prefix, i)) { continue; } \
            return_none(); \
        } \
        return_some(prefix.len); \
    } \
    $unscoped; \
//   Returns true if haystack ends with suffix
#define mem_endsWith$(_T, /* Sli_const$T */ _haystack, /* Sli_const$T */ _suffix... /* Opt$usize */) \
    tpl_id(mem_endsWith, _T)(_haystack, _suffix)
#define mem_endsWith_useT$(_T...) \
    static fn_((tpl_id(mem_endsWith, _T)(Sli_const$(_T) haystack, Sli_const$(_T) suffix))(Opt$usize)$scope) { \
        debug_assert_nonnull(haystack.ptr); \
        debug_assert_nonnull(suffix.ptr); \
        if (haystack.len < suffix.len) { return_none(); } \
        for (usize i = 0; i < suffix.len; ++i) { \
            if (Sli_getAt(haystack, haystack.len - suffix.len + i) == Sli_getAt(suffix, i)) { continue; } \
            return_none(); \
        } \
        return_some(haystack.len - suffix.len); \
    } \
    $unscoped;
#endif // CASE == 1

#include "dh/main.h"

mem_startsWith_useT$(i32);
TEST_fn_("mem_startsWith: i32" $scope) {
    let haystack = Arr_ref$(Sli$i32, Arr_from$(i32, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));
    let prefix   = Arr_ref$(Sli$i32, Arr_from$(i32, { 1, 2, 3, 4 }));
    let result   = mem_startsWith_i32(haystack.as_const, prefix.as_const);
    try_(TEST_expect(isSome(result)));
    try_(TEST_expect(unwrap(result) == 4));
} $unscoped_(TEST_fn);

mem_endsWith_useT$(i32);
TEST_fn_("mem_endsWith: i32" $scope) {
    let haystack = Arr_ref$(Sli$i32, Arr_from$(i32, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }));
    let suffix   = Arr_ref$(Sli$i32, Arr_from$(i32, { 6, 7, 8, 9, 10 }));
    let result   = mem_endsWith_i32(haystack.as_const, suffix.as_const);
    try_(TEST_expect(isSome(result)));
    try_(TEST_expect(unwrap(result) == 5));
} $unscoped_(TEST_fn);
