#include "dh-main.h"

#define $fits(...) , $_fits, __VA_ARGS__
#define $maps(...) , $_maps, __VA_ARGS__

/*((_Alias) $T)*/
/*((_Alias $fits($packed))(...))*/
/*((_Alias $fits($bits(8|16|32|64)))(...))*/
#define enum_(/*(_Alias <$fits(...)>)((_Enum)(_Value),...) <$T>*/...) \
    __type__enum_(__VA_ARGS__)
#define __type__enum_(...) \
    pp_overload(__step__enum_, __VA_ARGS__)(____enum___parseAlias __VA_ARGS__)

#define ____enum___parseAlias(...) __VA_ARGS__,
#define ____enum___parsePairEnumValueList(_Pair_Enum_Value...) _Pair_Enum_Value

#define __step__enum__1(...) \
    __gen__enum_(__VA_ARGS__)
#define __gen__enum_(...) \
    pp_overload(__gen__enum_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum__2(_Alias, _Pair_Enum_Value...) \
    __gen__enum___emit$_fits(_Alias, , ____enum___parsePairEnumValueList _Pair_Enum_Value)
#define __gen__enum__4(_Alias, _Attr, _Attr_Ctx, _Pair_Enum_Value...) \
    pp_cat(__gen__enum___emit, _Attr)(_Alias, _Attr_Ctx, ____enum___parsePairEnumValueList _Pair_Enum_Value)

#define __gen__enum___emit$_fits(_Alias, _Attr_Ctx, _Pair_Enum_Value...) \
    enum _Attr_Ctx _Alias { \
        _Pair_Enum_Value \
    }

#define __gen__enum___emitEnumTags(_Pair_Enum_Value...) \
    ____enum___emitEnumTags(_Pair_Enum_Value)
#define ____enum___emitEnumTags(_Pair_Enum_Value...) \
    pp_foreach(__gen__enum___emitEnumTag__each_emit, ~, _Pair_Enum_Value)
#define ____enum___expandPairEnumValue(_Pair_Enum_Value...) _Pair_Enum_Value
#define __gen__enum___emitEnumTag__each_emit(_$ignored, /*_Pair_Enum_Value*/...) __VA_OPT__( \
    __gen__enum___emitEnumTag__each(____enum___expandPairEnumValue __VA_ARGS__) \
)
#define __gen__enum___emitEnumTag__each(/*_Pair_Enum_Value*/...) \
    pp_overload(__gen__enum___emitEnumTag, __VA_ARGS__)(__VA_ARGS__)
#define __gen__enum___emitEnumTag_1(_Enum...) \
    _Enum, /* pp_join($, enum, _Enum) = _Enum, */
#define __gen__enum___emitEnumTag_2(_Enum, _Value...) \
    _Enum = _Value, /* pp_join($, enum, _Enum) = _Value, */

#define __step__enum__2(...) __gen__enum_raw(__VA_ARGS__)
#define __gen__enum_raw(_Alias, _$T...) enum _Alias


/*((_Alias) $T)*/
/*((_Alias)(...))*/
/*((_Alias $fits($packed))(...))*/
/*((_Alias $fits($bits(8|16|32|64)))(...))*/
/*((_Alias $maps(_Tag))(...))*/
#define variant_(/*(_Alias <$fits(...)|$maps(_Tag)>)((_Enum)(_Type),...) <$T>*/...) \
    __type__variant_(__VA_ARGS__)
#define __type__variant_(...) \
    pp_overload(__step__variant_, __VA_ARGS__)(____variant___parseAlias __VA_ARGS__)

#define ____variant___parseAlias(...) __VA_ARGS__,
#define ____variant___parsePairEnumTypeList(_Pair_Enum_Type...) _Pair_Enum_Type

#define __step__variant__1(...) \
    __gen__variant_(__VA_ARGS__)
#define __gen__variant_(...) \
    pp_overload(__gen__variant_, __VA_ARGS__)(__VA_ARGS__)
#define __gen__variant__2(_Alias, _Pair_Enum_Type...) \
    __gen__variant___emit$_fits(_Alias, , ____variant___parsePairEnumTypeList _Pair_Enum_Type)
