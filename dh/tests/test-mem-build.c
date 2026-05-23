#include "dh-main.h"
#include "dh/mem/common.h"

T_use$((u8)(
    mem_trimStart,
    mem_trimEnd,
    mem_trim,
    mem_cat,
    mem_catWithin,
    mem_join,
    mem_joinWithin,
    mem_padLeft,
    mem_padLeftWithin,
    mem_padRight,
    mem_padRightWithin,
    mem_padCenter,
    mem_padCenterWithin
));

TEST_fn_("mem: build - trim strips selected edge units" $scope) {
    let padded = u8_l(" \tvalue \n");

    try_(TEST_expect(mem_eqlBytes(mem_trimStart$u8(padded, u8_l(" \t\n")), u8_l("value \n"))));
    try_(TEST_expect(mem_eqlBytes(mem_trimEnd$u8(padded, u8_l(" \t\n")), u8_l(" \tvalue"))));
    try_(TEST_expect(mem_eqlBytes(mem_trim$u8(padded, u8_l(" \t\n")), u8_l("value"))));

    try_(TEST_expect(mem_eqlBytes(mem_trimBytes(u8_l("--value--"), u8_l("-")), u8_l("value"))));
} $unscoped(TEST_fn)

TEST_fn_("mem: build - cat and join write into caller buffers" $scope) {
    var cat_buf = u8_a("................");
    let cat = mem_catWithin$u8(u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(cat_buf)));
    try_(TEST_expect(mem_eqlBytes(cat.as_const, u8_l("abcd"))));

    var checked_cat_buf = u8_a("................");
    let checked_cat = try_(mem_cat$u8(u8_l("xy"), u8_l("z"), A_ref$((S$u8)(checked_cat_buf))));
    try_(TEST_expect(mem_eqlBytes(checked_cat.as_const, u8_l("xyz"))));

    var join_buf = u8_a("................");
    let joined = mem_joinWithin$u8(u8_l(","), u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(join_buf)));
    try_(TEST_expect(mem_eqlBytes(joined.as_const, u8_l("ab,cd"))));

    var checked_buf = u8_a("................");
    let checked = try_(mem_join$u8(u8_l(":"), u8_l("ab"), u8_l("cd"), A_ref$((S$u8)(checked_buf))));
    try_(TEST_expect(mem_eqlBytes(checked.as_const, u8_l("ab:cd"))));
} $unscoped(TEST_fn)

TEST_fn_("mem: build - pad writes left right and centered output" $scope) {
    var pad_buf = u8_a("................");

    try_(TEST_expect(mem_eqlBytes(mem_padLeftWithinBytes(u8_l("xy"), 5, u8_c('.'), A_ref$((S$u8)(pad_buf))).as_const, u8_l("...xy"))));
    try_(TEST_expect(mem_eqlBytes(try_(mem_padLeft$u8(u8_l("xy"), 4, u8_c('_'), A_ref$((S$u8)(pad_buf)))).as_const, u8_l("__xy"))));
    try_(TEST_expect(mem_eqlBytes(mem_padLeftWithin$u8(u8_l("wide"), 2, u8_c('_'), A_ref$((S$u8)(pad_buf))).as_const, u8_l("wide"))));
    try_(TEST_expect(mem_eqlBytes(try_(mem_padRight$u8(u8_l("xy"), 4, u8_c('_'), A_ref$((S$u8)(pad_buf)))).as_const, u8_l("xy__"))));
    try_(TEST_expect(mem_eqlBytes(mem_padRightWithin$u8(u8_l("xy"), 5, u8_c('.'), A_ref$((S$u8)(pad_buf))).as_const, u8_l("xy..."))));
    try_(TEST_expect(mem_eqlBytes(try_(mem_padCenter$u8(u8_l("xy"), 5, u8_c('.'), A_ref$((S$u8)(pad_buf)))).as_const, u8_l(".xy.."))));
    try_(TEST_expect(mem_eqlBytes(mem_padCenterWithin$u8(u8_l("xy"), 6, u8_c('_'), A_ref$((S$u8)(pad_buf))).as_const, u8_l("__xy__"))));
} $unscoped(TEST_fn)
