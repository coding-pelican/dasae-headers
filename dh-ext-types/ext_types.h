#ifndef EXT_TYPES_INCLUDED
#define EXT_TYPES_INCLUDED (1)

#include "core.h"
#include "Ptr.h"
#include "Sli.h"
#include "Opt.h"
#include "Res.h"

/* Make to Extension Type */
#define using_ExtTypes(T) IMPL_using_ExtTypes(T)
#define decl_ExtType(T)   IMPL_decl_ExtType(T)
#define impl_ExtType(T)   IMPL_impl_ExtType(T)

#define using_PtrTypes(T) IMPL_using_PtrTypes(T)
#define decl_PtrType(T)   IMPL_decl_PtrType(T)
#define impl_PtrType(T)   IMPL_impl_PtrType(T)

#define using_VarTypes(T) IMPL_using_VarTypes(T)
#define decl_VarType(T)   IMPL_decl_VarType(T)
#define impl_VarType(T)   IMPL_impl_VarType(T)

#define using_VarPtrTypes(T) IMPL_using_VarPtrTypes(T)
#define decl_VarPtrType(T)   IMPL_decl_VarPtrType(T)
#define impl_VarPtrType(T)   IMPL_impl_VarPtrType(T)

#define using_Ptr(T, TRef, TPtr, TPtrS) IMPL_using_Ptr(T, TRef, TPtr, TPtrS)
#define decl_Ptr(T, TRef, TPtr, TPtrS)  IMPL_decl_Ptr(T, TRef, TPtr, TPtrS)
#define impl_Ptr(T, TRef, TPtr, TPtrS)  IMPL_impl_Ptr(T, TRef, TPtr, TPtrS)

#define using_Sli(T, TRef, TPtr, TSli, TSliS) IMPL_using_Sli(T, TRef, TPtr, TSli, TSliS)
#define decl_Sli(T, TRef, TPtr, TSli, TSliS)  IMPL_decl_Sli(T, TRef, TPtr, TSli, TSliS)
#define impl_Sli(T, TRef, TPtr, TSli, TSliS)  IMPL_impl_Sli(T, TRef, TPtr, TSli, TSliS)

#define using_Opt(T, TOpt) IMPL_using_Opt(T, TOpt)
#define decl_Opt(T, TOpt)  IMPL_decl_Opt(T, TOpt)
#define impl_Opt(T, TOpt)  IMPL_impl_Opt(T, TOpt)

#define using_Res(T, E, TRes) IMPL_using_Res(T, E, TRes)
#define decl_Res(T, E, TRes)  IMPL_decl_Res(T, E, TRes)
#define impl_Res(T, E, TRes)  IMPL_impl_Res(T, E, TRes)

#define using_ResErr(T, TResErr) IMPL_using_ResErr(T, TResErr)
#define decl_ResErr(T, TResErr)  IMPL_decl_ResErr(T, TResErr)
#define impl_ResErr(T, TResErr)  IMPL_impl_ResErr(T, TResErr)

/* Pointer types */
#define _Ref(T)      pp_join(_, Ref, T)
#define _RefConst(T) pp_join(_, RefConst, T)

#define _Ptr(T)       pp_join(_, Ptr, T)
#define _PtrConst(T)  pp_join(_, PtrConst, T)
#define _PtrS(T)      pp_join(_, PtrS, T)
#define _PtrSConst(T) pp_join(_, PtrSConst, T)

#define _Sli(T)       pp_join(_, Sli, T)
#define _SliConst(T)  pp_join(_, SliConst, T)
#define _SliS(T)      pp_join(_, SliS, T)
#define _SliSConst(T) pp_join(_, SliSConst, T)

/* Variant types */
#define _Opt(T)    pp_join(_, Opt, T)
#define _Res(T, E) pp_join3(_, Res, T, E)
#define _ResErr(T) pp_join(_, ResErr, T)

/* Variant pointer types */
#define _Opt_Ptr(T)       pp_join(_, Opt_Ptr, T)
#define _Opt_PtrConst(T)  pp_join(_, Opt_PtrConst, T)
#define _Opt_PtrS(T)      pp_join(_, Opt_PtrS, T)
#define _Opt_PtrSConst(T) pp_join(_, Opt_PtrSConst, T)

