/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sbrk.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-19 (date of creation)
 * @updated 2026-01-21 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Sbrk
 *
 * @brief   Sbrk-based memory allocation for linear memory growth
 * @details Provides a Sbrk-based memory allocation implementation
            with configurable size classes for memory optimization.
 */
#ifndef heap_Sbrk__included
#define heap_Sbrk__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/heap/cfg.h"

/*========== Macros and Declarations ========================================*/

typedef variant_((heap_Sbrk_LocalRef) $T) heap_Sbrk_LocalRef;
typedef struct heap_Sbrk_LocalSmall heap_Sbrk_LocalSmall;
$attr($inline_always)
$static fn_((heap_Sbrk_LocalSmall_ref(heap_Sbrk_LocalSmall* self))(heap_Sbrk_LocalRef));
typedef struct heap_Sbrk_LocalMedium heap_Sbrk_LocalMedium;
$attr($inline_always)
$static fn_((heap_Sbrk_LocalMedium_ref(heap_Sbrk_LocalMedium* self))(heap_Sbrk_LocalRef));
typedef struct heap_Sbrk_LocalLarge heap_Sbrk_LocalLarge;
$attr($inline_always)
$static fn_((heap_Sbrk_LocalLarge_ref(heap_Sbrk_LocalLarge* self))(heap_Sbrk_LocalRef));

