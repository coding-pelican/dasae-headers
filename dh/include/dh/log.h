/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    log.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-03 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  log
 *
 * @brief   Provider-driven logging interface
 */
#pragma once
#ifndef log__included
#define log__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "io/Writer.h"
#include "io/std.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((log_Level $fits($packed))(
    log_Level_err = 0,
    log_Level_warn,
    log_Level_info,
    log_Level_debug
)) log_Level;
$extern fn_((log_Level_asText(log_Level self))(S_const$u8));

#define log_level_default pp_if_(debug_enabled)( \
    pp_then_(log_Level_debug), \
    pp_else_(log_Level_info) \
)
#define log_scope_default u8_l("")

T_alias$((log_Self_VTbl)(struct log_Self_VTbl));
/// Copyable logging capability. The provider owns filtering and output policy.
T_alias$((log_Self)(struct log_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(log_Self_VTbl));
}));
T_use_prl$(log_Self);
$attr($inline_always)
$static fn_((log_isValid(log_Self self))(bool));
$attr($inline_always)
$static fn_((log_assertValid(P$raw ctx, P_const$$(log_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((log_ensureValid(log_Self self))(log_Self));

$extern let_(log_Self_noop, log_Self);
$extern let_(log_Self_failing, log_Self);
/// Default logger backed by an explicitly supplied standard-I/O capability.
T_alias$((log_Default)(struct log_Default {
    var_(std, io_std_Self);
    var_(max_level, log_Level);
}));
T_use_O$(log_Default);
$attr($must_check)
$extern fn_((log_Default_init(io_std_Self std, log_Level max_level))(log_Default));
$attr($must_check)
$extern fn_((log_Default_self(log_Default* self))(log_Self));

$attr($must_check)
$extern fn_((log_enabled(log_Self self, log_Level level, S_const$u8 scope))(bool));
$extern fn_((log_stamp(log_Self self, log_Level level, S_const$u8 scope, S_const$u8 fmt, ...))(void));
$extern fn_((log_stampVaArgs(log_Self self, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void));

$extern fn_((log_err(log_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((log_warn(log_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((log_info(log_Self self, S_const$u8 fmt, ...))(void));
$extern fn_((log_debug(log_Self self, S_const$u8 fmt, ...))(void));

$extern fn_((log_scopedErr(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void));
$extern fn_((log_scopedWarn(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void));
$extern fn_((log_scopedInfo(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void));
$extern fn_((log_scopedDebug(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void));

struct log_Self_VTbl {
    $attr($must_check)
    fn_(((*enabledFn)(P$raw ctx, log_Level level, S_const$u8 scope))(bool));
    fn_(((*stampFn)(P$raw ctx, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void));
};
$attr($must_check)
$extern fn_((log_Self_VTbl_noEnabled(P$raw ctx, log_Level level, S_const$u8 scope))(bool));
$attr($must_check)
$extern fn_((log_Self_VTbl_unreachableEnabled(P$raw ctx, log_Level level, S_const$u8 scope))(bool));
$extern fn_((log_Self_VTbl_noStamp(P$raw ctx, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void));
$extern fn_((log_Self_VTbl_unreachableStamp(P$raw ctx, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void));

/// Caller-owned writer provider. Synchronization and writer lifetime remain
/// explicit caller responsibilities.
T_alias$((log_Writer)(struct log_Writer {
    var_(output, io_Writer);
    var_(max_level, log_Level);
}));
$attr($must_check)
$extern fn_((log_Writer_init(io_Writer output, log_Level max_level))(log_Writer));
$attr($must_check)
$extern fn_((log_Writer_self(log_Writer* self))(log_Self));


/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((log_isValid(log_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->enabledFn)
        && isNonnull(self.vtbl->stampFn);
};
fn_((log_assertValid(P$raw ctx, P_const$$(log_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->enabledFn);
    claim_assert_nonnull(vtbl->stampFn);
};
fn_((log_ensureValid(log_Self self))(log_Self)) {
    return log_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* log__included */
