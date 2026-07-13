/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    TEST.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-10 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Test framework for unit testing
 * @details This header provides a testing framework.
 *          Tests are organized into test cases that can be automatically discovered and run.
 *          Each test is isolated and reports detailed information on failures.
 */
#pragma once
#ifndef TEST__included
#define TEST__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "TEST/cfg.h"
#include "dh/core.h"
#include "ArrList.h"

/*========== Definitions ====================================================*/

/* Error codes */
errset_((TEST_E)(TEST_Fail, TEST_Skip));

/* Test case function type */
typedef fn_(((*)(void))(E$void) $T) TEST_UnitFn;

/* Test case structure */
typedef struct TEST_Unit {
    TEST_UnitFn fn;
    S_const$u8 name;
} TEST_Unit;
/* Use slice for test cases */
T_use$((TEST_Unit)(P, S, ArrList));

/*========== Public API =====================================================*/

/// @brief Test framework structure
typedef struct TEST_Framework {
    ArrList$TEST_Unit units $like_ref;
    struct {
        u32 total;
        u32 passed;
        u32 skipped;
        u32 failed;
    } stats;
    mem_Alctr gpa;
} TEST_Framework;

/// @brief Access test framework singleton instance
$extern fn_((TEST_Framework_instance(void))(TEST_Framework*));
/// @brief Bind test case to framework
$extern fn_((TEST_Framework_bindUnit(TEST_UnitFn fn, S_const$u8 name))(void));
/// @brief Run all registered tests
$extern fn_((TEST_Framework_run(void))(void));

/*========== Test Macros ====================================================*/

#define TEST_fn_(_$Name, _$Extension...) \
    pp_overload(__TEST_fn _$Extension)(_$Name _$Extension)
#define __TEST_fn_0(_$Name, _$Extension...) \
    pp_join(_, TEST_fn, _$Extension)(_$Name)

#define TEST_fn_$_scope(_$Name...) comp_syn__TEST_fn_$_scope(pp_join(_, TEST, pp_uniqTok(binder)), pp_join(_, TEST, pp_uniqTok(unitFn)), _$Name)
#define $unscoped_TEST_fn comp_syn__$unscoped_TEST_fn
#define TEST_fn_$_guard(_$Name...) comp_syn__TEST_fn_$_guard(pp_join(_, TEST, pp_uniqTok(binder)), pp_join(_, TEST, pp_uniqTok(unitFn)), _$Name)
#define $unguarded_TEST_fn comp_syn__$unguarded_TEST_fn

#if !on_comptime
/// @brief Check expression and record result
/// @brief Same as TEST_expect but with custom message
$attr($must_check)
$extern fn_((TEST_skip(void))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_skipMsg(S_const$u8 msg))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_expect(bool expr))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_expectMsg(bool expr, S_const$u8 msg))(TEST_E$void));
#endif /* !on_comptime */

/*========== Implementation Details ========================================*/

#define comp_syn__TEST_fn_$_scope(_$ID_binder, _$ID_unitFn, _$Name...) \
    TEST__binder(_$ID_binder, _$ID_unitFn, _$Name); \
    TEST__unitFn(_$ID_binder, _$ID_unitFn)

#define comp_syn__TEST_fn_$_guard(_$ID_binder, _$ID_unitFn, _$Name...) \
    TEST__binder(_$ID_binder, _$ID_unitFn, _$Name); \
    TEST__unitFn_ext(_$ID_binder, _$ID_unitFn)

#define TEST__binder(_$ID_binder, _$ID_unitFn, _$Name...) comp_fn_gen__TEST__binder(_$ID_binder, _$ID_unitFn, _$Name)
#define TEST__unitFn(_$ID_binder, _$ID_unitFn...) comp_fn_gen__TEST__unitFn(_$ID_binder, _$ID_unitFn)
#define TEST__unitFn_ext(_$ID_binder, _$ID_unitFn...) comp_fn_gen__TEST__unitFn_ext(_$ID_binder, _$ID_unitFn)

#define comp_fn_gen__TEST__binder(_$ID_binder, _$ID_unitFn, _$Name...) \
    $static comp_fn_gen__TEST__binder__sgn(_$ID_binder); \
    $static fn_((_$ID_unitFn(void))(E$void)) $must_check; \
    $static comp_fn_gen__TEST__binder__sgn(_$ID_binder) { \
        $static bool s_is_bound = !comp_fn_gen__TEST__binder__isEnabled(); \
        if (!s_is_bound) { \
            TEST_Framework_bindUnit(_$ID_unitFn, u8_l(_$Name)); \
            s_is_bound = true; \
        } \
    }
#if TEST_enabled
#define comp_fn_gen__TEST__binder__sgn(_$ID_binder) $on_load fn_((_$ID_binder(void))(void))
#define comp_fn_gen__TEST__binder__isEnabled() (true)
#else /* !TEST_enabled */
#define comp_fn_gen__TEST__binder__sgn(_$ID_binder) fn_((_$ID_binder(void))(void))
#define comp_fn_gen__TEST__binder__isEnabled() (false)
#endif /* !TEST_enabled */
// clang-format off
#define comp_fn_gen__TEST__unitFn(_$ID_binder, _$ID_unitFn...)      \
    /* TODO: Add case check if it has been run before $on_exit */ \
    $static fn_((_$ID_unitFn(void))(E$void) $scope) {                 \
        _$ID_binder();
#define comp_syn__$unscoped_TEST_fn \
        return_ok({});          \
    } $unscoped_fn

#define comp_fn_gen__TEST__unitFn_ext(_$ID_binder, _$ID_unitFn...) \
    $static fn_((_$ID_unitFn(void))(E$void) $guard) {            \
        _$ID_binder();
#define comp_syn__$unguarded_TEST_fn \
        return_ok({});              \
    } $unguarded_fn
// clang-format on

#if on_comptime
#define TEST_skip() TEST_skip_callTest(srcLoc())
#define TEST_skipMsg(_msg...) TEST_skipMsg_callTest(_msg, srcLoc())
#define TEST_expect(_expr...) TEST_expect_callTest(_expr, srcLoc(), u8_l(#_expr))
#define TEST_expectMsg(_expr, _msg...) TEST_expectMsg_callTest(_expr, _msg, srcLoc(), u8_l(#_expr))

#define TEST_skip_callTest(_loc) TEST_skip_test(_loc)
#define TEST_skipMsg_callTest(_msg, _loc) TEST_skipMsg_test(_msg, _loc)
#define TEST_expect_callTest(_expr, _loc, _eval_str) TEST_expect_test(_expr, _loc, _eval_str)
#define TEST_expectMsg_callTest(_expr, _msg, _loc, _eval_str) TEST_expectMsg_test(_expr, _msg, _loc, _eval_str)
#endif /* on_comptime */

$attr($must_check)
$extern fn_((TEST_skip_test(SrcLoc loc))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_skipMsg_test(S_const$u8 msg, SrcLoc loc))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_expect_test(bool expr, SrcLoc loc, S_const$u8 eval_str))(TEST_E$void));
$attr($must_check)
$extern fn_((TEST_expectMsg_test(bool expr, S_const$u8 msg, SrcLoc loc, S_const$u8 eval_str))(TEST_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST__included */
