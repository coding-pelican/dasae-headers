/*
내가 원하는 동작:
O$u_Val에서 some인 u_Val이 참조하고 있는 값을 꺼내 O$Foo로 변환하기
u_Sli에서 ptr이 참조하고 있는 값을 꺼내 S$Foo로 변환하기
*/

#include "dh/core.h"
#include "dh/claim/unreachable.h"

/* typedef enum u_TypeTag : u8 {
    u_TypeTag_void = 0,
    u_TypeTag_val = 1,
    u_TypeTag_arr = 2,
    u_TypeTag_ptr = 3,
    u_TypeTag_sli = 4,
} u_TypeTag;
typedef struct u_Type {
    TypeInfo type;
    u_TypeTag tag;
    union {
        u8* mem;
        void* untyped;
    };
    usize len;
} u_Type; */

typedef void* P$raw;
typedef struct S$raw {
    void* ptr;
    usize len;
} S$raw;

typedef struct u_V$raw {
    TypeInfo type;
    union {
        u8* mem;
        P$raw untyped;
        P$raw inner;
    };
    usize len;
} u_V$raw;
typedef u_V$raw u_A$raw;

typedef struct u_P$raw {
    TypeInfo type;
    union {
        u8* mem;
        P$raw untyped;
        P$raw inner[1];
    };
    usize len;
} u_P$raw;
typedef struct u_S$raw {
    TypeInfo type;
    union {
        struct {
            u8* ptr;
            usize len;
        };
        S$raw untyped;
        S$raw inner[1];
    };
} u_S$raw;

typedef struct Foo {
    u8 name[63];
    u8 name_len;
} Foo;
typedef struct A$4$Foo {
    Foo val[4];
} A$4$Foo;
typedef Foo* P$Foo;
typedef union S$Foo {
    struct {
        Foo* ptr;
        usize len;
    };
    S$raw untyped;
} S$Foo;

typedef struct O$Foo {
    bool is_some;
    union {
        Void none;
        Foo some;
    } payload;
} O$Foo;
typedef struct O$P$Foo {
    bool is_some;
    union {
        Void none;
        P$Foo some;
    } payload;
} O$P$Foo;
typedef struct O$A$4$Foo {
    bool is_some;
    union {
        Void none;
        A$4$Foo some;
    } payload;
} O$A$4$Foo;
typedef struct O$S$Foo {
    bool is_some;
    union {
        Void none;
        S$Foo some;
    } payload;
} O$S$Foo;

typedef struct O$u_V$raw {
    bool is_some;
    union {
        Void none;
        u_V$raw some;
    } payload;
} O$u_V$raw;
typedef struct O$u_P$raw {
    bool is_some;
    union {
        Void none;
        u_P$raw some;
    } payload;
} O$u_P$raw;
typedef struct O$u_A$raw {
    bool is_some;
    union {
        Void none;
        u_A$raw some;
    } payload;
} O$u_A$raw;
typedef struct O$u_S$raw {
    bool is_some;
    union {
        Void none;
        u_S$raw some;
    } payload;
} O$u_S$raw;

#define u_allocA(_len, _type...) \
    __u_allocA__step(_len, _type)
#define __u_allocA__step(...) \
    __u_allocA__emit(pp_uniqTok(len), pp_uniqTok(type), pp_uniqTok(mem), __VA_ARGS__)
#define __u_allocA__emit(__len, __type, __mem, _len, _type...) ({ \
    const usize __len = _len; \
    const TypeInfo __type = _type; \
    const P$raw __mem = alloca(__len * __type.size); \
    prim_memset(__mem, 0, __len * __type.size); \
    lit$((u_A$raw){ .type = __type, .mem = __mem, .len = __len }); \
})
#define u_prim_memcpyA(_dst, _src...) \
    __u_prim_memcpyA__step(_dst, _src)
#define __u_prim_memcpyA__step(...) \
    __u_prim_memcpyA__emit(pp_uniqTok(dst), pp_uniqTok(src), __VA_ARGS__)
#define __u_prim_memcpyA__emit(__dst, __src, _dst, _src...) ({ \
    const u_A$raw __dst = _dst; \
    const u_A$raw __src = _src; \
    prim_memcpy(__dst.mem, __src.mem, __src.len * __src.type.size); \
    __dst; \
})
#define u_copyA(_src...) \
    __u_copyA__step(_src)
#define __u_copyA__step(...) \
    __u_copyA__emit(pp_uniqTok(src), __VA_ARGS__)
#define __u_copyA__emit(__src, _src...) ({ \
    const u_A$raw __src = _src; \
    u_prim_memcpyA(u_allocA(__src.len, __src.type), __src); \
})

