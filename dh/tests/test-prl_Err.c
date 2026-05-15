#include "dh-main.h"

$static fn_((TEST__hashIdCollisionWithNotImplemented(void))(E_HashId)) {
    return E_hasher(u8_l("NotImplemented"));
};

TEST_fn_("prl/Err: resolve rejects hash-only collision" $scope) {
    $static let_(inner, E_Inner$General_E) = {
        .tag = E_Tag$Unexpected,
        .tag_id = u8_l("SyntheticCollision"),
        .hashId = TEST__hashIdCollisionWithNotImplemented,
    };
    let err = (General_E){ .opaq.inner = &inner };

    try_(TEST_expect(isNone(E_resolve$General_E(err))));
} $unscoped(TEST_fn);
