#include "dh-main.h"

#define test_builtin_pp__args_64 \
    1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1, \
        1, 1, 1, 1, 1, 1, 1, 1
#define test_builtin_pp__foreach_count(_$iter, _$arg) +(_$arg)
#define test_builtin_pp__fold_add(_$lhs, _$rhs) ((_$lhs) + (_$rhs))

enum {
    test_builtin_pp__count_64 = pp_countArg(test_builtin_pp__args_64),
    test_builtin_pp__foreach_64 = 0 pp_foreach(test_builtin_pp__foreach_count, ~, test_builtin_pp__args_64),
    test_builtin_pp__foldl_64 = pp_foldl(test_builtin_pp__fold_add, 0, test_builtin_pp__args_64),
    test_builtin_pp__foldr_64 = pp_foldr(test_builtin_pp__fold_add, 0, test_builtin_pp__args_64),
};

claim_assert_static(test_builtin_pp__count_64 == 64);
claim_assert_static(test_builtin_pp__foreach_64 == 64);
claim_assert_static(test_builtin_pp__foldl_64 == 64);
claim_assert_static(test_builtin_pp__foldr_64 == 64);

TEST_fn_("builtin/pp: 64-argument iterators" $scope) {
    try_(TEST_expect(test_builtin_pp__count_64 == 64));
    try_(TEST_expect(test_builtin_pp__foreach_64 == 64));
    try_(TEST_expect(test_builtin_pp__foldl_64 == 64));
    try_(TEST_expect(test_builtin_pp__foldr_64 == 64));
} $unscoped(TEST_fn);
