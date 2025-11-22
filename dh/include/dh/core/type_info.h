/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-17 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Type information for type-safe programming
 * @details Provides type information for type-safe programming:
 *          - Type size and alignment
 *          - Type comparison
 *          - Type casting
 *          - Type conversion
 *          - Type utility functions
 */

#ifndef core_type_info__included
#define core_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Definitions =========================================*/

/* Core type information (kept minimal - 8 bytes) */
typedef union TypeInfo {
    struct {
        u64 size  : 58;
        u64 align : 6;
    };
    u64 packed;
} TypeInfo;
/// Get type information for meta
#define typeInfo$(_T... /*(TypeInfo)*/) __step__typeInfo$(_T)
/// Compare equality of type information
#define TypeInfo_eq(_lhs /*: TypeInfo*/, _rhs /*: TypeInfo*/... /*(bool)*/) __step__TypeInfo_eq(_lhs, _rhs)

/// For explicit materialization type representation of abstract generic types
#define typeAsg(var_addr_dest, val_src... /* var_addr_dest */) comp_op__typeAsg(pp_uniqTok(addr_dest), var_addr_dest, val_src)
#define typeCast$(TDest_w_ValSrc...) pp_expand(pp_defer(type$)(pp_Tuple_unwrapSufCommaExpand TDest_w_ValSrc))

/*========== Macros and Implementations =====================================*/

#define __step__typeInfo$(_T...) lit$((TypeInfo){.size = sizeOf$(_T), .align = alignOf$(_T)})
#define __step__TypeInfo_eq(_lhs, _rhs...) ((_lhs).packed == (_rhs).packed)


#if UNUSED_CODE
#define comp_op__type$(__src, T_Dest, val_src...) \
    blk({ \
        var __src = val_src; \
        claim_assert_static_msg(!isSameType$(TypeOf(__src), meta_P$raw), "`meta_P$raw` is not compatible with `type$`. Use `meta_castP$`."); \
        claim_assert_static_msg(!isSameType$(TypeOf(__src), meta_S$raw), "`meta_S$raw` is not compatible with `type$`. Use `meta_castS$`."); \
        blk_return(*((T_Dest*)&(__src))); \
    })
#endif /* UNUSED_CODE */

#define comp_op__typeAsg(__addr_dest, var_addr_dest, val_src...) \
    blk({ \
        let __addr_dest = var_addr_dest; \
        *(__addr_dest) = type$(TypeOf(*(__addr_dest)), val_src); \
        blk_return __addr_dest; \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_type_info__included */

#if UNUSED_CODE
let a = (64 - 1) - __builtin_clzll(1);

// 0 -> 1
// 1 -> 2
// 2 -> 4
// 3 -> 8
usize b = 1ull << (3);
/* // let a = ((64 - 1) - __builtin_clzll(usize_limit_max));

// a(x) = log2(x)
// z(x) = x
// find 0 ~= min(pow2(z(x)) - pow2(a(64-x)))

#define log2$usize(x) ((64 - 1) - __builtin_clzll(x))
#define pow2$usize(x) (2ull << ((x) - 1))
#define find$usize(x) (pow2$usize(x) - pow2$usize(log2$usize(x)))

let z0 = (2ull << (63 - 0)) - 1; // 모든 비트를 사용함
let z1 = (2ull << (63 - 1)) - 1; // 마지막 비트 양보
let z2 = (2ull << (63 - 2)) - 1; // 마지막 2비트 양보
let z3 = (2ull << (63 - 3)) - 1; // 마지막 3비트 양보
let z4 = (2ull << (63 - 4)) - 1; // 마지막 4비트 양보
let z5 = (2ull << (63 - 5)) - 1; // 마지막 5비트 양보
let z6 = (2ull << (63 - 6)) - 1; // 마지막 6비트 양보
// let a0 = 2ull << (-1); // 1
let a1 = 1ull << ((2ull << (1 - 1)) - 1); //
let a2 = 1ull << ((2ull << (2 - 1)) - 1); //
let a3 = 1ull << ((2ull << (3 - 1)) - 1); //
let a4 = 1ull << ((2ull << (4 - 1)) - 1); //
let a5 = 1ull << ((2ull << (5 - 1)) - 1); //
let a6 = 1ull << ((2ull << (6 - 1)) - 1); //

let za1  = (f64)z1 - (f64)(a1);
let za2  = (f64)z2 - (f64)(a2);
let za3  = (f64)z3 - (f64)(a3);
let za4  = (f64)z4 - (f64)(a4);
let za5  = (f64)z5 - (f64)(a5);
let za6  = (f64)z6 - (f64)(a6);
let test = z5 < a5;
let test = z6 < a6;

// n -> m:(2^n)-1 = 2^m
// 0 -> 0 = 1
// 1 -> 1 = 2
//  2 = 4
// 2 -> 3 = 8
//  4 = 16
//  5 = 32
//  6 = 64
// 3 -> 7 = 128
//  8 = 256
//  9 = 512
//  10 = 1024
//  11 = 2048
//  12 = 4096
//  13 = 8192
//  14 = 16384
// 4 -> 15 = 32768
// 5 -> 31 = 2147483648
// 6 -> 63 = 9223372036854780000
//      64 = 0
let a = (2ull << 63);
let b = usize_limit_max;
18446744073709600000 18446744069414600000

    let a
    = log2$usize(63);
let b = pow2$usize(a);
let c = find$usize(b);

let n1 = find$usize(2);
let n2 = find$usize(4);
let n3 = find$usize(8);
let n4 = find$usize(16);
let n5 = find$usize(32);
let n6 = find$usize(63); */
#endif /* UNUSED_CODE */
