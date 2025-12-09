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
 * @prefix  (none)
 *
 * @brief   Container and field relationship utilities
 * @details Provides type-safe container operations and field analysis utilities
 */
#ifndef builtin_container_info__included
#define builtin_container_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "type_info.h"

/*========== Macros and Declarations ========================================*/

#define FieldType$(_T_Record, _field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _T_Record The container type \
     * @param _field The field name \
     * @return The type of the specified field \
     */ \
    ____FieldType$(_T_Record, _field)
#define FieldTypeUnqual$(_T_Record, _field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _T_Record The container type \
     * @param _field The field name \
     * @return The type of the specified field \
     */ \
    ____FieldTypeUnqual$(_T_Record, _field)

#define FieldType_eq$(_T_Record, _field, _T_Expected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _T_Record The container type \
     * @param _field The field name \
     * @param _T_Expected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    ____FieldType_eq$(_T_Record, _field, _T_Expected)
#define FieldType_eqUnqual$(_T_Record, _field, _T_Expected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _T_Record The container type \
     * @param _field The field name \
     * @param _T_Expected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    ____FieldType_eqUnqual$(_T_Record, _field, _T_Expected)

#define offsetTo(_T_Record, _field...) \
    /** \
     * @brief Get byte offset of a field within a type at compile time \
     * \
     * @param _T_Record The container type \
     * @param _field The field name within the container \
     * @return usize Byte offset of the field \
     */ \
    ____offsetTo(_T_Record, _field)
#define fieldPtr(_p_record, _field) \
    /** \
     * @brief Get pointer to a field within a container \
     * \
     * @param _p_record Pointer to the container \
     * @param _field Name of the field to access \
     * @return Pointer to the field \
     */ \
    ____fieldPtr(_p_record, _field)
#define recordPtr(_p_field, _T_Record, _field...) \
    /** \
     * @brief Get containing struct from pointer to member \
     * \
     * This is an optimized version that assumes type correctness. \
     * Use safeContainerOf for additional type checking. \
     * \
     * @param _p_field Pointer to the member field \
     * @param _T_Record Type of the container struct \
     * @param _field Name of the member field \
     * @return Pointer to the containing struct \
     */ \
    ____recordPtr(_p_field, _T_Record, _field)
#define fieldPadding$(_T_Record, _field) \
    /** \
     * @brief Calculate padding before a field in bytes \
     * \
     * @param _T_Record The container type \
     * @param _field The field to check \
     * @return usize Number of padding bytes before the field \
     */ \
    ____fieldPadding$(pp_uniqTok(offset), pp_uniqTok(align), _T_Record, _field)

/*========== Macros and Definitions =========================================*/

#define ____FieldType$(_T_Record, _field...) TypeOf((as$(_T_Record*)(null))->_field)
#define ____FieldTypeUnqual$(_T_Record, _field...) TypeOfUnqual((as$(_T_Record*)(null))->_field)

#define ____FieldType_eq$(_T_Record, _field, _T_Expected...) Type_eq$(FieldType$(_T_Record, _field), _T_Expected)
#define ____FieldType_eqUnqual$(_T_Record, _field, _T_Expected...) Type_eqUnqual$(FieldTypeUnqual$(_T_Record, _field), _T_Expected)

#define ____offsetTo(_T_Record, _field...) (as$(isize)(__builtin_offsetof(_T_Record, _field)))
#define ____fieldPtr(_p_record, _field...) (&((_p_record)->_field))
#define ____recordPtr(_p_field, _T_Record, _field...) (as$(_T_Record*)(as$(u8*)(as$(FieldType$(_T_Record, _field)*)(_p_field)) - offsetTo(_T_Record, _field)))
#define ____fieldPadding$(__offset, __align, _T_Record, _field...) ({ \
    const usize __offset = offsetTo(_T_Record, _field); \
    const usize __align = alignOf$(FieldType$(_T_Record, _field)); \
    __offset - (__offset & ~(__align - 1)); \
})

// #define ____fieldAnonTypeCastable(T_Generic, var_anon, T_FieldNamed, _Field...) \
//     Type_eq$(TypeOf(pp_join(_, T_FieldNamed, anonCast$)(FieldType$(T_Generic, _Field), (var_anon)._Field)), FieldType$(T_Generic, _Field_))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_container_info__included */
