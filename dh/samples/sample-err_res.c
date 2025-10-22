#include "dh/main.h"
#include "dh/io/stream.h"

#include "dh/Arr.h"
#include "dh/sli.h"

#include "dh/heap/Fixed.h"
#include "dh/mem/Allocator.h"

// Define custom error set
config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

// Function that returns an error result
use_ErrSet$(math_Err, i32);
$static $must_check
fn_((safeDivide(i32 numerator, i32 denominator))(math_Err$i32) $scope) {
    if (denominator == 0) {
        return_err(math_Err_DivisionByZero());
    }

    // Check for potential overflow
    if (numerator == i32_limit_min && denominator == -1) {
        return_err(math_Err_Overflow());
    }

    return_ok(numerator / denominator);
} $unscoped_(fn);

// Function demonstrating error propagation with try_
use_ErrSet$(math_Err, f32);
$static $must_check
fn_((calculateRatio(i32 a, i32 b, i32 c, i32 d))(math_Err$f32) $scope) {
    // try_ will return early if an error occurs
    let first_result  = try_(safeDivide(a, b));
    let second_result = try_(safeDivide(c, d));

    // Calculate the ratio
    return_ok(as$((f32)(first_result)) / as$((f32)(second_result)));
} $unscoped_(fn);

// Function demonstrating catch_from for error handling
$static
fn_((handleDivision(i32 a, i32 b))(i32)) {
    return catch_((safeDivide(a, b))(
        err, ({
            io_stream_eprintln(u8_l("Division error: [{:s}] {:s}"), Err_domainToStr(err), Err_codeToStr(err));
            eval_return 0;
        })
    ));
}

typedef variant_(
    (math_ErrRes),
    (math_ErrRes_i32, math_Err$i32),
    (math_ErrRes_f32, math_Err$f32)
) math_ErrRes;

// Function demonstrating if_err/else_ok pattern
$static
fn_((processResult(math_ErrRes result))(void)) {
    Opt$$(math_Err) maybe_err = none();
    match_(result, {
        pattern_(math_ErrRes_i32, (result), {
            if_err(*result, err) {
                toSome(&maybe_err, err);
            }
            else_ok(value) {
                io_stream_println(u8_l("Operation succeeded with value: {:d}"), value);
            };
        }) break;
        pattern_(math_ErrRes_f32, (result), {
            if_err(*result, err) {
                toSome(&maybe_err, err);
            }
            else_ok(value) {
                io_stream_println(u8_l("Operation succeeded with value: {:f}"), value);
            };
        }) break;
        fallback_(claim_unreachable);
    });
    if_some(maybe_err, err) {
        io_stream_println(u8_l("Error occurred: [{:s}] {:s}"), Err_domainToStr(err), Err_codeToStr(err));
    }
}

// Function demonstrating errdefer_
$static var_(memory, Arr$$(1024, u8)) = Arr_zero();
$static $must_check
fn_((performOperation(i32 a, i32 b))(math_Err$i32) $guard) {
    // Allocate resources
    var fixed     = heap_Fixed_init(Sli_arr$(Sli$u8, memory));
    var allocator = heap_Fixed_allocator(&fixed);
    var buffer    = meta_cast$(
        Sli$i32,
        catch_((mem_Allocator_alloc(allocator, typeInfo$(i32), 100))(err, {
            io_stream_eprintln(u8_l("Failed to allocate buffer: [{:s}] {:s}"), Err_domainToStr(err), Err_codeToStr(err));
            claim_unreachable;
        }))
    );
    // Cleanup on error
    errdefer_($ignore, mem_Allocator_free(allocator, anySli(buffer)));

    // If this fails, errdefer_ will free buffer
    let result = try_(safeDivide(a, b));

    // More operations with buffer...
    Sli_setAt(buffer, 0, result);

    // Clean up and return
    mem_Allocator_free(allocator, anySli(buffer));
    return_ok(result);
} $unguarded_(fn);

fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    let_ignore = args;
    io_stream_println(u8_l("---- Error Handling Examples ----"));

    // Basic error handling
    io_stream_println(u8_l("\nBasic division examples:"));
    processResult(variant_of$(math_ErrRes, math_ErrRes_i32, safeDivide(10, 2)));
    processResult(variant_of$(math_ErrRes, math_ErrRes_i32, safeDivide(10, 0)));

    // Error propagation
    io_stream_println(u8_l("\nCalculating ratios:"));
    with_(var ratio_result = calculateRatio(10, 2, 6, 3)) {
        processResult(variant_of$(math_ErrRes, math_ErrRes_f32, ratio_result));
    }
    with_(var ratio_result = calculateRatio(10, 0, 6, 3)) {
        processResult(variant_of$(math_ErrRes, math_ErrRes_f32, ratio_result));
    }

    // Error handling with catch_from
    io_stream_println(u8_l("\nHandling division with defaults:"));
    io_stream_println(u8_l("10/2 = {:d}"), handleDivision(10, 2));
    io_stream_println(u8_l("10/0 = {:d}"), handleDivision(10, 0));

    // Complex operation with cleanup
    io_stream_println(u8_l("\nComplex operation with cleanup:"));
    processResult(variant_of$(math_ErrRes, math_ErrRes_i32, performOperation(10, 2)));
    processResult(variant_of$(math_ErrRes, math_ErrRes_i32, performOperation(10, 0)));

    return_ok({});
} $unscoped_(fn);

#if README_SAMPLE
config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_ErrSet$(math_Err, i32); // or Generally `use_Err$(i32)`
$static $must_check
fn_((safeDivide(i32 lhs, i32 rhs))(math_Err$i32) $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} $unscoped_(fn);

$static $must_check
fn_((example(void))(Err$void) $scope) {
    // Allocate resources
    var buffer = meta_cast$(Sli$i32, try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 100)));
    // Cleanup always when function returns
    defer_(mem_Allocator_free(allocator, anySli(buffer)));
    // Cleanup only when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_l("Occurred error! [{:s}] {:s}"), Err_domainToStr(err), Err_codeToStr(err)));

    // Error propagation (early return)
    let result_invalid = try_(safeDivide(10, 0));

    // Error handling with default value
    let result_default = catch_((safeDivide(10, 0))($ignore, 1));

    // Error handling with error payload capture
    let result_handling = catch_((safeDivide(10, 0))(err, {
        Err_print(err);   // Print the error
        ErrTrace_print(); // Print the error trace
        return_err(err);  // Return with an error
    }));

    // Return a normally
    return_ok({});
} $unscoped_(fn);
#endif /* README_SAMPLE */
