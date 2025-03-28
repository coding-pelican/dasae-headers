/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp_ops.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-03 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.4
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_comp_ops
 *
 * @brief   Compiler-specific operations and optimizations
 * @details Provides compiler-specific operations, type casting, and optimization hints.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_COMP_OPS_INCLUDED
#define BTI_COMP_OPS_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "arch.h"
#include "comp_cfg.h"
#include "comp_attr.h"
#include "lang.h"
#include "plat.h"

/*========== Macros and Declarations =========================================*/

/* Type Operations */

#define bti_comp_ops_as$(T_Dest, val_src...) \
    /**                                      \
     * @brief Cast value to specified type   \
     * @param T_Dest Destination type        \
     * @param val_src Value to cast          \
     * @return Value cast to T_Dest          \
     */                                      \
    comp_impl__bti_comp_ops_as$(T_Dest, val_src)

#define bti_comp_ops_lit$(T_Lit, _Initial...)           \
    /**                                                 \
     * @brief Create compound literal of specified type \
     * @param T_Lit Type of compound literal            \
     * @param _Initial Initial values                   \
     * @return Compound literal of type T_Lit           \
     */                                                 \
    comp_impl__bti_comp_ops_lit$(T_Lit, _Initial)

#define bti_comp_ops_initial(_Initial...) \
    /**                                   \
     * @brief Create initializer list     \
     * @param _Initial Initial values     \
     * @return Initializer list           \
     */                                   \
    comp_impl__bti_comp_ops_initial(_Initial)

#define bti_comp_ops_cleared()                      \
    /**                                             \
     * @brief Create empty initializer list         \
     * @return Empty initializer list (zero-filled) \
     */                                             \
    comp_impl__bti_comp_ops_cleared()

#define bti_comp_ops_make$(T_Lit, _Initial...)          \
    /**                                                 \
     * @brief Create compound literal of specified type \
     * @param T_Lit Type of compound literal            \
     * @param _Initial Initial values                   \
     * @return Compound literal of type T_Lit           \
     */                                                 \
    comp_impl__bti_comp_ops_make$(T_Lit, _Initial)

#define bti_comp_ops_makeCleared$(T_Lit)                  \
    /**                                                   \
     * @brief Create zero-filled compound literal         \
     * @param T_Lit Type of compound literal              \
     * @return Zero-filled compound literal of type T_Lit \
     */                                                   \
    comp_impl__bti_comp_ops_makeCleared$(T_Lit)

/* Memory Operations */

#define bti_comp_ops_create$(T_Lit, _Initial...)         \
    /**                                                  \
     * @brief Create pointer to compound literal         \
     * @param T_Lit Type of compound literal             \
     * @param _Initial Initial values                    \
     * @return Pointer to compound literal of type T_Lit \
     */                                                  \
    comp_impl__bti_comp_ops_create$(T_Lit, _Initial)

#define bti_comp_ops_createCleared$(T_Lit)                   \
    /**                                                      \
     * @brief Create pointer to zero-filled compound literal \
     * @param T_Lit Type of compound literal                 \
     * @return Pointer to zero-filled compound literal       \
     */                                                      \
    comp_impl__bti_comp_ops_createCleared$(T_Lit)

#define bti_comp_ops_createFrom(var_src...) \
    /**                                     \
     * @brief Create pointer to variable    \
     * @param var_src Source variable       \
     * @return Pointer to var_src           \
     */                                     \
    comp_impl__bti_comp_ops_createFrom(var_src)

#define bti_comp_ops_createFrom$(T_Lit, var_src...) \
    /**                                             \
     * @brief Create pointer to typed variable      \
     * @param T_Lit Type of variable                \
     * @param var_src Source variable               \
     * @return Pointer to var_src cast to T_Lit     \
     */                                             \
    comp_impl__bti_comp_ops_createFrom$(T_Lit, var_src)

/* Generic Operations */

#define bti_comp_ops_Generic_match$(T, _Pattern...) \
    /**                                             \
     * @brief Perform type-based pattern matching   \
     * @param T Type to match                       \
     * @param _Pattern Pattern expressions          \
     * @return Result of _Generic selection         \
     */                                             \
    comp_impl__bti_comp_ops_Generic_match$(T, _Pattern)

