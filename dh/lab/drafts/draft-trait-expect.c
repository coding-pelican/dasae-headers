#include "dh/prl.h"
#include "dh/fmt/common.h"

typedef struct Shape_VT {
    fn_(((*Shape_area)(u_P_const$raw ctx))(f32));
    fn_(((*Shape_perimeter)(u_P_const$raw ctx))(f32));
} Shape_VT;
typedef struct Shape {
    var_(vt, P_const$$(Shape_VT));
    var_(type, debug_TypeInfo);
} Shape;
typedef struct ShapeDyn {
    var_(trait, Shape);
    var_(ctx, u_P_const$raw);
} ShapeDyn;
$attr($inline_always)
$static fn_((Shape_area(Shape self, u_P_const$raw ctx))(f32)) {
    return debug_assert_eqBy($typed(self.type), ctx.type, TypeInfo_eql), self.vt->Shape_area(ctx);
};
$attr($inline_always)
$static fn_((Shape_perimeter(Shape self, u_P_const$raw ctx))(f32)) {
    return debug_assert_eqBy($typed(self.type), ctx.type, TypeInfo_eql), self.vt->Shape_perimeter(ctx);
};

typedef struct Circ {
    f32 radius;
} Circ;
$attr($inline_always)
$static /* Generated begin */ fn_((Circ_Shape_area(const Circ* self))(f32));
$attr($inline_always)
$static fn_((Shape_area$Circ(const Circ* self))(f32)) { return Circ_Shape_area(self); };
$attr($inline)
$static fn_((trait$Shape_area$Circ(u_P_const$raw ctx))(f32)) { return Shape_area$Circ(u_castP$((const Circ*)(ctx))); };
fn_((Circ_Shape_area(const Circ* self))(f32)) { /* Generated end */ return 3.1415926f * self->radius * self->radius; };
$attr($inline_always)
$static /* Generated begin */ fn_((Circ_Shape_perimeter(const Circ* self))(f32));
$attr($inline_always)
$static fn_((Shape_perimeter$Circ(const Circ* self))(f32)) { return Circ_Shape_perimeter(self); };
$attr($inline)
$static fn_((trait$Shape_perimeter$Circ(u_P_const$raw ctx))(f32)) { return Shape_perimeter$Circ(u_castP$((const Circ*)(ctx))); };
fn_((Circ_Shape_perimeter(const Circ* self))(f32)) { /* Generated end */ return 2.0f * 3.1415926f * self->radius; };
$attr($inline)
$static fn_((Shape_for$Circ(void))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = trait$Shape_area$Circ,
        .Shape_perimeter = trait$Shape_perimeter$Circ
    };
    return (Shape){ .vt = &vt, .type = $typing(typeInfo$(Circ)) };
};
$attr($inline_always $maybe_unused)
$static fn_((Shape_dyn$Circ(const Circ* ctx))(ShapeDyn)) {
    return (ShapeDyn){ .trait = Shape_for$Circ(), .ctx = u_anyP(ctx) };
};

typedef struct Rect {
    f32 width;
    f32 height;
} Rect;
$attr($inline_always)
$static /* Generated begin */ fn_((Rect_Shape_area(const Rect* self))(f32));
$attr($inline_always)
$static fn_((Shape_area$Rect(const Rect* self))(f32)) { return Rect_Shape_area(self); };
$attr($inline)
$static fn_((trait$Shape_area$Rect(u_P_const$raw ctx))(f32)) { return Shape_area$Rect(u_castP$((const Rect*)(ctx))); };
fn_((Rect_Shape_area(const Rect* self))(f32)) { /* Generated end */ return self->width * self->height; };
$attr($inline_always)
$static /* Generated begin */ fn_((Rect_Shape_perimeter(const Rect* self))(f32));
$attr($inline_always)
$static fn_((Shape_perimeter$Rect(const Rect* self))(f32)) { return Rect_Shape_perimeter(self); };
$attr($inline)
$static fn_((trait$Shape_perimeter$Rect(u_P_const$raw ctx))(f32)) { return Shape_perimeter$Rect(u_castP$((const Rect*)(ctx))); };
fn_((Rect_Shape_perimeter(const Rect* self))(f32)) { /* Generated end */ return 2.0f * (self->width + self->height); };
$attr($inline)
$static fn_((Shape_for$Rect(void))(Shape)) {
    $static let_(vt, Shape_VT) = {
        .Shape_area = trait$Shape_area$Rect,
        .Shape_perimeter = trait$Shape_perimeter$Rect,
    };
    return (Shape){ .vt = &vt, .type = $typing(typeInfo$(Rect)) };
};
$attr($inline_always $maybe_unused)
$static fn_((Shape_dyn$Rect(const Rect* ctx))(ShapeDyn)) {
    return (ShapeDyn){ .trait = Shape_for$Rect(), .ctx = u_anyP(ctx) };
};


