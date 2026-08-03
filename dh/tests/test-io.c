#include "dh-main.h"
#include "dh/io/common.h"
#include "dh/mem/common.h"

TEST_fn_("io/common: native newline follows platform separator" $scope) {
    let expected = pp_if_(plat_is_windows)(
        pp_then_(u8_l(io_crlf)),
        pp_else_(u8_l(io_nl)));

    try_(TEST_expect(mem_eqlBytes(u8_l(io_nl_native), expected)));
} $unscoped(TEST_fn);
