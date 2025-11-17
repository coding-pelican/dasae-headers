#include "dh/main.h"
#include "dh/heap/Fixed.h"
#include "dh/io/stream.h"

// Define custom error set
errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));

// Function that returns an error result
T_use_E$($set(math_Err)(i32));
$must_check
$static fn_((safeDivide(i32 num, i32 denom))(math_Err$i32) $scope) {
    if (denom == 0) {
        return_err(math_Err_DivisionByZero());
    }
    // Check for potential overflow
    if (num == i32_limit_min && denom == -1) {
        return_err(math_Err_Overflow());
    }
    return_ok(num / denom);
} $unscoped_(fn);

// Function demonstrating error propagation with try_
T_use_E$($set(math_Err)(f32));
$must_check
$static fn_((calculateRatio(i32 a, i32 b, i32 c, i32 d))(math_Err$f32) $scope) {
    // try_ will return early if an error occurs
    let first_result  = try_(safeDivide(a, b));
    let second_result = try_(safeDivide(c, d));
    // Calculate the ratio
    return_ok(as$(f32)(first_result) / as$(f32)(second_result));
} $unscoped_(fn);

// Function demonstrating catch_from for error handling
$static fn_((handleDivision(i32 a, i32 b))(i32)) {
    return catch_((safeDivide(a, b))(
        err, (io_stream_eprintln(u8_l("Division error: {:e}"), err), 0)
    ));
}

typedef variant_((math_ErrRes $fits u8)(
    (math_ErrRes_i32, math_Err$i32),
    (math_ErrRes_f32, math_Err$f32)
)) math_ErrRes;

// Function demonstrating if_err/else_ok pattern
$static fn_((processResult(math_ErrRes result))(void)) {
    let maybe_err = expr_(O$$(math_Err) $scope) match_(result, {
        pattern_(math_ErrRes_i32, (result), {
            if_err((*result)(err)) {
                $break_(some(err));
            } else_ok(value) {
                io_stream_println(u8_l("Operation succeeded with value: {:d}"), value);
            };
        }) break;
        pattern_(math_ErrRes_f32, (result), {
            if_err((*result)(err)) {
                $break_(some(err));
            } else_ok(value) {
                io_stream_println(u8_l("Operation succeeded with value: {:f}"), value);
            };
        }) break;
        fallback_(claim_unreachable);
    }) $unscoped_(expr);
    if_some((maybe_err)(err)) {
        io_stream_println(u8_l("Error occurred: [{:s}] {:z}"), Err_domainToStr(err), Err_codeToStrZ(err));
    }
}

// Function demonstrating errdefer_
$static var_(memory, A$$(1024, u8)) = A_zero();
$must_check
$static fn_((performOperation(i32 a, i32 b))(math_Err$i32) $guard) {
    // Allocate resources
    var fixed = heap_Fixed_init(A_ref$((S$u8)(memory)));
    var gpa = heap_Fixed_allocator(&fixed);
    var buffer= u_castS$((S$i32)(catch_((mem_Allocator_alloc(gpa, typeInfo$(InnerType), 100))(err, {
        io_stream_eprintln(u8_l("Failed to allocate buffer: {:e}"), err);
        claim_unreachable;
    }))));
    // Cleanup on error
    errdefer_($ignore, mem_Allocator_free(gpa, u_anyS(buffer)));

    // If this fails, errdefer_ will free buffer
    let result = try_(safeDivide(a, b));

    // More operations with buffer...
    *atS(buffer, 0) = result;

    // Clean up and return
    mem_Allocator_free(gpa, u_anyS(buffer));
    return_ok(result);
} $unguarded_(fn);

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    io_stream_println(u8_l("---- Error Handling Examples ----"));

    // Basic error handling
    io_stream_println(u8_l("\nBasic division examples:"));
    processResult(union_of$((math_ErrRes)(math_ErrRes_i32)(safeDivide(10, 2))));
    processResult(union_of$((math_ErrRes)(math_ErrRes_i32)(safeDivide(10, 0))));

    // Error propagation
    io_stream_println(u8_l("\nCalculating ratios:"));
    with_(var ratio_result = calculateRatio(10, 2, 6, 3)) {
        processResult(union_of$((math_ErrRes)(math_ErrRes_f32)(ratio_result)));
    }
    with_(var ratio_result = calculateRatio(10, 0, 6, 3)) {
        processResult(union_of$((math_ErrRes)(math_ErrRes_f32)(ratio_result)));
    }

    // Error handling with catch_from
    io_stream_println(u8_l("\nHandling division with defaults:"));
    io_stream_println(u8_l("10/2 = {:d}"), handleDivision(10, 2));
    io_stream_println(u8_l("10/0 = {:d}"), handleDivision(10, 0));

    // Complex operation with cleanup
    io_stream_println(u8_l("\nComplex operation with cleanup:"));
    processResult(union_of$((math_ErrRes)(math_ErrRes_i32)(performOperation(10, 2))));
    processResult(union_of$((math_ErrRes)(math_ErrRes_i32)(performOperation(10, 0))));

    return_ok({});
} $unscoped_(fn);

#if README_SAMPLE
errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));

T_use_E$($set(math_Err)(i32)); // or Generally `use_E$(i32)`
$must_check
$static fn_((safeDivide(i32 lhs, i32 rhs))(math_Err$i32) $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} $unscoped_(fn);

$must_check
$static fn_((example(mem_Allocator gpa))(E$void) $scope) {
    // Allocate resources
    var buffer = u_castS$((S$i32)(try_(mem_Allocator_alloc(gpa, typeInfo$(InnerType), 100))));
    // Cleanup always when function returns
    defer_(mem_Allocator_free(gpa, u_anyS(buffer)));
    // Cleanup only when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_l("Occurred error!: {:e}"), err));

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