#define u_allocV(_type...)            ((u_V$raw)u_allocA(1, _type))
#define u_prim_memcpyV(_dst, _src...) ((u_V$raw)u_prim_memcpyA(_dst, _src))
#define u_copyV(_src...)              ((u_V$raw)u_copyA(_src))

#define u_copyO(_src...) \
    __u_copyO__step(_src)
#define __u_copyO__step(...) \
    __u_copyO__emit(pp_uniqTok(dst), pp_uniqTok(src), __VA_ARGS__)
#define __u_copyO__emit(__dst, __src, _src...) ({ \
    let_(__src, TypeOf(_src)) = _src; \
    var_(__dst, TypeOf(__src)) = { \
        .is_some = __src.is_some, \
        .payload = { .some = u_allocA(__src.payload.some.len, __src.payload.some.type) }, \
    }; \
    if (__dst.is_some) { \
        prim_memcpy( \
            __dst.payload.some.mem, \
            __src.payload.some.mem, \
            __src.payload.some.len * __src.payload.some.type.size \
        ); \
    } \
    __dst; \
})

#define u_copyE(_src...) \
    __u_copyE__step(_src)
#define __u_copyE__step(...) \
    __u_copyE__emit(pp_uniqTok(dst), pp_uniqTok(src), __VA_ARGS__)
#define __u_copyE__emit(__dst, __src, _src...) ({ \
    let_(__src, TypeOf(_src)) = _src; \
    var_(__dst, TypeOf(__src)) = { \
        .is_ok = __src.is_ok, \
        .payload = { .ok = u_allocA(__src.payload.ok.len, __src.payload.ok.type) }, \
    }; \
    if (__dst.is_ok) { \
        prim_memcpy( \
            __dst.payload.ok.mem, \
            __src.payload.ok.mem, \
            __src.payload.ok.len * __src.payload.ok.type.size \
        ); \
    } else { \
        __dst.payload.err = __src.payload.err; \
    } \
    __dst; \
})

#define isSome(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)((_o).is_some)
#define isNone(_o /*: O$$(_T)*/... /*(bool)*/) as$(bool)(!(_o).is_some)

/* Unwraps optional value (similar to Zig's orelse and .?) */
#define orelse_(/*(_Expr: O$$(_T))(_DefaultExpr_OR_Body...: _T|void)*/... /*(_T)*/) \
    pp_expand(pp_defer(__block_inline__orelse_)(__param_expand__orelse_ __VA_ARGS__))
#define unwrap_(/*(_Expr: O$$(_T))*/... /*(_T)*/) \
    __unwrap(__VA_ARGS__)

#define __param_expand__orelse_(...)                                       __VA_ARGS__, pp_expand
#define __block_inline__orelse_(_Expr, _DefaultExpr_OR_Body...)            __block_inline1__orelse_(pp_uniqTok(result), _Expr, ({ _DefaultExpr_OR_Body; }))
#define __block_inline1__orelse_(__result, _Expr, _DefaultExpr_OR_Body...) pragma_guard_( \
    "clang diagnostic push", \
    "clang diagnostic ignored \"-Wcompound-token-split-by-macro\"", \
    "clang diagnostic pop", \
    ({ \
        var __result = _Expr; \
        if (isNone(__result)) { \
            __result.payload.some = _Generic( \
                TypeOfUnqual(_DefaultExpr_OR_Body), \
                void: ({ \
                    $ignore_void _DefaultExpr_OR_Body; \
                    lit$((TypeOf(__result.payload.some)){}); \
                }), \
                default: _DefaultExpr_OR_Body \
            ); \
        } \
        __result.payload.some; \
    }) \
)
#define __unwrap(_Expr...) orelse_((_Expr)(claim_unreachable))

#define u_castV$$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __u_castV$$__step(__u_castV$$__parseT __VA_ARGS__)
#define __u_castV$$__step(...)           __u_castV$$__emit(__VA_ARGS__)
#define __u_castV$$__parseT(_T...)       _T, __u_castV$$__parseExpr
#define __u_castV$$__parseExpr(_Expr...) _Expr
#define __u_castV$$__emit(_T, _Expr...)  (*as$(_T*)(prim_memcpy(&lit$((_T){}), (_Expr).mem, sizeOf$(_T))))