#define _Opt_Sli(T)       pp_join(_, Opt_Sli, T)
#define _Opt_SliConst(T)  pp_join(_, Opt_SliConst, T)
#define _Opt_SliS(T)      pp_join(_, Opt_SliS, T)
#define _Opt_SliSConst(T) pp_join(_, Opt_SliSConst, T)

#define _ResErr_Ptr(T)       pp_join(_, ResErr_Ptr, T)
#define _ResErr_PtrConst(T)  pp_join(_, ResErr_PtrConst, T)
#define _ResErr_PtrS(T)      pp_join(_, ResErr_PtrS, T)
#define _ResErr_PtrSConst(T) pp_join(_, ResErr_PtrSConst, T)

#define _ResErr_Sli(T)       pp_join(_, ResErr_Sli, T)
#define _ResErr_SliConst(T)  pp_join(_, ResErr_SliConst, T)
#define _ResErr_SliS(T)      pp_join(_, ResErr_SliS, T)
#define _ResErr_SliSConst(T) pp_join(_, ResErr_SliSConst, T)

/* Implementations */
#define IMPL_using_ExtTypes(T) \
    using_PtrTypes(T);         \
    using_VarTypes(T);         \
    using_VarPtrTypes(T)

#define IMPL_using_PtrTypes(T)                                     \
    using_Ptr(T, _Ref(T), _Ptr(T), _PtrS(T));                      \
    using_Ptr(const T, _RefConst(T), _PtrConst(T), _PtrSConst(T)); \
    using_Sli(T, _Ref(T), _Ptr(T), _Sli(T), _SliS(T));             \
    using_Sli(const T, _RefConst(T), _PtrConst(T), _SliConst(T), _SliSConst(T))

#define IMPL_using_VarTypes(T) \
    using_Opt(T, _Opt(T));     \
    using_ResErr(T, _ResErr(T))

#define IMPL_using_VarPtrTypes(T)                        \
    using_Opt(_Ptr(T), _Opt(_Ptr(T)));                   \
    using_Opt(_PtrConst(T), _Opt(_PtrConst(T)));         \
    using_Opt(_PtrS(T), _Opt(_PtrS(T)));                 \
    using_Opt(_PtrSConst(T), _Opt(_PtrSConst(T)));       \
    using_Opt(_Sli(T), _Opt(_Sli(T)));                   \
    using_Opt(_SliConst(T), _Opt(_SliConst(T)));         \
    using_Opt(_SliS(T), _Opt(_SliS(T)));                 \
    using_Opt(_SliSConst(T), _Opt(_SliSConst(T)));       \
    using_ResErr(_Ptr(T), _ResErr(_Ptr(T)));             \
    using_ResErr(_PtrConst(T), _ResErr(_PtrConst(T)));   \
    using_ResErr(_PtrS(T), _ResErr(_PtrS(T)));           \
    using_ResErr(_PtrSConst(T), _ResErr(_PtrSConst(T))); \
    using_ResErr(_Sli(T), _ResErr(_Sli(T)));             \
    using_ResErr(_SliConst(T), _ResErr(_SliConst(T)));   \
    using_ResErr(_SliS(T), _ResErr(_SliS(T)));           \
    using_ResErr(_SliSConst(T), _ResErr(_SliSConst(T)))

#define IMPL_using_Ptr(T, TRef, TPtr, TPtrS) \
    IMPL_decl_Ptr(T, TRef, TPtr, TPtrS);     \
    IMPL_impl_Ptr(T, TRef, TPtr, TPtrS)

#define IMPL_using_Sli(T, TRef, TPtr, TSli, TSliS) \
    IMPL_decl_Sli(T, TRef, TPtr, TSli, TSliS);     \
    IMPL_impl_Sli(T, TRef, TPtr, TSli, TSliS)

#define IMPL_using_Opt(T, TOpt) \
    IMPL_decl_Opt(T, TOpt);     \
    IMPL_impl_Opt(T, TOpt)

#define IMPL_using_Res(T, E, TRes) \
    IMPL_decl_Res(T, E, TRes);     \
    IMPL_impl_Res(T, E, TRes)

