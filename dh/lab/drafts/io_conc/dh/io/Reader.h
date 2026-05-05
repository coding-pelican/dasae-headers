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
#ifndef io_Reader__included
#define io_Reader__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

typedef struct io_Reader {
    var_(ctx, P$raw);
    $attr($must_check)
    fn_(((*read)(P$raw ctx, S$u8 buf))(E$usize));
} io_Reader;

$attr($must_check)
$extern fn_((io_Reader_read(io_Reader self, S$u8 buf))(E$usize));
$attr($must_check)
$extern fn_((io_Reader_readExact(io_Reader self, S$u8 buf))(E$void));
$attr($must_check)
$extern fn_((io_Reader_readByte(io_Reader self))(E$u8));
$attr($must_check)
$extern fn_((io_Reader_skip(io_Reader self, usize n))(E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Reader__included */
