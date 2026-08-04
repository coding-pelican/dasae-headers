/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Count.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-08-04 (date of creation)
 * @updated 2026-08-04 (date of last update)
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Count
 *
 * @brief   Reader and writer wrappers that count transferred bytes
 */
#pragma once
#ifndef io_Count__included
#define io_Count__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Reader.h"
#include "Writer.h"

/*========== Macros and Declarations ========================================*/

T_alias$((io_Count_Reader)(struct io_Count_Reader {
    var_(inner, io_Reader);
    var_(count, u64);
}));
$extern fn_((io_Count_Reader_from(io_Reader inner))(io_Count_Reader));
$extern fn_((io_Count_Reader_count(io_Count_Reader self))(u64));
$extern fn_((io_Count_reader(io_Count_Reader* self))(io_Reader));

T_alias$((io_Count_Writer)(struct io_Count_Writer {
    var_(inner, io_Writer);
    var_(count, u64);
}));
$extern fn_((io_Count_Writer_from(io_Writer inner))(io_Count_Writer));
$extern fn_((io_Count_Writer_count(io_Count_Writer self))(u64));
$extern fn_((io_Count_writer(io_Count_Writer* self))(io_Writer));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Count__included */
