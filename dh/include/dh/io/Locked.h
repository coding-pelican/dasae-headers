/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Locked.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Locked
 *
 * @brief   Reader and writer leases bound to an acquired lock
 */
#pragma once
#ifndef io_Locked__included
#define io_Locked__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Reader.h"
#include "Writer.h"

/*========== Macros and Declarations ========================================*/

T_alias$((io_Locked)(struct io_Locked {
    var_(ctx, P$raw);
    fn_(((*unlockFn)(P$raw ctx))(void));
}));
$attr($inline_always)
$static fn_((io_Locked_isValid(io_Locked self))(bool));
$attr($inline_always)
$static fn_((io_Locked_assertValid(P$raw ctx, fn_(((*unlockFn)(P$raw ctx))(void))))(void));
$attr($inline_always)
$static fn_((io_Locked_ensureValid(io_Locked self))(io_Locked));

$extern let_(io_Locked_noop, io_Locked);
$extern let_(io_Locked_failing, io_Locked);

$extern fn_((io_Locked_unlock(io_Locked self))(void));

T_alias$((io_Locked_Reader)(struct io_Locked_Reader {
    var_(inner, io_Reader);
    var_(lock, io_Locked);
}));
T_use_O$(io_Locked_Reader);
$attr($must_check)
$extern fn_((io_Locked_Reader_from(io_Reader inner, io_Locked lock))(io_Locked_Reader));
$extern fn_((io_Locked_Reader_unlock(io_Locked_Reader self))(void));
$attr($must_check)
$extern fn_((io_Locked_reader(io_Locked_Reader self))(io_Reader));

T_alias$((io_Locked_Writer)(struct io_Locked_Writer {
    var_(inner, io_Writer);
    var_(lock, io_Locked);
}));
T_use_O$(io_Locked_Writer);
$attr($must_check)
$extern fn_((io_Locked_Writer_from(io_Writer inner, io_Locked lock))(io_Locked_Writer));
$extern fn_((io_Locked_Writer_unlock(io_Locked_Writer self))(void));
$attr($must_check)
$extern fn_((io_Locked_writer(io_Locked_Writer self))(io_Writer));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((io_Locked_isValid(io_Locked self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.unlockFn);
};
fn_((io_Locked_assertValid(P$raw ctx, fn_(((*unlockFn)(P$raw ctx))(void))))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(unlockFn);
};
fn_((io_Locked_ensureValid(io_Locked self))(io_Locked)) {
    return io_Locked_assertValid(self.ctx, self.unlockFn), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Locked__included */
