/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    prefetch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-19 (date of creation)
 * @updated 2026-05-19 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  prefetch
 */
#pragma once
#ifndef builtin_prefetch__included
#define builtin_prefetch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

typedef enum $packed prefetch_RW {
    prefetch_RW_read = 0,
    prefetch_RW_write = 1
} prefetch_RW;

typedef enum $packed prefetch_Locality {
    prefetch_Locality_nta = 0,
    prefetch_Locality_l3 = 1,
    prefetch_Locality_l2 = 2,
    prefetch_Locality_l1 = 3
} prefetch_Locality;

#define prefetch(_$addr /*: PtrType*/, _$rw /*: prefetch_RW*/, _$locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch(_$addr, _$rw, _$locality)
#define prefetch_read(_$addr /*: PtrType*/, _$locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch_read(_$addr, _$locality)
#define prefetch_write(_$addr /*: PtrType*/, _$locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch_write(_$addr, _$locality)

/*========== Macros and Definitions =========================================*/

#define __op__prefetch(_$addr, _$rw, _$locality...) comp_prefetch( \
    _$addr, as$(int)(as$(prefetch_RW)(_$rw)), as$(int)(as$(prefetch_Locality)(_$locality)) \
)
#define __op__prefetch_read(_$addr, _$locality...) prefetch(_$addr, prefetch_RW_read, _$locality)
#define __op__prefetch_write(_$addr, _$locality...) prefetch(_$addr, prefetch_RW_write, _$locality)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_prefetch__included */