#define u_castA$$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __u_castA$$__step(__u_castA$$__parseT __VA_ARGS__)
#define __u_castA$$__step(...)           __u_castA$$__emit(__VA_ARGS__)
#define __u_castA$$__parseT(_T...)       _T, __u_castA$$__parseExpr
#define __u_castA$$__parseExpr(_Expr...) _Expr
#define __u_castA$$__emit(_T, _Expr...)  (*as$(_T*)(prim_memcpy(&lit$((_T){}), (_Expr).mem, sizeOf$(_T))))

#define u_castP$$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __u_castP$$__step(__u_castP$$__parseT __VA_ARGS__)
#define __u_castP$$__step(...)           __u_castP$$__emit(__VA_ARGS__)
#define __u_castP$$__parseT(_T...)       _T, __u_castP$$__parseExpr
#define __u_castP$$__parseExpr(_Expr...) _Expr
#define __u_castP$$__emit(_T, _Expr...)  ({ as$(_T)((_Expr).inner[0]); })

#define u_castS$$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __u_castS$$__step(__u_castS$$__parseT __VA_ARGS__)
#define __u_castS$$__step(...)           __u_castS$$__emit(__VA_ARGS__)
#define __u_castS$$__parseT(_T...)       _T, __u_castS$$__parseExpr
#define __u_castS$$__parseExpr(_Expr...) _Expr
#define __u_castS$$__emit(_T, _Expr...)  ({ *as$(_T*)((_Expr).inner); })

#define u_castO$$(/*(_T)(_Expr...)*/... /*(_T)*/) \
    __u_castO$$__step(__u_castO$$__parseT __VA_ARGS__)
#define __u_castO$$__step(...)           __u_castO$$__emit(__VA_ARGS__)
#define __u_castO$$__parseT(_T...)       _T, __u_castO$$__parseExpr
#define __u_castO$$__parseExpr(_Expr...) _Expr
#define __u_castO$$__emit(_T, _Expr...)  (*as$(_T*)(prim_memcpy(&lit$((_T){}), (_Expr).inner, sizeOf$(_T))))

#include <stdio.h>

int main(void) {
    Foo foo = { .name = "Hello, World!", .name_len = 13 };
    O$Foo o_foo = { .is_some = true, .payload = { .some = foo } };
    var some_foo = unwrap_(o_foo);

    u_V$raw ufoo = { .type = typeInfo$(TypeOf(foo)), .untyped = &copy(foo), .len = 1 };
    var p_ufoo = as$(Foo *) ((ufoo.mem));
    O$u_V$raw o_ufoo = { .is_some = true, .payload = { .some = ufoo } };
    var some_ufoo = u_copyV(unwrap_(o_ufoo));
    var some_p_ufoo = as$(Foo *) ((some_ufoo.mem));

    var casted_ufoo = u_castV$$((Foo)(ufoo));
    var casted_some_ufoo = u_castV$$((Foo)(unwrap_(o_ufoo))); // Same as `u_copyV(unwrap_(o_ufoo)); as$(Foo*)(some_ufoo.mem);`
    prim_memcpy(some_foo.name + some_foo.name_len, " It's Foo!", 11);
    prim_memcpy(some_p_ufoo->name + some_p_ufoo->name_len, " It's meta Foo!", 16);
    prim_memcpy(casted_ufoo.name + casted_ufoo.name_len, " It's casted Foo!", 18);
    prim_memcpy(casted_some_ufoo.name + casted_some_ufoo.name_len, " It's casted some Foo!", 23);

    u_P$raw u_p_foo = { .type = typeInfo$(TypeOf(foo)), .untyped = &copy(foo), .len = 1 };
    O$u_P$raw o_u_p_foo = { .is_some = true, .payload = { .some = u_p_foo } };
    var casted_u_p_foo = u_castP$$((Foo*)(u_p_foo));
    var casted_some_u_p_foo = u_castP$$((Foo*)(unwrap_(o_u_p_foo)));

    A$4$Foo a_4_foo = { .val = { foo, foo, foo, foo } };
    u_A$raw u_a_foo = { .type = typeInfo$(TypeOf(a_4_foo)), .untyped = &copy(a_4_foo), .len = countOf$(TypeOf(a_4_foo.val)) };
    O$u_A$raw o_u_a_foo = { .is_some = true, .payload = { .some = u_a_foo } };
    var casted_u_a_foo = u_castA$$((A$4$Foo)(u_a_foo));
    var casted_some_u_a_foo = u_castA$$((A$4$Foo)(unwrap_(o_u_a_foo)));

    S$Foo s_foo = { .ptr = a_4_foo.val, .len = countOf$(TypeOf(a_4_foo.val)) };
    u_S$raw u_s_foo = { .type = typeInfo$(TypeOf(s_foo.ptr[0])), .untyped = s_foo.untyped };
    O$u_S$raw o_u_s_foo = { .is_some = true, .payload = { .some = u_s_foo } };
    var casted_u_s_foo = u_castS$$((S$Foo)(u_s_foo));
    var casted_some_u_s_foo = u_castS$$((S$Foo)(unwrap_(o_u_s_foo)));

    printf("foo: %s\n", foo.name);
    printf("some_foo: %s\n", some_foo.name);
    printf("ufoo: %s\n", p_ufoo->name);
    printf("some_ufoo: %s\n", some_p_ufoo->name);
    printf("casted_ufoo: %s\n", casted_ufoo.name);
    printf("casted_some_ufoo: %s\n", casted_some_ufoo.name);

    printf("casted_u_p_foo: %s\n", casted_u_p_foo->name);
    printf("casted_some_u_p_foo: %s\n", casted_some_u_p_foo->name);
    printf("casted_u_a_foo: %s\n", casted_u_a_foo.val[0].name);
    printf("casted_some_u_a_foo: %s\n", casted_some_u_a_foo.val[0].name);
    printf("casted_u_s_foo: %s\n", casted_u_s_foo.ptr[0].name);
    printf("casted_some_u_s_foo: %s\n", casted_some_u_s_foo.ptr[0].name);
    return 0;
}



