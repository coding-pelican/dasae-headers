
/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Writer.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-05-13 (date of creation)
 * @updated 2025-05-13 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Writer
 */

#ifndef IO_WRITER_INCLUDED
#define IO_WRITER_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/io/common.h"
#include "dh/meta/common.h"

/*========== Macros and Declarations ========================================*/

typedef struct io_Writer {
    const anyptr ctx;
    fn_((*const write)(const anyptr ctx, Sli_const$u8 bytes), Err$usize) $must_check;
} io_Writer;

extern fn_(io_Writer_write(io_Writer self, Sli_const$u8 bytes), Err$usize) $must_check;
extern fn_(io_Writer_writeAll(io_Writer self, Sli_const$u8 bytes), Err$void) $must_check;
extern fn_(io_Writer_writeByte(io_Writer self, u8 byte), Err$void) $must_check;
extern fn_(io_Writer_writeByteNTimes(io_Writer self, u8 byte, usize n), Err$void) $must_check;
extern fn_(io_Writer_writeBytesNTimes(io_Writer self, Sli_const$u8 bytes, usize n), Err$void) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* IO_WRITER_INCLUDED */
