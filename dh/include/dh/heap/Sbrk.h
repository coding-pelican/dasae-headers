/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Sbrk.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-19 (date of creation)
 * @updated 2026-03-08 (date of last update)
 * @ingroup dasae-headers(dh)/heap
 * @prefix  heap_Sbrk
 *
 * @brief   Sbrk-based memory allocation for linear memory growth
 * @details Provides a Sbrk-based memory allocation implementation
            with configurable size classes for memory optimization.
 */
#pragma once
#ifndef heap_Sbrk__included
#define heap_Sbrk__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "VMem.h"

/*========== Macros and Declarations ========================================*/

typedef variant_((heap_Sbrk_local_Ref) $T) heap_Sbrk_local_Ref;
typedef struct heap_Sbrk_local_Small heap_Sbrk_local_Small;
$attr($inline_always)
$static fn_((heap_Sbrk_local_Small_ref(heap_Sbrk_local_Small* self))(heap_Sbrk_local_Ref));
typedef struct heap_Sbrk_local_Medium heap_Sbrk_local_Medium;
$attr($inline_always)
$static fn_((heap_Sbrk_local_Medium_ref(heap_Sbrk_local_Medium* self))(heap_Sbrk_local_Ref));
typedef struct heap_Sbrk_local_Large heap_Sbrk_local_Large;
$attr($inline_always)
$static fn_((heap_Sbrk_local_Large_ref(heap_Sbrk_local_Large* self))(heap_Sbrk_local_Ref));

