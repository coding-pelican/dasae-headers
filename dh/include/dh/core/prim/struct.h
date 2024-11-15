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


#ifndef CORE_PRIM_STRUCT_INCLUDED
#define CORE_PRIM_STRUCT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define make(TYPE)              IMPL_make(TYPE)
#define makeWith(TYPE, INIT...) IMPL_makeWith(TYPE, INIT)

#define create(TYPE)              IMPL_create(TYPE)
#define createWith(TYPE, INIT...) IMPL_createWith(TYPE, INIT)

#define array(TYPE, INIT...)              IMPL_array(TYPE, INIT)
#define nArray(LENGTH, TYPE)              IMPL_nArray(TYPE, LENGTH)
#define nArrayWith(LENGTH, TYPE, INIT...) IMPL_nArrayWith(TYPE, LENGTH, INIT)

/*========== Macros Implementation ==========================================*/

#define IMPL_make(TYPE) \
    literal(TYPE) { 0 }
#define IMPL_makeWith(TYPE, INIT...) \
    literal(TYPE) { INIT }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_create(TYPE) \
    ((TYPE[1]){           \
        literal(TYPE){ 0 } })
#define IMPL_createWith(TYPE, INIT...) \
    ((TYPE[1]){                        \
        literal(TYPE){ INIT } })

#define IMPL_array(TYPE, INIT...) \
    ((TYPE[]){ INIT })
#define IMPL_nArray(LENGTH, TYPE) \
    ((TYPE[LENGTH]){ 0 })
#define IMPL_nArrayWith(LENGTH, TYPE, INIT...) \
    ((TYPE[(LENGTH)]){ INIT })
// NOLINTEND(bugprone-macro-parentheses)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */
