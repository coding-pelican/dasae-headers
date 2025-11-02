#include "dh/main.h"
#include "dh/io/stream.h"

#include "dh/Arr.h"
#include "dh/sli.h"

// Function that may return an optional i32
$static
fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_(($rf(0), $s(items))(index, item) {
        if (*item == value) {
            return_some(index); // Return optional with a value
        }
    });
    return_none(); // Return optional with no value
} $unscoped_(fn);

// Function that uses unwrap and orelse for default values
$static $maybe_unused
fn_((demonstrateUnwrapOrelse(O$i32 opt, i32 default_val))(i32)) {
    // unwrap() will cause an assertion failure if opt is none
    // Only use when you're confident the option has a value
    let value_unsafe = unwrap(opt);

    // orelse returns the default value if opt is none
    let value_safe = orelse_((opt)(default_val));

    // orelse with a block allows more complex handling
    let value_computed = orelse_((opt)({
        io_stream_println(u8_l("Option was none, computing default and returning"));
        return default_val * 2;
    }));

    let_ignore = value_unsafe;
    let_ignore = value_computed;
    return value_safe;
}

// Function showing if_some and else_none pattern
$static
fn_((processOptionalValue(O$i32 opt))(void)) {
    if_some(opt, value) {
        // This block runs if opt has a value
        io_stream_println(u8_l("Found value: {:d}"), value);

        // You can do complex operations with the value
        if (10 < value) {
            io_stream_println(u8_l("Value is greater than 10"));
        }
    } else_none {
        // This block runs if opt is none
        io_stream_println(u8_l("No value found"));
    }
}

// Example of nested optional handling
$static
fn_((processNestedOptionals(O$i32 maybe_outer, O$i32 maybe_inner))(i32) $scope) {
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
} $unscoped_(fn);

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;

    // Create some optional values
    let opt_with_value = some$(O$i32, 42);
    let opt_empty      = none$(O$i32);

    // Array for demonstration
    A$$(5, i32) numbers = A_init({ 1, 3, 5, 7, 9 });

    // Demonstrate finding values
    let found_index = findValueIndex(5, Sli_arr$(S_const$i32, numbers));
    let not_found   = findValueIndex(6, Sli_arr$(S_const$i32, numbers));

    io_stream_println(u8_l("---- Optional Value Demonstration ----"));

    // Demonstrate if_some pattern
    io_stream_print(u8_l("Looking for value 5: "));
    if_some(found_index, index) {
        io_stream_println(u8_l("Found at index {:d}"), index);
    } else_none {
        io_stream_println(u8_l("Not found"));
    }

    io_stream_print(u8_l("Looking for value 6: "));
    if_some(not_found, index) {
        io_stream_println(u8_l("Found at index {:d}"), index);
    } else_none {
        io_stream_println(u8_l("Not found"));
    }

    // Demonstrate orelse
    io_stream_println(u8_l("\n---- Default Values Demonstration ----"));
    io_stream_println(u8_l("Value with default: {:d}"), orelse(not_found, -1));
    io_stream_println(u8_l("Value from some(): {:d}"), orelse(opt_with_value, -1));

    // Demonstrate the two helper functions
    io_stream_println(u8_l("\n---- Processing Optionals ----"));
    processOptionalValue(opt_with_value);
    processOptionalValue(opt_empty);

    io_stream_println(u8_l("\n---- Nested Optionals Result ----"));
    io_stream_println(u8_l("Both values: {:d}"), processNestedOptionals(opt_with_value, some$(O$i32, 10)));
    io_stream_println(u8_l("First value only: {:d}"), processNestedOptionals(opt_with_value, opt_empty));
    io_stream_println(u8_l("Second value only: {:d}"), processNestedOptionals(opt_empty, some$(O$i32, 10)));
    io_stream_println(u8_l("No values: {:d}"), processNestedOptionals(opt_empty, opt_empty));

    // Example of unwrap (only use when you're sure it's not none)
    io_stream_println(u8_l("\n---- Unwrap Example ----"));
    io_stream_println(u8_l("Unwrapped value: {:d}"), unwrap(opt_with_value));
    // Uncommenting this would cause an assertion failure:
    // io_stream_println(u8_l("This would fail: {:d}"), unwrap(opt_empty));

    return_ok({});
} $unscoped_(fn);

#if README_SAMPLE
$static fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_(($rf(0), $s(items))(index, item) {
        if (*item != value) { continue; }
        return_some(index); // Return with a value
    });
    return_none(); // Return with no value
} $unscoped_(fn);

$static fn_((example(void))(void) $scope) {
    A$$(5, i32) nums = A_init({ 10, 20, 30, 40, 50 });

    // Create optional values
    let opt_value = some$(O$i32, 42);
    let opt_empty = none$(O$i32);

    // Find a value in array
    let found = findValueIndex(30, Sli_arr$(S_const$i32, nums));

    // Check if option has value
    if_some(found, index) {
        io_stream_println(u8_l("Found at: {:d}"), index);
    } else_none {
        io_stream_println(u8_l("Not found"));
    }

    // Default values
    let value = orelse(found, -1); // Use -1 if not found

    // Unsafe extraction (assertion if option might be none)
    let unsafe_value = unwrap(opt_value);
} $unscoped_(fn);
#endif /* README_SAMPLE */
