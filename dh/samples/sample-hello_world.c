// Include the main header that provides program entry point
#include "dh/main.h"
// Include for `io_getStdOut()`
#include "dh/io/common.h"
// Include for `fs_File_writer()`
#include "dh/fs/File.h"
// Include for `io_Writer_print()`
#include "dh/io/Writer.h"

// Define the main function with scope and error handling
// Takes command line arguments and returns an error result with void payload
fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $scope) {
    // if you want to ignore variable, you can use $ignore
    let_ignore = args;
    // Get the standard output file writer
    let out    = fs_File_writer(io_getStdOut());
    // Print the message to the standard output with optional message
    let msg    = some$(Opt$Sli_const$u8, u8_l("world"));
    try_(io_Writer_print(out, u8_l("Hello, {:s}!\n"), unwrap(msg)));
    // Return success (void value with no error)
    return_ok({});
} $unscoped_(fn); // End the scope block
