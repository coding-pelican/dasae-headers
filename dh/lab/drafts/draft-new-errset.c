#include "dh/prl/types/raw.h"
#include "dh/prl/types/Opt.h"

/*========== Macros and Declarations ========================================*/

#define $union_errset_(_E_Id_Other...) __syn__$union_errset_(_E_Id_Other)
#define errset_(/*(_E_Id_Self)(_E_Tag...) $union_errset_(_E_Id_Other...)*/...) __stmt__errset_(__VA_ARGS__)

/*========== Macros and Definitions =========================================*/

#define __pp__errset__unwrap(...) __VA_ARGS__
#define __pp__errset__unwrapWrapped(...) __VA_ARGS__
#define __pp__errset__end __pp__errset__end
#define __syn__$union_errset_(...) , ((__VA_ARGS__))
#define __stmt__errset_(...) __pp__errset__parseId __VA_ARGS__ , __pp__errset__end)
#define __pp__errset__parseId(_Id...) __pp__errset__emit(_Id,
#define __pp__errset__emit(...) \
    pp_overload(__pp__errset__emit, __VA_ARGS__)(__VA_ARGS__)
#define __pp__errset__emit_3(_Id, _Members, _End) \
    __syn__errset__emit(_Id, _Members, ())
#define __pp__errset__emit_4(_Id, _Members, _Imports, _End) \
    __syn__errset__emit(_Id, _Members, __pp__errset__unwrapWrapped _Imports)

#define __syn__errset__emitTagMember(_Id, _Member...) \
    pp_cat4(E_Tag$, _Id, _, _Member),

#define __syn__errset__emitTypeAlias(_Id, _Member...) \
    typedef pp_cat(E_Opaq$, _Id) pp_cat4(E_Type$, _Id, _, _Member);

#define __syn__errset__emitPayloadMember(_Id, _Member...) \
    pp_cat4(E_Type$, _Id, _, _Member) _Member;

#define __syn__errset__emitImportedField(_Id, _Imported...) \
    _Imported _Imported;

#define __syn__errset__emitOccurrenceAlias(_Id, _Member...) \
    typedef _Id pp_cat3(_Id, _, _Member);

