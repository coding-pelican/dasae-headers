/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    container_info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
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

/* Container Operations */
#define offsetTo(_Type, _field)                                        \
    /**                                                                \
     * @brief Get byte offset of a field within a type at compile time \
     *                                                                 \
     * @param _Type The container type                                 \
     * @param _field The field name within the container               \
     * @return usize Byte offset of the field                          \
     */                                                                \
    FUNC__offsetTo(_Type, _field)

#define containerOf(_ptr, _Type, _field)                           \
    /**                                                            \
     * @brief Get containing struct from pointer to member         \
     *                                                             \
     * This is an optimized version that assumes type correctness. \
     * Use safeContainerOf for additional type checking.           \
     *                                                             \
     * @param _ptr Pointer to the member field                     \
     * @param _Type Type of the container struct                   \
     * @param _field Name of the member field                      \
     * @return Pointer to the containing struct                    \
     */                                                            \
    FUNC__containerOf(_ptr, _Type, _field)

#define safeContainerOf(_ptr, _Type, _field)                    \
    /**                                                         \
     * @brief Type-safe version of containerOf                  \
     *                                                          \
     * Includes additional type checking to ensure type safety. \
     * Preferred over containerOf when type safety is critical. \
     *                                                          \
     * @param _ptr Pointer to the member field                  \
     * @param _Type Type of the container struct                \
     * @param _field Name of the member field                   \
     * @return Pointer to the containing struct                 \
     */                                                         \
    FUNC__safeContainerOf(_ptr, _Type, _field)

/* Field Type Operations */
#define FieldTypeOf(_Type, _field)                            \
    /**                                                       \
     * @brief Get the type of a field within a container type \
     *                                                        \
     * @param _Type The container type                        \
     * @param _field The field name                           \
     * @return The type of the specified field                \
     */                                                       \
    FUNC__FieldTypeOf(_Type, _field)

/* Field Validation */
#define hasField(_Type, _field)                                    \
    /**                                                            \
     * @brief Check if a type has a specific field at compile time \
     *                                                             \
     * @param _Type The type to check                              \
     * @param _field The field name to look for                    \
     * @return bool True if the field exists                       \
     */                                                            \
    FUNC__hasField(_Type, _field)

#define validateField(_Type, _field, _ExpectedType)                  \
    /**                                                              \
     * @brief Validate that a field's type matches the expected type \
     *                                                               \
     * @param _Type The container type                               \
     * @param _field The field name to check                         \
     * @param _ExpectedType The type to compare against              \
     * @return bool True if types match                              \
     */                                                              \
    FUNC__validateField(_Type, _field, _ExpectedType)

/* Field Access Utilities */
#define fieldPtrFrom(_container_ptr, _field)            \
    /**                                                 \
     * @brief Get pointer to a field within a container \
     *                                                  \
     * @param _container_ptr Pointer to the container   \
     * @param _field Name of the field to access        \
     * @return Pointer to the field                     \
     */                                                 \
    FUNC__fieldPtrFrom(_container_ptr, _field)

#define fieldPadding(_Type, _field)                           \
    /**                                                       \
     * @brief Calculate padding before a field in bytes       \
     *                                                        \
     * @param _Type The container type                        \
     * @param _field The field to check                       \
     * @return usize Number of padding bytes before the field \
     */                                                       \
    FUNC__fieldPadding(_Type, _field)

/*========== Implementation Macros =========================================*/

/* Core operations implementations */
#define FUNC__offsetTo(Type, field) \
    __builtin_offsetof(Type, field)

#define FUNC__containerOf(ptr, Type, field) eval({             \
    const TypeOf(((Type*)0)->field)* __mptr = (ptr);           \
    eval_return((Type*)((u8*)__mptr - offsetTo(Type, field))); \
})

#define FUNC__safeContainerOf(ptr, Type, field) eval({         \
    TypeOf(ptr) __ptr                 = (ptr);                 \
    TypeOf(((Type*)0)->field)* __mptr = __ptr;                 \
    eval_return((Type*)((u8*)__mptr - offsetTo(Type, field))); \
})

/* Field operations implementations */
#define FUNC__FieldTypeOf(Type, field) \
    TypeOf(((Type*)0)->field)

#define FUNC__hasField(Type, field) eval({                              \
    _Generic(                                                           \
        (Type){ 0 },                                                    \
        default: (isConstantExpr(offsetTo(TypeOf((Type){ 0 }), field))) \
    );                                                                  \
})

#define FUNC__validateField(Type, field, ExpectedType) \
    isSameType(FieldTypeOf(Type, field), ExpectedType)

#define FUNC__fieldPtrFrom(container_ptr, field) \
    (&(container_ptr)->field)

#define FUNC__fieldPadding(Type, field) eval({                \
    const usize __offset = offsetTo(Type, field);             \
    const usize __align  = alignOf(FieldTypeOf(Type, field)); \
    eval_return(__offset - (__offset & ~(__align - 1)));      \
})

#define fieldAnonTypeCastable(T_Generic, var_anon, T_FieldNamed, _Field...) \
    isSameType(TypeOf(pp_join(_, T_FieldNamed, anonCast$)(FieldTypeOf(T_Generic, _Field), (var_anon)._Field)), FieldTypeOf(T_Generic, _Field))

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
