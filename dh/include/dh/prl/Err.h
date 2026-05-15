#ifndef prl_Err__included
#define prl_Err__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "CompHash.h"

/*========== Macros and Declarations ========================================*/

T_alias$((E_Tag)(i16));
T_alias$((E_HashId)(usize));
$attr($inline_always)
$static fn_((E_hasher(S_const$u8 str))(E_HashId));

$attr($inline_always)
$static fn_((E_tag(const EAny* err))(E_Tag));
$attr($inline_always)
$static fn_((E_strfy(const EAny* err))(S_const$u8));
$attr($inline_always)
$static fn_((E_hashId(const EAny* err))(E_HashId));

$extern fn_((E_eql(const EAny* lhs, const EAny* rhs))(bool));
$extern fn_((E_neq(const EAny* lhs, const EAny* rhs))(bool));

$extern fn_((E_print(const EAny* err))(void));

T_alias$((E_Tag$General_E)(enum_((E_Tag$General_E $fits($packed))(
    E_Tag$NotImplemented = -2,
    E_Tag$General_E_NotImplemented = E_Tag$NotImplemented,
    E_Tag$Unexpected = -1,
    E_Tag$General_E_Unexpected = E_Tag$Unexpected,
    E_Tag$Any = 0,
    E_Tag$General_E_Any = E_Tag$Any
))));
T_alias$((E_Inner$General_E)(struct E_Inner$General_E {
    var_(tag, E_Tag$General_E);
    var_(tag_id, S_const$u8);
    fn_(((*hashId)(void))(E_HashId));
}));
T_alias$((E_Opaq$General_E)(union E_Opaq$General_E {
    var_(inner, const E_Inner$General_E*);
    var_(any, E_OpaqAny);
    var_(as_any, E_OpaqAny) $like_ref;
}));
T_alias$((E_Opaq$NotImplemented)(E_Opaq$General_E));
T_alias$((E_Opaq$Unexpected)(E_Opaq$General_E));
T_alias$((E_Payload$General_E)(union E_Payload$General_E {
    var_(NotImplemented, E_Opaq$NotImplemented);
    var_(Unexpected, E_Opaq$Unexpected);
}));
T_alias$((General_E)(union General_E {
    T_embed$(E_Payload$General_E);
    var_(opaq, E_Opaq$General_E);
    var_(any, EAny);
    var_(as_any, EAny) $like_ref;
}));

$attr($inline_always)
$static fn_((E_tag$General_E(General_E self))(E_Tag$General_E));
$attr($inline_always)
$static fn_((E_strfy$General_E(General_E self))(S_const$u8));
$attr($inline_always)
$static fn_((E_hashId$General_E(General_E self))(E_HashId));
$attr($inline_always)
$static fn_((E_handle$General_E(General_E self))(General_E));
T_use_O$(General_E);
$attr($inline_always)
$static fn_((E_resolve$General_E(General_E self))(O$General_E));

T_alias$((E_Type$NotImplemented)(General_E));
$attr($inline_always)
$static fn_((E_cause$NotImplemented(void))(E_Type$NotImplemented));
T_alias$((E_Type$Unexpected)(General_E));
$attr($inline_always)
$static fn_((E_cause$Unexpected(void))(E_Type$Unexpected));

T_alias$((General_E$Void)(union General_E$Void {
    T_embed$(struct {
        var_(is_ok, bool);
        var_(payload, union {
            var_(ok, Void);
            var_(err, General_E);
        });
    });
    var_(as_raw, E$raw) $like_ref;
}));
T_alias$((General_E$void)(General_E$Void));

/*========== Macros and Definitions =========================================*/

/*---------- Internal Declarations ------------------------------------------*/

$attr($inline_always)
$static fn_((E__hashId$NotImplemented(void))(E_HashId));
$attr($inline_always)
$static fn_((E__hashId$Unexpected(void))(E_HashId));

/*---------- External Definitions -------------------------------------------*/

fn_((E_hasher(S_const$u8 str))(E_HashId)) {
    return CompHash_calc(ensureNonnullS(str));
};

fn_((E_tag(const EAny* any))(E_Tag)) {
    let self = ptrAlignCast$((const General_E*)(ensureNonnull(any)));
    return self->opaq.inner->tag;
};

fn_((E_strfy(const EAny* any))(S_const$u8)) {
    let self = ptrAlignCast$((const General_E*)(ensureNonnull(any)));
    return self->opaq.inner->tag_id;
};

fn_((E_hashId(const EAny* any))(E_HashId)) {
    let self = ptrAlignCast$((const General_E*)(ensureNonnull(any)));
    return self->opaq.inner->hashId();
};

fn_((E_tag$General_E(General_E self))(E_Tag$General_E)) {
    let resolved = orelse_((E_resolve$General_E(self))(return E_Tag$General_E_Any));
    return resolved.opaq.inner->tag;
};

fn_((E_strfy$General_E(General_E self))(S_const$u8)) {
    debug_assert(E_tag(self.as_any) != E_Tag$Any);
    return self.opaq.inner->tag_id;
};

fn_((E_hashId$General_E(General_E self))(E_HashId)) {
    debug_assert(E_tag(self.as_any) != E_Tag$Any);
    return self.opaq.inner->hashId();
};

fn_((E_handle$General_E(General_E self))(General_E)) {
    let resolved = orelse_((E_resolve$General_E(self))(return self));
    return resolved;
};

fn_((E_resolve$General_E(General_E self))(O$General_E) $scope) {
    let errs = A_from$((General_E){
        E_cause$Unexpected(),
        E_cause$NotImplemented(),
    });
    for_(($s(A_ref(errs)))(err)) {
        if (!E_eql(self.as_any, err->as_any)) continue;
        return_some(*err);
    } $end(for);
    return_none();
} $unscoped(fn);

fn_((E_cause$NotImplemented(void))(E_Type$NotImplemented)) {
    $static let_(inner, E_Inner$General_E) = {
        .tag = E_Tag$NotImplemented,
        .tag_id = u8_l("NotImplemented"),
        .hashId = E__hashId$NotImplemented,
    };
    return (General_E){ .opaq.inner = &inner };
};

fn_((E_cause$Unexpected(void))(E_Type$Unexpected)) {
    $static let_(inner, E_Inner$General_E) = {
        .tag = E_Tag$Unexpected,
        .tag_id = u8_l("Unexpected"),
        .hashId = E__hashId$Unexpected,
    };
    return (General_E){ .opaq.inner = &inner };
};

/*---------- Internal Definitions -------------------------------------------*/

fn_((E__hashId$NotImplemented(void))(E_HashId)) {
    return E_hasher(u8_l("NotImplemented"));
};

fn_((E__hashId$Unexpected(void))(E_HashId)) {
    return E_hasher(u8_l("Unexpected"));
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* prl_Err__included */
