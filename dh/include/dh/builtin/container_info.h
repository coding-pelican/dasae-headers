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
#pragma once
#ifndef builtin_container_info__included
#define builtin_container_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "type_info.h"

/*========== Macros and Declarations ========================================*/

#define memberName$(_$path, _$Tok...) __expr__memberName$(_$path, _$Tok)
#define __expr__memberName$(_$path, _$Tok...) #_$Tok

#define fieldName$(_$path, _$Tok...) __expr__fieldName$( \
    FieldType$(TypeOf(_$path), _$Tok), _$Tok \
)
#define __expr__fieldName$(_$T_Path_Tok, _$Tok...) #_$Tok

#define FieldType$(_$T_Record, _$field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field name \
     * @return The type of the specified field \
     */ \
    ____FieldType$(_$T_Record, _$field)
#define FieldTypeUnqual$(_$T_Record, _$field...) \
    /** \
     * @brief Get the type of a field within a container type \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field name \
     * @return The type of the specified field \
     */ \
    ____FieldTypeUnqual$(_$T_Record, _$field)

#define eqlFieldType$(_$T_Record, _$field, _$T_Expected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field name \
     * @param _$T_Expected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    ____eqlFieldType$(_$T_Record, _$field, _$T_Expected)
#define eqlFieldTypeUnqual$(_$T_Record, _$field, _$T_Expected...) \
    /** \
     * @brief Check if a field has the expected type \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field name \
     * @param _$T_Expected The expected type \
     * @return True if the field has the expected type, false otherwise \
     */ \
    ____eqlFieldTypeUnqual$(_$T_Record, _$field, _$T_Expected)

#define offsetTo(_$T_Record, _$field...) \
    /** \
     * @brief Get byte offset of a field within a type at compile time \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field name within the container \
     * @return isize Byte offset of the field \
     */ \
    ____offsetTo(_$T_Record, _$field)
#define fieldPtr(_$p_record, _$field) \
    /** \
     * @brief Get pointer to a field within a container \
     * \
     * @param _$p_record Pointer to the container \
     * @param _$field Name of the field to access \
     * @return Pointer to the field \
     */ \
    ____fieldPtr(_$p_record, _$field)
#define recordPtr(_$p_field, _$T_Record, _$field...) \
    /** \
     * @brief Get containing struct from pointer to member \
     * \
     * This is an optimized version that assumes type correctness. \
     * Use safeContainerOf for additional type checking. \
     * \
     * @param _$p_field Pointer to the member field \
     * @param _$T_Record Type of the container struct \
     * @param _$field Name of the member field \
     * @return Pointer to the containing struct \
     */ \
    ____recordPtr(_$p_field, _$T_Record, _$field)
#define fieldPadding$(_$T_Record, _$field) \
    /** \
     * @brief Calculate padding before a field in bytes \
     * \
     * @param _$T_Record The container type \
     * @param _$field The field to check \
     * @return usize Number of padding bytes before the field \
     */ \
    ____fieldPadding$(pp_uniqTok(offset), pp_uniqTok(align), _$T_Record, _$field)

/*========== Macros and Definitions =========================================*/

#define ____FieldType$(_$T_Record, _$field...) TypeOf((as$(_$T_Record*)(null))->_$field)
#define ____FieldTypeUnqual$(_$T_Record, _$field...) TypeOfUnqual((as$(_$T_Record*)(null))->_$field)

#define ____eqlFieldType$(_$T_Record, _$field, _$T_Expected...) eqlType$(FieldType$(_$T_Record, _$field), _$T_Expected)
#define ____eqlFieldTypeUnqual$(_$T_Record, _$field, _$T_Expected...) Type_eqUnqual$(FieldTypeUnqual$(_$T_Record, _$field), _$T_Expected)

#define ____offsetTo(_$T_Record, _$field...) (as$(usize)(__builtin_offsetof(_$T_Record, _$field)))
#define ____fieldPtr(_$p_record, _$field...) (&((_$p_record)->_$field))
#define ____recordPtr(_$p_field, _$T_Record, _$field...) ptrAlignCast$((_$T_Record*)(as$(u8*)(as$(FieldType$(_$T_Record, _$field)*)(_$p_field)) - offsetTo(_$T_Record, _$field)))
#define ____fieldPadding$(__offset, __align, _$T_Record, _$field...) ({ \
    const usize __offset = offsetTo(_$T_Record, _$field); \
    const usize __align = alignOf$(FieldType$(_$T_Record, _$field)); \
    __offset - (__offset & ~(__align - 1)); \
})

#if UNUSED_CODE
#define ____fieldAnonTypeCastable(T_Generic, var_anon, T_FieldNamed, _Field...) \
    eqlType$(TypeOf(pp_join(_, T_FieldNamed, anonCast$)(FieldType$(T_Generic, _Field), (var_anon)._Field)), FieldType$(T_Generic, _Field_))
#endif /* UNUSED_CODE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_container_info__included */
