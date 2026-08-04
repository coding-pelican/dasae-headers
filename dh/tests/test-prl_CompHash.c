#include "dh-main.h"
#include "dh/prl/CompHash.h"

TEST_fn_("prl/CompHash: explicit widths preserve their container values" $scope) {
    let hash16 = CompHash16_from(u8_l("dasae"));
    let hash32 = CompHash32_from(u8_l("dasae"));
    let hash64 = CompHash64_from(u8_l("dasae"));

    try_(TEST_expect(CompHash16_val(hash16) == compHash16("dasae")));
    try_(TEST_expect(CompHash32_val(hash32) == compHash32("dasae")));
    try_(TEST_expect(CompHash64_val(hash64) == compHash64("dasae")));
    try_(TEST_expect(CompHash16_val(hash16) == as$(u16)(CompHash32_val(hash32))));
} $unscoped(TEST_fn);

TEST_fn_("prl/CompHash: native hash follows usize ABI width" $scope) {
    let expected = pp_switch_((abi_size_unit)(
        pp_case_((abi_bits_unit_16bit)(as$(usize)(compHash16("native")))),
        pp_case_((abi_bits_unit_32bit)(as$(usize)(compHash32("native")))),
        pp_case_((abi_bits_unit_64bit)(as$(usize)(compHash64("native"))))
    ));

    try_(TEST_expect(compHash("native") == expected));
    try_(TEST_expect(CompHash_val(CompHash_from(u8_l("native"))) == expected));
} $unscoped(TEST_fn);
