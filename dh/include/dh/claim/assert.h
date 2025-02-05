/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.1
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
    comp_inline__claim_assert(_Expr)

#define claim_assert_true(_Expr)              \
    /**                                       \
     * @brief Assert that expression is true. \
     */                                       \
    comp_inline__claim_assert_true(_Expr)

#define claim_assert_false(_Expr)              \
    /**                                        \
     * @brief Assert that expression is false. \
     */                                        \
    comp_inline__claim_assert_false(_Expr)

#define claim_assert_eq(_Expr1, _Expr2)              \
    /**                                              \
     * @brief Assert that two expressions are equal. \
     */                                              \
    comp_inline__claim_assert_eq(_Expr1, _Expr2)

#define claim_assert_ne(_Expr1, _Expr2)                  \
    /**                                                  \
     * @brief Assert that two expressions are not equal. \
     */                                                  \
    comp_inline__claim_assert_ne(_Expr1, _Expr2)

#define claim_assert_null(_Expr)               \
    /**                                        \
     * @brief Assert that expressions is null. \
     */                                        \
    comp_inline__claim_assert_null(_Expr)

#define claim_assert_nonnull(_Expr)                \
    /**                                            \
     * @brief Assert that expressions is non null. \
     */                                            \
    comp_inline__claim_assert_nonnull(_Expr)


#define claim_assert_fmt(_Expr, _fmt...)                \
    /**                                                 \
     * @brief Assert expression with formatted message. \
     */                                                 \
    comp_inline__claim_assert_fmt(_Expr, _fmt)

#define claim_assert_true_fmt(_Expr, _fmt...)                        \
    /**                                                              \
     * @brief Assert that expression is true with formatted message. \
     */                                                              \
    comp_inline__claim_assert_true_fmt(_Expr, _fmt)

#define claim_assert_false_fmt(_Expr, _fmt...)                        \
    /**                                                               \
     * @brief Assert that expression is false with formatted message. \
     */                                                               \
    comp_inline__claim_assert_false_fmt(_Expr, _fmt)

#define claim_assert_eq_fmt(_Expr1, _Expr2, _fmt...)                        \
    /**                                                                     \
     * @brief Assert that two expressions are equal with formatted message. \
     */                                                                     \
    comp_inline__claim_assert_eq_fmt(_Expr1, _Expr2, _fmt)

#define claim_assert_ne_fmt(_Expr1, _Expr2, _fmt...)                            \
    /**                                                                         \
     * @brief Assert that two expressions are not equal with formatted message. \
     */                                                                         \
    comp_inline__claim_assert_ne_fmt(_Expr1, _Expr2, _fmt)

#define claim_assert_null_fmt(_Expr, _fmt...)                         \
    /**                                                               \
     * @brief Assert that expressions is null with formatted message. \
     */                                                               \
    comp_inline__claim_assert_null_fmt(_Expr, _fmt)

#define claim_assert_nonnull_fmt(_Expr, _fmt...)                          \
    /**                                                                   \
     * @brief Assert that expressions is non null with formatted message. \
     */                                                                   \
    comp_inline__claim_assert_nonnull_fmt(_Expr, _fmt)

/*========== Macros Implementation ==========================================*/

#if COMP_TIME
#define comp_inline__claim_assert(_Expr) \
    ignore((!!(_Expr)) || (claim_assert_fail(#_Expr, __func__, __FILE__, __LINE__), 0))

#define comp_inline__claim_assert_true(_Expr)        comp_inline__claim_assert_fmt((_Expr) == true, "%s is not true", #_Expr)
#define comp_inline__claim_assert_false(_Expr)       comp_inline__claim_assert_fmt((_Expr) == false, "%s is not false", #_Expr)
#define comp_inline__claim_assert_eq(_Expr1, _Expr2) comp_inline__claim_assert_fmt((_Expr1) == (_Expr2), "%s is not equal to %s", #_Expr1, #_Expr2)
#define comp_inline__claim_assert_ne(_Expr1, _Expr2) comp_inline__claim_assert_fmt((_Expr1) != (_Expr2), "%s is equal to %s", #_Expr1, #_Expr2)
#define comp_inline__claim_assert_null(_Expr)        comp_inline__claim_assert_fmt((_Expr) == null, "%s is non null", #_Expr)
#define comp_inline__claim_assert_nonnull(_Expr)     comp_inline__claim_assert_fmt((_Expr) != null, "%s is null", #_Expr)

#define comp_inline__claim_assert_fmt(_Expr, ...) \
    ignore((!!(_Expr)) || (claim_assert_fail_fmt(#_Expr, __func__, __FILE__, __LINE__, __VA_ARGS__), 0))

#define comp_inline__claim_assert_true_fmt(_Expr, ...)        comp_inline__claim_assert_fmt((_Expr) == true, __VA_ARGS__)
#define comp_inline__claim_assert_false_fmt(_Expr, ...)       comp_inline__claim_assert_fmt((_Expr) == false, __VA_ARGS__)
#define comp_inline__claim_assert_eq_fmt(_Expr1, _Expr2, ...) comp_inline__claim_assert_fmt((_Expr1) == (_Expr2), __VA_ARGS__)
#define comp_inline__claim_assert_ne_fmt(_Expr1, _Expr2, ...) comp_inline__claim_assert_fmt((_Expr1) != (_Expr2), __VA_ARGS__)
#define comp_inline__claim_assert_null_fmt(_Expr, ...)        comp_inline__claim_assert_fmt((_Expr) == null, __VA_ARGS__)
#define comp_inline__claim_assert_nonnull_fmt(_Expr, ...)     comp_inline__claim_assert_fmt((_Expr) != null, __VA_ARGS__)
#else
force_inline void comp_inline__claim_assert(bool);

force_inline void comp_inline__claim_assert_true(bool);
force_inline void comp_inline__claim_assert_false(bool);
force_inline void comp_inline__claim_assert_eq(bool, bool);
force_inline void comp_inline__claim_assert_ne(bool, bool);
force_inline void comp_inline__claim_assert_null(bool);
force_inline void comp_inline__claim_assert_nonnull(bool);

force_inline void comp_inline__claim_assert_fmt(bool, const char*, ...);

force_inline void comp_inline__claim_assert_true_fmt(bool, const char*, ...);
force_inline void comp_inline__claim_assert_false_fmt(bool, const char*, ...);
force_inline void comp_inline__claim_assert_eq_fmt(bool, bool, const char*, ...);
force_inline void comp_inline__claim_assert_ne_fmt(bool, bool, const char*, ...);
force_inline void comp_inline__claim_assert_null_fmt(bool, const char*, ...);
force_inline void comp_inline__claim_assert_nonnull_fmt(bool, const char*, ...);
#endif /* COMP_TIME */

/*========== Extern Function Prototypes =====================================*/

#if COMP_TIME
/**
 * @brief Logs an assertion failure with the given expression, function, file, and line.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 */
extern void claim_assert_fail(const char* /* expr */, const char* /* func */, const char* /* file */, i32 /* line */);
/**
 * @brief Logs an assertion failure with the given expression, function, file, line, and formatted message.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param fmt The formatted message to include in the assertion failure.
 * @param ... The arguments for the formatted message.
 */
extern void claim_assert_fail_fmt(const char* /* expr */, const char* /* func */, const char* /* file */, i32 /* line */, const char* /* fmt */, ...);
#endif /* COMP_TIME */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_ASSERT_INCLUDED */
