#define main_no_args (1)
#include "dh/main.h"
#include "dh/core.h"
#include "dh/Arr.h"
#include "dh/sli.h"
#include "dh/opt.h"
#include "dh/log.h"

// Function that may return an optional i32
static fn_ext_scope(findValueIndex(i32 value, Sli_const$i32 items), Opt$i32) {
    for_slice_indexed (items, item, index) {
        if (*item == value) {
            return_some(index); // Return optional with a value
        }
    }
    return_none(); // Return optional with no value
} ext_unscoped;

// Function that uses unwrap and orelse for default values
static fn_(demonstrateUnwrapOrelse(Opt$i32 opt, i32 default_val), i32) {
    // unwrap() will cause an assertion failure if opt is none
    // Only use when you're confident the option has a value
    let value_unsafe = unwrap(opt);

    // orelse returns the default value if opt is none
    let value_safe = orelse(opt, default_val);

    // orelse with a block allows more complex handling
    let value_computed = orelse(opt, eval({
        log_debug("Option was none, computing default and returning");
        return default_val * 2;
    }));

    $ignore value_unsafe;
    $ignore value_computed;
    return value_safe;
}

// Function showing if_some and else_none pattern
static fn_(processOptionalValue(Opt$i32 opt), void) {
    if_some(opt, value) {
        // This block runs if opt has a value
        printf("Found value: %d\n", value);

        // You can do complex operations with the value
        if (value > 10) {
            printf("Value is greater than 10\n");
        }
    } else_none {
        // This block runs if opt is none
        printf("No value found\n");
    }
}

// Example of nested optional handling
static fn_(processNestedOptionals(Opt$i32 maybe_outer, Opt$i32 maybe_inner), i32) {
    if_some(maybe_outer, outer) {
        if_some(maybe_inner, inner) {
            return outer + inner;
        } else_none {
            return outer;
        }
    } else_none {
        if_some(maybe_inner, inner) {
            return inner;
        } else_none {
            return 0;
        }
    }
    claim_unreachable;
}

fn_ext_scope(dh_main(void), Err$void) {
    // Create some optional values
    let opt_with_value = some$(Opt$i32, 42);
    let opt_empty      = none$(Opt$i32);

    // Array for demonstration
    Arr$$(5, i32) numbers = Arr_init({ 1, 3, 5, 7, 9 });

    // Demonstrate finding values
    let found_index = findValueIndex(5, Sli_arr$(Sli_const$i32, numbers));
    let not_found   = findValueIndex(6, Sli_arr$(Sli_const$i32, numbers));

    printf("---- Optional Value Demonstration ----\n");

    // Demonstrate if_some pattern
    printf("Looking for value 5: ");
    if_some(found_index, index) {
        printf("Found at index %d\n", index);
    } else_none {
        printf("Not found\n");
    }

    printf("Looking for value 6: ");
    if_some(not_found, index) {
        printf("Found at index %d\n", index);
    } else_none {
        printf("Not found\n");
    }

    // Demonstrate orelse
    printf("\n---- Default Values Demonstration ----\n");
    printf("Value with default: %d\n", orelse(not_found, -1));
    printf("Value from some(): %d\n", orelse(opt_with_value, -1));

    // Demonstrate the two helper functions
    printf("\n---- Processing Optionals ----\n");
    processOptionalValue(opt_with_value);
    processOptionalValue(opt_empty);

    printf("\n---- Nested Optionals Result ----\n");
    printf("Both values: %d\n", processNestedOptionals(opt_with_value, some$(Opt$i32, 10)));
    printf("First value only: %d\n", processNestedOptionals(opt_with_value, opt_empty));
    printf("Second value only: %d\n", processNestedOptionals(opt_empty, some$(Opt$i32, 10)));
    printf("No values: %d\n", processNestedOptionals(opt_empty, opt_empty));

    // Example of unwrap (only use when you're sure it's not none)
    printf("\n---- Unwrap Example ----\n");
    printf("Unwrapped value: %d\n", unwrap(opt_with_value));
    // Uncommenting this would cause an assertion failure:
    // printf("This would fail: %d\n", unwrap(opt_empty));

    return_ok({});
} ext_unscoped;

#if README_SAMPLE
fn_ext_scope(findValueIndex(i32 value, Sli_const$i32 items), Opt$i32) {
    for_slice_indexed (items, item, index) {
        if (*item != value) { continue; }
        return_some(index); // Return with a value
    }
    return_none(); // Return with no value
} ext_unscoped;

fn_(example(void), void) {
    Arr$$(5, i32) nums = Arr_init({ 10, 20, 30, 40, 50 });

    // Create optional values
    let opt_value = some$(Opt$i32, 42);
    let opt_empty = none$(Opt$i32);

    // Find a value in array
    let found = findValueIndex(30, Sli_arr$(Sli_const$i32, nums));

    // Check if option has value
    if_some(found, index) {
        printf("Found at: %d\n", index);
    } else_none {
        printf("Not found\n");
    }

    // Default values
    let value = orelse(found, -1); // Use -1 if not found

    // Unsafe extraction (assertion if option might be none)
    let unsafe_value = unwrap(opt_value);
}
#endif /* README_SAMPLE */