#define __gen__variant__4(_Alias, _Attr, _Attr_Ctx, _Pair_Enum_Type...) \
    pp_cat(__gen__variant___emit, _Attr)(_Alias, _Attr_Ctx, ____variant___parsePairEnumTypeList _Pair_Enum_Type)

#define __gen__variant___emit$_fits(_Alias, _Attr_Ctx, _Pair_Enum_Type...) \
    struct _Alias { \
        enum _Attr_Ctx { \
            __gen__variant___emitEnumTags(_Pair_Enum_Type) \
        } tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_none(), _Pair_Enum_Type) \
        } payload; \
    }
#define __gen__variant___emit$_maps(_Alias, _Attr_Ctx, _Pair_Enum_Type...) \
    struct _Alias { \
        _Attr_Ctx tag; \
        union { \
            __gen__variant___emitUnionPayloads(pp_some(_Attr_Ctx), _Pair_Enum_Type) \
        } payload; \
    }

#define __gen__variant___emitEnumTags(_Pair_Enum_Type...) \
    ____variant___emitEnumTags(_Pair_Enum_Type)
#define ____variant___emitEnumTags(_Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitEnumTag, ~, _Pair_Enum_Type)
#define __gen__variant___emitEnumTag(_$ignored, /*_Pair_Enum_Type*/...) __VA_OPT__( \
    pp_Tuple_get1st __VA_ARGS__, \
)

#define __gen__variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type...) \
    ____variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type)
#define ____variant___emitUnionPayloads(_Opt_UnderlayEnum, _Pair_Enum_Type...) \
    pp_foreach(__gen__variant___emitUnionPayload, _Opt_UnderlayEnum, _Pair_Enum_Type)
#define __gen__variant___emitUnionPayload(_Opt_UnderlayEnum, /*_Pair_Enum_Type*/...) __VA_OPT__( \
    var_(pp_join($, tag, pp_Tuple_get1st __VA_ARGS__) $like_ref, pp_Tuple_get2nd __VA_ARGS__); \
    claim_assert_static( \
        pp_Tuple_get1st __VA_ARGS__ \
        == as$(pp_orelse_((_Opt_UnderlayEnum)(TypeOf(pp_Tuple_get1st __VA_ARGS__))))(pp_Tuple_get1st __VA_ARGS__) \
    ); /* claim_assert_static(pp_Tuple_get1st __VA_ARGS__ == pp_join($, enum, pp_Tuple_get1st __VA_ARGS__)); */ \
)

#define __step__variant__2(...) __gen__variant_raw(__VA_ARGS__)
#define __gen__variant_raw(_Alias, _$T...) struct _Alias


typedef enum_((EnumA $fits($packed))(
    EnumA_A = 0u,
    EnumA_B = 1u,
)) EnumA;

typedef variant_((EnumA_Mapped $maps(EnumA))(
    (EnumA_A, u8),
    (EnumA_B, u16),
)) EnumA_Mapped;

typedef variant_((EnumA_MappedDouble $maps(FieldType$(EnumA_Mapped, tag)))(
    (EnumA_A, u16),
    (EnumA_B, u32),
)) EnumA_MappedDouble;

enum_((ERRNO $fits($bits(32)))(
    ERRNO_EACCES = 13,
    ERRNO_EAGAIN = 11,
    ERRNO_EALREADY = 114,
    ERRNO_EBADF = 9,
    ERRNO_EBADMSG = 104,
    ERRNO_EBUSY = 16,
    ERRNO_ECANCELED = 125,
    ERRNO_ECHILD = 10,
    ERRNO_EDEADLK = 35,
    ERRNO_EDOM = 33,
    ERRNO_EEXIST = 17,
));

typedef variant_((ERRNO_Mapped $maps(enum ERRNO))(
    (ERRNO_EACCES, u8),
    (ERRNO_EAGAIN, u8),
    (ERRNO_EALREADY, u8),
    (ERRNO_EBADF, u8),
    (ERRNO_EBADMSG, u8),
    (ERRNO_EBUSY, u8),
    (ERRNO_ECANCELED, u8),
    (ERRNO_ECHILD, u8),
    (ERRNO_EDEADLK, u8),
    (ERRNO_EDOM, u8),
    (ERRNO_EEXIST, u8),
)) ERRNO_Mapped;
