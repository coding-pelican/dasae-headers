


#define Ref(T)    pp_join(_, Ref, T)
#define Ptr(T)    pp_join(_, Ptr, T)
#define SPtr(T)   pp_join(_, SPtr, T)
#define Sli(T)    pp_join(_, Sli, T)
#define SSli(T)   pp_join(_, SSli, T)
#define Arr(N, T) pp_join3(_, Arr, N, T)

/* Single-item pointer (*T in Zig) */
// #define typedef_Ref(T) \
//     typedef struct {   \
//         T value;       \
//     }* Ref(T)
#define typedef_Ref(T) \
    typedef rawptr(T) Ref(T)

/* Many-item pointer ([*]T in Zig) */
#define typedef_Ptr(T) \
    typedef struct {   \
        Ref(T) addr;   \
    } Ptr(T)

/* Sentinel-terminated pointer ([*:sentinel]T in Zig) */
#define typedef_SPtr(T) \
    typedef struct {    \
        Ref(T) addr;    \
        T sentinel;     \
    } SPtr(T)

/* Slice ([]T in Zig) */
#define typedef_Sli(T) \
    typedef struct {   \
        Ptr(T) ptr;    \
        usize len;     \
    } Sli(T)

/* Sentinel-terminated slice ([:sentinel]T in Zig) */
#define typedef_SSli(T) \
    typedef struct {    \
        SPtr(T) ptr;    \
        usize len;      \
    } SSli(T)

/* Array ([N]T in Zig) */
#define typedef_Arr(N, T) \
    typedef struct {      \
        T items[N];       \
    } Arr(N, T)

#define typedef_PtrTypes(T) \
    typedef_Ref(T);         \
    typedef_Ptr(T);         \
    typedef_SPtr(T);        \
    typedef_Sli(T);         \
    typedef_SSli(T);

typedef_PtrTypes(i32);
typedef_Arr(16, i32);
typedef_PtrTypes(f64);
typedef_Arr(8, f64);

#define ref(_var)   (&(_var))
#define deref(_ptr) (*(_ptr))

#define Ptr_init(_addr)                           { .addr = (_addr) }
#define SPtr_init(_addr, _sentinel)               { .addr = (_addr), .sentinel = (_sentinel) }
#define Sli_init(_addr, _begin, _end)             { .ptr = Ptr_init((_addr) + (_begin)), .len = (_end) - (_begin) }
#define SSli_init(_addr, _begin, _end, _sentinel) { .ptr = SPtr_init((_addr) + (_begin), _sentinel), .len = (_end) - (_begin) }

#define Ptr_at(_ptr, _idx)            ((_ptr).addr[_idx])
#define Ptr_slice(_ptr, _begin, _end) { .addr = (_ptr).addr + (_begin), .len = (_end) - (_begin) }
#define Ptr_add(_ptr, _idx)           { .addr = (_ptr).addr + (_idx) }
#define Ptr_sub(_ptr, _idx)           { .addr = (_ptr).addr - (_idx) }

#define Sli_at(_sli, _idx)            ((_sli).ptr.addr[_idx])
#define Sli_slice(_sli, _begin, _end) { .addr = (_sli).ptr.addr + (_begin), .len = (_end) - (_begin) }

#define Arr_at(_arr, _index)          (_arr).items[_index]
#define Arr_slice(_arr, _begin, _end) Sli_init(((_arr).items), _begin, _end)
#define Arr_len(_arr)                 (sizeof((_arr).items) / sizeof((_arr).items[0]))

int main(void) {
    Ref_i32 test_ref = ref(make(i32, 42));
    debug_assert(deref(test_ref) == 42);

    Arr_16_i32 test_array = { 1, 2, 3, 4, 5 };
    Ptr_i32    test_ptr   = Ptr_init(test_array.items);

    debug_assert(Ptr_at(test_ptr, 0) == Arr_at(test_array, 0));

    Sli_i32 test_sli = Arr_slice(test_array, 1, 4);
    debug_assert(Sli_at(test_sli, 0) == Arr_at(test_array, 1));
    debug_assert(Sli_at(test_sli, 1) == Arr_at(test_array, 2));
    debug_assert(Sli_at(test_sli, 2) == Arr_at(test_array, 3));
    debug_assert(Sli_at(test_sli, 3) == Arr_at(test_array, 4));

    return 0;
}
