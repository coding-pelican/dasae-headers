// Include the main header that provides program entry point
#include "dh/main.h"
#include "dh/io/common.h" /* for `io_getStdOut()` */
#include "dh/fs/File.h"   /* for `fs_File_writer()` */
#include "dh/io/Writer.h" /* for `io_Writer_print()` */

// Define the main function with scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    // if you want to ignore variable, you can use let_ignore
    let_ignore = args;
    // Get the standard output file writer
    let out = fs_File_writer(io_getStdOut());
    // Print the message to the standard output with optional message
    let_(msg, O$S_const$u8) = some(u8_l("world"));
    try_(io_Writer_print(out, u8_l("Hello, {:?s}!\n"), msg));
    // Return success (void value with no error)
    return_ok({});
} $unscoped_(fn); // End the scope block
