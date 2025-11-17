#include "dh/main.h"
#include "dh/mem.h"
#include "dh/io/stream.h"

T_use_mem_copyFwds$(u8);
TEST_fn_("test_mem_copy" $scope) {
    var dst = u8_a("............................");
    let src = u8_l("Hello, World!");
    mem_copyFwds$u8(A_ref$((S$u8)dst), src);
    io_stream_println(u8_l("{:s}"), A_ref$((S$u8)dst));
    io_stream_println(u8_l("{:s}"), src);
} $unscoped_(TEST_fn)
