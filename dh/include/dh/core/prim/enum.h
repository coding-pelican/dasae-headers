/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    enum.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-30 (date of creation)
 * @updated 2025-12-30 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 */
#ifndef core_prim_enum__included
#define core_prim_enum__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#define enum_(/*_Ident $bits(8|16|32|64)*/...) __enum_(__VA_ARGS__)
#define $bits(_width /*8|16|32|64*/) \
    , pp_join(_, __$bits, _width)

/*========== Macros and Definitions =========================================*/

#define __enum_(...) pp_overload(__enum, __VA_ARGS__)(__VA_ARGS__)
#define __enum_0(...) enum __attribute__((packed))
#define __enum_1(_Ident...) enum __attribute__((packed)) _Ident
#define __enum_2(_Ident, _bits_width...) enum _bits_width _Ident

#define __$bits_0 "0 bits is not allowed"
#define __$bits_8 \
    /* mode: __QI__ | meaning: Quarter Integer  | size: 1 byte  | corresponding type: "u8, i8"*/ \
    __attribute__((__mode__(__QI__)))
#define __$bits_16 \
    /* mode: __HI__ | meaning: Half Integer     | size: 2 bytes | corresponding type: "u16, i16"*/ \
    __attribute__((__mode__(__HI__)))
#define __$bits_32 \
    /* mode: __SI__ | meaning: Standard Integer | size: 4 bytes | corresponding type: "u32, i32"*/ \
    __attribute__((__mode__(__SI__)))
#define __$bits_64 \
    /* mode: __DI__ | meaning: Double Integer   | size: 8 bytes | corresponding type: "u64, i64"*/ \
    __attribute__((__mode__(__DI__)))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_prim_enum__included */
