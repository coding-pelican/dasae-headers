/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    std.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-08-02 (date of creation)
 * @updated 2026-08-02 (date of last update)
 * @ingroup dasae-headers(dh)/fs/File
 * @prefix  fs_File_std
 *
 * @brief   Injectable standard file capability
 */
#pragma once
#ifndef fs_File_std__included
#define fs_File_std__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"

/*========== Macros and Declarations ========================================*/

errset_((fs_File_std_direct_E)(fs_File_std_direct_Unsupported));

T_alias$((fs_File_std_Self_VTbl)(struct fs_File_std_Self_VTbl));
$extern let_(fs_File_std_VTbl_failing, fs_File_std_Self_VTbl);

struct fs_File_std_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(fs_File_std_Self_VTbl));
};
T_use_prl$(fs_File_std_Self);
T_use_E$($set(fs_File_std_direct_E)(fs_File_std_Self));
$attr($inline_always $must_check)
$static fn_((fs_File_std_isValid(fs_File_std_Self self))(bool));
$attr($inline_always)
$static fn_((fs_File_std_assertValid(P$raw ctx, P_const$$(fs_File_std_Self_VTbl) vtbl))(void));
$attr($inline_always $must_check)
$static fn_((fs_File_std_ensureValid(fs_File_std_Self self))(fs_File_std_Self));

$extern let_(fs_File_std_failing, fs_File_std_Self);
$attr($must_check)
$extern fn_((fs_File_std_direct(void))(fs_File_std_direct_E$fs_File_std_Self));

$attr($must_check)
$extern fn_((fs_File_std_in(fs_File_std_Self self))(fs_File));
$attr($must_check)
$extern fn_((fs_File_std_out(fs_File_std_Self self))(fs_File));
$attr($must_check)
$extern fn_((fs_File_std_err(fs_File_std_Self self))(fs_File));

struct fs_File_std_Self_VTbl {
    fn_(((*inFn)(P$raw ctx))(fs_File));
    fn_(((*outFn)(P$raw ctx))(fs_File));
    fn_(((*errFn)(P$raw ctx))(fs_File));
};
$extern fn_((fs_File_std_VTbl_unreachableIn(P$raw ctx))(fs_File));
$extern fn_((fs_File_std_VTbl_unreachableOut(P$raw ctx))(fs_File));
$extern fn_((fs_File_std_VTbl_unreachableErr(P$raw ctx))(fs_File));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((fs_File_std_isValid(fs_File_std_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->inFn)
        && isNonnull(self.vtbl->outFn)
        && isNonnull(self.vtbl->errFn);
};
fn_((fs_File_std_assertValid(P$raw ctx, P_const$$(fs_File_std_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->inFn);
    claim_assert_nonnull(vtbl->outFn);
    claim_assert_nonnull(vtbl->errFn);
};
fn_((fs_File_std_ensureValid(fs_File_std_Self self))(fs_File_std_Self)) {
    return fs_File_std_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* fs_File_std__included */
