/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Reader.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-06 (date of creation)
 * @updated 2025-06-06 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_Reader
 */
#ifndef io_Reader__included
#define io_Reader__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include <stdarg.h> /* For va_list, va_start(), va_end() */

/*========== Macros and Declarations ========================================*/

typedef struct io_Reader {
    var_(ctx, P$raw);
    fn_(((*read)(P$raw ctx, S$u8 buf))(E$usize)) $must_check;
} io_Reader;

$extern fn_((io_Reader_read(io_Reader self, S$u8 buf))(E$usize)) $must_check;
$extern fn_((io_Reader_readExact(io_Reader self, S$u8 buf))(E$void)) $must_check;
$extern fn_((io_Reader_readByte(io_Reader self))(E$u8)) $must_check;
$extern fn_((io_Reader_skip(io_Reader self, usize n))(E$void)) $must_check;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Reader__included */
