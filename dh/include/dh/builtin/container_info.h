/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    container_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-11 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Container and field relationship utilities
 * @details Provides type-safe container operations and field analysis utilities
 */

#ifndef BUILTIN_CONTAINER_INFO_INCLUDED
#define BUILTIN_CONTAINER_INFO_INCLUDED (1)

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "type_info.h"

/*========== Macros and Definitions =========================================*/

#define offsetTo(_T, _field...) \
    /** \
     * @brief Get byte offset of a field within a type at compile time \
     * \
     * @param _T The container type \
     * @param _field The field name within the container \
     * @return usize Byte offset of the field \
     */ \
    block_inline__offsetTo(_T, _field)

#define structPtrFrom(_p_field, _T, _field...) \
    /** \
     * @brief Get containing struct from pointer to member \
     * \
     * This is an optimized version that assumes type correctness. \
     * Use safeContainerOf for additional type checking. \
     * \
     * @param _p_field Pointer to the member field \
     * @param _T Type of the container struct \
     * @param _field Name of the member field \
     * @return Pointer to the containing struct \
     */ \
    block_inline__structPtrFrom(_p_field, _T, _field)

#define FieldTypeOf(_T, _field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _T The container type \
     * @param _field The field name \
     * @return The type of the specified field \
     */ \
    block_inline__FieldTypeOf(_T, _field)

#define FieldTypeOfUnqual(_T, _field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _T The container type \
     * @param _field The field name \
     * @return The type of the specified field \
     */ \
    block_inline__FieldTypeOfUnqual(_T, _field)

#define sameFieldType(_T, _field, _TExpected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _T The container type \
     * @param _field The field name \
     * @param _TExpected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    block_inline__sameFieldType(_T, _field, _TExpected)

#define sameFieldTypeUnqual(_T, _field, _TExpected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _T The container type \
     * @param _field The field name \
     * @param _TExpected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    block_inline__sameFieldTypeUnqual(_T, _field, _TExpected)

#define fieldPtrFrom(_p_struct, _field) \
    /** \
     * @brief Get pointer to a field within a container \
     * \
     * @param _p_struct Pointer to the container \
     * @param _field Name of the field to access \
     * @return Pointer to the field \
     */ \
    block_inline__fieldPtrFrom(_p_struct, _field)

#define fieldPadding(_T, _field) \
    /** \
     * @brief Calculate padding before a field in bytes \
     * \
     * @param _T The container type \
     * @param _field The field to check \
     * @return usize Number of padding bytes before the field \
     */ \
    block_inline__fieldPadding(pp_uniqTok(offset), pp_uniqTok(align), _T, _field)

/*========== Implementation Macros =========================================*/

#define block_inline__offsetTo(_T, _field...) \
    __builtin_offsetof(_T, _field)

#define block_inline__structPtrFrom(_p_field, _T, _field...) \
    as$((_T*)(as$((u8*)((FieldTypeOf(_T, _field)*, _p_field))) - offsetTo(_T, _field)))

#define block_inline__FieldTypeOf(_T, _field...) \
    TypeOf((as$((_T*)(0)))->_field)

#define block_inline__FieldTypeOfUnqual(_T, _field...) \
    TypeOfUnqual((as$((_T*)(0)))->_field)

#define block_inline__sameFieldType(_T, _field, _TExpected...) \
    isSameType$(FieldTypeOf(_T, _field), _TExpected)

#define block_inline__sameFieldTypeUnqual(_T, _field, _TExpected...) \
    isSameTypeUnqual(FieldTypeOfUnqual(_T, _field), _TExpected)

#define block_inline__fieldPtrFrom(_p_struct, _field...) \
    (&((_p_struct)->_field))

#define block_inline__fieldPadding(__offset, __align, _T, _field...) ({ \
    const usize __offset = offsetTo(_T, _field); \
    const usize __align  = alignOf(FieldTypeOf(_T, _field)); \
    __offset - (__offset & ~(__align - 1)); \
})

#define block_inline__fieldAnonTypeCastable(T_Generic, var_anon, T_FieldNamed, _Field...) \
    isSameType$(TypeOf(pp_join(_, T_FieldNamed, anonCast$)(FieldTypeOf(T_Generic, _Field), (var_anon)._Field)), FieldTypeOf(T_Generic, _Field_))

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
#include "dh/core.h"

struct Data {
    i32 id;
    u8  name[32];
    f64 value;
};

struct Container {
    struct Data data;
    i32         extra;
};

void example_usage(void) {
    struct Container cont     = { 0 };
    struct Data*     data_ptr = &cont.data;

    /* Container operations */
    struct Container* cont_ptr = containerOf(data_ptr, struct Container, data);
    struct Container* safe_ptr = safeContainerOf(data_ptr, struct Container, data);

    /* Field validation */
    claim_assert_static_msg(hasField(struct Data, value), "Data must have 'value' field");
    claim_assert_static_msg(validateField(struct Data, id, i32), "Data.id must be of type i32");

    /* Field analysis */
    usize name_padding = fieldPadding(struct Data, name);
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_CONTAINER_INFO_INCLUDED */
