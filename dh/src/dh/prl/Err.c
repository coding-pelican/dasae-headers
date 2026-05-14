#include "dh/prl/Err.h"
#include "dh/io/stream.h"

fn_((E_print(const EAny* err))(void)) {
    io_stream_eprintln(u8_l("Program failed: {:e}"), *err);
};