#define IMPL_using_ResErr(T, TResErr) \
    IMPL_decl_ResErr(T, TResErr);     \
    IMPL_impl_ResErr(T, TResErr)

#define IMPL_decl_Ptr(T, TRef, TPtr, TPtrS)                                  \
    /* Reference wrapper struct */                                           \
    typedef struct TRef TRef;                                                \
    /* Pointer types with reference wrapper */                               \
    typedef union TPtr  TPtr;                                                \
    typedef union TPtrS TPtrS;                                               \
    /* Function declarations */                                              \
    force_inline TPtr   pp_join(_, TPtr, from)(rawptr(T) addr);              \
    force_inline TRef*  pp_join(_, TPtr, at)(TPtr self, usize index);        \
    force_inline TPtrS  pp_join(_, TPtrS, from)(rawptr(T) addr, T sentinel); \
    force_inline TRef*  pp_join(_, TPtrS, at)(TPtrS self, usize index);      \
    force_inline TRef*  pp_join(_, TPtrS, sentinel)(TPtrS self);             \
    force_inline bool   pp_join(_, TPtrS, isSentinel)(TPtrS self, usize index);

#define IMPL_decl_Sli(T, TRef, TPtr, TSli, TSliS)                                                    \
    /* Slice types with reference wrapper */                                                         \
    typedef union TSli  TSli;                                                                        \
    typedef union TSliS TSliS;                                                                       \
    /* Function declarations */                                                                      \
    force_inline TSli   pp_join(_, TSli, from)(rawptr(T) addr, usize begin, usize end);              \
    force_inline TRef*  pp_join(_, TSli, at)(TSli self, usize index);                                \
    force_inline TSli   pp_join(_, TSli, slice)(TSli self, usize begin, usize end);                  \
    force_inline TSliS  pp_join(_, TSliS, from)(rawptr(T) addr, usize begin, usize end, T sentinel); \
    force_inline TRef*  pp_join(_, TSliS, at)(TSliS self, usize index);                              \
    force_inline TSli   pp_join(_, TSliS, slice)(TSliS self, usize begin, usize end);                \
    force_inline TSliS  pp_join(_, TSliS, sliceS)(TSliS self, usize begin, usize end, T sentinel);

#define IMPL_decl_Opt(T, TOpt)                              \
    /* Optional type with reference wrapper */              \
    typedef union TOpt TOpt;                                \
    /* Function declarations */                             \
    force_inline TOpt  pp_join(_, TOpt, some)(T value);     \
    force_inline TOpt  pp_join(_, TOpt, none)(void);        \
    force_inline bool  pp_join(_, TOpt, isSome)(TOpt self); \
    force_inline T     pp_join(_, TOpt, unwrap)(TOpt self); \
    force_inline T     pp_join(_, TOpt, unwrapOr)(TOpt self, T default_value);

#define IMPL_decl_Res(T, E, TRes)                                               \
    /* Result type with reference wrapper */                                    \
    typedef struct TRes TRes;                                                   \
    /* Function declarations */                                                 \
    force_inline TRes   pp_join(_, TRes, ok)(T value);                          \
    force_inline TRes   pp_join(_, TRes, err)(E error);                         \
    force_inline bool   pp_join(_, TRes, isOk)(TRes self);                      \
    force_inline T      pp_join(_, TRes, unwrap)(TRes self);                    \
    force_inline T      pp_join(_, TRes, unwrapOr)(TRes self, T default_value); \
    force_inline E      pp_join(_, TRes, unwrapErr)(TRes self);

#define IMPL_decl_ResErr(T, TResErr)                                                     \
    /* Result type with reference wrapper */                                             \
    typedef struct TResErr TResErr;                                                      \
    /* Function declarations */                                                          \
    force_inline TResErr   pp_join(_, TResErr, ok)(T value);                             \
    force_inline TResErr   pp_join(_, TResErr, err)(Err error);                          \
    force_inline bool      pp_join(_, TResErr, isOk)(TResErr self);                      \
    force_inline T         pp_join(_, TResErr, unwrap)(TResErr self);                    \
    force_inline T         pp_join(_, TResErr, unwrapOr)(TResErr self, T default_value); \
    force_inline Err       pp_join(_, TResErr, unwrapErr)(TResErr self);

