#ifndef CORE_CFG_INCLUDED
#define CORE_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "dh/builtin.h"
#include "dh/claim/assert_static.h"

/*========== Macros and Definitions =========================================*/

#define fallthrough                             \
    /**                                         \
     * @brief fallthrough for `switch` internal \
     */                                         \
    IMPL_fallthrough

#define unused(EXPR...)                                                   \
    /**                                                                   \
     * @brief Marks variables or expressions as intentionally unused      \
    to suppress compiler warnings                                         \
     * @param EXPR... Variable number of arguments to be marked as unused \
     */                                                                   \
    IMPL_unused(EXPR)

#define ignore                                                           \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    IMPL_ignore

#define as(TYPE, VAL) \
    IMPL_as(TYPE, VAL)

#define literal(TYPE, INIT...)                                               \
    /**                                                                      \
     * @brief Literal macro for creating a compound literal                  \
     * @details Plain structures in C++ (without constructors) can be        \
        initialized with { } This is called aggregate initialization (C++11) \
     * @note MSVC C++ compiler does not support compound literals (C99)      \
     */                                                                      \
    IMPL_literal(TYPE, INIT)

/*========== Macros Implementation ==========================================*/

#define IMPL_fallthrough

#define IMPL_unused(EXPR...) \
    ((void)(EXPR))

#define IMPL_ignore \
    (void)

#if defined(__cplusplus)
#define IMPL_as(TYPE, VAL) \
    static_cast<TYPE>(VAL)
#else
#define IMPL_as(TYPE, VAL) \
    ((TYPE)(VAL))
#endif

#if defined(__cplusplus)
#define IMPL_literal(TYPE, INIT...) \
    TYPE { INIT }
#else
#define IMPL_literal(TYPE, INIT...) \
    (TYPE) { INIT }
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* CORE_CFG_INCLUDED */
