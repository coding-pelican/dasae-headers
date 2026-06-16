/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Reader.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-06 (date of creation)
 * @updated 2026-02-06 (date of last update)
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

#include "base.h"

/*========== Macros and Declarations ========================================*/

struct io_Reader {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*readFn)(P$raw ctx, S$u8 out_buf))(E$usize));
};

$attr($must_check)
$extern fn_((io_Reader_read(io_Reader self, S$u8 out_bytes))(E$usize));
$attr($must_check)
$extern fn_((io_Reader_readByte(io_Reader self))(E$u8));
/// Read until out_bytes is full, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_readExact(io_Reader self, S$u8 out_bytes))(E$void));
/// Read until at least_len bytes are read, EOF is reached, or out_bytes is full.
$attr($must_check)
$extern fn_((io_Reader_readAtLeast(io_Reader self, S$u8 out_bytes, usize least_len))(E$usize));

/// Skip exactly discard_len bytes, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_skip(io_Reader self, usize discard_len))(E$void));
$attr($must_check)
$extern fn_((io_Reader_skipByte(io_Reader self))(E$void));
/// Skip until at least_len bytes are discarded or EOF is reached.
$attr($must_check)
$extern fn_((io_Reader_skipAtLeast(io_Reader self, usize least_len))(E$usize));

/// Copy all bytes from reader to writer until EOF.
$attr($must_check)
$extern fn_((io_Reader_copy(io_Reader self, io_Writer writer))(E$usize));
$attr($must_check)
$extern fn_((io_Reader_copyByte(io_Reader self, io_Writer writer))(E$void));
/// Copy exactly copy_len bytes, or return `UnexpectedEOF`.
$attr($must_check)
$extern fn_((io_Reader_copyExact(io_Reader self, io_Writer writer, usize copy_len))(E$void));
/// Copy until at least_len bytes are copied or EOF is reached.
$attr($must_check)
$extern fn_((io_Reader_copyAtLeast(io_Reader self, io_Writer writer, usize least_len))(E$usize));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Reader__included */