/* Composite declarations */
#define IMPL_decl_ExtType(T) \
    IMPL_decl_PtrType(T);    \
    IMPL_decl_VarType(T);    \
    IMPL_decl_VarPtrType(T)

#define IMPL_decl_PtrType(T)                                           \
    IMPL_decl_Ptr(T, _Ref(T), _Ptr(T), _PtrS(T));                      \
    IMPL_decl_Ptr(const T, _RefConst(T), _PtrConst(T), _PtrSConst(T)); \
    IMPL_decl_Sli(T, _Ref(T), _Ptr(T), _Sli(T), _SliS(T));             \
    IMPL_decl_Sli(const T, _RefConst(T), _PtrConst(T), _SliConst(T), _SliSConst(T))

#define IMPL_decl_VarType(T)   \
    IMPL_decl_Opt(T, _Opt(T)); \
    IMPL_decl_ResErr(T, _ResErr(T))

#define IMPL_decl_VarPtrType(T)                            \
    IMPL_decl_Opt(_Ptr(T), _Opt_Ptr(T));                   \
    IMPL_decl_Opt(_PtrConst(T), _Opt_PtrConst(T));         \
    IMPL_decl_Opt(_PtrS(T), _Opt_PtrS(T));                 \
    IMPL_decl_Opt(_PtrSConst(T), _Opt_PtrSConst(T));       \
    IMPL_decl_Opt(_Sli(T), _Opt_Sli(T));                   \
    IMPL_decl_Opt(_SliConst(T), _Opt_SliConst(T));         \
    IMPL_decl_Opt(_SliS(T), _Opt_SliS(T));                 \
    IMPL_decl_Opt(_SliSConst(T), _Opt_SliSConst(T));       \
    IMPL_decl_ResErr(_Ptr(T), _ResErr_Ptr(T));             \
    IMPL_decl_ResErr(_PtrConst(T), _ResErr_PtrConst(T));   \
    IMPL_decl_ResErr(_PtrS(T), _ResErr_PtrS(T));           \
    IMPL_decl_ResErr(_PtrSConst(T), _ResErr_PtrSConst(T)); \
    IMPL_decl_ResErr(_Sli(T), _ResErr_Sli(T));             \
    IMPL_decl_ResErr(_SliConst(T), _ResErr_SliConst(T));   \
    IMPL_decl_ResErr(_SliS(T), _ResErr_SliS(T));           \
    IMPL_decl_ResErr(_SliSConst(T), _ResErr_SliSConst(T))

#define IMPL_impl_Ptr(T, TRef, TPtr, TPtrS)                                                      \
    struct TRef {                                                                                \
        T value;                                                                                 \
    };                                                                                           \
    union TPtr {                                                                                 \
        Ptr base;                                                                                \
        union {                                                                                  \
            T*    addr;                                                                          \
            TRef* ref;                                                                           \
        };                                                                                       \
    };                                                                                           \
    union TPtrS {                                                                                \
        PtrS base;                                                                               \
        struct {                                                                                 \
            union {                                                                              \
                TPtr core;                                                                       \
                union {                                                                          \
                    T*    addr;                                                                  \
                    TRef* ref;                                                                   \
                };                                                                               \
            };                                                                                   \
            TRef sentinel[1];                                                                    \
        };                                                                                       \
    };                                                                                           \
    /* Pointer interface */                                                                      \
    force_inline TPtr pp_join(_, TPtr, from)(rawptr(T) addr) {                                   \
        return (TPtr){ .base = Ptr_from(typeInfo(TRef), as(anyptr, addr)) };                     \
    }                                                                                            \
    force_inline TRef* pp_join(_, TPtr, at)(TPtr self, usize index) {                            \
        return Ptr_at(typeInfo(TRef), self.base, index);                                         \
    }                                                                                            \
    /* Sentinel pointer interface */                                                             \
    force_inline TPtrS pp_join(_, TPtrS, from)(rawptr(T) addr, T sentinel) {                     \
        TPtrS result = {                                                                         \
            .sentinel[0].value = sentinel                                                        \
        };                                                                                       \
        return (TPtrS){                                                                          \
            .base = PtrS_from(typeInfo(TRef), as(anyptr, addr), as(anyptr, &result.sentinel[0])) \
        };                                                                                       \
    }                                                                                            \
    force_inline TRef* pp_join(_, TPtrS, at)(TPtrS self, usize index) {                          \
        return PtrS_at(typeInfo(TRef), self.base, index);                                        \
    }                                                                                            \
    force_inline TRef* pp_join(_, TPtrS, sentinel)(TPtrS self) {                                 \
        return PtrS_sentinel(typeInfo(TRef), self.base);                                         \
    }                                                                                            \
    force_inline bool pp_join(_, TPtrS, isSentinel)(TPtrS self, usize index) {                   \
        return PtrS_isSentinel(typeInfo(TRef), self.base, index);                                \
    }

