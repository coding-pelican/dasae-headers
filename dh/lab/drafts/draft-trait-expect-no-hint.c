#include "dh/prl.h"
#include "dh/fmt/common.h"

typedef struct Shape_VT {
    fn_(((*Shape_area)(u_P_const$raw ctx))(f32));
    fn_(((*Shape_perimeter)(u_P_const$raw ctx))(f32));
} Shape_VT;
typedef struct Shape {
    var_(vt, P_const$$(Shape_VT));
    debug_only(var_(type, TypeInfo);)
} Shape;
typedef struct ShapeDyn {
    var_(trait, Shape);
    var_(ctx, u_P_const$raw);
} ShapeDyn;
$static fn_((Shape_area(Shape self, u_P_const$raw ctx))(f32)) {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eql), self.vt->Shape_area(ctx);
};
$static fn_((Shape_perimeter(Shape self, u_P_const$raw ctx))(f32)) {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eql), self.vt->Shape_perimeter(ctx);
};

typedef struct Circ {
    f32 radius;
} Circ;
$static /* Generated begin */ fn_((Circ_Shape_area(const Circ* self))(f32));
$static fn_((Shape_area$Circ(u_P_const$raw ctx))(f32)) { return Circ_Shape_area(u_castP$((const Circ*)(ctx))); };
fn_((Circ_Shape_area(const Circ* self))(f32)) { /* Generated end */ return 3.1415926f * self->radius * self->radius; };
$static /* Generated begin */ fn_((Circ_Shape_perimeter(const Circ* self))(f32));
$static fn_((Shape_perimeter$Circ(u_P_const$raw ctx))(f32)) { return Circ_Shape_perimeter(u_castP$((const Circ*)(ctx))); };
fn_((Circ_Shape_perimeter(const Circ* self))(f32)) { /* Generated end */ return 2.0f * 3.1415926f * self->radius; };
$static fn_((Shape_for$Circ(void))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = Shape_area$Circ,
        .Shape_perimeter = Shape_perimeter$Circ
    };
    return (Shape){ .vt = &vt, debug_only(.type = typeInfo$(Circ)) };
};
$attr($maybe_unused)
$static fn_((Shape_dyn$Circ(const Circ* ctx))(ShapeDyn)) {
    return (ShapeDyn){ .trait = Shape_for$Circ(), .ctx = u_anyP(ctx) };
};

typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
$static /* Generated begin */ fn_((Rect_Shape_area(const Rect* self))(f32));
$static fn_((Shape_area$Rect(u_P_const$raw ctx))(f32)) { return Rect_Shape_area(u_castP$((const Rect*)(ctx))); };
fn_((Rect_Shape_area(const Rect* self))(f32)) { /* Generated end */ return self->width * self->height; };
$static /* Generated begin */ fn_((Rect_Shape_perimeter(const Rect* self))(f32));
$static fn_((Shape_perimeter$Rect(u_P_const$raw ctx))(f32)) { return Rect_Shape_perimeter(u_castP$((const Rect*)(ctx))); };
fn_((Rect_Shape_perimeter(const Rect* self))(f32)) { /* Generated end */ return 2.0f * (self->width + self->height); };
$static fn_((Shape_for$Rect(void))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = Shape_area$Rect,
        .Shape_perimeter = Shape_perimeter$Rect,
    };
    return (Shape){ .vt = &vt, debug_only(.type = typeInfo$(Rect)) };
};
$attr($maybe_unused)
$static fn_((Shape_dyn$Rect(const Rect* ctx))(ShapeDyn)) {
    return (ShapeDyn){ .trait = Shape_for$Rect(), .ctx = u_anyP(ctx) };
};


typedef struct fmt_Display {
    fn_(((*fmt_display)(u_P_const$raw ctx, io_Writer writer))(E$void));
    debug_only(var_(type, TypeInfo);)
} fmt_Display;
typedef struct fmt_DisplayDyn {
    var_(trait, fmt_Display);
    var_(ctx, u_P_const$raw);
} fmt_DisplayDyn;
$attr($must_check)
$static fn_((fmt_display(fmt_Display self, u_P_const$raw ctx, io_Writer writer))(E$void)) {
    return debug_assert_eqBy(self.type, ctx.type, TypeInfo_eql), self.fmt_display(ctx, writer);
};

typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
$attr($must_check)
$static /* Generated begin */ fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Entity(u_P_const$raw ctx, io_Writer writer))(E$void)) { return Entity_fmt_display(u_castP$((const Entity*)(ctx)), writer); };
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
}
$unscoped(fn);
$static fn_((fmt_Display_for$Entity(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$Entity, debug_only(.type = typeInfo$(Entity)) };
};
$attr($maybe_unused)
$static fn_((fmt_Display_dyn$Entity(const Entity* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Entity(), .ctx = u_anyP(ctx) };
};

$attr($must_check)
$static /* Generated begin */ fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Circ(u_P_const$raw ctx, io_Writer writer))(E$void)) { return Circ_fmt_display(u_castP$((const Circ*)(ctx)), writer); };
fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Circ {")));
    try_(io_Writer_println(writer, u8_l("    radius: {:.2f}"), self->radius));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
}
$unscoped(fn);
$static fn_((fmt_Display_for$Circ(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$Circ, debug_only(.type = typeInfo$(Circ)) };
};
$attr($maybe_unused)
$static fn_((fmt_Display_dyn$Circ(const Circ* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Circ(), .ctx = u_anyP(ctx) };
};

$attr($must_check)
$static /* Generated begin */ fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void));
$static fn_((fmt_display$Rect(u_P_const$raw ctx, io_Writer writer))(E$void)) { return Rect_fmt_display(u_castP$((const Rect*)(ctx)), writer); };
fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Rect {")));
    try_(io_Writer_println(writer, u8_l("    width: {:.2f}"), self->width));
    try_(io_Writer_println(writer, u8_l("    height: {:.2f}"), self->height));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
}
$unscoped(fn);
$static fn_((fmt_Display_for$Rect(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$Rect, debug_only(.type = typeInfo$(Rect)) };
};
$attr($maybe_unused)
$static fn_((fmt_Display_dyn$Rect(const Rect* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Rect(), .ctx = u_anyP(ctx) };
};

/// ShapeDyn_fmt_display
$attr($must_check)
$static /* Generated begin */ fn_((ShapeDyn_fmt_display(const ShapeDyn* self, io_Writer writer))(E$void));
$static fn_((fmt_display$ShapeDyn(u_P_const$raw ctx, io_Writer writer))(E$void)) { return ShapeDyn_fmt_display(u_castP$((const ShapeDyn*)(ctx)), writer); };
fn_((ShapeDyn_fmt_display(const ShapeDyn* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    try_(io_Writer_println(writer, u8_l("Shape {")));
    try_(io_Writer_println(writer, u8_l("    Area: {:.2f}"), Shape_area(self->trait, self->ctx)));
    try_(io_Writer_println(writer, u8_l("    Perimeter: {:.2f}"), Shape_perimeter(self->trait, self->ctx)));
    try_(io_Writer_print(writer, u8_l("}")));
    return_ok({});
}
$unscoped(fn);
$static fn_((fmt_Display_for$ShapeDyn(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$ShapeDyn, debug_only(.type = typeInfo$(ShapeDyn)) };
};
$attr($maybe_unused)
$static fn_((fmt_Display_dyn$ShapeDyn(const ShapeDyn* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$ShapeDyn(), .ctx = u_anyP(ctx) };
};


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
    let rects = ({ var initial = l0$((A$$(16, Rect))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
        item->width = as$(f32)(idx + 1);
        item->height = as$(f32)(idx + 2);
    }); initial; });
    let circs = ({ var initial = l0$((A$$(16, Circ))); for_(($rf(0), $s(A_ref(initial)))(idx, item) {
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
}
$unscoped(fn);


#include "dh-main.h"
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(example_usage()));
}
$unscoped(fn);
