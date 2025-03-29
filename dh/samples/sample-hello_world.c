// Include the main header that provides program entry point
#include "dh/main.h"
// Include string utilities for working with text
#include "dh/Str.h"

// Define the main function with extended scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    $ignore args;

    // Create a string literal using Str_l
    let hello_world = Str_l("Hello, world!");

    // Print the string to the console with a newline
    Str_println(hello_world);

    // Return success (void value with no error)
    return_ok({});
} ext_unscoped; // End the extended scope block
