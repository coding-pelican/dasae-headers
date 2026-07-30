// Include the main header that provides program entry point
#include "dh-main.h"
#include "dh/io/self.h" /* for `io_handleStdOut()` */
#include "dh/fs/File/self.h" /* for `fs_File_writer()` */
#include "dh/io/Writer.h" /* for `io_Writer_writeBytes()` */

// Define the main function with scope and error handling
// Takes the explicit process startup context and returns an error result with void payload
fn_((main(proc_Self self))(E$void) $scope) {
    let args = self.args.items;
    // if you want to ignore variable, you can use let_ignore
    let_ignore = args;
    // Get the standard output file writer
    let out = fs_File_writer(io_handleStdOut());
    // Write the message to the standard output
    try_(io_Writer_writeBytes(out, u8_l("Hello, world!\n")));
    // Return success (void value with no error)
    return_ok({});
} $unscoped(fn); // End the scope block