variant_((heap_Sbrk_LocalRef $bits(8))(
    (heap_Sbrk_LocalRef_small, heap_Sbrk_LocalSmall*),
    (heap_Sbrk_LocalRef_medium, heap_Sbrk_LocalMedium*),
    (heap_Sbrk_LocalRef_large, heap_Sbrk_LocalLarge*),
));
$attr($inline_always)
$static fn_((heap_Sbrk_LocalRef_frees(heap_Sbrk_LocalRef self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_LocalRef_big_frees(heap_Sbrk_LocalRef self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_LocalRef_next_addrs(heap_Sbrk_LocalRef self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_LocalRef_calcSelfSize(heap_Sbrk_LocalRef self))(usize));

typedef struct heap_Sbrk_Ctx {
    var_(inner, u_P$raw);
    fn_(((*sbrkFn)(u_P$raw inner, usize n))(usize));
} heap_Sbrk_Ctx;
T_use_P$(heap_Sbrk_Ctx);

/// SbrkAllocator with configurable size classes for memory optimization
///
/// Size variants:
/// - `heap_Sbrk` + `heap_Sbrk_LocalSmall`:  ~200 bytes (embedded systems, max 1MB pools)
/// - `heap_Sbrk` + `heap_Sbrk_LocalMedium`: ~400 bytes (typical use, max 16MB pools)
/// - `heap_Sbrk` + `heap_Sbrk_LocalLarge`:  ~632 bytes (default, max 4GB pools)
typedef struct heap_Sbrk {
    var_(local_ref, heap_Sbrk_LocalRef);
    var_(ctx, P_const$heap_Sbrk_Ctx);
} heap_Sbrk;
$extern let_(heap_Sbrk_vt_local_small, mem_Allocator_VT);
$extern let_(heap_Sbrk_vt_local_medium, mem_Allocator_VT);
$extern let_(heap_Sbrk_vt_local_large, mem_Allocator_VT);
$extern fn_((heap_Sbrk_allocator(heap_Sbrk* self))(mem_Allocator));
$extern fn_((heap_Sbrk_from(heap_Sbrk_LocalRef local_ref, P_const$heap_Sbrk_Ctx ctx))(heap_Sbrk));

#if plat_is_windows
/// Windows VirtualAlloc context
typedef struct heap_Sbrk_Sys_Windows {
    var_(base_addr, usize);
    var_(reserved_size, usize);
    var_(committed_size, usize);
} heap_Sbrk_Sys_Windows;
$extern fn_((heap_Sbrk_Sys_Windows_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Windows_ctx(heap_Sbrk_Sys_Windows* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Windows_init(usize reserve_size))(heap_Sbrk_Sys_Windows));
$extern fn_((heap_Sbrk_Sys_Windows_fini(heap_Sbrk_Sys_Windows* self))(void));
#endif /* plat_is_windows */

#if plat_is_posix
/// POSIX mmap or sbrk context
typedef struct heap_Sbrk_Sys_Posix {
    var_(base_addr, usize);
    var_(mapped_size, usize);
} heap_Sbrk_Sys_Posix;
$extern fn_((heap_Sbrk_Sys_Posix_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Posix_ctx(heap_Sbrk_Sys_Posix* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Posix_init(void))(heap_Sbrk_Sys_Posix));
$extern fn_((heap_Sbrk_Sys_Posix_fini(heap_Sbrk_Sys_Posix* self))(void));
#endif /* plat_is_posix */

#if plat_is_wasi
/// WASM memory.grow context
typedef struct heap_Sbrk_Sys_Wasm {
    var_(page_count, usize); // Current page count
} heap_Sbrk_Sys_Wasm;
$extern fn_((heap_Sbrk_Sys_Wasm_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Wasm_ctx(heap_Sbrk_Sys_Wasm* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Wasm_init(void))(heap_Sbrk_Sys_Wasm));
$extern fn_((heap_Sbrk_Sys_Wasm_fini(heap_Sbrk_Sys_Wasm* self))(void));
#endif /* plat_is_wasi */

/// Fixed-size buffer context for Sbrk allocator
/// Usage: Embedded systems, stack-allocated arenas, static buffers
typedef struct heap_Fixed heap_Sbrk_Fixed;
$extern fn_((heap_Sbrk_Fixed_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Fixed_ctx(heap_Sbrk_Fixed* self))(heap_Sbrk_Ctx));

/// Arena/bump allocator context for Sbrk
/// Usage: When you already have a linear allocator and want to add freelists
typedef struct heap_Arena heap_Sbrk_Arena;
$extern fn_((heap_Sbrk_Arena_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Arena_ctx(heap_Sbrk_Arena* self))(heap_Sbrk_Ctx));

/*========== Macros and Definitions =========================================*/

#define heap_Sbrk_LocalRef__pp_enum_small 0
#define heap_Sbrk_LocalRef__pp_enum_medium 1
#define heap_Sbrk_LocalRef__pp_enum_large 2
#define ____heap_Sbrk_LocalRef__pp_enum_expand(...) __VA_ARGS__
#define heap_Sbrk_LocalRef__pp_enum_parse(_enum_tok...) pp_if_(pp_Tok_isComparable(_enum_tok))( \
    pp_then_(____heap_Sbrk_LocalRef__pp_enum_expand( \
        pp_switch_ pp_begin(_enum_tok)( \
            pp_case_((heap_Sbrk_LocalRef__pp_enum_small)(heap_Sbrk_LocalRef__pp_enum_small)), \
            pp_case_((heap_Sbrk_LocalRef_small)(heap_Sbrk_LocalRef__pp_enum_small)), \
            pp_case_((heap_Sbrk_LocalRef__pp_enum_medium)(heap_Sbrk_LocalRef__pp_enum_medium)), \
            pp_case_((heap_Sbrk_LocalRef_medium)(heap_Sbrk_LocalRef__pp_enum_medium)), \
            pp_case_((heap_Sbrk_LocalRef__pp_enum_large)(heap_Sbrk_LocalRef__pp_enum_large)), \
            pp_case_((heap_Sbrk_LocalRef_large)(heap_Sbrk_LocalRef__pp_enum_large)) \
        ) pp_end \
    )), \
    pp_else_(pp_join($, ____heap_Sbrk_LocalRef__pp_enum, _enum_tok)) \
)
#define ____heap_Sbrk_LocalRef__pp_enum$heap_Sbrk_LocalRef_small heap_Sbrk_LocalRef__pp_enum_small
#define ____heap_Sbrk_LocalRef__pp_enum$heap_Sbrk_LocalRef_medium heap_Sbrk_LocalRef__pp_enum_medium
#define ____heap_Sbrk_LocalRef__pp_enum$heap_Sbrk_LocalRef_large heap_Sbrk_LocalRef__pp_enum_large
#define pp_Tok_cmp$heap_Sbrk_LocalRef_small(_x) _x
#define pp_Tok_cmp$heap_Sbrk_LocalRef_medium(_x) _x
#define pp_Tok_cmp$heap_Sbrk_LocalRef_large(_x) _x

#define ____heap_Sbrk__bigpage_size_static__expand(...) __VA_ARGS__
#define heap_Sbrk__bigpage_size_static(_enum_tok...) ____heap_Sbrk__bigpage_size_static__expand( \
    pp_switch_ pp_begin(heap_Sbrk_LocalRef__pp_enum_parse(_enum_tok))( \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_small)(16ull * 1024)), \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_medium)(64ull * 1024)), \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_large)(64ull * 1024)) \
    ) pp_end \
)
$attr($inline_always)
$static fn_((heap_Sbrk__bigpage_size(heap_Sbrk self))(usize));

#define ____heap_Sbrk__max_pool_size_static__expand(...) __VA_ARGS__
#define heap_Sbrk__max_pool_size_static(_enum_tok...) ____heap_Sbrk__max_pool_size_static__expand( \
    pp_switch_ pp_begin(heap_Sbrk_LocalRef__pp_enum_parse(_enum_tok))( \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_small)(1ull * 1024 * 1024)), \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_medium)(16ull * 1024 * 1024)), \
        pp_case_((heap_Sbrk_LocalRef__pp_enum_large)(usize_limit_max)) \
    ) pp_end \
)
$attr($inline_always)
$static fn_((heap_Sbrk__max_pool_size(heap_Sbrk self))(usize));

#define heap_Sbrk__pages_per_bigpage_static(_enum_tok...) \
    (heap_Sbrk__bigpage_size_static(_enum_tok) / heap_page_size)
$attr($inline_always)
$static fn_((heap_Sbrk__pages_per_bigpage(heap_Sbrk self))(usize));

#define heap_Sbrk__bigpage_count_static(_enum_tok...) \
    (heap_Sbrk__max_pool_size_static(_enum_tok) / heap_Sbrk__bigpage_size_static(_enum_tok))