#define IMPL_impl_Sli(T, TRef, TPtr, TSli, TSliS)                                                            \
    union TSli {                                                                                             \
        Sli base;                                                                                            \
        struct {                                                                                             \
            union {                                                                                          \
                TPtr ptr;                                                                                    \
                rawptr(T) addr;                                                                              \
                rawptr(TRef) ref;                                                                            \
            };                                                                                               \
            usize len;                                                                                       \
        };                                                                                                   \
    };                                                                                                       \
    union TSliS {                                                                                            \
        SliS base;                                                                                           \
        struct {                                                                                             \
            union {                                                                                          \
                TSli core;                                                                                   \
                struct {                                                                                     \
                    union {                                                                                  \
                        TPtr ptr;                                                                            \
                        rawptr(T) addr;                                                                      \
                        rawptr(TRef) ref;                                                                    \
                    };                                                                                       \
                    usize len;                                                                               \
                };                                                                                           \
            };                                                                                               \
            TRef sentinel[1];                                                                                \
        };                                                                                                   \
    };                                                                                                       \
    /* Slice interface */                                                                                    \
    force_inline TSli pp_join(_, TSli, from)(rawptr(T) addr, usize begin, usize end) {                       \
        return (TSli){                                                                                       \
            .base = Sli_from(typeInfo(TRef), as(anyptr, addr), begin, end)                                   \
        };                                                                                                   \
    }                                                                                                        \
    force_inline TRef* pp_join(_, TSli, at)(TSli self, usize index) {                                        \
        return Sli_at(typeInfo(TRef), self.base, index);                                                     \
    }                                                                                                        \
    force_inline TSli pp_join(_, TSli, slice)(TSli self, usize begin, usize end) {                           \
        return (TSli){                                                                                       \
            .base = Sli_slice(typeInfo(TRef), self.base, begin, end)                                         \
        };                                                                                                   \
    }                                                                                                        \
    /* Sentinel slice interface */                                                                           \
    force_inline TSliS pp_join(_, TSliS, from)(rawptr(T) addr, usize begin, usize end, T sentinel) {         \
        TSliS result = {                                                                                     \
            .sentinel[0].value = sentinel,                                                                   \
        };                                                                                                   \
        return (TSliS){                                                                                      \
            .base = SliS_from(typeInfo(TRef), as(anyptr, addr), begin, end, as(anyptr, &result.sentinel[0])) \
        };                                                                                                   \
    }                                                                                                        \
    force_inline TRef* pp_join(_, TSliS, at)(TSliS self, usize index) {                                      \
        return SliS_at(typeInfo(TRef), self.base, index);                                                    \
    }                                                                                                        \
    force_inline TSli pp_join(_, TSliS, slice)(TSliS self, usize begin, usize end) {                         \
        return (TSli){                                                                                       \
            .base = SliS_slice(typeInfo(TRef), self.base, begin, end)                                        \
        };                                                                                                   \
    }                                                                                                        \
    force_inline TSliS pp_join(_, TSliS, sliceS)(TSliS self, usize begin, usize end, T sentinel) {           \
        TSliS result = {                                                                                     \
            .sentinel[0].value = sentinel,                                                                   \
        };                                                                                                   \
        return (TSliS){                                                                                      \
            .base = SliS_sliceS(typeInfo(TRef), self.base, begin, end, as(anyptr, &result.sentinel[0]))      \
        };                                                                                                   \
    }

