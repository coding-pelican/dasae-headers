/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Fixed.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-18 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Fixed
 */
#ifndef io_Fixed__included
#define io_Fixed__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/io/Reader.h"
#include "dh/io/Writer.h"

/*========== Macros and Declarations ========================================*/

/* --- Fixed Stream ---*/

typedef struct io_Fixed {
    S_const$u8 buf;
    usize pos;
} io_Fixed;
typedef union io_FixedMut {
    struct {
        S$u8 buf;
        usize pos;
    };
    io_Fixed as_const;
} io_FixedMut;
/// Initialize fixed buffer for reading
$extern fn_((io_Fixed_reading(S_const$u8 buf))(io_Fixed));
/// Initialize fixed buffer for writing
$extern fn_((io_Fixed_writing(S$u8 buf))(io_FixedMut));
/// Get written portion of buffer
$extern fn_((io_Fixed_written(io_FixedMut self))(S$u8));
/// Reset position to start
$extern fn_((io_Fixed_reset(io_Fixed* self))(void));

/* --- Fixed Stream Reader ---*/

typedef struct io_Fixed_Reader {
    io_Fixed stream;
} io_Fixed_Reader;
/// Initialize fixed reader with fixed buffer
$extern fn_((io_Fixed_Reader_init(io_Fixed stream))(io_Fixed_Reader));
/// Get io_Reader interface
$extern fn_((io_Fixed_reader(io_Fixed_Reader* self))(io_Reader));

/* --- Fixed Stream Writer ---*/

typedef struct io_Fixed_Writer {
    io_FixedMut stream;
} io_Fixed_Writer;
/// Initialize fixed writer with fixed buffer
$extern fn_((io_Fixed_Writer_init(io_FixedMut stream))(io_Fixed_Writer));
/// Get io_Writer interface
$extern fn_((io_Fixed_writer(io_Fixed_Writer* self))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Fixed__included */
