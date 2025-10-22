
// /**
//  * @copyright Copyright (c) 2025 Gyeongtae Kim
//  * @license   MIT License - see LICENSE file for details
//  *
//  * @file    Reader.h
//  * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
//  * @date    2025-06-06 (date of creation)
//  * @updated 2025-06-06 (date of last update)
//  * @version v0.1-alpha
//  * @ingroup dasae-headers(dh)/io
//  * @prefix  io_Reader
//  */

// #ifndef IO_READER_INCLUDED
// #define IO_READER_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "dh/meta/common.h"

// /*========== Macros and Declarations ========================================*/

// typedef struct io_Reader {
//     const anyptr ctx;
//     fn_((*const read)(const anyptr ctx, Sli$u8 bytes), Err$usize) $must_check;
// } io_Reader;

// extern fn_(io_Reader_read(io_Reader self, Sli$u8 bytes), Err$usize) $must_check;
// extern fn_(io_Reader_readAll(io_Reader self, Sli$u8 bytes), Err$usize) $must_check;
// extern fn_(io_Reader_readByte(io_Reader self, Sli$u8 bytes), Err$u8) $must_check;

// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* IO_READER_INCLUDED */
