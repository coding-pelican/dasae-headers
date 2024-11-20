#define typedef_union(_Alias) IMPL_typedef_union(_Alias)
#define IMPL_typedef_union(_Alias) \
    typedef union _Alias _Alias;   \
    union _Alias
