/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_assert
 *
 * @brief   Header for unconditional assertion checks
 * @details Provides assertion macros that work regardless of debug settings
 */

#ifndef CLAIM_ASSERT_INCLUDED
#define CLAIM_ASSERT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim.h"
#include "dh/core/pp.h"

/*========== Macros and Definitions =========================================*/

#define claim_assert(_Expr)      \
    /**                          \
     * @brief Assert expression. \
     */                          \
    IMPL_claim_assert(_Expr)

#define claim_assert_true(_Expr)              \
    /**                                       \
     * @brief Assert that expression is true. \
     */                                       \
    IMPL_claim_assert_true(_Expr)

#define claim_assert_false(_Expr)              \
    /**                                        \
     * @brief Assert that expression is false. \
     */                                        \
    IMPL_claim_assert_false(_Expr)

#define claim_assert_eq(_Expr1, _Expr2)              \
    /**                                              \
     * @brief Assert that two expressions are equal. \
     */                                              \
    IMPL_claim_assert_eq(_Expr1, _Expr2)

#define claim_assert_ne(_Expr1, _Expr2)                  \
    /**                                                  \
     * @brief Assert that two expressions are not equal. \
     */                                                  \
    IMPL_claim_assert_ne(_Expr1, _Expr2)

#define claim_assert_null(_Expr)               \
    /**                                        \
     * @brief Assert that expressions is null. \
     */                                        \
    IMPL_claim_assert_null(_Expr)

#define claim_assert_nonnull(_Expr)                \
    /**                                            \
     * @brief Assert that expressions is non null. \
     */                                            \
    IMPL_claim_assert_nonnull(_Expr)


#define claim_assert_fmt(_Expr, _fmt...)                \
    /**                                                 \
     * @brief Assert expression with formatted message. \
     */                                                 \
    IMPL_claim_assert_fmt(_Expr, _fmt)

#define claim_assert_true_fmt(_Expr, _fmt...)                        \
    /**                                                              \
     * @brief Assert that expression is true with formatted message. \
     */                                                              \
    IMPL_claim_assert_true_fmt(_Expr, _fmt)

#define claim_assert_false_fmt(_Expr, _fmt...)                        \
    /**                                                               \
     * @brief Assert that expression is false with formatted message. \
     */                                                               \
    IMPL_claim_assert_false_fmt(_Expr, _fmt)

#define claim_assert_eq_fmt(_Expr1, _Expr2, _fmt...)                        \
    /**                                                                     \
     * @brief Assert that two expressions are equal with formatted message. \
     */                                                                     \
    IMPL_claim_assert_eq_fmt(_Expr1, _Expr2, _fmt)

#define claim_assert_ne_fmt(_Expr1, _Expr2, _fmt...)                            \
    /**                                                                         \
     * @brief Assert that two expressions are not equal with formatted message. \
     */                                                                         \
    IMPL_claim_assert_ne_fmt(_Expr1, _Expr2, _fmt)

#define claim_assert_null_fmt(_Expr, _fmt...)                         \
    /**                                                               \
     * @brief Assert that expressions is null with formatted message. \
     */                                                               \
    IMPL_claim_assert_null_fmt(_Expr, _fmt)

#define claim_assert_nonnull_fmt(_Expr, _fmt...)                          \
    /**                                                                   \
     * @brief Assert that expressions is non null with formatted message. \
     */                                                                   \
    IMPL_claim_assert_nonnull_fmt(_Expr, _fmt)

/*========== Macros Implementation ==========================================*/

#define IMPL_claim_assert(_Expr) \
    ignore((!!(_Expr)) || (claim_assert__fail(#_Expr, __func__, __FILE__, __LINE__), 0))

#define IMPL_claim_assert_true(_Expr)        IMPL_claim_assert_fmt((_Expr) == true, "%s is not true", #_Expr)
#define IMPL_claim_assert_false(_Expr)       IMPL_claim_assert_fmt((_Expr) == false, "%s is not false", #_Expr)
#define IMPL_claim_assert_eq(_Expr1, _Expr2) IMPL_claim_assert_fmt((_Expr1) == (_Expr2), "%s is not equal to %s", #_Expr1, #_Expr2)
#define IMPL_claim_assert_ne(_Expr1, _Expr2) IMPL_claim_assert_fmt((_Expr1) != (_Expr2), "%s is equal to %s", #_Expr1, #_Expr2)
#define IMPL_claim_assert_null(_Expr)        IMPL_claim_assert_fmt((_Expr) == null, "%s is non null", #_Expr)
#define IMPL_claim_assert_nonnull(_Expr)     IMPL_claim_assert_fmt((_Expr) != null, "%s is null", #_Expr)

#define IMPL_claim_assert_fmt(_Expr, ...) \
    ignore((!!(_Expr)) || (claim_assert__fail_fmt(#_Expr, __func__, __FILE__, __LINE__, __VA_ARGS__), 0))

#define IMPL_claim_assert_true_fmt(_Expr, ...)        IMPL_claim_assert_fmt((_Expr) == true, __VA_ARGS__)
#define IMPL_claim_assert_false_fmt(_Expr, ...)       IMPL_claim_assert_fmt((_Expr) == false, __VA_ARGS__)
#define IMPL_claim_assert_eq_fmt(_Expr1, _Expr2, ...) IMPL_claim_assert_fmt((_Expr1) == (_Expr2), __VA_ARGS__)
#define IMPL_claim_assert_ne_fmt(_Expr1, _Expr2, ...) IMPL_claim_assert_fmt((_Expr1) != (_Expr2), __VA_ARGS__)
#define IMPL_claim_assert_null_fmt(_Expr, ...)        IMPL_claim_assert_fmt((_Expr) == null, __VA_ARGS__)
#define IMPL_claim_assert_nonnull_fmt(_Expr, ...)     IMPL_claim_assert_fmt((_Expr) != null, __VA_ARGS__)

/*========== Extern Function Prototypes =====================================*/

extern void claim_assert__fail(const char* expr, const char* func, const char* file, i32 line);
extern void claim_assert__fail_fmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_INCLUDED */