#define bti_comp_ops_Generic_pattern$(T)       \
    /**                                        \
     * @brief Create pattern case for _Generic \
     * @param T Type to match                  \
     * @return Type pattern for _Generic       \
     */                                        \
    comp_impl__bti_comp_ops_Generic_pattern$(T)

#define bti_comp_ops_Generic_fallback_()       \
    /**                                        \
     * @brief Create default case for _Generic \
     * @return Default pattern for _Generic    \
     */                                        \
    comp_impl__bti_comp_ops_Generic_fallback_()

/* Evaluation Operations */

#define bti_comp_ops_eval                \
    /**                                  \
     * @brief Evaluate expression        \
     * @param ... Expression to evaluate \
     * @return Result of expression      \
     */                                  \
    /* just comment */

#define bti_comp_ops_eval_return   \
    /**                            \
     * @brief Return from function \
     * @return Return statement    \
     */                            \
    /* just comment */

#define bti_comp_ops_eval_return_(_Expr...)     \
    /**                                         \
     * @brief Return expression from function   \
     * @param _Expr Expression to return        \
     * @return Return statement with expression \
     */                                         \
    /* just comment */ _Expr

/* Branch Prediction */

#define bti_comp_ops_likely(_Expr...)       \
    /**                                     \
     * @brief Mark condition as likely true \
     * @param _Expr Condition expression    \
     * @return Condition with branch hint   \
     */                                     \
    comp_impl__bti_comp_ops_likely(_Expr)

#define bti_comp_ops_unlikely(_Expr...)      \
    /**                                      \
     * @brief Mark condition as likely false \
     * @param _Expr Condition expression     \
     * @return Condition with branch hint    \
     */                                      \
    comp_impl__bti_comp_ops_unlikely(_Expr)

/*========== Macros and Definitions =========================================*/

/* Type Operations - Implementations */

#define comp_impl__bti_comp_ops_as$(T_Dest, val_src...)   ((T_Dest)(val_src))
#define comp_impl__bti_comp_ops_lit$(T_Lit, _Initial...)  ((T_Lit){ _Initial })
#define comp_impl__bti_comp_ops_initial(_Initial...)      { _Initial }
#define comp_impl__bti_comp_ops_cleared()                 { 0 }
#define comp_impl__bti_comp_ops_make$(T_Lit, _Initial...) ((T_Lit){ _Initial })
#define comp_impl__bti_comp_ops_makeCleared$(T_Lit)       ((T_Lit){ 0 })

/* Memory Operations - Implementations */

// NOLINTBEGIN(bugprone-macro-parentheses)
#define comp_impl__bti_comp_ops_create$(T_Lit, _Inital...)     (&*comp_impl__bti_comp_ops_lit$(T_Lit[1], [0] = make$(T_Lit, _Inital)))
#define comp_impl__bti_comp_ops_createCleared$(T_Lit)          (&*comp_impl__bti_comp_ops_lit$(T_Lit[1], [0] = makeCleared$(T_Lit)))
#define comp_impl__bti_comp_ops_createFrom(var_src...)         (&*comp_impl__bti_comp_ops_lit$(bti_type_TypeUnqualOf(var_src)[1], [0] = var_src))
#define comp_impl__bti_comp_ops_createFrom$(T_Lit, var_src...) (&*comp_impl__bti_comp_ops_lit$(T_Lit[1], [0] = var_src))
// NOLINTEND(bugprone-macro-parentheses)

/* Generic Operations - Implementations */

#define comp_impl__bti_comp_ops_Generic_match$(T, _Pattern...) _Generic((T), _Pattern)
#define comp_impl__bti_comp_ops_Generic_pattern$(T) \
    T:
#define comp_impl__bti_comp_ops_Generic_fallback_() default:

/* Branch Prediction - Implementations */

#if bti_comp_gcc || bti_comp_clang
#define comp_impl__bti_comp_ops_likely(_Expr...)   __builtin_expect(!!(_Expr), 1)
#define comp_impl__bti_comp_ops_unlikely(_Expr...) __builtin_expect(!!(_Expr), 0)
#else
#define comp_impl__bti_comp_ops_likely(_Expr...)   (_Expr)
#define comp_impl__bti_comp_ops_unlikely(_Expr...) (_Expr)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_COMP_OPS_INCLUDED */
