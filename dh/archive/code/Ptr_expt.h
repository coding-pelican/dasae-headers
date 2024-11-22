// /**
//  * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
//  *
//  * @file    Ptr_expt.h
//  * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
//  * @date    2024-11-19 (date of creation)
//  * @updated 2024-11-19 (date of last update)
//  * @version v1.0.0
//  * @ingroup dasae-headers(dh)/core
//  * @prefix  Ptr
//  *
//  * @brief   Header of some software
//  * @details Some detailed explanation
//  */


// #ifndef CORE_PTR_INCLUDED
// #define CORE_PTR_INCLUDED (1)
// #if defined(__cplusplus)
// extern "C" {
// #endif /* defined(__cplusplus) */

// /*========== Includes =======================================================*/

// #include "prim.h"

// /*========== Macros and Definitions =========================================*/

// /* Core types */
// typedef struct Ptr {
//     anyptr raw_;
//     usize  size_;
// } Ptr;

// /* Constants */
// #define Ptr_null IMPL_Ptr_null

// /* Construction and validation */
// extern Ptr Ptr_from(anyptr raw, usize size);
// #define Ptr_addr(_var)                                \
//     /* Create Ptr from variable (Address of variable) \
//      * i32 variable = 10;                             \
//      * Ptr_addr(variable);                            \
//      */                                               \
//     IMPL_Ptr_addr(_var)
// #define Ptr_make(_val)                                     \
//     /* Create Ptr from value (Address of compound literal) \
//      * Ptr_make(literal(i32, 10));                         \
//      */                                                    \
//     IMPL_Ptr_make(_val)
// extern Ptr  Ptr_withAlloc(usize size);        // Allocates new memory (Not cleared)
// extern Ptr  Ptr_withAllocCleared(usize size); // Allocates new memory (Cleared)
// extern Ptr  Ptr_nullWithSize(usize size);     // Explicit null with size
// extern bool Ptr_isValid(Ptr p);               // Validates pointer state
// extern bool Ptr_isNull(Ptr p);
// extern bool Ptr_hasMinSize(Ptr p, usize required_size);

// /* Metadata access */
// extern usize  Ptr_size(Ptr p);
// extern anyptr Ptr_raw(Ptr p);
// extern bool   Ptr_hasData(Ptr p);

// /* type casting */
// #define Ptr_as(TYPE, _p)    \
//     /* Fast type casting */ \
//     IMPL_Ptr_as(TYPE, _p)
// #define Ptr_cast(TYPE, _p)  \
//     /* Safe type casting */ \
//     IMPL_Ptr_raw(TYPE, _p)

// /*========== Macro Implementations ==========================================*/

// #define IMPL_Ptr_null ((Ptr){ .raw_ = null, .size_ = 0 })

// #define IMPL_Ptr_addr(_var) \
//     Ptr_from(&(_var), sizeof(_var))
// #define IMPL_Ptr_make(_val) \
//     Ptr_from(&(_var), sizeof(_var))

// #define IMPL_Ptr_as(TYPE, _p) \
//     (TYPE*)Ptr_raw(_p)
// #define IMPL_Ptr_cast(TYPE, _p) \
//     Ptr_isValid(_p) && Ptr_hasMinSize(_p, sizeof(TYPE)) ? (TYPE*)Ptr_raw(_p) : null


// #if defined(__cplusplus)
// } /* extern "C" */
// #endif /* defined(__cplusplus) */
// #endif /* CORE_PTR_INCLUDED */
