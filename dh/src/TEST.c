/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    TEST.c
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-10 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Implementation of the test framework
 */

#include "dh/TEST.h"
#include "dh/heap/Page.h"
#include "dh/Str.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"
#include "dh/ErrTrace.h"

/* ANSI color codes */
#define TEST_color_reset  "\033[0m"
#define TEST_color_red    "\033[31m"
#define TEST_color_green  "\033[32m"
#define TEST_color_yellow "\033[33m"
#define TEST_color_blue   "\033[34m"

fn_((TEST_Framework_instance(void))(TEST_Framework*)) {
    /* Singleton instance */
    static TEST_Framework s_instance = cleared();
    static bool s_is_initialized = false;
    static heap_Page s_allocator = cleared();
    if (!s_is_initialized) {
        s_instance.cases = type$((ArrList$TEST_Case)(ArrList_init(
            typeInfo$(TEST_Case),
            heap_Page_allocator(&s_allocator)
        )));
        s_is_initialized = true;
    }
    return &s_instance;
}

fn_((TEST_Framework_bindCase(TEST_CaseFn fn, S_const$u8 name))(void)) {
    let instance = TEST_Framework_instance();
    let result = ArrList_append(
        instance->cases.base,
        meta_refPtr(&(TEST_Case){ .fn = fn, .name = name })
    );
    if (isErr(result)) { return; } /* Occurs when heap is full (Out of memory) */
}

fn_((TEST_Framework_run(void))(void)) {
    static let print = io_Writer_print;
    let out = fs_File_writer(io_getStdOut());
    let instance = TEST_Framework_instance();

    // Print header
    catch_((print(out, u8_l("\n")))($ignore, claim_unreachable));
    catch_((print(out, u8_l(TEST_color_blue "=== Running Tests ===" TEST_color_reset)))($ignore, claim_unreachable));
    catch_((print(out, u8_l("\n")))($ignore, claim_unreachable));

    // Run each test case
    for_(($s(instance->cases.items))(test_case) {
        instance->stats.total++;
        catch_((print(
            out, u8_l("Running test: {:s}{:s}{:s}\n"),
            u8_l(TEST_color_yellow), test_case->name, u8_l(TEST_color_reset)
        ))($ignore, claim_unreachable));

        // Run the test
        if_err(test_case->fn(), err) {
            instance->stats.failed++;
            catch_((print(
                out, u8_l("    {:s}: [{:s}] {:s}\n"),
                u8_l(TEST_color_red "[FAIL]" TEST_color_reset),
                Err_domainToStr(err),
                Err_codeToStr(err)
            ))($ignore, claim_unreachable));
            Err_print(err);
            ErrTrace_print();
            ErrTrace_reset();
        } else_void {
            instance->stats.passed++;
            catch_((print(
                out, u8_l("    {:s}\n"),
                u8_l(TEST_color_green "[PASS]" TEST_color_reset)
            ))($ignore, claim_unreachable));
        }
    });

    // Print summary
    catch_((print(out, u8_l("\n")))($ignore, claim_unreachable));
    {
        catch_((print(out, u8_l(TEST_color_blue "=== Test Summary ===" TEST_color_reset "\n")))($ignore, claim_unreachable));
        catch_((print(out, u8_l("Total: {:u}\n"), instance->stats.total))($ignore, claim_unreachable));
        catch_((print(out, u8_l(TEST_color_green "Passed: {:u}" TEST_color_reset "\n"), instance->stats.passed))($ignore, claim_unreachable));
        catch_((print(out, u8_l(TEST_color_red "Failed: {:u}" TEST_color_reset "\n"), instance->stats.failed))($ignore, claim_unreachable));
    }
    catch_((print(out, u8_l("\n")))($ignore, claim_unreachable));

    ArrList_fini(instance->cases.ref_raw);
}

/* Debug versions of test functions */
fn_((TEST_expect_test(bool expr, SrcLoc loc, S_const$u8 eval_str))(E$void) $scope) {
    let_ignore = loc;
    let_ignore = eval_str;

    if (!expr) {
        return_err(TEST_Err_Unexpected());
    }
    return_ok({});
} $unscoped_(fn);

fn_((TEST_expectMsg_test(bool expr, S_const$u8 msg, SrcLoc loc, S_const$u8 eval_str))(E$void) $scope) {
    let_ignore = msg;
    let_ignore = loc;
    let_ignore = eval_str;

    if (!expr) {
        return_err(TEST_Err_Unexpected());
    }
    return_ok({});
} $unscoped_(fn);