variant_((heap_Sbrk_local_Ref $fits($packed))(
    (heap_Sbrk_local_Ref_small, heap_Sbrk_local_Small*),
    (heap_Sbrk_local_Ref_medium, heap_Sbrk_local_Medium*),
    (heap_Sbrk_local_Ref_large, heap_Sbrk_local_Large*),
));
$attr($inline_always)
$static fn_((heap_Sbrk_local_Ref_frees(heap_Sbrk_local_Ref self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_local_Ref_big_frees(heap_Sbrk_local_Ref self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_local_Ref_next_addrs(heap_Sbrk_local_Ref self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_local_Ref_end_addrs(heap_Sbrk_local_Ref self))(S$usize));
$attr($inline_always)
$static fn_((heap_Sbrk_local_Ref_calcSelfSize(heap_Sbrk_local_Ref self))(usize));

typedef struct heap_Sbrk_Ctx {
    var_(inner, u_P$raw);
    fn_(((*sbrkFn)(u_P$raw inner, usize n))(usize));
} heap_Sbrk_Ctx;
T_use_P$(heap_Sbrk_Ctx);

/// SbrkAlctr with configurable size classes for memory optimization
///
/// Size variants:
/// - `heap_Sbrk` + `heap_Sbrk_local_Small`:  ~200 bytes (embedded systems, max 1MB pools)
/// - `heap_Sbrk` + `heap_Sbrk_local_Medium`: ~400 bytes (typical use, max 16MB pools)
/// - `heap_Sbrk` + `heap_Sbrk_local_Large`:  ~632 bytes (default, max 4GB pools)
typedef struct heap_Sbrk {
    var_(local_ref, heap_Sbrk_local_Ref);
    var_(ctx, P_const$heap_Sbrk_Ctx);
} heap_Sbrk;
$extern let_(heap_Sbrk_vtbl_local_small, mem_Alctr_VTbl);
$extern let_(heap_Sbrk_vtbl_local_medium, mem_Alctr_VTbl);
$extern let_(heap_Sbrk_vtbl_local_large, mem_Alctr_VTbl);
$extern fn_((heap_Sbrk_alctr(heap_Sbrk* self))(mem_Alctr));

$extern fn_((heap_Sbrk_from(heap_Sbrk_local_Ref local_ref, P_const$heap_Sbrk_Ctx ctx))(heap_Sbrk));

#if in_analysis || plat_is_windows
/// Windows VirtualAlloc context
typedef struct heap_Sbrk_Sys_Windows {
    var_(vmem, heap_VMem);
    var_(base_addr, usize);
    var_(reserved_size, usize);
    var_(committed_size, usize);
} heap_Sbrk_Sys_Windows;
T_use_E$($set(heap_VMem_E)(heap_Sbrk_Sys_Windows));
$extern fn_((heap_Sbrk_Sys_Windows_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Windows_ctx(heap_Sbrk_Sys_Windows* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Windows_from(heap_VMem vmem, usize reserve_size))(heap_Sbrk_Sys_Windows));
$attr($must_check)
$extern fn_((heap_Sbrk_Sys_Windows_init(usize reserve_size))(heap_VMem_E$heap_Sbrk_Sys_Windows));
$extern fn_((heap_Sbrk_Sys_Windows_fini(heap_Sbrk_Sys_Windows* self))(void));
#endif /* in_analysis || plat_is_windows */

#if in_analysis || plat_is_posix
/// POSIX mmap or sbrk context
typedef struct heap_Sbrk_Sys_Posix {
    var_(vmem, heap_VMem);
    var_(base_addr, usize);
    var_(reserved_size, usize);
    var_(committed_size, usize);
} heap_Sbrk_Sys_Posix;
T_use_E$($set(heap_VMem_E)(heap_Sbrk_Sys_Posix));
$extern fn_((heap_Sbrk_Sys_Posix_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Posix_ctx(heap_Sbrk_Sys_Posix* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Posix_from(heap_VMem vmem, usize reserve_size))(heap_Sbrk_Sys_Posix));
$attr($must_check)
$extern fn_((heap_Sbrk_Sys_Posix_init(usize reserve_size))(heap_VMem_E$heap_Sbrk_Sys_Posix));
$extern fn_((heap_Sbrk_Sys_Posix_fini(heap_Sbrk_Sys_Posix* self))(void));
#endif /* in_analysis || plat_is_posix */

#if in_analysis || plat_is_wasi
/// WASM memory.grow context
typedef struct heap_Sbrk_Sys_Wasm {
    var_(page_count, usize); // Current page count
} heap_Sbrk_Sys_Wasm;
$extern fn_((heap_Sbrk_Sys_Wasm_sbrk(u_P$raw ctx, usize n))(usize));
$extern fn_((heap_Sbrk_Sys_Wasm_ctx(heap_Sbrk_Sys_Wasm* self))(heap_Sbrk_Ctx));
$extern fn_((heap_Sbrk_Sys_Wasm_init(void))(heap_Sbrk_Sys_Wasm));
$extern fn_((heap_Sbrk_Sys_Wasm_fini(heap_Sbrk_Sys_Wasm* self))(void));
#endif /* in_analysis || plat_is_wasi */

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

/*---------- Internal Declarations ------------------------------------------*/

/*--- heap_Sbrk_local_Ref ---*/
#define heap_Sbrk_local_Ref__enum_small 0
#define heap_Sbrk_local_Ref__enum_medium 1
#define heap_Sbrk_local_Ref__enum_large 2
#define __pp__heap_Sbrk_local_Ref__expand(...) __VA_ARGS__
#define __pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok...) pp_if_(pp_Tok_hasCmp(_enum_tok))( \
    pp_then_(__pp__heap_Sbrk_local_Ref__expand( \
        pp_switch_ pp_begin(_enum_tok)( \
            pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Ref__enum_small)), \
            pp_case_((heap_Sbrk_local_Ref_small)(heap_Sbrk_local_Ref__enum_small)), \
            pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Ref__enum_medium)), \
            pp_case_((heap_Sbrk_local_Ref_medium)(heap_Sbrk_local_Ref__enum_medium)), \
            pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Ref__enum_large)), \
            pp_case_((heap_Sbrk_local_Ref_large)(heap_Sbrk_local_Ref__enum_large)) \
        ) pp_end \
    )), \
    pp_else_(pp_join($, __pp__heap_Sbrk_local_Ref__enum_fromDirectTok, _enum_tok)) \
)
#define __pp__heap_Sbrk_local_Ref__enum_fromDirectTok$heap_Sbrk_local_Ref_small heap_Sbrk_local_Ref__enum_small
#define __pp__heap_Sbrk_local_Ref__enum_fromDirectTok$heap_Sbrk_local_Ref_medium heap_Sbrk_local_Ref__enum_medium
#define __pp__heap_Sbrk_local_Ref__enum_fromDirectTok$heap_Sbrk_local_Ref_large heap_Sbrk_local_Ref__enum_large
#define pp_Tok_cmp$heap_Sbrk_local_Ref_small(_x) _x
#define pp_Tok_cmp$heap_Sbrk_local_Ref_medium(_x) _x
#define pp_Tok_cmp$heap_Sbrk_local_Ref_large(_x) _x

