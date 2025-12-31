#undef true
#undef false
#undef null
#define true ((_Bool)1)
#define false ((_Bool)0)
#define null ((void*)0)

void test(void) {
    const int a = 123;
    __auto_type b = (__typeof__((const int)(a))){};

    typedef struct Foo {
        int value;
    } Foo;
    const Foo foo = {};
    __auto_type fooo = (__typeof__((const Foo)(foo))){};

    const Foo* foo_ptr = &foo;
    __auto_type foo_cast = (__typeof__((__typeof__(*foo_ptr))(*foo_ptr)))((__typeof__(*foo_ptr))(*foo_ptr));

    __auto_type typeof_unqual_mockable = _Generic(
        (__typeof__((__typeof__(*foo_ptr))(*foo_ptr))*)null,
        Foo*: true,
        const Foo*: false
    );

#define lit$(/*(_T){_initial...}*/... /*(_T)*/) __lit$(__VA_ARGS__)
#if defined(__cplusplus)
#define __lit$(...) (__lit$__expandT __VA_ARGS__)
#define __lit$__expandT(_T...) _T
#else
#define __lit$(...) (__VA_ARGS__)
#endif
#define lit0$(/*(_T)*/... /*(_T)*/) __lit0$(__step__lit0$__parse __VA_ARGS__)
#define __step__lit0$__parse(_T...) _T
#define __lit0$(_T...) (lit$((struct { _T val; }){}).val)
#define TypeOf(_Expr...) __typeof__(_Expr)
#define TypeOfUnqual(_Expr...) TypeOf((TypeOf(_Expr))(lit0$((TypeOf(_Expr)))))
    (void)b, (void)fooo, (void)foo_cast, (void)typeof_unqual_mockable;
}
