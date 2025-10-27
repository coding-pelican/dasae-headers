#define main_no_args 1
#include "dh/main.h"

#include "dh/io/stream.h"

fn_((dh_main(void))(E$void) $scope) {
    let loc = srcLoc();
    io_stream_print(u8_l("file_path: {:z}\n"), loc.file_path);
    io_stream_print(u8_l("file_name: {:z}\n"), loc.file_name);
    io_stream_print(u8_l("fn_name:   {:z}\n"), loc.fn_name);
    io_stream_print(u8_l("line:      {:u}\n"), loc.line);
    io_stream_print(u8_l("column:    {:u}\n"), loc.column);
    io_stream_print(u8_l("{:z}:{:u}:{:u}\n"), loc.file_path, loc.line, loc.column);
    return_ok({});
} $unscoped_(fn);
