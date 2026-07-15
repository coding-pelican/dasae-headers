#pragma once
#ifndef heap_common__included
#define heap_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/mem/common.h"

/*========== Macros and Declarations ========================================*/

typedef struct heap_Geom {
    var_(page_size, usize);
    var_(reserve_align, usize);
    var_(commit_align, usize);
    var_(map_align, usize);
} heap_Geom;

$attr($inline_always)
$static fn_((heap_Geom_isValid(heap_Geom self))(bool));
$attr($inline_always)
$static fn_((heap_Geom_assertValid(
    usize page_size,
    usize reserve_align,
    usize commit_align,
    usize map_align
))(void));
$attr($inline_always)
$static fn_((heap_Geom_ensureValid(heap_Geom self))(heap_Geom));

$attr($inline_always)
$static fn_((heap_Geom_default(void))(heap_Geom));
$attr($inline_always)
$static fn_((heap_Geom_from(usize page_size))(heap_Geom));
$attr($inline_always)
$static fn_((heap_Geom_alignPageWith(heap_Geom self, usize size))(usize));
$attr($inline_always)
$static fn_((heap_Geom_alignReserveWith(heap_Geom self, usize size))(usize));
$attr($inline_always)
$static fn_((heap_Geom_alignCommitWith(heap_Geom self, usize size))(usize));

$attr($inline_always)
$static fn_((heap_alignPage(usize size))(usize));
$attr($inline_always)
$static fn_((heap_alignReserve(usize size))(usize));
$attr($inline_always)
$static fn_((heap_alignCommit(usize size))(usize));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((heap_Geom_isValid(heap_Geom self))(bool)) {
    return mem_isValidAlign(self.page_size)
        && mem_isValidAlign(self.reserve_align)
        && mem_isValidAlign(self.commit_align)
        && mem_isValidAlign(self.map_align)
        && self.page_size <= self.reserve_align
        && self.page_size <= self.commit_align
        && self.page_size <= self.map_align;
};

fn_((heap_Geom_assertValid(
    usize page_size,
    usize reserve_align,
    usize commit_align,
    usize map_align
))(void)) {
    claim_assert(mem_isValidAlign(page_size));
    claim_assert(mem_isValidAlign(reserve_align));
    claim_assert(mem_isValidAlign(commit_align));
    claim_assert(mem_isValidAlign(map_align));
    claim_assert(page_size <= reserve_align);
    claim_assert(page_size <= commit_align);
    claim_assert(page_size <= map_align);
};

fn_((heap_Geom_ensureValid(heap_Geom self))(heap_Geom)) {
    heap_Geom_assertValid(
        self.page_size,
        self.reserve_align,
        self.commit_align,
        self.map_align
    );
    return self;
};

fn_((heap_Geom_default(void))(heap_Geom)) {
    return heap_Geom_from(heap_page_size);
};

fn_((heap_Geom_from(usize page_size))(heap_Geom)) {
    claim_assert(mem_isValidAlign(page_size));
    return heap_Geom_ensureValid((heap_Geom){
        .page_size = page_size,
        .reserve_align = page_size,
        .commit_align = page_size,
        .map_align = page_size,
    });
};

fn_((heap_Geom_alignPageWith(heap_Geom self, usize size))(usize)) {
    self = heap_Geom_ensureValid(self);
    return mem_alignFwd(size != 0 ? size : self.page_size, self.page_size);
};

fn_((heap_Geom_alignReserveWith(heap_Geom self, usize size))(usize)) {
    self = heap_Geom_ensureValid(self);
    return mem_alignFwd(size != 0 ? size : self.reserve_align, self.reserve_align);
};

fn_((heap_Geom_alignCommitWith(heap_Geom self, usize size))(usize)) {
    self = heap_Geom_ensureValid(self);
    return mem_alignFwd(size != 0 ? size : self.commit_align, self.commit_align);
};

fn_((heap_alignPage(usize size))(usize)) {
    return heap_Geom_alignPageWith(heap_Geom_default(), size);
};

fn_((heap_alignReserve(usize size))(usize)) {
    return heap_Geom_alignReserveWith(heap_Geom_default(), size);
};

fn_((heap_alignCommit(usize size))(usize)) {
    return heap_Geom_alignCommitWith(heap_Geom_default(), size);
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* heap_common__included */