#define IMPL_impl_Opt(T, TOpt)                                                 \
    union TOpt {                                                               \
        Opt base;                                                              \
        struct {                                                               \
            bool has_value;                                                    \
            T    value;                                                        \
        };                                                                     \
    };                                                                         \
    /* Optional interface */                                                   \
    force_inline TOpt pp_join(_, TOpt, some)(T value) {                        \
        return (TOpt){                                                         \
            .has_value = true,                                                 \
            .value     = value                                                 \
        };                                                                     \
    }                                                                          \
    force_inline TOpt pp_join(_, TOpt, none)(void) {                           \
        return (TOpt){ .has_value = false };                                   \
    }                                                                          \
    force_inline bool pp_join(_, TOpt, isSome)(TOpt self) {                    \
        return self.has_value;                                                 \
    }                                                                          \
    force_inline bool pp_join(_, TOpt, isNone)(TOpt self) {                    \
        return !self.has_value;                                                \
    }                                                                          \
    force_inline T pp_join(_, TOpt, unwrap)(TOpt self) {                       \
        debug_assert_fmt(self.has_value, "Attempted to unwrap none optional"); \
        return self.value;                                                     \
    }                                                                          \
    force_inline T pp_join(_, TOpt, unwrapOr)(TOpt self, T default_value) {    \
        return self.has_value ? self.value : default_value;                    \
    }

#define IMPL_impl_Res(T, E, TRes)                                                           \
    struct TRes {                                                                           \
        union {                                                                             \
            Res base;                                                                       \
            struct {                                                                        \
                ResTag tag;                                                                 \
                union {                                                                     \
                    T value;                                                                \
                    E error;                                                                \
                };                                                                          \
            };                                                                              \
        };                                                                                  \
    };                                                                                      \
    /* Result interface */                                                                  \
    force_inline TRes pp_join(_, TRes, ok)(T value) {                                       \
        return (TRes){                                                                      \
            .tag   = ResTag_ok,                                                             \
            .value = value                                                                  \
        };                                                                                  \
    }                                                                                       \
    force_inline TRes pp_join(_, TRes, err)(E error) {                                      \
        return (TRes){                                                                      \
            .tag   = ResTag_err,                                                            \
            .error = error                                                                  \
        };                                                                                  \
    }                                                                                       \
    force_inline bool pp_join(_, TRes, isOk)(TRes self) {                                   \
        return self.tag == ResTag_ok;                                                       \
    }                                                                                       \
    force_inline bool pp_join(_, TRes, isErr)(TRes self) {                                  \
        return self.tag == ResTag_err;                                                      \
    }                                                                                       \
    force_inline T pp_join(_, TRes, unwrap)(TRes self) {                                    \
        debug_assert_fmt(self.tag == ResTag_ok, "Attempted to unwrap error result");        \
        return self.value;                                                                  \
    }                                                                                       \
    force_inline T pp_join(_, TRes, unwrapOr)(TRes self, T default_value) {                 \
        return self.tag == ResTag_ok ? self.value : default_value;                          \
    }                                                                                       \
    force_inline E pp_join(_, TRes, unwrapErr)(TRes self) {                                 \
        debug_assert_fmt(self.tag == ResTag_err, "Attempted to unwrap ok result as error"); \
        return self.error;                                                                  \
    }

#define IMPL_impl_ResErr(T, TResErr)                                                        \
    struct TResErr {                                                                        \
        union {                                                                             \
            ResErr base;                                                                    \
            struct {                                                                        \
                ResTag tag;                                                                 \
                union {                                                                     \
                    T   value;                                                              \
                    Err error;                                                              \
                };                                                                          \
            };                                                                              \
        };                                                                                  \
    };                                                                                      \
    /* Result interface */                                                                  \
    force_inline TResErr pp_join(_, TResErr, ok)(T value) {                                 \
        return (TResErr){                                                                   \
            .tag   = ResTag_ok,                                                             \
            .value = value                                                                  \
        };                                                                                  \
    }                                                                                       \
    force_inline TResErr pp_join(_, TResErr, err)(Err error) {                              \
        return (TResErr){                                                                   \
            .tag   = ResTag_err,                                                            \
            .error = error                                                                  \
        };                                                                                  \
    }                                                                                       \
    force_inline bool pp_join(_, TResErr, isOk)(TResErr self) {                             \
        return self.tag == ResTag_ok;                                                       \
    }                                                                                       \
    force_inline bool pp_join(_, TResErr, isErr)(TResErr self) {                            \
        return self.tag == ResTag_err;                                                      \
    }                                                                                       \
    force_inline T pp_join(_, TResErr, unwrap)(TResErr self) {                              \
        if (self.tag == ResTag_err) { Err_unreachable(self.error); }                        \
        return self.value;                                                                  \
    }                                                                                       \
    force_inline T pp_join(_, TResErr, unwrapOr)(TResErr self, T default_value) {           \
        return self.tag == ResTag_ok ? self.value : default_value;                          \
    }                                                                                       \
    force_inline Err pp_join(_, TResErr, unwrapErr)(TResErr self) {                         \
        debug_assert_fmt(self.tag == ResTag_err, "Attempted to unwrap ok result as error"); \
        return self.error;                                                                  \
    }