#if UNUSED_CODE
typedef struct Bar {
    int num1;
    int num2;
    float value1;
    float value2;
} Bar;

typedef struct O$Bar {
    bool is_some;
    union {
        struct {
            Void none;
            union {
                Bar some;
                Bar some_ref[1];
            };
        } payload;
        struct {
            Void none;
            union {
                Bar some;
                Bar some_ref[1];
            };
        } payload_ref[1];
    };
} O$Bar;

typedef struct uVal {
    void* addr;
    TypeInfo type;
} uVal;

typedef struct O$uVal {
    bool is_some;
    union {
        struct {
            Void none;
            union {
                uVal some;
                uVal some_ref[1];
            };
        } payload;
        struct {
            Void none;
            union {
                uVal some;
                uVal some_ref[1];
            };
        } payload_ref[1];
    };
} O$uVal;

#include <stdio.h>

int main(void) {
    O$uVal o_val = ({
        let p_val = &(Bar){ .num1 = 1, .num2 = 41, .value1 = 3.0f, .value2 = 0.14f };
        let p_o_val = &(O$uVal){ .is_some = true, .payload.some = { .addr = alloca(sizeof(Bar)) } };
        prim_memcpy(p_o_val->payload.some.addr, p_val, sizeof(Bar));
        *p_o_val;
    });

    /* O$Bar o_bar = ({
        let converted = (struct {
            bool is_some;
            union {
                Void none;
                Bar* some;
            } payload;
        }*)&o_val;
        (O$Bar){ .is_some = converted->is_some, .payload = { .some = *converted->payload.some } };
    }); */

    O$Bar o_bar = ({
        let converted = (struct {
            bool is_some;
            union {
                Void none;
                struct {
                    Bar* inner;
                    TypeInfo type;
                } some;
            } payload;
        }*)&o_val;
        let direct_ptr = &o_val.payload;
        let converted_ptr = &converted->payload;
        printf("direct_ptr: %p\n", direct_ptr);
        printf("converted_ptr: %p\n", converted_ptr);

        printf("direct_ptr: %p\n", *(Bar**)direct_ptr);
        printf("converted_ptr: %p\n", *(Bar**)converted_ptr);

        (O$Bar){ .is_some = converted->is_some, .payload = { .some = **(Bar**)converted_ptr } };
    });
    {
        Bar bar = o_bar.payload.some;
        printf("bar.num: %d %d\n", bar.num1, bar.num2);
        printf("bar.value: %.0f %.2f\n", bar.value1, bar.value2);
    }

    O$Bar o_bar2 = ({
        let converted = (struct {
            bool is_some;
            union {
                Void none;
                struct {
                    Bar* inner;
                    TypeInfo type;
                } some;
            } payload;
        }*)&o_val;
        let direct_ptr = &o_val.payload.some;
        let converted_ptr = &converted->payload.some;
        printf("direct_ptr: %p\n", direct_ptr);
        printf("converted_ptr: %p\n", converted_ptr);

        printf("direct_ptr: %p\n", *(Bar**)direct_ptr);
        printf("converted_ptr: %p\n", *(Bar**)converted_ptr);

        (O$Bar){ .is_some = converted->is_some, .payload = { .some = **(Bar**)converted_ptr } };
    });
    {
        Bar bar = o_bar2.payload.some;
        printf("bar.num: %d %d\n", bar.num1, bar.num2);
        printf("bar.value: %.0f %.2f\n", bar.value1, bar.value2);
    }

    typedef union TypeInfo {
        struct {
            u64 size  : 58;
            u64 align : 6;
        };
        u64 packed;
    } TypeInfo;
    // #define typeInfo$(_T...) lit$((TypeInfo){ .size = sizeOf$(_T), .align = alignOf$(_T) })

    typedef struct Bar {
        int num1;
        int num2;
        float value1;
        float value2;
    } Bar;
    static Bar bar = {
        .num1 = 20,
        .num2 = 25,
        .value1 = 11.0f,
        .value2 = 0.01f,
    };

    struct HasIn {
        struct {
            Bar data;
        } inner;
    } inside_value = { .inner = { .data = { [0] = bar } } };

    struct HasOut {
        struct {
            void* data;
            TypeInfo type;
        } inner;
    } outside_ptr = { .inner = { .data = &bar, .type = typeInfo$(TypeOf(bar)) } };

    struct HasNestedOut {
        struct {
            void* data;
            TypeInfo type;
        } inner;
    } outside_ptr = { .inner = { .data = &bar, .type = typeInfo$(TypeOf(bar)) } };

#define someOp(_T, _container) \
    (*(_T*)((_container).inner.data))

    Bar owns_inside = someOp(Bar, inside_value);
    Bar owns_outside = someOp(Bar, outside_ptr);
    printf("owns_inside: %d %d %.0f %.2f\n", owns_inside.num1, owns_inside.num2, owns_inside.value1, owns_inside.value2);
    printf("owns_outside: %d %d %.0f %.2f\n", owns_outside.num1, owns_outside.num2, owns_outside.value1, owns_outside.value2);
}

