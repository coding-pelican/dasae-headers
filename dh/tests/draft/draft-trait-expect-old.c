#include "dh/prl.h"
#include "dh/fmt/common.h"


typedef struct Shape_VT {
    fn_(((*Shape_area)(P_const$raw ctx))(f32));
    fn_(((*Shape_perimeter)(P_const$raw ctx))(f32));
} Shape_VT;
typedef struct Shape {
    var_(ctx, P_const$raw);
    var_(vt, P_const$$(Shape_VT));
} Shape;
$static fn_((Shape_area(Shape self))(f32)) { return self.vt->Shape_area(self.ctx); };
$static fn_((Shape_perimeter(Shape self))(f32)) { return self.vt->Shape_perimeter(self.ctx); };

typedef struct Circ {
    f32 radius;
} Circ;
$static /* Generated begin */ fn_((Circ_area(const Circ* self))(f32));
$static fn_((Shape_area$Circ(P_const$raw ctx))(f32)) { return Circ_area(ptrAlignCast$((const Circ*)(ctx))); };
fn_((Circ_area(const Circ* self))(f32)) { /* Generated end */ return 3.1415926f * self->radius * self->radius; };
$static /* Generated begin */ fn_((Circ_perimeter(const Circ* self))(f32));
$static fn_((Shape_perimeter$Circ(P_const$raw ctx))(f32)) { return Circ_perimeter(ptrAlignCast$((const Circ*)(ctx))); };
fn_((Circ_perimeter(const Circ* self))(f32)) { /* Generated end */ return 2.0f * 3.1415926f * self->radius; };
$attr($maybe_unused)
$static fn_((Shape_for$Circ(const Circ* self))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = Shape_area$Circ,
        .Shape_perimeter = Shape_perimeter$Circ,
    };
    return (Shape){
        .ctx = ptrCast$((P_const$raw)(self)),
        .vt = &vt,
    };
};

typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
$static /* Generated begin */ fn_((Rect_area(const Rect* self))(f32));
$static fn_((Shape_area$Rect(P_const$raw ctx))(f32)) { return Rect_area(ptrAlignCast$((const Rect*)(ctx))); };
fn_((Rect_area(const Rect* self))(f32)) { /* Generated end */ return self->width * self->height; };
$static /* Generated begin */ fn_((Rect_perimeter(const Rect* self))(f32));
$static fn_((Shape_perimeter$Rect(P_const$raw ctx))(f32)) { return Rect_perimeter(ptrAlignCast$((const Rect*)(ctx))); };
fn_((Rect_perimeter(const Rect* self))(f32)) { /* Generated end */ return 2.0f * (self->width + self->height); };
$attr($maybe_unused)
$static fn_((Shape_for$Rect(const Rect* self))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = Shape_area$Rect,
        .Shape_perimeter = Shape_perimeter$Rect,
    };
    return (Shape){
        .ctx = ptrCast$((P_const$raw)(self)),
        .vt = &vt,
    };
};


typedef struct fmt_Display {
    var_(ctx, P_const$raw);
    fn_(((*fmt_display)(P_const$raw ctx, io_Writer writer))(E$void));
} fmt_Display;
$attr($must_check)
$static fn_((fmt_display(fmt_Display self, io_Writer writer))(E$void) $scope) {
    return_ok(try_(self.fmt_display(self.ctx, writer)));
} $unscoped_(fn);

typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
$attr($must_check)
$static /* Generated begin */ fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Entity(P_const$raw ctx, io_Writer writer))(E$void)) { return Entity_fmt_display(ptrAlignCast$((const Entity*)(ctx)), writer); };
fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Entity {")));
    try_(io_Writer_println(writer, u8_l("    id: {:uz}"), self->id));
    try_(io_Writer_println(writer, u8_l("    px: {:.2f}"), self->px));
    try_(io_Writer_println(writer, u8_l("    py: {:.2f}"), self->py));
    try_(io_Writer_println(writer, u8_l("    vx: {:.2f}"), self->vx));
    try_(io_Writer_println(writer, u8_l("    vy: {:.2f}"), self->vy));
    try_(io_Writer_println(writer, u8_l("    health: {:d}"), self->health));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$attr($maybe_unused)
$static fn_((fmt_Display_for$Entity(const Entity* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = ptrCast$((P_const$raw)(self)), .fmt_display = fmt_display$Entity };
};


$attr($must_check)
$static /* Generated begin */ fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Circ(P_const$raw ctx, io_Writer writer))(E$void)) { return Circ_fmt_display(ptrAlignCast$((const Circ*)(ctx)), writer); };
fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Circ {")));
    try_(io_Writer_println(writer, u8_l("    radius: {:.2f}"), self->radius));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$attr($maybe_unused)
$static fn_((fmt_Display_for$Circ(const Circ* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = ptrCast$((P_const$raw)(self)), .fmt_display = fmt_display$Circ };
};

$attr($must_check)
$static /* Generated begin */ fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Rect(P_const$raw ctx, io_Writer writer))(E$void)) { return Rect_fmt_display(ptrAlignCast$((const Rect*)(ctx)), writer); };
fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Rect {")));
    try_(io_Writer_println(writer, u8_l("    width: {:.2f}"), self->width));
    try_(io_Writer_println(writer, u8_l("    height: {:.2f}"), self->height));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$attr($maybe_unused)
$static fn_((fmt_Display_for$Rect(const Rect* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = ptrCast$((P_const$raw)(self)), .fmt_display = fmt_display$Rect };
};


/// Shape_fmt_display
$attr($must_check)
$static /* Generated begin */ fn_((Shape_fmt_display(const Shape* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Shape(P_const$raw ctx, io_Writer writer))(E$void)) { return Shape_fmt_display(ptrAlignCast$((const Shape*)(ctx)), writer); };
fn_((Shape_fmt_display(const Shape* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Shape {")));
    try_(io_Writer_println(writer, u8_l("    Area: {:.2f}"), Shape_area(*self)));
    try_(io_Writer_println(writer, u8_l("    Perimeter: {:.2f}"), Shape_perimeter(*self)));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$attr($maybe_unused)
$static fn_((fmt_Display_for$Shape(const Shape* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = ptrCast$((P_const$raw)(self)), .fmt_display = fmt_display$Shape };
};


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
    let rects = ({ var initial = lit0$((A$$(16, Rect))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->width = as$(f32)(idx + 1);
        item->height = as$(f32)(idx + 2);
    }); initial; });
    let circs = ({ var initial = lit0$((A$$(16, Circ))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
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
} $unscoped_(fn);


#include "dh/main.h"
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(example_usage()));
} $unscoped_(fn);