$attr($inline_always)
$static fn_((heap_Sbrk__bigpage_count(heap_Sbrk self))(usize));

#define heap_Sbrk__min_class \
    (uint_log2_static(uint_exp2_static$((usize)(1 + sizeOf$(usize)))))

#define heap_Sbrk__size_class_count_static(_enum_tok...) \
    (uint_log2_static(heap_Sbrk__bigpage_size_static(_enum_tok)) - heap_Sbrk__min_class)
$attr($inline_always)
$static fn_((heap_Sbrk__size_class_count(heap_Sbrk self))(usize));

#define heap_Sbrk__big_size_class_count_static(_enum_tok...) \
    (uint_log2_static(heap_Sbrk__bigpage_count_static(_enum_tok)))
$attr($inline_always)
$static fn_((heap_Sbrk__big_size_class_count(heap_Sbrk self))(usize));

struct heap_Sbrk_LocalSmall {
    var_(frees, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_small), usize));
    var_(big_frees, A$$(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_small), usize));
    var_(next_addrs, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_small), usize));
};
fn_((heap_Sbrk_LocalSmall_ref(heap_Sbrk_LocalSmall* self))(heap_Sbrk_LocalRef) $scope) {
    return_(union_of((heap_Sbrk_LocalRef_small)(self)));
} $unscoped_(fn);

struct heap_Sbrk_LocalMedium {
    var_(frees, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_medium), usize));
    var_(big_frees, A$$(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_medium), usize));
    var_(next_addrs, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_medium), usize));
};
fn_((heap_Sbrk_LocalMedium_ref(heap_Sbrk_LocalMedium* self))(heap_Sbrk_LocalRef) $scope) {
    return_(union_of((heap_Sbrk_LocalRef_medium)(self)));
} $unscoped_(fn);

struct heap_Sbrk_LocalLarge {
    var_(frees, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_large), usize));
    var_(big_frees, A$$(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_large), usize));
    var_(next_addrs, A$$(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_large), usize));
};
fn_((heap_Sbrk_LocalLarge_ref(heap_Sbrk_LocalLarge* self))(heap_Sbrk_LocalRef) $scope) {
    return_(union_of((heap_Sbrk_LocalRef_large)(self)));
} $unscoped_(fn);

fn_((heap_Sbrk_LocalRef_frees(heap_Sbrk_LocalRef self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self){
        pattern_((heap_Sbrk_LocalRef_small)(local))  $break_(A_ref$((S$usize)((*local)->frees))) $end(case);
        pattern_((heap_Sbrk_LocalRef_medium)(local)) $break_(A_ref$((S$usize)((*local)->frees))) $end(case);
        pattern_((heap_Sbrk_LocalRef_large)(local)) $break_(A_ref$((S$usize)((*local)->frees))) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk_LocalRef_big_frees(heap_Sbrk_LocalRef self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self){
        pattern_((heap_Sbrk_LocalRef_small)(local))  $break_(A_ref$((S$usize)((*local)->big_frees))) $end(case);
        pattern_((heap_Sbrk_LocalRef_medium)(local)) $break_(A_ref$((S$usize)((*local)->big_frees))) $end(case);
        pattern_((heap_Sbrk_LocalRef_large)(local)) $break_(A_ref$((S$usize)((*local)->big_frees))) $end(case);
    } $end(match)) $unscoped_(expr);
};
fn_((heap_Sbrk_LocalRef_next_addrs(heap_Sbrk_LocalRef self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self){
        pattern_((heap_Sbrk_LocalRef_small)(local))  $break_(A_ref$((S$usize)((*local)->next_addrs))) $end(case);
        pattern_((heap_Sbrk_LocalRef_medium)(local)) $break_(A_ref$((S$usize)((*local)->next_addrs))) $end(case);
        pattern_((heap_Sbrk_LocalRef_large)(local)) $break_(A_ref$((S$usize)((*local)->next_addrs))) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk_LocalRef_calcSelfSize(heap_Sbrk_LocalRef self))(usize)) {
    return expr_(usize $scope)(match_(self) {
        case_((heap_Sbrk_LocalRef_small))  $break_(sizeOf$(heap_Sbrk_LocalSmall)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(sizeOf$(heap_Sbrk_LocalMedium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(sizeOf$(heap_Sbrk_LocalLarge)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__bigpage_size(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__bigpage_size_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__bigpage_size_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__bigpage_size_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__max_pool_size(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__max_pool_size_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__max_pool_size_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__max_pool_size_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__pages_per_bigpage(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__pages_per_bigpage_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__pages_per_bigpage_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__pages_per_bigpage_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__bigpage_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__bigpage_count_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__bigpage_count_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__bigpage_count_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__size_class_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__size_class_count_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

fn_((heap_Sbrk__big_size_class_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_small)) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_medium)) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(heap_Sbrk__big_size_class_count_static(heap_Sbrk_LocalRef_large)) $end(case);
    } $end(match)) $unscoped_(expr);
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Sbrk__included */
