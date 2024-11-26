#ifndef CORE_CFG_INCLUDED
#define CORE_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin.h"
#include "dh/claim/assert_static.h"

/*========== Macros and Definitions =========================================*/

#define used(_Expr...)                                                    \
    /**                                                                   \
     * @brief Marks variables or expressions as used to suppress compiler \
    warnings                                                              \
     * @details In macro functions, the arguments are marked as used      \
     * @param _Expr... Variable number of arguments to be marked as used  \
     */                                                                   \
    IMPL_used(_Expr)

#define unused(_Expr...)                                                   \
    /**                                                                    \
     * @brief Marks variables or expressions as intentionally unused       \
    to suppress compiler warnings                                          \
     * @param _Expr... Variable number of arguments to be marked as unused \
     */                                                                    \
    IMPL_unused(_Expr)

#define ignore                                                           \
    /**                                                                  \
     * @brief Attribute explicitly ignores an expression or return value \
     * @details Used to suppress compiler warnings about unused values   \
     */                                                                  \
    IMPL_ignore

#define as(_T, _val) \
    IMPL_as(_T, _val)

#define literal(_T, _Inital...)                                              \
    /**                                                                      \
     * @brief Literal macro for creating a compound literal                  \
     * @details Plain structures in C++ (without constructors) can be        \
        initialized with { } This is called aggregate initialization (C++11) \
     * @note MSVC C++ compiler does not support compound literals (C99)      \
     */                                                                      \
    IMPL_literal(_T, _Inital)

/*========== Macro Implementations ==========================================*/

#define IMPL_used(_Expr...) \
    _Expr

#define IMPL_unused(_Expr...) \
    ((void)(_Expr))

#define IMPL_ignore \
    (void)

#if defined(__cplusplus)
#define IMPL_as(_T, _val) \
    static_cast<_T>(_val)
#else
#define IMPL_as(_T, _val) \
    ((_T)(_val))
#endif

#if defined(__cplusplus)
#define IMPL_literal(_T, _Inital...) \
    _T { _Inital }
#else
#define IMPL_literal(_T, _Inital...) \
    (_T) { _Inital }
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* CORE_CFG_INCLUDED */
