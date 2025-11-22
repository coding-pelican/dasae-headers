/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Buf.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-10-25 (date of creation)
 * @updated 2025-10-25 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Buf
 */

#ifndef io_Buf__included
#define io_Buf__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/io/Reader.h"
#include "dh/io/Writer.h"

/*========== Macros and Declarations ========================================*/

/* BufReader ========================*/

typedef struct io_Buf_Reader {
    io_Reader inner;
    S$u8 buf;
    usize start;
    usize end;
} io_Buf_Reader;
/// Initialize buffered reader with external buffer
$extern fn_((io_Buf_Reader_init(io_Reader inner, S$u8 buf))(io_Buf_Reader));
/// Fill buffer with more data from inner reader
$extern fn_((io_Buf_Reader_fill(io_Buf_Reader* self))(E$void)) $must_check;
/// Peek at next byte without consuming (look-ahead)
/// WARNING: Returns raw byte, NOT character or codepoint
/// Use case: Protocol parsing (e.g., check if next byte is '{' for JSON)
$extern fn_((io_Buf_Reader_peekByte(io_Buf_Reader* self))(E$u8)) $must_check;
/// Read until delimiter byte (delimiter consumed, not included in output)
/// WARNING: Operates on raw bytes, NOT character-aware
/// Use case: ASCII protocols (CSV, HTTP headers, etc.)
/// NOT for: UTF-8 text (may split multibyte characters)
/// Returns number of bytes written to output
$extern fn_((io_Buf_Reader_readUntilByte(io_Buf_Reader* self, u8 delim, S$u8 out_buf))(E$S$u8)) $must_check;
/// Skip until delimiter byte (delimiter consumed)
/// WARNING: Operates on raw bytes
/// Use case: Skip to next section in ASCII protocol
$extern fn_((io_Buf_Reader_skipUntilByte(io_Buf_Reader* self, u8 delim))(E$void)) $must_check;
/// Skip n bytes
$extern fn_((io_Buf_Reader_skip(io_Buf_Reader* self, usize n))(E$void)) $must_check;
/// Get io_Reader interface
$extern fn_((io_Buf_Reader_reader(io_Buf_Reader* self))(io_Reader));

/* BufWriter ========================*/

typedef struct io_Buf_Writer {
    io_Writer inner;
    S$u8    buf;
    usize     used;
} io_Buf_Writer;
/// Initialize buffered writer with external buffer
$extern fn_((io_Buf_Writer_init(io_Writer inner, S$u8 buf))(io_Buf_Writer));
 /// Flush buffered data to inner writer
$extern fn_((io_Buf_Writer_flush(io_Buf_Writer* self))(E$void)) $must_check;
 /// Get io_Writer interface
$extern fn_((io_Buf_Writer_writer(io_Buf_Writer* self))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Buf__included */
