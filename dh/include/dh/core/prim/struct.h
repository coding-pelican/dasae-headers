/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    struct.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_STRUCT_INCLUDED
#define PRIM_STRUCT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define make(TYPE)                IMPL_make(TYPE)
#define makeWith(TYPE, INITAL...) IMPL_makeWith(TYPE, INITAL)

#define create(TYPE)                IMPL_create(TYPE)
#define createWith(TYPE, INITAL...) IMPL_createWith(TYPE, INITAL)

#define array(TYPE, INITAL...)              IMPL_array(TYPE, INITAL)
#define nArray(LENGTH, TYPE)                IMPL_nArray(TYPE, LENGTH)
#define nArrayWith(LENGTH, TYPE, INITAL...) IMPL_nArrayWith(TYPE, LENGTH, INITAL)

/*========== Macros Implementation ==========================================*/

#define IMPL_make(TYPE) \
    (TYPE) { 0 }
#define IMPL_makeWith(TYPE, INITAL...) \
    (TYPE) { INITAL }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_create(TYPE) \
    (TYPE[1]) {           \
        { 0 }             \
    }
#define IMPL_createWith(TYPE, INITAL...) \
    (TYPE[1]) {                          \
        { INITAL }                       \
    }

#define IMPL_array(TYPE, INITAL...) \
    (TYPE[]) { INITAL }
#define IMPL_nArray(LENGTH, TYPE) \
    (TYPE[LENGTH]) { 0 }
#define IMPL_nArrayWith(LENGTH, TYPE, INITAL...) \
    (TYPE[(LENGTH)]) { INITAL }
// NOLINTEND(bugprone-macro-parentheses)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_STRUCT_INCLUDED */
