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

#define prefetch(_addr /*: PtrType*/, _rw /*: prefetch_RW*/, _locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch(_addr, _rw, _locality)
#define prefetch_read(_addr /*: PtrType*/, _locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch_read(_addr, _locality)
#define prefetch_write(_addr /*: PtrType*/, _locality /*: prefetch_Locality*/... /*(void)*/) \
    __op__prefetch_write(_addr, _locality)

/*========== Macros and Definitions =========================================*/

#define __op__prefetch(_addr, _rw, _locality...) comp_prefetch( \
    _addr, as$(int)(as$(prefetch_RW)(_rw)), as$(int)(as$(prefetch_Locality)(_locality)) \
)
#define __op__prefetch_read(_addr, _locality...) prefetch(_addr, prefetch_RW_read, _locality)
#define __op__prefetch_write(_addr, _locality...) prefetch(_addr, prefetch_RW_write, _locality)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_prefetch__included */
