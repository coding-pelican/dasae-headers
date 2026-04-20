/* clang-format off */
#include "dh/prl.h"
#include "dh/fmt/common.h"

/// Shape_VT + Shape
trait_((Shape $vt)(
    fn_(((*Shape_area)(P_const$raw ctx))(f32)),
    fn_(((*Shape_perimeter)(P_const$raw ctx))(f32)),
));
$static fn_((Shape_area(Shape self))(f32)) { return self.vt->Shape_area(self.ctx); };
$static fn_((Shape_perimeter(Shape self))(f32)) { return self.vt->Shape_perimeter(self.ctx); };


typedef struct Circ {
    f32 radius;
} Circ;
/// Circ_Shape_area
$static trait_fn_(((Shape_area $for(Circ))(const Circ* self))(f32)) { return 3.1415926f * self->radius * self->radius; };
/// Circ_Shape_perimeter
$static trait_fn_(((Shape_perimeter $for(Circ))(const Circ* self))(f32)) { return 2.0f * 3.1415926f * self->radius; };
/// Shape_for$Circ
$static trait_impl_((Shape $for(Circ))(
    Shape_area,
    Shape_perimeter,
));


typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
/// Rect_Shape_area
$static trait_fn_(((Shape_area $for(Rect))(const Rect* self))(f32)) { return self->width * self->height; };
/// Rect_Shape_perimeter
$static trait_fn_(((Shape_perimeter $for(Rect))(const Rect* self))(f32)) { return 2.0f * (self->width + self->height); };
/// Shape_for$Rect
$static trait_impl_((Shape $for(Rect))(
    Shape_area,
    Shape_perimeter,
));


/// fmt_Display
trait_((fmt_Display)(
    $attr($must_check)
    fn_(((*fmt_display)(P_const$raw ctx, io_Writer writer))(E$void))
));
$attr($must_check)
$static fn_(((fmt_display)(fmt_Display self, io_Writer writer))(E$void)) { return self.fmt_display(self.ctx, writer); };


typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
/// Entity_fmt_display
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
} $unscoped(trait_fn);
/// fmt_Display_for$Entity
$static trait_impl_((fmt_Display $for(Entity))(fmt_display));


/// Circ_fmt_display
$attr($must_check)
$static trait_fn_(((fmt_display $for(Circ))(const Circ* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Circ {")));
    try_(io_Writer_println(writer, u8_l("    radius: {:.2f}"), self->radius));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped(trait_fn);
/// fmt_Display_for$Circ
$static trait_impl_((fmt_Display $for(Circ))(fmt_display));


/// Rect_fmt_display
$attr($must_check)
$static trait_fn_(((fmt_display $for(Rect))(const Rect* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Rect {")));
    try_(io_Writer_println(writer, u8_l("    width: {:.2f}"), self->width));
    try_(io_Writer_println(writer, u8_l("    height: {:.2f}"), self->height));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped(trait_fn);
/// fmt_Display_for$Rect
$static trait_impl_((fmt_Display $for(Rect))(fmt_display));
/* clang-format on */


#include "dh/io/common.h"
#include "dh/io/stream.h"
#include "dh/fs/File.h"
#include "dh/ArrList.h"
T_use_prl$(Shape);
T_use$((Shape)(ArrList, ArrList_fixed, ArrList_appendFixed));

$static var_(shapes_mem, A$$(128, Shape)) = A_zero();
$static let shapes_buf = A_ref$((S$Shape)(shapes_mem));
$attr($must_check)
$static fn_((example_usage(void))(E$void) $scope) {
    let rects = ({ var initial = l0$((A$$(16, Rect))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->width = as$(f32)(idx + 1);
        item->height = as$(f32)(idx + 2);
    }); initial; });
    let circs = ({ var initial = l0$((A$$(16, Circ))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->radius = as$(f32)(idx + 1);
    }); initial; });

    var shapes = ArrList_fixed$Shape(shapes_buf);
    for_(($s(A_ref(rects)))(item)try_(ArrList_appendFixed$Shape(&shapes, Shape_for$Rect(item))));
    for_(($s(A_ref(circs)))(item)try_(ArrList_appendFixed$Shape(&shapes, Shape_for$Circ(item))));

    let out = fs_File_writer(io_getStdOut());
    for_(($s(shapes.items))(item) {
        try_(fmt_display(fmt_Display_for$Shape(item), out));
        try_(io_Writer_nl(out));
    });
    return_ok({});
}
$unscoped(fn);


#include "dh-main.h"
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(example_usage()));
}
$unscoped(fn);
