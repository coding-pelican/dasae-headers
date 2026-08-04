/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Limited.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-08-04 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Limited
 *
 * @brief   Reader view bounded by a remaining byte count
 */
#pragma once
#ifndef io_Limited__included
#define io_Limited__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Reader.h"

/*========== Macros and Declarations ========================================*/

T_alias$((io_Limited_Reader)(struct io_Limited_Reader {
    var_(inner, io_Reader);
    var_(remaining, usize);
}));

/// Borrow inner through a reader that reports EOF after limit bytes.
$extern fn_((io_Limited_Reader_from(io_Reader inner, usize limit))(io_Limited_Reader));
/// Return the number of bytes that may still be read through this view.
$extern fn_((io_Limited_Reader_remaining(io_Limited_Reader self))(usize));
/// Borrow the limited reader interface for no longer than self remains alive.
$extern fn_((io_Limited_reader(io_Limited_Reader* self))(io_Reader));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Limited__included */
