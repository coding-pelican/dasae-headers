/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Buf.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-25 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_Buf
 */
#pragma once
#ifndef io_Buf__included
#define io_Buf__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Reader.h"
#include "Writer.h"

/*========== Macros and Declarations ========================================*/

/*---------- Buffered Reader ------------------------------------------------*/

errset_((io_Buf_ReadE)() $union_errset_(
    io_ReadExactE,
    io_TooSmallBufferE
));
T_useBy$(($spec(E, $set(io_Buf_ReadE)))(u8, S_const$u8, S$u8));
errset_((io_Buf_StreamE)() $union_errset_(
    io_Buf_ReadE,
    io_WriteE
));
T_use_E$($set(io_Buf_StreamE)(usize));

typedef struct io_Buf_Reader {
    var_(inner, io_Reader);
    var_(buf, S$u8);
    var_(start, usize);
    var_(end, usize);
} io_Buf_Reader;
/// Initialize buffered reader with external buffer
$extern fn_((io_Buf_Reader_from(io_Reader inner, S$u8 buf))(io_Buf_Reader));
/// Fill buffer with more data from inner reader
$attr($must_check)
$extern fn_((io_Buf_Reader_fill(io_Buf_Reader* self))(io_ReadE$void));
/// Buffer at least min_len bytes for local inspection.
$attr($must_check)
$extern fn_((io_Buf_Reader_require(io_Buf_Reader* self, usize min_len))(io_Buf_ReadE$void));
/// Return currently buffered bytes without reading.
$extern fn_((io_Buf_Reader_ready(io_Buf_Reader self))(S_const$u8));
$extern fn_((io_Buf_Reader_readyMut(io_Buf_Reader self))(S$u8));
/// Get io_Reader interface
$extern fn_((io_Buf_reader(io_Buf_Reader* self))(io_Reader));

/*--- Reader cursor ---*/

/// Peek at next byte without consuming.
$attr($must_check)
$extern fn_((io_Buf_Reader_peekByte(io_Buf_Reader* self))(io_Buf_ReadE$u8));
/// Peek at len bytes without consuming.
$attr($must_check)
$extern fn_((io_Buf_Reader_peekBytes(io_Buf_Reader* self, usize len))(io_Buf_ReadE$S_const$u8));
/// Take next byte.
$attr($must_check)
$extern fn_((io_Buf_Reader_takeByte(io_Buf_Reader* self))(io_Buf_ReadE$u8));
/// Take len bytes.
$attr($must_check)
$extern fn_((io_Buf_Reader_takeBytes(io_Buf_Reader* self, usize len))(io_Buf_ReadE$S_const$u8));
/// Skip len bytes.
$attr($must_check)
$extern fn_((io_Buf_Reader_skip(io_Buf_Reader* self, usize len))(io_Buf_ReadE$void));
/// Consume len bytes from current view.
$extern fn_((io_Buf_Reader_drop(io_Buf_Reader* self, usize len))(void));
/// Consume all bytes from current view.
$extern fn_((io_Buf_Reader_dropAll(io_Buf_Reader* self))(void));

/*--- Reader delimited read ---*/

/// Read until delimiter byte; delimiter is consumed and excluded from output.
$attr($must_check)
$extern fn_((io_Buf_Reader_readUntilByte(io_Buf_Reader* self, u8 delim, S$u8 out_buf))(io_Buf_ReadE$S$u8));
/// Read until delimiter byte sequence; delimiter is consumed and excluded from output.
$attr($must_check)
$extern fn_((io_Buf_Reader_readUntilSeq(io_Buf_Reader* self, S_const$u8 delim, S$u8 out_buf))(io_Buf_ReadE$S$u8));
/// Read until any delimiter byte; delimiter is consumed and excluded from output.
$attr($must_check)
$extern fn_((io_Buf_Reader_readUntilAny(io_Buf_Reader* self, S_const$u8 delims, S$u8 out_buf))(io_Buf_ReadE$S$u8));

/*--- Reader delimited stream ---*/

/// Stream through delimiter byte; delimiter is consumed and excluded from output.
$attr($must_check)
$extern fn_((io_Buf_Reader_streamUntilByte(io_Buf_Reader* self, u8 delim, io_Writer out))(io_Buf_StreamE$usize));
/// Stream through delimiter sequence; delimiter is consumed and excluded from output.
$attr($must_check)
$extern fn_((io_Buf_Reader_streamUntilSeq(io_Buf_Reader* self, S_const$u8 delim, io_Writer out))(io_Buf_StreamE$usize));
/// Stream through the first matching delimiter byte; it is consumed and excluded.
$attr($must_check)
$extern fn_((io_Buf_Reader_streamUntilAny(io_Buf_Reader* self, S_const$u8 delims, io_Writer out))(io_Buf_StreamE$usize));

/*--- Reader delimited skip ---*/

/// Skip until delimiter byte; delimiter is consumed.
$attr($must_check)
$extern fn_((io_Buf_Reader_skipUntilByte(io_Buf_Reader* self, u8 delim))(io_Buf_ReadE$void));
/// Skip until delimiter byte sequence; delimiter is consumed.
$attr($must_check)
$extern fn_((io_Buf_Reader_skipUntilSeq(io_Buf_Reader* self, S_const$u8 delim))(io_Buf_ReadE$void));
/// Skip until any delimiter byte; delimiter is consumed.
$attr($must_check)
$extern fn_((io_Buf_Reader_skipUntilAny(io_Buf_Reader* self, S_const$u8 delims))(io_Buf_ReadE$void));

/*---------- Buffered Writer ------------------------------------------------*/

typedef struct io_Buf_Writer {
    var_(inner, io_Writer);
    var_(buf, S$u8);
    var_(cursor, usize);
} io_Buf_Writer;
/// Initialize buffered writer with external buffer
$extern fn_((io_Buf_Writer_from(io_Writer inner, S$u8 buf))(io_Buf_Writer));
/// Return currently pending data before the cursor.
$extern fn_((io_Buf_Writer_pending(io_Buf_Writer self))(S_const$u8));
$extern fn_((io_Buf_Writer_pendingMut(io_Buf_Writer self))(S$u8));
/// Return writable capacity after the cursor without advancing it.
$extern fn_((io_Buf_Writer_unusedMut(io_Buf_Writer self))(S$u8));
/// Commit bytes initialized through `io_Buf_Writer_unusedMut`.
$extern fn_((io_Buf_Writer_advance(io_Buf_Writer* self, usize len))(void));
/// Remove the most recently committed pending bytes.
$extern fn_((io_Buf_Writer_undo(io_Buf_Writer* self, usize len))(void));
/// Flush buffered data to inner writer
$attr($must_check)
$extern fn_((io_Buf_Writer_flush(io_Buf_Writer* self))(io_WriteE$void));
/// Get io_Writer interface
$extern fn_((io_Buf_writer(io_Buf_Writer* self))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Buf__included */
