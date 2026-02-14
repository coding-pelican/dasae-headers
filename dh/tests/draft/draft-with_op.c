#include "dh/main.h"

#define with_(/*(_expr: _T)(_initial...: (_field)(_asg)*/... /*(_T)*/) __step__with_(__VA_ARGS__)
#define __step__with_(...) __step__with$__emit(__step__with$__parseExpr __VA_ARGS__)
#define __step__with$__parseExpr(_expr...) pp_uniqTok(expr_copied), _expr, __step__with$__parseInitial
#define __step__with$__parseInitial(_initial...) _initial
#define __step__with$__emit(...) \
    ____with_(__VA_ARGS__)
#define ____with_(__expr_copied, _expr, _initial...) blk({ \
    var __expr_copied = _expr; \
    pp_foreach(____with___each, __expr_copied, _initial); \
    blk_return_(__expr_copied); \
})
#define ____with___each(__expr_copied, /*_initial*/...) __VA_OPT__( \
    ____with___each__emit(__expr_copied, ____with___each__parseField __VA_ARGS__) \
)
#define ____with___each__parseField(_field...) _field, ____with___each__parseAsg
#define ____with___each__parseAsg(_asg...) _asg
#define ____with___each__emit(...) \
    ____with_____each(__VA_ARGS__)
#define ____with_____each(__expr_copied, _field, _asg...) \
    asg_lit((&__expr_copied _field)(_asg));

TEST_fn_("thin struct binding" $scope) {
    typedef struct Foo {
        i32 a;
        i32 b;
        i32 c;
    } Foo;
    let_(foo0, Foo) = {
        .a = 0,
        .b = 0,
        .c = 0,
    };
    let foo1 = with_((foo0)((.a)(1)));
    let foo2 = with_((foo0)((.b)(2), (.c)(3)));

    try_(TEST_expect(foo0.a == 0));
    try_(TEST_expect(foo0.b == 0));
    try_(TEST_expect(foo0.c == 0));

    try_(TEST_expect(foo1.a == 1));
    try_(TEST_expect(foo1.b == foo0.b));
    try_(TEST_expect(foo1.c == foo0.c));

    try_(TEST_expect(foo2.a == foo1.a));
    try_(TEST_expect(foo2.b == foo0.b));
    try_(TEST_expect(foo2.c == foo0.c));
    return_ok({});
} $unscoped_(TEST_fn);

TEST_fn_("nested struct binding" $scope) {
    typedef struct Bar_Id {
        S_const$u8 name;
        i32 num;
    } Bar_Id;
    typedef struct Bar_Point {
        i32 x;
        i32 y;
    } Bar_Point;
    typedef struct Bar {
        Bar_Id id;
        Bar_Point point;
    } Bar;
    let Bar_id_name_default = u8_l(pp_strfy(Bar));

    let_(bar0, Bar) = {
        .id = {
            .name = Bar_id_name_default,
            .num = 0,
        },
        .point = {
            .x = 0,
            .y = 0,
        },
    };
    let bar1 = with_((bar0)(
        (.id.num)(bar0.id.num + 1),
        (.point)({
            .x = 1,
            .y = 2,
        })
    ));
    let bar2_name = u8_l("custom_name");
    let bar2 = with_((bar1)(
        (.id.name)(bar2_name),
        (.point)({
            .x = 1,
            .y = 2,
        })
    ));

    try_(TEST_expect(mem_eqlBytes(bar0.id.name, Bar_id_name_default)));
    try_(TEST_expect(bar0.id.num == 0));
    try_(TEST_expect(bar0.point.x == 0));
    try_(TEST_expect(bar0.point.y == 0));

    try_(TEST_expect(mem_eqlBytes(bar1.id.name, bar0.id.name)));
    try_(TEST_expect(bar1.id.num == bar0.id.num + 1));
    try_(TEST_expect(bar1.point.x == 1));
    try_(TEST_expect(bar1.point.y == 2));

    try_(TEST_expect(mem_eqlBytes(bar2.id.name, bar2_name)));
    try_(TEST_expect(bar2.id.num == bar1.id.num));
    try_(TEST_expect(bar2.point.x == bar1.point.x));
    try_(TEST_expect(bar2.point.y == bar1.point.y));
} $unscoped_(TEST_fn);
