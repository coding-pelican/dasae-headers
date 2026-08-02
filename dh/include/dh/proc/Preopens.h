/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Preopens.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Preopens
 *
 * @brief   Injectable named resources supplied by the parent process
 */
#pragma once
#ifndef proc_Preopens__included
#define proc_Preopens__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../fs/Dir.h"
#include "../fs/File/self.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_Preopens_direct_E)(proc_Preopens_direct_Unsupported));

T_alias$((proc_Preopens_Resrc)(variant_((proc_Preopens_Resrc $fits($packed))(
    (proc_Preopens_Resrc_file, fs_File),
    (proc_Preopens_Resrc_dir, fs_Dir)
))));
T_use_O$(proc_Preopens_Resrc);

T_alias$((proc_Preopens_VTbl)(struct proc_Preopens_VTbl));
T_alias$((proc_Preopens)(struct proc_Preopens {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Preopens_VTbl));
}));
T_use_prl$(proc_Preopens);
T_use_E$($set(proc_Preopens_direct_E)(proc_Preopens));
$attr($inline_always)
$static fn_((proc_Preopens_isValid(proc_Preopens self))(bool));
$attr($inline_always)
$static fn_((proc_Preopens_assertValid(P$raw ctx, P_const$$(proc_Preopens_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_Preopens_ensureValid(proc_Preopens self))(proc_Preopens));

$extern let_(proc_Preopens_empty, proc_Preopens);
$attr($must_check)
$extern fn_((proc_Preopens_direct(void))(proc_Preopens_direct_E$proc_Preopens));

$extern fn_((proc_Preopens_by(proc_Preopens self, S_const$u8 name))(O$proc_Preopens_Resrc));

struct proc_Preopens_VTbl {
    fn_(((*byFn)(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Preopens_isValid(proc_Preopens self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->byFn);
};
fn_((proc_Preopens_assertValid(P$raw ctx, P_const$$(proc_Preopens_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->byFn);
};
fn_((proc_Preopens_ensureValid(proc_Preopens self))(proc_Preopens)) {
    return proc_Preopens_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Preopens__included */
