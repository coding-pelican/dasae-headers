/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Writer.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-13 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_Writer
 */
#pragma once
#ifndef io_Writer__included
#define io_Writer__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../mem/common.h"
#include "../fmt/errors.h"

/*========== Macros and Declarations ========================================*/

errset_((io_PrintFailed_E)(io_PrintFailed));
errset_((io_PrintE)() $union_errset_(
    io_WriteE,
    fmt_E,
    io_PrintFailed_E
));

struct io_Writer {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize));
};
$extern let_(io_Writer_noop, io_Writer);
$extern let_(io_Writer_failing, io_Writer);
$attr($must_check)
$extern fn_((io_Writer_noWrite(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize));
$attr($must_check)
$extern fn_((io_Writer_failingWrite(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize));

$attr($inline_always)
$static fn_((io_Writer_isValid(io_Writer self))(bool));
$attr($inline_always)
$static fn_((io_Writer_assertValid(
    P$raw ctx, fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize))
))(void));
$attr($inline_always)
$static fn_((io_Writer_ensureValid(io_Writer self))(io_Writer));

$attr($must_check)
$extern fn_((io_Writer_write(io_Writer self, S_const$u8 bytes))(io_WriteE$usize));
$attr($must_check)
$extern fn_((io_Writer_writeBytes(io_Writer self, S_const$u8 bytes))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeBytesN(io_Writer self, S_const$u8 bytes, usize n))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeByte(io_Writer self, u8 byte))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeByteN(io_Writer self, u8 byte, usize n))(io_WriteE$void));

$attr($must_check)
$extern fn_((io_Writer_writeInt$usize(io_Writer self, usize value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$u64(io_Writer self, u64 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$ulong(io_Writer self, ulong value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$u32(io_Writer self, u32 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$u16(io_Writer self, u16 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$u8(io_Writer self, u8 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$isize(io_Writer self, isize value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$i64(io_Writer self, i64 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$ilong(io_Writer self, ilong value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$i32(io_Writer self, i32 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$i16(io_Writer self, i16 value, mem_Endian endian))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_writeInt$i8(io_Writer self, i8 value, mem_Endian endian))(io_WriteE$void));

$attr($must_check)
$extern fn_((io_Writer_lf(io_Writer self))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_crlf(io_Writer self))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_nl(io_Writer self))(io_WriteE$void));
$attr($must_check)
$extern fn_((io_Writer_print(io_Writer self, S_const$u8 fmt, ...))(io_PrintE$void));
$attr($must_check)
$extern fn_((io_Writer_printVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(io_PrintE$void));
$attr($must_check)
$extern fn_((io_Writer_println(io_Writer self, S_const$u8 fmt, ...))(io_PrintE$void));
$attr($must_check)
$extern fn_((io_Writer_printlnVaArgs(io_Writer self, S_const$u8 fmt, va_list va_args))(io_PrintE$void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((io_Writer_isValid(io_Writer self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.writeFn);
}
fn_((io_Writer_assertValid(
    P$raw ctx,
    fn_(((*writeFn)(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize))
))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(writeFn);
}
fn_((io_Writer_ensureValid(io_Writer self))(io_Writer)) {
    return io_Writer_assertValid(self.ctx, self.writeFn), self;
}
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Writer__included */