typedef struct fmt_Display {
    $attr($must_check)
    fn_(((*fmt_display)(u_P_const$raw ctx, io_Writer writer))(E$void));
    var_(type, debug_TypeInfo);
} fmt_Display;
typedef struct fmt_DisplayDyn {
    var_(trait, fmt_Display);
    var_(ctx, u_P_const$raw);
} fmt_DisplayDyn;
$attr($inline_always $must_check)
$static fn_((fmt_display(fmt_Display self, u_P_const$raw ctx, io_Writer writer))(E$void)) {
    return debug_assert_eqBy($typed(self.type), ctx.type, TypeInfo_eql), self.fmt_display(ctx, writer);
};

typedef struct Entity {
    usize id;
    f32 px, py;
    f32 vx, vy;
    i32 health;
} Entity;
$attr($inline_always $must_check)
$static /* Generated begin */ fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void));
$attr($inline_always $must_check)
$static fn_((fmt_display$Entity(const Entity* self, io_Writer writer))(E$void)) { return Entity_fmt_display(self, writer); };
$attr($inline)
$static fn_((trait$fmt_display$Entity(const Entity* self, io_Writer writer))(E$void)) { return fmt_display$Entity(self, writer); };
fn_((Entity_fmt_display(const Entity* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    let fmt = u8_l(
        "" nameOf(Entity) " {{" io_nl $str
        "    " fieldName$(Entity, id) ": {:uz}" io_nl $str
        "    " fieldName$(Entity, px) ": {:.2f}" io_nl $str
        "    " fieldName$(Entity, py) ": {:.2f}" io_nl $str
        "    " fieldName$(Entity, vx) ": {:.2f}" io_nl $str
        "    " fieldName$(Entity, vy) ": {:.2f}" io_nl $str
        "    " fieldName$(Entity, health) ": {:d}" io_nl $str
        "}}"
    );
    try_(io_Writer_print(writer, fmt, self->id, self->px, self->py, self->vx, self->vy, self->health));
    return_ok({});
} $unscoped(fn);
$attr($inline)
$static fn_((fmt_Display_for$Entity(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = trait$fmt_display$Entity, .type = $typing(typeInfo$(Entity)) };
};
$attr($inline_always $maybe_unused)
$static fn_((fmt_Display_dyn$Entity(const Entity* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Entity(), .ctx = u_anyP(ctx) };
};

$attr($inline_always $must_check)
$static /* Generated begin */ fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void));
$attr($inline_always $must_check)
$static fn_((fmt_display$Circ(const Circ* self, io_Writer writer))(E$void)) { return Circ_fmt_display(self, writer); };
$attr($inline_always)
$static fn_((trait$fmt_display$Circ(const Circ* self, io_Writer writer))(E$void)) { return fmt_display$Circ(self, writer); };
fn_((Circ_fmt_display(const Circ* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    let fmt = u8_l(
        "" nameOf(Circ) " {{" io_nl $str
        "    " fieldName$(Circ, radius) ": {:.2f}" io_nl $str
        "}}"
    );
    try_(io_Writer_print(writer, fmt, self->radius));
    return_ok({});
} $unscoped(fn);
$attr($inline)
$static fn_((fmt_Display_for$Circ(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = trait$fmt_display$Circ, .type = $typing(typeInfo$(Circ)) };
};
$attr($inline_always $maybe_unused)
$static fn_((fmt_Display_dyn$Circ(const Circ* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Circ(), .ctx = u_anyP(ctx) };
};

$attr($inline_always $must_check)
$static /* Generated begin */ fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void));
$attr($inline)
$static fn_((fmt_display$Rect(const Rect* self, io_Writer writer))(E$void)) { return Rect_fmt_display(self, writer); };
$attr($inline_always)
$static fn_((trait$fmt_display$Rect(const Rect* self, io_Writer writer))(E$void)) { return fmt_display$Rect(self, writer); };
fn_((Rect_fmt_display(const Rect* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    let fmt = u8_l(
        "" nameOf(Rect) " {{" io_nl $str
        "    " fieldName$(Rect, width) ": {:.2f}" io_nl $str
        "    " fieldName$(Rect, height) ": {:.2f}" io_nl $str
        "}}"
    );
    try_(io_Writer_print(writer, fmt, self->width, self->height));
    return_ok({});
} $unscoped(fn);
$attr($inline)
$static fn_((fmt_Display_for$Rect(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$Rect, .type = $typing(typeInfo$(Rect)) };
};
$attr($inline_always $maybe_unused)
$static fn_((fmt_Display_dyn$Rect(const Rect* ctx))(fmt_DisplayDyn)) {
    return (fmt_DisplayDyn){ .trait = fmt_Display_for$Rect(), .ctx = u_anyP(ctx) };
};

/// ShapeDyn_fmt_display
$attr($inline_always $must_check)
$static /* Generated begin */ fn_((ShapeDyn_fmt_display(const ShapeDyn* self, io_Writer writer))(E$void));
$attr($inline)
$static fn_((fmt_display$ShapeDyn(u_P_const$raw ctx, io_Writer writer))(E$void)) { return ShapeDyn_fmt_display(u_castP$((const ShapeDyn*)(ctx)), writer); };
fn_((ShapeDyn_fmt_display(const ShapeDyn* self, io_Writer writer))(E$void) $scope) {
    /* Generated end */
    let fmt = u8_l(
        "" nameOf(Shape) " {{" io_nl $str
        "    " fieldName$(Shape, area) ": {:.2f}" io_nl $str
        "    " fieldName$(Shape, perimeter) ": {:.2f}" io_nl $str
        "}}"
    );
    try_(io_Writer_print(writer, fmt, Shape_area(self->trait, self->ctx), Shape_perimeter(self->trait, self->ctx)));
    return_ok({});
} $unscoped(fn);
$attr($inline)
$static fn_((fmt_Display_for$ShapeDyn(void))(fmt_Display)) {
    return (fmt_Display){ .fmt_display = fmt_display$ShapeDyn, .type = $typing(typeInfo$(ShapeDyn)) };
};
$attr($inline_always $maybe_unused)
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
    let rects = local_({ var initial = l0$((A$$(16, Rect))); for_(($rf(0), $s(A_ref(initial)))(idx, item)) {
        item->width = as$(f32)(idx + 1);
        item->height = as$(f32)(idx + 2);
    } $end(for); local_return_(initial); });
    let circs = local_({ var initial = l0$((A$$(16, Circ))); for_(($rf(0), $s(A_ref(initial)))(idx, item)) {
        item->radius = as$(f32)(idx + 1);
    } $end(for); local_return_(initial); });

    var shapes = ArrList_fixed$ShapeDyn(shapes_buf);
    for_(($s(A_ref(rects)))(item)) try_(ArrList_appendFixed$ShapeDyn(&shapes, Shape_dyn$Rect(item))) $end(for);
    for_(($s(A_ref(circs)))(item)) try_(ArrList_appendFixed$ShapeDyn(&shapes, Shape_dyn$Circ(item))) $end(for);

    let out = fs_File_writer(io_getStdOut());
    for_(($s(shapes.items.as_const))(item)) {
        try_(fmt_display(fmt_Display_for$ShapeDyn(), u_anyP(item), out));
        try_(io_Writer_nl(out));
    } $end(for);
    return_ok({});
} $unscoped(fn);


#include "dh-main.h"
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    return_ok(try_(example_usage()));
} $unscoped(fn);
