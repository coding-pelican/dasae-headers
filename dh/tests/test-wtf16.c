#include "dh-main.h"
#include "dh/wtf16.h"

TEST_fn_("wtf16: iterator preserves unpaired surrogates and combines pairs" $scope) {
    var units = A_from$((u16){
        0x0041, 0xD800, 0xD83D, 0xDE00, 0xDC00 $listed
    });
    var it = wtf16_iter(A_ref$((S_const$u16)(units)));

    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0x41));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0xD800));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0x1F600));
    try_(TEST_expect(unwrap_(wtf16_Iter_next(&it)) == 0xDC00));
    try_(TEST_expect(isNone(wtf16_Iter_next(&it))));
} $unscoped(TEST_fn);
