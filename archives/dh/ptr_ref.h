#ifndef PTR_REF_INCLUDED
#  define PTR_REF_INCLUDED (1)
#  if defined(__cplusplus)
extern "C" {
#  endif /* defined(__cplusplus) */


#  include "assert.h"
#  include "primitive_types.h"


/* Mutable Versions */

#  define TypePtr(_T)                          \
        /* Generic pointer type declaration */ \
        RETURN_PtrType(_T)

#  define RETURN_PtrType(_T) \
        typedef RawPtr(_T) Ptr_##_T

#  define Ptr(_T)              \
        /* Type alias macro */ \
        RETURN_Ptr(_T)

#  define RETURN_Ptr(_T) \
        Ptr_##_T

#  define ptr(_address)              \
        /* Pointer creation macro */ \
        RETURN_ptr((anyopaque)(_address))

#  ifndef NDEBUG
static inline anyopaque RETURN_ptr(anyopaque address) { return address; }
#  else
#    define RETURN_ptr(_address) address
#  endif


#  define TypeRef(_T)                            \
        /* Generic reference type declaration */ \
        RETURN_RefType(_T)

#  define RETURN_RefType(_T)      \
        typedef struct Ref_##_T { \
            _T value;             \
        }* const Ref_##_T

#  define Ref(_T)              \
        /* Type alias macro */ \
        RETURN_Ref(_T)

#  define RETURN_Ref(_T) \
        Ref_##_T

#  define ref(_variable)               \
        /* Reference creation macro */ \
        RETURN_ref((anyopaque)(&(_variable)))

#  ifndef NDEBUG
static inline anyopaque RETURN_ref(anyopaque address) {
    assertNotNullFmt(address, "Attempting to create a reference to NULL");
    return address;
}
#  else
#    define RETURN_ref(_address) _address
#  endif


#  define TypePtrRef(_T)                                 \
        /* Generic pointer-reference type declaration */ \
        TypePtr(_T);                                     \
        TypePtr(Ptr(_T));                                \
        TypePtr(Ptr(Ptr(_T)));                           \
        TypeRef(_T);                                     \
        TypeRef(Ptr(_T));                                \
        TypeRef(Ptr(Ptr(_T)))


/* Const Versions */

#  define TypePtrConst(_T)                           \
        /* Generic const pointer type declaration */ \
        RETURN_PtrConstType(_T)

#  define RETURN_PtrConstType(_T) \
        typedef RawPtr(const _T) PtrConst_##_T

#  define PtrConst(_T)                            \
        /* Type alias macro for const pointers */ \
        RETURN_PtrConst(_T)

#  define RETURN_PtrConst(_T) \
        PtrConst_##_T

#  define ptr_const(_address)              \
        /* Const pointer creation macro */ \
        RETURN_ptr_const((anyopaque)(_address))

#  ifndef NDEBUG
static inline anyopaque RETURN_ptr_const(anyopaque address) { return address; }
#  else
#    define RETURN_ptr_const(_address) _address
#  endif


#  define TypeRefConst(_T)                             \
        /* Generic const reference type declaration */ \
        RETURN_RefConstType(_T)

#  define RETURN_RefConstType(_T)      \
        typedef struct RefConst_##_T { \
            const _T value;            \
        }* const RefConst_##_T

#  define RefConst(_T)                              \
        /* Type alias macro for const references */ \
        RETURN_RefConst(_T)

#  define RETURN_RefConst(_T) \
        RefConst_##_T

#  define ref_const(_variable)               \
        /* Const reference creation macro */ \
        RETURN_ref_const((anyopaque)(&(_variable)))

#  ifndef NDEBUG
static inline anyopaque RETURN_ref_const(anyopaque address) {
    assertNotNullFmt(address, "Attempting to create a const reference to NULL");
    return address;
}
#  else
#    define RETURN_ref_const(_address) _address
#  endif


