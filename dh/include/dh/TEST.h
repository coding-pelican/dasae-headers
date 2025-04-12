/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    TEST.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-10 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Test framework for unit testing
 * @details This header provides a testing framework.
 *          Tests are organized into test cases that can be automatically discovered and run.
 *          Each test is isolated and reports detailed information on failures.
 */

#ifndef TEST_INCLUDED
#define TEST_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/TEST/cfg.h"

#include "dh/core.h"

#include "dh/fn.h"
#include "dh/err_res.h"

#include "dh/sli.h"
#include "dh/Str.h"
#include "dh/ArrList.h"

/*========== Definitions ====================================================*/

/* Test case function type */
typedef fn_((*TEST_CaseFn)(void), Err$void);

/* Test case structure */
typedef struct TEST_Case {
    TEST_CaseFn fn;
    Str_const   name;
} TEST_Case;
/* Use slice for test cases */
use_Sli$(TEST_Case);
/* Use array list for test cases */
use_ArrList$(TEST_Case);

/*========== Public API =====================================================*/

/// @brief Test framework structure
typedef struct TEST_Framework {
    ArrList$TEST_Case cases;
    struct {
        u32 total;
        u32 passed;
        u32 failed;
    } stats;
} TEST_Framework;

/// @brief Access test framework singleton instance
extern fn_(TEST_Framework_instance(void), TEST_Framework*);
/// @brief Bind test case to framework
extern fn_(TEST_Framework_bindCase(TEST_CaseFn fn, Str_const name), void);
/// @brief Run all registered tests
extern fn_(TEST_Framework_run(void), void);

/*========== Test Macros ====================================================*/

#define TEST_only(...)              comp_syn__TEST_only(__VA_ARGS__)
#define fn_TEST_scope(_Name...)     comp_syn__fn_TEST_scope(pp_join(_, TEST, pp_uniqTok(binder)), pp_join(_, TEST, pp_uniqTok(caseFn)), _Name)
#define TEST_unscoped               comp_syn__TEST_unscoped
#define fn_TEST_scope_ext(_Name...) comp_syn__fn_TEST_scope_ext(pp_join(_, TEST, pp_uniqTok(binder)), pp_join(_, TEST, pp_uniqTok(caseFn)), _Name)
#define TEST_unscoped_ext           comp_syn__TEST_unscoped_ext

#if !COMP_TIME
/// @brief Check expression and record result
extern fn_( TEST_expect(bool expr), $must_check Err$void);
/// @brief Same as TEST_expect but with custom message
extern fn_( TEST_expectMsg(bool expr, Str_const msg), $must_check Err$void);
#endif /* !COMP_TIME */

/*========== Implementation Details ========================================*/

#if TEST_comp_enabled
#define comp_syn__TEST_only(...) __VA_ARGS__
#else /* !TEST_comp_enabled */
#define comp_syn__TEST_only(...)
#endif /* TEST_comp_enabled */

#define comp_syn__fn_TEST_scope(_ID_binder, _ID_caseFn, _Name...) \
    TEST__binder(_ID_binder, _ID_caseFn, _Name);                  \
    TEST__caseFn(_ID_binder, _ID_caseFn)

#define comp_syn__fn_TEST_scope_ext(_ID_binder, _ID_caseFn, _Name...) \
    TEST__binder(_ID_binder, _ID_caseFn, _Name);                      \
    TEST__caseFn_ext(_ID_binder, _ID_caseFn)

#define TEST__binder(_ID_binder, _ID_caseFn, _Name...) comp_fn_gen__TEST__binder(_ID_binder, _ID_caseFn, _Name)
#define TEST__caseFn(_ID_binder, _ID_caseFn...)        comp_fn_gen__TEST__caseFn(_ID_binder, _ID_caseFn)
#define TEST__caseFn_ext(_ID_binder, _ID_caseFn...)    comp_fn_gen__TEST__caseFn_ext(_ID_binder, _ID_caseFn)

#define comp_fn_gen__TEST__binder(_ID_binder, _ID_caseFn, _Name...)       \
    static fn_(_ID_caseFn(void), $must_check Err$void);                     \
    static comp_fn_gen__TEST__binder__sgn(_ID_binder) {                   \
        static bool s_is_bound = !comp_fn_gen__TEST__binder__isEnabled(); \
        if (!s_is_bound) {                                                \
            TEST_Framework_bindCase(_ID_caseFn, Str_l(_Name));            \
            s_is_bound = true;                                            \
        }                                                                 \
    }
#if TEST_comp_enabled
#define comp_fn_gen__TEST__binder__sgn(_ID_binder) $on_load fn_(_ID_binder(void), void)
#define comp_fn_gen__TEST__binder__isEnabled()     (true)
#else /* !TEST_comp_enabled */
#define comp_fn_gen__TEST__binder__sgn(_ID_binder) fn_(_ID_binder(void), void)
#define comp_fn_gen__TEST__binder__isEnabled()     (false)
#endif /* !TEST_comp_enabled */
// clang-format off
#define comp_fn_gen__TEST__caseFn(_ID_binder, _ID_caseFn...)      \
    /* TODO: Add case check if it has been run before $on_exit */ \
    static fn_scope(_ID_caseFn(void), Err$void) {                 \
        _ID_binder();
#define comp_syn__TEST_unscoped \
        return_ok({});          \
    } unscoped

#define comp_fn_gen__TEST__caseFn_ext(_ID_binder, _ID_caseFn...) \
    static fn_scope_ext(_ID_caseFn(void), Err$void) {            \
        _ID_binder();
#define comp_syn__TEST_unscoped_ext \
        return_ok({});              \
    } unscoped_ext
// clang-format on

#if COMP_TIME
extern fn_( TEST_expect_test(bool expr, SrcLoc loc, Str_const expr_str), $must_check Err$void);
extern fn_( TEST_expectMsg_test(bool expr, Str_const msg, SrcLoc loc, Str_const expr_str), $must_check Err$void);

#define TEST_expect(_expr...)          TEST_expect_callTest(_expr, srcLoc(), Str_l(#_expr))
#define TEST_expectMsg(_expr, _msg...) TEST_expectMsg_callTest(_expr, _msg, srcLoc(), Str_l(#_expr))

#define TEST_expect_callTest(_expr, _loc, _expr_str)          TEST_expect_test(_expr, _loc, _expr_str)
#define TEST_expectMsg_callTest(_expr, _msg, _loc, _expr_str) TEST_expectMsg_test(_expr, _msg, _loc, _expr_str)
#endif /* COMP_TIME */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST_INCLUDED */
