#include "dh/core/pp.h"
#include "dh/core/pp/Tuple.h"
#include "dh/claim/assert_static.h"
#include "dh/core/pp/common.h"
#include "dh/main.h"
#include "dh/mem.h"

#define TUPLE_0_to_8 (0)(1)(2)(3)(4)(5)(6)(7)

#define pp_TupleChain_ignore8(...) pp_TupleChain_ignore7
#define pp_TupleChain_ignore7(...) pp_TupleChain_ignore6
#define pp_TupleChain_ignore6(...) pp_TupleChain_ignore5
#define pp_TupleChain_ignore5(...) pp_TupleChain_ignore4
#define pp_TupleChain_ignore4(...) pp_TupleChain_ignore3
#define pp_TupleChain_ignore3(...) pp_TupleChain_ignore2
#define pp_TupleChain_ignore2(...) pp_TupleChain_ignore1
#define pp_TupleChain_ignore1(...)
#define pp_TupleChain_expand1AndIgnoreN(N...) \
    pp_cat(pp_TupleChain_ignore, N)

#define pp_TupleChain_collect8(...)                                                                         pp_TupleChain_expand1AndIgnoreN(8)
#define pp_TupleChain_collect7(_cache0, _cache1, ...)                                                       pp_TupleChain_expand1AndIgnoreN(7)
#define pp_TupleChain_collect6(_cache0, _cache1, _cache2, ...)                                              pp_TupleChain_collect5
#define pp_TupleChain_collect5(_cache0, _cache1, _cache2, _cache3, ...)                                     pp_TupleChain_collect4
#define pp_TupleChain_collect4(_cache0, _cache1, _cache2, _cache3, _cache4, ...)                            pp_TupleChain_collect3
#define pp_TupleChain_collect3(_cache0, _cache1, _cache2, _cache3, _cache4, _cache5, ...)                   pp_TupleChain_collect2
#define pp_TupleChain_collect2(_cache0, _cache1, _cache2, _cache3, _cache4, _cache5, _cache6, ...)          pp_TupleChain_collect1
#define pp_TupleChain_collect1(_cache0, _cache1, _cache2, _cache3, _cache4, _cache5, _cache6, _cache7, ...) pp_TupleChain_collect0
#define pp_TupleChain_collect0(_cache0, _cache1, _cache2, _cache3, _cache4, _cache5, _cache6, _cache7, _cache8, ...) \
    (_cache0, _cache1, _cache2, _cache3, _cache4, _cache5, _cache6, _cache7, _cache8)

// pp_expand(pp_TupleChain_collect8 TUPLE_0_to_8)

// pp_expand(pp_TupleChain_ignore6 TUPLE_0_to_8) pp_expand(pp)

// Extract 1st tuple (index 0) from chain of 3
#define pp_get1st(chain)                   pp_expand(pp_TupleChain_expand1_ignore2 chain)
#define pp_TupleChain_expand1_ignore2(...) __VA_ARGS__ pp_TupleChain_ignore2
#define pp_TupleChain_ignore2(...)         pp_TupleChain_ignore1
#define pp_TupleChain_ignore1(...)

// Extract 2nd tuple (index 1) from chain of 3
#define pp_get2nd(chain)                           pp_expand(pp_TupleChain_ignore1_expand1_ignore1 chain)
#define pp_TupleChain_ignore1_expand1_ignore1(...) pp_TupleChain_expand1_ignore1
#define pp_TupleChain_expand1_ignore1(...)         __VA_ARGS__ pp_TupleChain_ignore1_2
#define pp_TupleChain_ignore1_2(...)

// Extract 3rd tuple (index 2) from chain of 3
#define pp_get3rd(chain)                   pp_expand(pp_TupleChain_ignore2_expand1 chain)
#define pp_TupleChain_ignore2_expand1(...) pp_TupleChain_ignore1_expand1
#define pp_TupleChain_ignore1_expand1(...) pp_TupleChain_expand1
#define pp_TupleChain_expand1(...)         __VA_ARGS__

TEST_fn_("TupleList get n'th" $scope) {
    try_(TEST_expect(Str_eql(u8_l(pp_stringify(pp_get1st((a, b)()(c)))), u8_l("(a, b)"))));
    try_(TEST_expect(Str_eql(u8_l(pp_stringify(pp_get2nd((a, b)()(c)))), u8_l("()"))));
    try_(TEST_expect(Str_eql(u8_l(pp_stringify(pp_get3rd((a, b)()(c)))), u8_l("(c)"))));
} $unscoped_(TEST_fn);