#  define TypePtrRefConst(_T)                                  \
        /* Generic const pointer-reference type declaration */ \
        TypePtrConst(_T);                                      \
        TypePtrConst(Ptr(_T));                                 \
        TypePtrConst(PtrConst_##_T);                           \
        TypeRefConst(_T);                                      \
        TypeRefConst(Ptr(_T));                                 \
        TypeRefConst(PtrConst_##_T)


#  if defined(__cplusplus)
}
#  endif /* defined(__cplusplus) */
#endif /* PTR_REF_INCLUDED */


/* // Example usage
void TEST_Pointer(Ptr(int) p) {
    // Use the pointer
    *p = 42;
}

void TEST_Reference(Ref(int) r) {
    // Use the reference
    r->value = 42;
}

int main() {
    // Basic reference usage with safety checks
    int  x                  = 42;
    int* prim_ptr_x         = &x;
    Ptr(int) ptr_x          = ptr(&prim_ptr_x);
    Ptr(Ptr(int)) ptr_ptr_x = ptr(&ptr_x);

    TEST_Pointer(ptr_x);

    Ref(int) ref_x = ref(x);
    TEST_Reference(ref_x);
    return 0;
} */

// // Ptrerence validation function
// #if REF_DEBUG
// static bool ref_is_valid(const void* ref_ptr, size_t ref_size, const char* type_name) {
//     if (!ref_ptr) {
//         fprintf(stderr, "Invalid NULL reference of type %s\n", type_name);
//         return false;
//     }

//     // Additional runtime checks could be added here
//     return true;
// }
// #endif

// // Example of safe reference usage in functions
// void increment(Ptr(int) target) {
//     assertNotNullFmt(target.value, "NULL reference passed to increment");
//     deref(target)++;
// }

// // Example of container with reference safety
// #define DECLARE_SAFE_REF_ARRAY(T)                                                      \
//     typedef struct {                                                                   \
//         size_t len;                                                                    \
//         Ptr(T) * data;                                                                 \
//         _DEBUG_ONLY(const char* debug_name;)                                           \
//     } T##PtrArray;                                                                     \
//                                                                                        \
//     static inline T##PtrArray T##PtrArray_create(size_t len, const char* debug_name) { \
//         T##PtrArray arr = {                                                            \
//             .len  = len,                                                               \
//             .data = calloc(len, sizeof(Ptr(T))),                                       \
//             _DEBUG_ONLY(.debug_name = debug_name)                                      \
//         };                                                                             \
//         REF_ASSERT(arr.data != NULL, "Failed to allocate reference array");            \
//         return arr;                                                                    \
//     }                                                                                  \
//                                                                                        \
//     static inline void T##PtrArray_destroy(T##PtrArray* arr) {                         \
//         REF_ASSERT(arr != NULL, "Destroying NULL array");                              \
//         free(arr->data);                                                               \
//         arr->data = NULL;                                                              \
//         arr->len  = 0;                                                                 \
//     }

// DECLARE_SAFE_REF_ARRAY(int);

// int main() {
//     // Basic reference usage with safety checks
//     int  x        = 42;
//     int* null_ptr = NULL;

//     // This will compile
//     Ptr(int) ref_x = ptr(&x);

//     // These will fail compilation or runtime checks:
//     // Ptr(int*) ref_ptr;  // Error: Cannot create reference to pointer type
//     // Ptr(int) ref_null = ptr(null_ptr);  // Runtime error: NULL reference
//     // Ptr(Ptr(int)) ref_ref;  // Error: Cannot create reference to reference type

//     const int const_x = 42;
//     // Ptr(int) const_ref = ptr(&const_x);  // Error: Cannot create non-const reference to const value

//     // Safe dereferencing
//     printf("Value through safe deref: %d\n", deref(ref_x));

//     // Function with safety checks
//     increment(ref_x);
//     printf("After safe increment: %d\n", x);

//     // Safe container usage
//     IntPtrArray arr = IntPtrArray_create(2, "test_array");
//     arr.data[0]     = ptr(&x);

//     deref(arr.data[0]) = 100;
//     printf("Modified through array: %d\n", x);

//     IntPtrArray_destroy(&arr);

//     return 0;
// }

// // Additional safety features for custom types
// typedef struct {
//     int x, y;
// } Point;

// TypePtr(Point);

// void move_point(Ptr(Point) ptr, int dx, int dy) {
//     REF_ASSERT(ptr.value != NULL, "NULL point reference");
//     ptr.value->x += dx;
//     ptr.value->y += dy;
// }
