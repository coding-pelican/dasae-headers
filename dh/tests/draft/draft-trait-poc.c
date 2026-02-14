#include "dh/prl.h"
#include "dh/fmt/common.h"


// trait Shape {
//     fn area(&self) -> f32;
//     fn perimeter(&self) -> f32;
// }
typedef struct Shape_VT {
    fn_(((*area)(P_const$raw ctx))(f32));
    fn_(((*perimeter)(P_const$raw ctx))(f32));
} Shape_VT;
typedef struct Shape {
    var_(ctx, P_const$raw);
    var_(vt, P_const$$(Shape_VT));
} Shape;
$static fn_((Shape_area(Shape self))(f32)) { return self.vt->area(self.ctx); };
$static fn_((Shape_perimeter(Shape self))(f32)) { return self.vt->perimeter(self.ctx); };


typedef struct Circ {
    f32 radius;
} Circ;
$static fn_((Circ_area(const Circ* self))(f32)) { return 3.1415926f * self->radius * self->radius; };
$static fn_((Circ_perimeter(const Circ* self))(f32)) { return 2.0f * 3.1415926f * self->radius; };
// $static fn_((Circ_shape(const Circ* self))(Shape))
$static fn_((Shape_trait$Circ(const Circ* self))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .area = as$(FieldType$(Shape_VT, area))(Circ_area),
        .perimeter = as$(FieldType$(Shape_VT, perimeter))(Circ_perimeter),
    };
    return (Shape){
        .ctx = self,
        .vt = &vt,
    };
};

typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
$static fn_((Rect_area(const Rect* self))(f32)) { return self->width * self->height; };
$static fn_((Rect_perimeter(const Rect* self))(f32)) { return 2.0f * (self->width + self->height); };
// $static fn_((Rect_shape(const Rect* self))(Shape))
$static fn_((Shape_trait$Rect(const Rect* self))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .area = as$(FieldType$(Shape_VT, area))(Rect_area),
        .perimeter = as$(FieldType$(Shape_VT, perimeter))(Rect_perimeter),
    };
    return (Shape){
        .ctx = self,
        .vt = &vt,
    };
};


typedef struct fmt_Display {
    var_(ctx, P_const$raw);
    fn_(((*fn)(P_const$raw ctx, io_Writer writer))(E$void));
} fmt_Display;
$attr($must_check)
$static fn_((fmt_display(fmt_Display self, io_Writer writer))(E$void) $scope) {
    return_ok(try_(self.fn(self.ctx, writer)));
} $unscoped_(fn);

typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
$attr($must_check)
$static fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void) $scope) {
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
$static fn_((fmt_Display_trait$Entity(const Entity* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = self, .fn = as$(FieldType$(fmt_Display, fn))(Entity_fmt_display) };
};


$attr($must_check)
$static fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Circ {")));
    try_(io_Writer_println(writer, u8_l("    radius: {:.2f}"), self->radius));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$static fn_((fmt_Display_trait$Circ(const Circ* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = self, .fn = as$(FieldType$(fmt_Display, fn))(Circ_fmt_display) };
};

$attr($must_check)
$static fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Rect {")));
    try_(io_Writer_println(writer, u8_l("    width: {:.2f}"), self->width));
    try_(io_Writer_println(writer, u8_l("    height: {:.2f}"), self->height));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$static fn_((fmt_Display_trait$Rect(const Rect* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = self, .fn = as$(FieldType$(fmt_Display, fn))(Rect_fmt_display) };
};


$attr($must_check)
$static fn_((Shape_fmt_display(const Shape* self, io_Writer writer))(E$void) $scope) {
    try_(io_Writer_println(writer, u8_l("Shape {")));
    try_(io_Writer_println(writer, u8_l("    Area: {:.2f}"), Shape_area(*self)));
    try_(io_Writer_println(writer, u8_l("    Perimeter: {:.2f}"), Shape_perimeter(*self)));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
} $unscoped_(fn);
$static fn_((fmt_Display_trait$Shape(const Shape* self))(fmt_Display)) {
    return (fmt_Display){ .ctx = ptrCast$((P_const$raw)(self)), .fn = as$(FieldType$(fmt_Display, fn))(Shape_fmt_display) };
};
