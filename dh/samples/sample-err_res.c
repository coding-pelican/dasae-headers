#define main_no_args (1)
#include "dh/main.h"
#include "dh/log.h"

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
static fn_(safeDivide(i32 numerator, i32 denominator), math_Err$i32, $scope) {
    if (denominator == 0) {
        return_err(math_Err_DivisionByZero());
    }

    // Check for potential overflow
    if (numerator == i32_limit_min && denominator == -1) {
        return_err(math_Err_Overflow());
    }

    return_ok(numerator / denominator);
} $unscoped;

// Function demonstrating error propagation with try_
use_ErrSet$(math_Err, f32);
static fn_(calculateRatio(i32 a, i32 b, i32 c, i32 d), math_Err$f32, $scope) {
    // try_ will return early if an error occurs
    let first_result  = try_(safeDivide(a, b));
    let second_result = try_(safeDivide(c, d));

    // Calculate the ratio
    return_ok(as$(f32, first_result) / as$(f32, second_result));
} $unscoped;

// Function demonstrating catch_from for error handling
static fn_(handleDivision(i32 a, i32 b), i32) {
    return catch_from(safeDivide(a, b), err, eval({
        log_error("\nDivision error: [%s] %s\n", Err_domainToCStr(err), Err_codeToCStr(err));
        return 0; // Default value
    }));
}

config_UnionEnum(math_ErrRes,
    (math_ErrRes_i32, math_Err$i32),
    (math_ErrRes_f32, math_Err$f32)
);
// Function demonstrating if_err/else_ok pattern
static fn_(processResult(math_ErrRes result), void) {
    Opt$$(math_Err) maybe_err = none();
    match_(result) {
    pattern_(math_ErrRes_i32, result) {
        if_err(*result, err) {
            toSome(&maybe_err, err);
        } else_ok(value) {
            printf("Operation succeeded with value: %d\n", value);
        }
    } break;
    pattern_(math_ErrRes_f32, result) {
        if_err(*result, err) {
            toSome(&maybe_err, err);
        } else_ok(value) {
            printf("Operation succeeded with value: %f\n", value);
        }
    } break;
    fallback_()
        claim_unreachable;
    }
    if_some(maybe_err, err) {
        printf("\nError occurred: [%s] %s\n", Err_domainToCStr(err), Err_codeToCStr(err));
    }
}

// Function demonstrating errdefer_
static var_(memory, Arr$$(1024, u8)) = Arr_zero();
static fn_(performOperation(i32 a, i32 b), math_Err$i32, $guard) {
    // Allocate resources
    var fixed     = heap_Fixed_init(Sli_arr$(Sli$u8, memory));
    var allocator = heap_Fixed_allocator(&fixed);
    var buffer    = meta_cast$(Sli$i32,
        catch_from(mem_Allocator_alloc(allocator, typeInfo$(i32), 100), err, eval({
            log_error("\nFailed to allocate buffer: [%s] %s\n", Err_domainToCStr(err), Err_codeToCStr(err));
            claim_unreachable;
        }))
    );
    // Cleanup on error
    errdefer_(mem_Allocator_free(allocator, anySli(buffer)));

    // If this fails, errdefer_ will free buffer
    let result = try_(safeDivide(a, b));

    // More operations with buffer...
    Sli_setAt(buffer, 0, result);

    // Clean up and return
    mem_Allocator_free(allocator, anySli(buffer));
    return_ok(result);
} $unguarded;

fn_(dh_main(void), Err$void, $scope) {
    printf("---- Error Handling Examples ----\n");

    // Basic error handling
    printf("\nBasic division examples:\n");
    processResult(tagUnion$(math_ErrRes, math_ErrRes_i32, safeDivide(10, 2)));
    processResult(tagUnion$(math_ErrRes, math_ErrRes_i32, safeDivide(10, 0)));

    // Error propagation
    printf("\nCalculating ratios:\n");
    with_(var ratio_result = calculateRatio(10, 2, 6, 3)) {
        processResult(tagUnion$(math_ErrRes, math_ErrRes_f32, ratio_result));
    }
    with_(var ratio_result = calculateRatio(10, 0, 6, 3)) {
        processResult(tagUnion$(math_ErrRes, math_ErrRes_f32, ratio_result));
    }

    // Error handling with catch_from
    printf("\nHandling division with defaults:\n");
    printf("10/2 = %d\n", handleDivision(10, 2));
    printf("10/0 = %d\n", handleDivision(10, 0));

    // Complex operation with cleanup
    printf("\nComplex operation with cleanup:\n");
    processResult(tagUnion$(math_ErrRes, math_ErrRes_i32, performOperation(10, 2)));
    processResult(tagUnion$(math_ErrRes, math_ErrRes_i32, performOperation(10, 0)));

    return_ok({});
} $unscoped;

#if README_SAMPLE
config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_ErrSet$(math_Err, i32); // or Generally `use_Err$(i32)`
static fn_(safeDivide(i32 lhs, i32 rhs), math_Err$i32, $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // Return with an error
    }
    return_ok(lhs / rhs); // Return with a value
} $unscoped;

static fn_(example(void), Err$void, $scope) {
    // Allocate resources
    var buffer = meta_cast$(Sli$i32,
        try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 100))
    );
    // Cleanup always when function returns
    defer_(mem_Allocator_free(allocator, anySli(buffer)));
    // Cleanup only when an error occurs and propagates
    errdefer_(log_error("Occurred error!"));

    // Error propagation (early return)
    let result_invalid = try_(safeDivide(10, 0));

    // Error handling with default value
    let result_default = catch_(safeDivide(10, 0), 1);

    // Error handling with error payload capture
    let result_handling = catch_from(safeDivide(10, 0), err, eval({
        Err_print(err);   // Print the error
        ErrTrace_print(); // Print the error trace
        return_err(err); // Return with an error
    }));

    // Return a normally
    return_ok({});
} $unscoped;
#endif /* README_SAMPLE */
