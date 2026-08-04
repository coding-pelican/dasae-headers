/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Reader.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-06 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_Reader
 */
#pragma once
#ifndef io_Reader__included
#define io_Reader__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../mem/common.h"

/*========== Macros and Declarations ========================================*/

struct io_Reader {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*readFn)(P$raw ctx, S$u8 out_buf))(io_ReadE$usize));
};
$extern let_(io_Reader_noop, io_Reader);
$extern let_(io_Reader_failing, io_Reader);
$attr($must_check)
$extern fn_((io_Reader_noRead(P$raw ctx, S$u8 out_buf))(io_ReadE$usize));
$attr($must_check)
$extern fn_((io_Reader_failingRead(P$raw ctx, S$u8 out_buf))(io_ReadE$usize));

$attr($inline_always)
$static fn_((io_Reader_isValid(io_Reader self))(bool));
$attr($inline_always)
$static fn_((io_Reader_assertValid(
    P$raw ctx, fn_(((*readFn)(P$raw ctx, S$u8 out_buf))(io_ReadE$usize))
))(void));
$attr($inline_always)
$static fn_((io_Reader_ensureValid(io_Reader self))(io_Reader));

$attr($must_check)
/// Read at most out_bytes.len bytes. Zero means EOF for a non-empty output.
$extern fn_((io_Reader_read(io_Reader self, S$u8 out_bytes))(io_ReadE$usize));
$attr($must_check)
$extern fn_((io_Reader_readByte(io_Reader self))(io_ReadExactE$u8));
/// Read until out_bytes is full, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_readExact(io_Reader self, S$u8 out_bytes))(io_ReadExactE$void));
/// Read until at least_len bytes are read, EOF is reached, or out_bytes is full.
$attr($must_check)
$extern fn_((io_Reader_readAtLeast(io_Reader self, S$u8 out_bytes, usize least_len))(io_ReadE$usize));

$attr($must_check)
$extern fn_((io_Reader_readInt$usize(io_Reader self, mem_Endian endian))(io_ReadExactE$usize));
$attr($must_check)
$extern fn_((io_Reader_readInt$u64(io_Reader self, mem_Endian endian))(io_ReadExactE$u64));
$attr($must_check)
$extern fn_((io_Reader_readInt$ulong(io_Reader self, mem_Endian endian))(io_ReadExactE$ulong));
$attr($must_check)
$extern fn_((io_Reader_readInt$u32(io_Reader self, mem_Endian endian))(io_ReadExactE$u32));
$attr($must_check)
$extern fn_((io_Reader_readInt$u16(io_Reader self, mem_Endian endian))(io_ReadExactE$u16));
$attr($must_check)
$extern fn_((io_Reader_readInt$u8(io_Reader self, mem_Endian endian))(io_ReadExactE$u8));
$attr($must_check)
$extern fn_((io_Reader_readInt$isize(io_Reader self, mem_Endian endian))(io_ReadExactE$isize));
$attr($must_check)
$extern fn_((io_Reader_readInt$i64(io_Reader self, mem_Endian endian))(io_ReadExactE$i64));
$attr($must_check)
$extern fn_((io_Reader_readInt$ilong(io_Reader self, mem_Endian endian))(io_ReadExactE$ilong));
$attr($must_check)
$extern fn_((io_Reader_readInt$i32(io_Reader self, mem_Endian endian))(io_ReadExactE$i32));
$attr($must_check)
$extern fn_((io_Reader_readInt$i16(io_Reader self, mem_Endian endian))(io_ReadExactE$i16));
$attr($must_check)
$extern fn_((io_Reader_readInt$i8(io_Reader self, mem_Endian endian))(io_ReadExactE$i8));

/// Skip exactly discard_len bytes, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_skip(io_Reader self, usize discard_len))(io_ReadExactE$void));
$attr($must_check)
$extern fn_((io_Reader_skipByte(io_Reader self))(io_ReadExactE$void));
/// Skip until at least_len bytes are discarded or EOF is reached.
$attr($must_check)
$extern fn_((io_Reader_skipAtLeast(io_Reader self, usize least_len))(io_ReadE$usize));

/// Copy all bytes from reader to writer until EOF.
$attr($must_check)
$extern fn_((io_Reader_copy(io_Reader self, io_Writer writer))(io_CopyE$usize));
$attr($must_check)
$extern fn_((io_Reader_copyByte(io_Reader self, io_Writer writer))(io_CopyExactE$void));
/// Copy exactly copy_len bytes, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_copyExact(io_Reader self, io_Writer writer, usize copy_len))(io_CopyExactE$void));
/// Copy until at least_len bytes are copied or EOF is reached.
$attr($must_check)
$extern fn_((io_Reader_copyAtLeast(io_Reader self, io_Writer writer, usize least_len))(io_CopyE$usize));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((io_Reader_isValid(io_Reader self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.readFn);
};
fn_((io_Reader_assertValid(
    P$raw ctx,
    fn_(((*readFn)(P$raw ctx, S$u8 out_buf))(io_ReadE$usize))
))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(readFn);
};
fn_((io_Reader_ensureValid(io_Reader self))(io_Reader)) {
    return io_Reader_assertValid(self.ctx, self.readFn), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Reader__included */