/*--- heap_Sbrk__bigpage_size ---*/
enum pp_if_(abi_size_is_16bit)(
    pp_then_({
        heap_Sbrk_local_Small__bigpage_size = u16_(4) * 1024,
        heap_Sbrk_local_Medium__bigpage_size = u16_(8) * 1024,
        heap_Sbrk_local_Large__bigpage_size = u16_(16) * 1024,
    }),
    pp_else_({
        heap_Sbrk_local_Small__bigpage_size = u16_(16) * 1024,
        heap_Sbrk_local_Medium__bigpage_size = u32_(64) * 1024,
        heap_Sbrk_local_Large__bigpage_size = u32_(64) * 1024,
    })
);
#define __pp__heap_Sbrk__bigpage_size_static__expand(...) __VA_ARGS__
#define heap_Sbrk__bigpage_size_static(_enum_tok...) usize_(__pp__heap_Sbrk__bigpage_size_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__bigpage_size)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__bigpage_size)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__bigpage_size)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__bigpage_size(heap_Sbrk self))(usize));

/*--- heap_Sbrk__max_pool_size ---*/
enum pp_if_(abi_size_is_16bit)(
    pp_then_({
        heap_Sbrk_local_Small__max_pool_size = u16_(16) * 1024,
        heap_Sbrk_local_Medium__max_pool_size = u16_(32) * 1024,
        heap_Sbrk_local_Large__max_pool_size = u16_(32) * 1024,
    }),
    pp_else_({
        heap_Sbrk_local_Small__max_pool_size = u32_(1) * 1024 * 1024,
        heap_Sbrk_local_Medium__max_pool_size = u32_(16) * 1024 * 1024,
        heap_Sbrk_local_Large__max_pool_size = usize_limit_max,
    })
);
#define __pp__heap_Sbrk__max_pool_size_static__expand(...) __VA_ARGS__
#define heap_Sbrk__max_pool_size_static(_enum_tok...) usize_(__pp__heap_Sbrk__max_pool_size_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__max_pool_size)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__max_pool_size)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__max_pool_size)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__max_pool_size(heap_Sbrk self))(usize));

/*--- heap_Sbrk__pages_per_bigpage ---*/
#define heap_Sbrk_local__pages_per_bigpage_static(_enum_tok...) \
    (heap_Sbrk__bigpage_size_static(_enum_tok) / heap_page_size)
enum {
    heap_Sbrk_local_Small__pages_per_bigpage = heap_Sbrk_local__pages_per_bigpage_static(heap_Sbrk_local_Ref_small),
    heap_Sbrk_local_Medium__pages_per_bigpage = heap_Sbrk_local__pages_per_bigpage_static(heap_Sbrk_local_Ref_medium),
    heap_Sbrk_local_Large__pages_per_bigpage = heap_Sbrk_local__pages_per_bigpage_static(heap_Sbrk_local_Ref_large)
};
#define __pp__heap_Sbrk__pages_per_bigpage_static__expand(...) __VA_ARGS__
#define heap_Sbrk__pages_per_bigpage_static(_enum_tok...) usize_(__pp__heap_Sbrk__pages_per_bigpage_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__pages_per_bigpage)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__pages_per_bigpage)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__pages_per_bigpage)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__pages_per_bigpage(heap_Sbrk self))(usize));

/*--- heap_Sbrk__bigpage_count ---*/
#define heap_Sbrk_local__bigpage_count_static(_enum_tok...) \
    (usize_(heap_Sbrk__max_pool_size_static(_enum_tok)) / usize_(heap_Sbrk__bigpage_size_static(_enum_tok)))
enum {
    heap_Sbrk_local_Small__bigpage_count = heap_Sbrk_local__bigpage_count_static(heap_Sbrk_local_Ref_small),
    heap_Sbrk_local_Medium__bigpage_count = heap_Sbrk_local__bigpage_count_static(heap_Sbrk_local_Ref_medium),
    heap_Sbrk_local_Large__bigpage_count = heap_Sbrk_local__bigpage_count_static(heap_Sbrk_local_Ref_large)
};
#define __pp__heap_Sbrk__bigpage_count_static__expand(...) __VA_ARGS__
#define heap_Sbrk__bigpage_count_static(_enum_tok...) usize_(__pp__heap_Sbrk__bigpage_count_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__bigpage_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__bigpage_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__bigpage_count)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__bigpage_count(heap_Sbrk self))(usize));