#define __syn__errset__emitHashDecl(_Id, _Member...) /* clang-format off */ \
    $attr($inline_always $maybe_unused) \
    static E_HashId pp_cat(E_hashId$, _Member)(void) { \
        return E_hasher(#_Member); \
    } /* clang-format on */

#define __syn__errset__emitCauseDecl(_Id, _Member...) /* clang-format off */ \
    $attr($inline_always $maybe_unused) \
    static pp_cat3(_Id, _, _Member) pp_cat(E_cause$, _Member)(void) { \
        static const pp_cat(E_Inner$, _Id) inner = { \
            .tag = pp_cat4(E_Tag$, _Id, _, _Member), \
            .tag_id_z = #_Member, \
            .hashId = pp_cat(E_hashId$, _Member), \
        }; \
        return (pp_cat3(_Id, _, _Member)){ .opaq.inner = &inner }; \
    } /* clang-format on */

#define __syn__errset__emitLocalErr(_Id, _Member...) \
    pp_cat(E_cause$, _Member)(),

#define __syn__errset__emit(_Id, _Members, _Imports...) /* clang-format off */ \
    typedef enum $packed pp_cat(E_Tag$, _Id) { \
        pp_cat3(E_Tag$, _Id, _Any) = E_Tag$Any, \
        pp_foreach(__syn__errset__emitTagMember, _Id, __pp__errset__unwrap _Members) \
    } pp_cat(E_Tag$, _Id); \
    typedef struct pp_cat(E_Inner$, _Id) { \
        pp_cat(E_Tag$, _Id) tag; \
        const char* tag_id_z; \
        E_HashId (*hashId)(void); \
    } pp_cat(E_Inner$, _Id); \
    typedef struct pp_cat(E_Opaq$, _Id) { \
        const pp_cat(E_Inner$, _Id)* inner; \
    } pp_cat(E_Opaq$, _Id); \
    pp_foreach(__syn__errset__emitTypeAlias, _Id, __pp__errset__unwrap _Members) \
    typedef union pp_cat(E_Payload$, _Id) { \
        pp_foreach(__syn__errset__emitPayloadMember, _Id, __pp__errset__unwrap _Members) \
    } pp_cat(E_Payload$, _Id); \
    typedef union _Id { \
        pp_cat(E_Opaq$, _Id) opaq; \
        pp_cat(E_Payload$, _Id) payload; \
        General_E as_general[1]; \
        General_E General_E; \
        pp_foreach(__syn__errset__emitImportedField, _Id, __pp__errset__unwrap _Imports) \
    } _Id; \
    $attr($inline_always $maybe_unused) \
    static const char* pp_cat(E_strfy$, _Id)(_Id self) { \
        return self.opaq.inner->tag_id_z; \
    } \
    pp_foreach(__syn__errset__emitOccurrenceAlias, _Id, __pp__errset__unwrap _Members) \
    pp_foreach(__syn__errset__emitHashDecl, _Id, __pp__errset__unwrap _Members) \
    pp_foreach(__syn__errset__emitCauseDecl, _Id, __pp__errset__unwrap _Members) \
    typedef struct pp_cat(O$, _Id) { \
        bool is_some; \
        union { \
            _Id some; \
            Void none; \
        } payload; \
    } pp_cat(O$, _Id); \
    $attr($inline_always $maybe_unused) \
    static pp_cat(O$, _Id) pp_cat(E_resolve$, _Id)(_Id self) { \
        const _Id errs[] = { \
            pp_foreach(__syn__errset__emitLocalErr, _Id, __pp__errset__unwrap _Members) \
        }; \
        const usize errs_len = sizeof(errs) / sizeof(errs[0]); \
        for (usize i = 0; i < errs_len; ++i) { \
            if (self.opaq.inner->hashId() != errs[i].opaq.inner->hashId()) continue; \
            return (pp_cat(O$, _Id)){ \
                .is_some = true, \
                .payload = { .some = errs[i] }, \
            }; \
        } \
        return (pp_cat(O$, _Id)){ .is_some = false }; \
    } \
    $attr($inline_always $maybe_unused) \
    static _Id pp_cat(E_handle$, _Id)(_Id self) { \
        pp_cat(O$, _Id) resolved = pp_cat(E_resolve$, _Id)(self); \
        if (resolved.is_some) return resolved.payload.some; \
        return (_Id)E_cause$Any(self.as_general); \
    } \
    $attr($inline_always $maybe_unused) \
    static pp_cat(E_Tag$, _Id) pp_cat(E_tag$, _Id)(_Id self) { \
        pp_cat(O$, _Id) resolved = pp_cat(E_resolve$, _Id)(self); \
        if (resolved.is_some) return resolved.payload.some.opaq.inner->tag; \
        return pp_cat3(E_Tag$, _Id, _Any); \
    } /* clang-format on */

fn_((E_hasher(const u8* str_ptr, usize str_len))(E_HashId)) {
    return CompHash_calc(S_const$u8 str)
}



static O$General_E E_resolve$General_E(General_E self) {
    const General_E errs[] = {
        E_cause$Unexpected(),
        E_cause$NotImplemented(),
    };
    const usize errs_len = sizeof(errs) / sizeof(errs[0]);
    for (usize i = 0; i < errs_len; ++i) {
        if (self.opaq.inner->hashId() != errs[i].opaq.inner->hashId()) continue;
        return (O$General_E){
            .is_some = true,
            .payload = { .some = errs[i] },
        };
    }
    return (O$General_E){ .is_some = false };
}

$attr($inline_always $maybe_unused)
static General_E E_handle$General_E(General_E self) {
    O$General_E resolved = E_resolve$General_E(self);
    if (resolved.is_some) return resolved.payload.some;
    return E_cause$Any(&self);
}

$attr($inline_always $maybe_unused)
static E_Tag$General_E E_tag$General_E(General_E self) {
    O$General_E resolved = E_resolve$General_E(self);
    if (resolved.is_some) return resolved.payload.some.opaq.inner->tag;
    return E_Tag$General_E_Any;
}

$attr($inline_always $maybe_unused)
static const char* E_strfy$General_E(General_E self) {
    return self.opaq.inner->tag_id_z;
}



errset_((Conc_E)(ConcUnavailable));
errset_((Cancelable)(Canceled));
errset_((Timeout_E)(Timeout));
errset_((Foo_E)(
    FooBar,
    FooBas,
    FooBaz
) $union_errset_(Conc_E, Cancelable, Timeout_E));
errset_((Foo_E)(
    FooBar,
    FooBas,
    FooBaz
)($union_errset_(Conc_E, Cancelable, Timeout_E)));

static Foo_E mustFail(int kind) {
    switch (kind) {
    case 0:
        return E_cause$FooBar();
    case 1:
        return (Foo_E)E_cause$ConcUnavailable();
    case 2:
        return (Foo_E)E_cause$Canceled();
    case 3:
        return (Foo_E)E_cause$Unexpected();
    default:
        return (Foo_E)E_cause$NotImplemented();
    }
}

int main(void) {
    for (int kind = 0; kind < 5; ++kind) {
        Foo_E err = mustFail(kind);

        printf("raw=%s\n", E_strfy$Foo_E(err));

        {
            O$Foo_E maybe_foo = E_resolve$Foo_E(err);
            if (maybe_foo.is_some) {
                printf("  foo-local=%s\n", E_strfy$Foo_E(maybe_foo.payload.some));
                continue;
            }
        }
        {
            O$Conc_E maybe_conc = E_resolve$Conc_E(err.Conc_E);
            if (maybe_conc.is_some) {
                printf("  imported-conc=%s\n", E_strfy$Conc_E(maybe_conc.payload.some));
                continue;
            }
        }
        {
            O$Cancelable maybe_cancelable = E_resolve$Cancelable(err.Cancelable);
            if (maybe_cancelable.is_some) {
                printf("  imported-cancelable=%s\n", E_strfy$Cancelable(maybe_cancelable.payload.some));
                continue;
            }
        }
        {
            O$General_E maybe_general = E_resolve$General_E(err.General_E);
            if (maybe_general.is_some) {
                printf("  imported-general=%s\n", E_strfy$General_E(maybe_general.payload.some));
                continue;
            }
        }

        printf("  unresolved=%s\n", E_strfy$Foo_E(E_handle$Foo_E(err)));
    }

    return 0;
}
