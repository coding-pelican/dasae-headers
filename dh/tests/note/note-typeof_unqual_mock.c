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

    (void)b, (void)fooo, (void)foo_cast, (void)typeof_unqual_mockable;
}