/*--- heap_Sbrk__min_size_class ---*/
enum {
    heap_Sbrk__min_size_class__pow2ceil_1_plus_size_of_usize = uint_pow2Ceil_static$((usize)(1 + sizeOf$(usize))),
    heap_Sbrk__min_size_class = uint_log2_static(heap_Sbrk__min_size_class__pow2ceil_1_plus_size_of_usize),
};

/*--- heap_Sbrk__size_class_count ---*/
#define heap_Sbrk_local__size_class_count_static(_enum_tok...) \
    usize_(uint_log2_static(heap_Sbrk__bigpage_size_static(_enum_tok)) - usize_(heap_Sbrk__min_size_class))
enum {
    heap_Sbrk_local_Small__size_class_count = heap_Sbrk_local__size_class_count_static(heap_Sbrk_local_Ref_small),
    heap_Sbrk_local_Medium__size_class_count = heap_Sbrk_local__size_class_count_static(heap_Sbrk_local_Ref_medium),
    heap_Sbrk_local_Large__size_class_count = heap_Sbrk_local__size_class_count_static(heap_Sbrk_local_Ref_large)
};
#define __pp__heap_Sbrk__size_class_count_static__expand(...) __VA_ARGS__
#define heap_Sbrk__size_class_count_static(_enum_tok...) usize_(__pp__heap_Sbrk__size_class_count_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__size_class_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__size_class_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__size_class_count)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__size_class_count(heap_Sbrk self))(usize));


/*--- heap_Sbrk__big_size_class_count ---*/
#define heap_Sbrk_local__big_size_class_count_static(_enum_tok...) \
    usize_(uint_log2_static(heap_Sbrk__bigpage_count_static(_enum_tok)) + 1)
enum {
    heap_Sbrk_local_Small__big_size_class_count = heap_Sbrk_local__big_size_class_count_static(heap_Sbrk_local_Ref_small),
    heap_Sbrk_local_Medium__big_size_class_count = heap_Sbrk_local__big_size_class_count_static(heap_Sbrk_local_Ref_medium),
    heap_Sbrk_local_Large__big_size_class_count = heap_Sbrk_local__big_size_class_count_static(heap_Sbrk_local_Ref_large)
};
#define __pp__heap_Sbrk__big_size_class_count_static__expand(...) __VA_ARGS__
#define heap_Sbrk__big_size_class_count_static(_enum_tok...) usize_(__pp__heap_Sbrk__big_size_class_count_static__expand( \
    pp_switch_ pp_begin(__pp__heap_Sbrk_local_Ref__enum_fromTok(_enum_tok))( \
        pp_case_((heap_Sbrk_local_Ref__enum_small)(heap_Sbrk_local_Small__big_size_class_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_medium)(heap_Sbrk_local_Medium__big_size_class_count)), \
        pp_case_((heap_Sbrk_local_Ref__enum_large)(heap_Sbrk_local_Large__big_size_class_count)) \
    ) pp_end \
))
$attr($inline_always)
$static fn_((heap_Sbrk__big_size_class_count(heap_Sbrk self))(usize));

/*---------- Internal Definitions -------------------------------------------*/

/*--- heap_Sbrk_local_Small ---*/
struct heap_Sbrk_local_Small {
    var_(frees, A$$(heap_Sbrk_local_Small__size_class_count, usize));
    var_(big_frees, A$$(heap_Sbrk_local_Small__big_size_class_count, usize));
    var_(next_addrs, A$$(heap_Sbrk_local_Small__size_class_count, usize));
    var_(end_addrs, A$$(heap_Sbrk_local_Small__size_class_count, usize));
};
fn_((heap_Sbrk_local_Small_ref(heap_Sbrk_local_Small* self))(heap_Sbrk_local_Ref) $scope) {
    return_(union_of((heap_Sbrk_local_Ref_small)(self)));
} $unscoped(fn);

/*--- heap_Sbrk_local_Medium ---*/
struct heap_Sbrk_local_Medium {
    var_(frees, A$$(heap_Sbrk_local_Medium__size_class_count, usize));
    var_(big_frees, A$$(heap_Sbrk_local_Medium__big_size_class_count, usize));
    var_(next_addrs, A$$(heap_Sbrk_local_Medium__size_class_count, usize));
    var_(end_addrs, A$$(heap_Sbrk_local_Medium__size_class_count, usize));
};
fn_((heap_Sbrk_local_Medium_ref(heap_Sbrk_local_Medium* self))(heap_Sbrk_local_Ref) $scope) {
    return_(union_of((heap_Sbrk_local_Ref_medium)(self)));
} $unscoped(fn);

