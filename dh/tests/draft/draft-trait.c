/* clang-format off */
#include "dh/prl.h"
#include "dh/fmt/common.h"

/// Shape_VT + Shape
/// ShapeDyn
trait_((Shape $vt)(
    fn_(((*Shape_area)(u_P_const$raw ctx))(f32)),
    fn_(((*Shape_perimeter)(u_P_const$raw ctx))(f32)),
));
$static fn_((Shape_area(Shape self, u_P_const$raw ctx))(f32)) /* TODO: make this abstract */ {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eq), self.vt->Shape_area(self.ctx);
};
$static fn_((Shape_perimeter(Shape self, u_P_const$raw ctx))(f32)) /* TODO: make this abstract */ {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eq), self.vt->Shape_perimeter(self.ctx);
};


typedef struct Circ {
    f32 radius;
} Circ;
/// Circ_Shape_area + Shape_area$Circ
$static trait_fn_(((Shape_area $for(Circ))(const Circ* self))(f32)) { return 3.1415926f * self->radius * self->radius; };
/// Circ_Shape_perimeter + Shape_perimeter$Circ
$static trait_fn_(((Shape_perimeter $for(Circ))(const Circ* self))(f32)) { return 2.0f * 3.1415926f * self->radius; };
/// Shape_for$Circ
/// Shape_dyn$Circ
$static trait_impl_((Shape $for(Circ))(
    Shape_area,
    Shape_perimeter,
));

typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
/// Rect_Shape_area + Shape_area$Rect
$static trait_fn_(((Shape_area $for(Rect))(const Rect* self))(f32)) { return self->width * self->height; };
/// Rect_Shape_perimeter + Shape_perimeter$Rect
$static trait_fn_(((Shape_perimeter $for(Rect))(const Rect* self))(f32)) { return 2.0f * (self->width + self->height); };
/// Shape_for$Rect
/// Shape_dyn$Rect
$static trait_impl_((Shape $for(Rect))(
    Shape_area,
    Shape_perimeter,
));


/// fmt_Display + fmt_DisplayDyn
trait_((fmt_Display)(
    $attr($must_check)
    fn_(((*fmt_display)(u_P_const$raw ctx, io_Writer writer))(E$void))
));
$attr($must_check)
$static fn_(((fmt_display)(fmt_Display self, u_P_const$raw ctx, io_Writer writer))(E$void)) /* TODO: make this abstract */ {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eq), self.fmt_display(ctx, writer);
};


typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
/// Entity_fmt_display + fmt_display$Entity
$attr($must_check)
$static trait_fn_(((fmt_display $for(Entity))(const Entity* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Entity {")));
    try_(io_Writer_println(writer, u8_l("    id: {:uz}"), self->id));
    try_(io_Writer_println(writer, u8_l("    px: {:.2f}"), self->px));
    try_(io_Writer_println(writer, u8_l("    py: {:.2f}"), self->py));
    try_(io_Writer_println(writer, u8_l("    vx: {:.2f}"), self->vx));
    try_(io_Writer_println(writer, u8_l("    vy: {:.2f}"), self->vy));
    try_(io_Writer_println(writer, u8_l("    health: {:d}"), self->health));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(trait_fn);
/// fmt_Display_for$Entity
/// fmt_Display_dyn$Entity
$static trait_impl_((fmt_Display $for(Entity))(fmt_display));


/// Circ_fmt_display + fmt_display$Circ
$attr($must_check)
$static trait_fn_(((fmt_display $for(Circ))(const Circ* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Circ {")));
    try_(io_Writer_println(writer, u8_l("    radius: {:.2f}"), self->radius));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(trait_fn);
/// fmt_Display_for$Circ
/// fmt_Display_dyn$Circ
$static trait_impl_((fmt_Display $for(Circ))(fmt_display));


/// Rect_fmt_display + fmt_display$Rect
$attr($must_check)
$static trait_fn_(((fmt_display $for(Rect))(const Rect* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Rect {")));
    try_(io_Writer_println(writer, u8_l("    width: {:.2f}"), self->width));
    try_(io_Writer_println(writer, u8_l("    height: {:.2f}"), self->height));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(trait_fn);
/// fmt_Display_for$Rect
/// fmt_Display_dyn$Rect
$static trait_impl_((fmt_Display $for(Rect))(fmt_display));


/// ShapeDyn_fmt_display + fmt_display$ShapeDyn
$attr($must_check)
$static trait_fn_(((fmt_display $for(ShapeDyn))(const ShapeDyn* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Shape {")));
    try_(io_Writer_println(writer, u8_l("    Area: {:.2f}"), Shape_area(self->trait, self->ctx)));
    try_(io_Writer_println(writer, u8_l("    Perimeter: {:.2f}"), Shape_perimeter(self->trait, self->ctx)));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(trait_fn);
/// fmt_Display_for$ShapeDyn
/// fmt_Display_dyn$ShapeDyn
$static trait_impl_((fmt_Display $for(ShapeDyn))(fmt_display));
/* clang-format on */


#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/fs/File.h"
#include "dh/ArrList.h"
T_use_prl$(ShapeDyn);
T_use$((ShapeDyn)(ArrList, ArrList_fixed, ArrList_appendFixed));

$static var_(shapes_mem, A$$(128, ShapeDyn)) = A_zero();
$static let shapes_buf = A_ref$((S$ShapeDyn)(shapes_mem));
$attr($must_check)
$static fn_((example_usage(void))(E$void) $scope) {
    let rects = ({ var initial = lit0$((A$$(16, Rect))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->width = as$(f32)(idx + 1);
        item->height = as$(f32)(idx + 2);
    }); initial; });
    let circs = ({ var initial = lit0$((A$$(16, Circ))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->radius = as$(f32)(idx + 1);
    }); initial; });

    var shapes = ArrList_fixed$ShapeDyn(shapes_buf);
    for_(($s(A_ref(rects)))(item)try_(ArrList_appendFixed$ShapeDyn(&shapes, Shape_dyn$Rect(item))));
    for_(($s(A_ref(circs)))(item)try_(ArrList_appendFixed$ShapeDyn(&shapes, Shape_dyn$Circ(item))));

    let out = fs_File_writer(io_getStdOut());
    for_(($s(shapes.items.as_const))(item) {
        try_(fmt_display(fmt_Display_for$ShapeDyn(), u_anyP(item), out));
        try_(io_Writer_nl(out));
    });
    return_ok({});
} $unscoped_(fn);


#include "dh/main.h"
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(example_usage()));
} $unscoped_(fn);
