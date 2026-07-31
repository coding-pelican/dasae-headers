// Include the main header that provides program entry point
#include "dh-main.h"
#include "dh/proc/std.h" /* for `proc_std_out()` */
#include "dh/fs/File/self.h" /* for `fs_File_writer()` */
#include "dh/io/Writer.h" /* for `io_Writer_writeBytes()` */

// Define the main function with scope and error handling
// Takes the explicit process startup context and returns an error result with void payload
fn_((main(proc_Entry entry))(E$void) $scope) {
    // if you want to ignore variable, you can use let_ignore
    let_ignore = entry;
    // Get the standard output file writer
    let out = fs_File_writer(proc_std_out());
    // Write the message to the standard output
    try_(io_Writer_writeBytes(out, u8_l("Hello, world!\n")));
    // Return success (void value with no error)
    return_ok({});
} $unscoped(fn); // End the scope block