/* Composite implementations */
#define IMPL_impl_ExtType(T) \
    IMPL_impl_PtrType(T);    \
    IMPL_impl_VarType(T);    \
    IMPL_impl_VarPtrType(T)

#define IMPL_impl_PtrType(T)                                           \
    IMPL_impl_Ptr(T, _Ref(T), _Ptr(T), _PtrS(T));                      \
    IMPL_impl_Ptr(const T, _RefConst(T), _PtrConst(T), _PtrSConst(T)); \
    IMPL_impl_Sli(T, _Ref(T), _Ptr(T), _Sli(T), _SliS(T));             \
    IMPL_impl_Sli(const T, _RefConst(T), _PtrConst(T), _SliConst(T), _SliSConst(T))

#define IMPL_impl_VarType(T)   \
    IMPL_impl_Opt(T, _Opt(T)); \
    IMPL_impl_ResErr(T, _ResErr(T))

#define IMPL_impl_VarPtrType(T)                            \
    IMPL_impl_Opt(_Ptr(T), _Opt_Ptr(T));                   \
    IMPL_impl_Opt(_PtrConst(T), _Opt_PtrConst(T));         \
    IMPL_impl_Opt(_PtrS(T), _Opt_PtrS(T));                 \
    IMPL_impl_Opt(_PtrSConst(T), _Opt_PtrSConst(T));       \
    IMPL_impl_Opt(_Sli(T), _Opt_Sli(T));                   \
    IMPL_impl_Opt(_SliConst(T), _Opt_SliConst(T));         \
    IMPL_impl_Opt(_SliS(T), _Opt_SliS(T));                 \
    IMPL_impl_Opt(_SliSConst(T), _Opt_SliSConst(T));       \
    IMPL_impl_ResErr(_Ptr(T), _ResErr_Ptr(T));             \
    IMPL_impl_ResErr(_PtrConst(T), _ResErr_PtrConst(T));   \
    IMPL_impl_ResErr(_PtrS(T), _ResErr_PtrS(T));           \
    IMPL_impl_ResErr(_PtrSConst(T), _ResErr_PtrSConst(T)); \
    IMPL_impl_ResErr(_Sli(T), _ResErr_Sli(T));             \
    IMPL_impl_ResErr(_SliConst(T), _ResErr_SliConst(T));   \
    IMPL_impl_ResErr(_SliS(T), _ResErr_SliS(T));           \
    IMPL_impl_ResErr(_SliSConst(T), _ResErr_SliSConst(T))

/* Builtin types */
using_ExtTypes(u8);
using_ExtTypes(u16);
using_ExtTypes(u32);
using_ExtTypes(u64);
using_ExtTypes(usize);

using_ExtTypes(i8);
using_ExtTypes(i16);
using_ExtTypes(i32);
using_ExtTypes(i64);
using_ExtTypes(isize);

using_ExtTypes(f32);
using_ExtTypes(f64);

using_ExtTypes(bool);
using_ExtTypes(char);
using_ExtTypes(voidptr);

using_ExtTypes(Ptr);
using_ExtTypes(PtrS);
using_ExtTypes(Sli);
using_ExtTypes(SliS);

using_ResErr(Void, ResErr_Void);

#endif /* EXT_TYPES_INCLUDED */