#endif /* UNUSED_CODE */

#if UNUSED_CODE
#define O$(_T) \
    struct { \
        bool is_some; \
        union { \
            union { \
                Void none; \
                union { \
                    _T some; \
                    _T some_ref[1]; \
                }; \
            } payload; \
            union { \
                Void none; \
                union { \
                    _T some; \
                    _T some_ref[1]; \
                }; \
            } payload_ref[1]; \
        }; \
    }

typedef struct Bar {
    int num1;
    int num2;
    float value1;
    float value2;
} Bar;
typedef O$(Bar) O$Bar;
/* typedef struct {
    bool is_some;
    union {
        union {
            Void none;
            union {
                Bar some;
                Bar some_ref[1];
            };
        } payload;
        union {
            Void none;
            union {
                Bar some;
                Bar some_ref[1];
            };
        } payload_ref[1];
    };
} O$Bar; */

typedef struct uVal {
    void* addr;
    TypeInfo type;
} uVal;
/* typedef struct {
    bool is_some;
    union {
        union {
            Void none;
            union {
                uVal some;
                uVal some_ref[1];
            };
        } payload;
        union {
            Void none;
            union {
                uVal some;
                uVal some_ref[1];
            };
        } payload_ref[1];
    };
} O$uVal; */

// unwrap 매크로
#define unwrapOpt(_T, _opt) \
    (*(_T*)((_opt).payload_ref->some_ref))

#include <stdio.h>

int main(void) {
    Bar bar = { .num1 = 20, .num2 = 25, .value1 = 11.0f, .value2 = 0.01f };

    // O$Bar - payload에 직접 저장
    O$Bar opt_bar1 = {
        .is_some = true,
        .payload = { .some = bar }
    };

    // O$Bar - payload_ref를 통해 저장
    O$Bar opt_bar2 = {
        .is_some = true,
        .payload_ref = { [0] = { .some = bar } }
    };

    Bar result1 = unwrapOpt(Bar, opt_bar1);
    Bar result2 = unwrapOpt(Bar, opt_bar2);

    printf("result1: num1=%d, num2=%d, value1=%.2f, value2=%.4f\n", result1.num1, result1.num2, result1.value1, result1.value2);
    printf("result2: num1=%d, num2=%d, value1=%.2f, value2=%.4f\n", result2.num1, result2.num2, result2.value1, result2.value2);

    // O$uVal
    uVal uval = { .addr = &bar, .type = typeInfo$(Bar) };
    O$uVal opt_uval = {
        .is_some = true,
        .payload = { .some = uval }
    };

    uVal result3 = unwrapOpt(uVal, opt_uval);
    printf("result3: addr=%p, type.size=%llu\n", result3.addr, result3.type.size);

    return 0;
}

#endif /* UNUSED_CODE */