/*--- heap_Sbrk_local_Large ---*/
struct heap_Sbrk_local_Large {
    var_(frees, A$$(heap_Sbrk_local_Large__size_class_count, usize));
    var_(big_frees, A$$(heap_Sbrk_local_Large__big_size_class_count, usize));
    var_(next_addrs, A$$(heap_Sbrk_local_Large__size_class_count, usize));
    var_(end_addrs, A$$(heap_Sbrk_local_Large__size_class_count, usize));
};
fn_((heap_Sbrk_local_Large_ref(heap_Sbrk_local_Large* self))(heap_Sbrk_local_Ref) $scope) {
    return_(union_of((heap_Sbrk_local_Ref_large)(self)));
} $unscoped(fn);

#if in_analysis_active_only || in_comptime
fn_((heap_Sbrk_local_Ref_frees(heap_Sbrk_local_Ref self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self) {
        patts_((
            heap_Sbrk_local_Ref_small,
            heap_Sbrk_local_Ref_medium,
            heap_Sbrk_local_Ref_large
        )(local)$break_(A_ref$((S$usize)(local->frees))));
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk_local_Ref_big_frees(heap_Sbrk_local_Ref self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self) {
        patts_((
            heap_Sbrk_local_Ref_small,
            heap_Sbrk_local_Ref_medium,
            heap_Sbrk_local_Ref_large
        )(local)$break_(A_ref$((S$usize)(local->big_frees))));
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk_local_Ref_next_addrs(heap_Sbrk_local_Ref self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self) {
        patts_((
            heap_Sbrk_local_Ref_small,
            heap_Sbrk_local_Ref_medium,
            heap_Sbrk_local_Ref_large
        )(local)$break_(A_ref$((S$usize)(local->next_addrs))));
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk_local_Ref_end_addrs(heap_Sbrk_local_Ref self))(S$usize)) {
    return expr_(S$usize $scope)(match_(self) {
        patts_((
            heap_Sbrk_local_Ref_small,
            heap_Sbrk_local_Ref_medium,
            heap_Sbrk_local_Ref_large
        )(local)$break_(A_ref$((S$usize)(local->end_addrs))));
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk_local_Ref_calcSelfSize(heap_Sbrk_local_Ref self))(usize)) {
    return expr_(usize $scope)(match_(self) {
        case_((heap_Sbrk_local_Ref_small)) $break_(sizeOf$(heap_Sbrk_local_Small)) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(sizeOf$(heap_Sbrk_local_Medium)) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(sizeOf$(heap_Sbrk_local_Large)) $end(case);
    } $end(match)) $unscoped(expr);
};

fn_((heap_Sbrk__bigpage_size(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_local_Ref_small)) $break_(heap_Sbrk_local_Small__bigpage_size) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(heap_Sbrk_local_Medium__bigpage_size) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(heap_Sbrk_local_Large__bigpage_size) $end(case);
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk__max_pool_size(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_local_Ref_small)) $break_(heap_Sbrk_local_Small__max_pool_size) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(heap_Sbrk_local_Medium__max_pool_size) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(heap_Sbrk_local_Large__max_pool_size) $end(case);
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk__pages_per_bigpage(heap_Sbrk self))(usize)) {
    return heap_Sbrk__bigpage_size(self) / (heap_page_size);
};
fn_((heap_Sbrk__bigpage_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_local_Ref_small)) $break_(heap_Sbrk_local_Small__bigpage_count) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(heap_Sbrk_local_Medium__bigpage_count) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(heap_Sbrk_local_Large__bigpage_count) $end(case);
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk__size_class_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_local_Ref_small)) $break_(heap_Sbrk_local_Small__size_class_count) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(heap_Sbrk_local_Medium__size_class_count) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(heap_Sbrk_local_Large__size_class_count) $end(case);
    } $end(match)) $unscoped(expr);
};
fn_((heap_Sbrk__big_size_class_count(heap_Sbrk self))(usize)) {
    return expr_(usize $scope)(match_(self.local_ref) {
        case_((heap_Sbrk_local_Ref_small)) $break_(heap_Sbrk_local_Small__big_size_class_count) $end(case);
        case_((heap_Sbrk_local_Ref_medium)) $break_(heap_Sbrk_local_Medium__big_size_class_count) $end(case);
        case_((heap_Sbrk_local_Ref_large)) $break_(heap_Sbrk_local_Large__big_size_class_count) $end(case);
    } $end(match)) $unscoped(expr);
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_Sbrk__included */
