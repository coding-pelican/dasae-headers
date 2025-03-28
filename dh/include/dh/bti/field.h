/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    field.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_field
 *
 * @brief   Field relationship and access utilities
 * @details Provides type-safe field operations and analysis utilities.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_FIELD_INCLUDED
#define BTI_FIELD_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "type.h"
#include "comp_ops.h"

/*========== Macros and Declarations =========================================*/

/* Field Location Utilities */

#define bti_field_offset$(_Type, _field)                               \
    /**                                                                \
     * @brief Get byte offset of a field within a type at compile time \
     * @param _Type The containing type                                \
     * @param _field The field name                                    \
     * @return usize Byte offset of the field                          \
     */                                                                \
    comp_impl__bti_field_offset$(_Type, _field)
#define bti_field_parentPtr(_ptr, _Type, _field)              \
    /**                                                       \
     * @brief Get pointer to parent struct from field pointer \
     * @param _ptr Pointer to the field                       \
     * @param _Type Type of the parent struct                 \
     * @param _field Name of the field                        \
     * @return Pointer to the parent struct                   \
     */                                                       \
    comp_impl__bti_field_parentPtr(_ptr, _Type, _field)
#define bti_field_parentPtrSafe(_ptr, _Type, _field) \
    /**                                              \
     * @brief Type-safe version of field_parentPtr   \
     * @param _ptr Pointer to the field              \
     * @param _Type Type of the parent struct        \
     * @param _field Name of the field               \
     * @return Pointer to the parent struct          \
     */                                              \
    comp_impl__bti_field_parentPtrSafe(_ptr, _Type, _field)

/* Field Type Analysis */

#define bti_field_TypeOf$(_Type, _field)       \
    /**                                        \
     * @brief Get the type of a field          \
     * @param _Type The containing type        \
     * @param _field The field name            \
     * @return The type of the specified field \
     */                                        \
    comp_impl__bti_field_TypeOf$(_Type, _field)
#define bti_field_exists$(_Type, _field)           \
    /**                                            \
     * @brief Check if a type has a specific field \
     * @param _Type The type to check              \
     * @param _field The field name to look for    \
     * @return bool True if the field exists       \
     */                                            \
    comp_impl__bti_field_exists$(_Type, _field)
#define bti_field_matchesType$(_Type, _field, _ExpectedType)     \
    /**                                                          \
     * @brief Validate that a field's type matches expected type \
     * @param _Type The containing type                          \
     * @param _field The field name to check                     \
     * @param _ExpectedType The type to compare against          \
     * @return bool True if types match                          \
     */                                                          \
    comp_impl__bti_field_matchesType$(_Type, _field, _ExpectedType)

/* Field Access Utilities */

#define bti_field_ptr(_parent_ptr, _field)                 \
    /**                                                    \
     * @brief Get pointer to a field                       \
     * @param _parent_ptr Pointer to the containing struct \
     * @param _field Name of the field to access           \
     * @return Pointer to the field                        \
     */                                                    \
    comp_impl__bti_field_ptr(_parent_ptr, _field)
#define bti_field_padding$(_Type, _field)                     \
    /**                                                       \
     * @brief Calculate padding before a field in bytes       \
     * @param _Type The containing type                       \
     * @param _field The field to check                       \
     * @return usize Number of padding bytes before the field \
     */                                                       \
    comp_impl__bti_field_padding$(_Type, _field)

/*========== Macros and Definitions =========================================*/

/* Field Location Utilities - Implementations */

#define comp_impl__bti_field_offset$(_Type, _field) __builtin_offsetof(_Type, _field)
#define comp_impl__bti_field_parentPtr(_ptr, _Type, _field)                                 \
    bti_comp_ops_eval({                                                                     \
        const bti_type_TypeOf((((_Type*)null)->_field))* __mptr = (_ptr);                   \
        bti_comp_ops_eval_return((_Type*)((u8*)__mptr - bti_field_offset$(_Type, _field))); \
    })
#define comp_impl__bti_field_parentPtrSafe(_ptr, _Type, _field)                             \
    bti_comp_ops_eval({                                                                     \
        bti_type_TypeOf(_ptr) __ptr                       = (_ptr);                         \
        bti_type_TypeOf((((_Type*)null)->_field))* __mptr = __ptr;                          \
        bti_comp_ops_eval_return((_Type*)((u8*)__mptr - bti_field_offset$(_Type, _field))); \
    })

/* Field Type Analysis - Implementations */

#define comp_impl__bti_field_TypeOf$(_Type, _field) bti_type_TypeOf$(((_Type*)null)->_field)
#define comp_impl__bti_field_exists$(_Type, _field)       \
    bti_comp_ops_eval({                                   \
        _Generic(                                         \
            (_Type){ 0 },                                 \
            default: (bti_type_isConstant(                \
                bti_field_offset$(                        \
                    bti_type_TypeOf((_Type){ 0 }), _field \
                )                                         \
            ))                                            \
        );                                                \
    })
#define comp_impl__bti_field_matchesType$(_Type, _field, _ExpectedType) \
    bti_type_isSame$(                                                   \
        bti_field_TypeOf$(_Type, _field),                               \
        _ExpectedType                                                   \
    )

/* Field Access Utilities - Implementations */

#define comp_impl__bti_field_ptr(_parent_ptr, _field) (&(_parent_ptr)->_field)
#define comp_impl__bti_field_padding$(_Type, _field)                      \
    bti_comp_ops_eval({                                                   \
        const usize __offset = bti_field_offset$(_Type, _field);          \
        const usize __align  = bti_type_alignOf$(                         \
            bti_field_TypeOf$(_Type, _field)                             \
        );                                                               \
        bti_comp_ops_eval_return(__offset - (__offset & ~(__align - 1))); \
    })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_FIELD_INCLUDED */
