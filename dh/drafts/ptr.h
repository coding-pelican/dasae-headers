#include "dh/core.h"
#include "dh/claim/assert_static.h"

#define Ref(T)     \
    union {        \
        T value;   \
        T addr[1]; \
    }*

#define Ptr(T)      \
    union {         \
        T* addr;    \
        Ref(T) ref; \
    }

#define using_Ptr(T) \
    decl_Ptr(T);     \
    impl_Ptr(T)

#define decl_Ptr(T) \
    typedef union pp_join($, Ptr, T) pp_join($, Ptr, T)

#define impl_Ptr(T)            \
    union pp_join($, Ptr, T) { \
        T* addr;               \
        Ref(T) ref;            \
    }

using_Ptr(i32);
#define points(var_target) \
    { .addr = &(var_target) }

#define ptr(var_target) \
    ((Ptr(TypeOf(var_target))){ .addr = &(var_target) })

#define ref(var_target) \
    ptr(var_target).ref

void test() {
    i32 value  = 123;
    let points = (Ptr$i32)points(value);
    let a      = ref(value);
    let a_ptr  = a->addr;
    *a_ptr     = 123;
}
