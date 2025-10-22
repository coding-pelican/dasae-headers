#include "dh/Err.h"
#include "dh/main.h"


#define m_Val_from(val) \
    ((m_Val){ .info = typeInfo$(TypeOf(val)), .ref = &val })
#define m_Val_into$(T, val) ({ \
    let __val = (val); \
    debug_assert(__val.info.size == sizeOf(T)); \
    debug_assert(__val.info.align == alignOf(T)); \
    *as$((T*)(__val.ref)); \
})
#define m_Opt$Val_into$(Opt$T, opt) ({ \
    let __opt = (opt); \
    !__opt.has_value \
        ? none$(Opt$T) \
        : some$( \
              Opt$T, \
              m_Val_into$( \
                  FieldTypeOf(Opt$T, value), \
                  __opt.value \
              ) \
          ); \
})

#define m_Val_ref(self)
#define m_Ptr_deref(self)

typedef struct m_Val {
    TypeInfo info;
    void*    ref;
} m_Val;
typedef struct m_Opt$Val {
    bool  has_value;
    m_Val value;
} m_Opt$Val;

typedef struct m_P_c$Val {
    TypeInfo    info;
    const void* addr;
} m_P_c$Val;
typedef union m_P$Val {
    m_P_c$Val as_const;
    struct {
        TypeInfo info;
        void*    addr;
    };
} m_P$Val;

typedef struct m_S_c$Val {
    m_P$Val ptr;
    usize   len;
} m_S_c$Val;
typedef union m_S$Val {
    m_S_c$Val as_const;
    struct {
        m_P$Val ptr;
        usize   len;
    };
} m_S$Val;

typedef struct Vec {
    m_S$Val       data;
    usize         cap;
    mem_Allocator allocator;
} Vec;
static fn_((Vec_init(TypeInfo type, mem_Allocator allocator))(Vec));
static fn_((Vec_push(Vec* self, m_Val val))(Err$void)) $must_check;
static fn_((Vec_pop(Vec* self, m_Val mem))(m_Opt$Val));
static fn_((Vec_fini(Vec* self))(void));

#define Vec$(T) tpl_T(Vec, T)
#define Vec_useT$(T) \
    typedef union Vec$(T) { \
        Vec base[1]; \
        struct { \
            TypeInfo info; \
            Sli$(T) data; \
        }; \
    } Vec$(T); \
    use_Ptr$(Vec$(T)); \
    /** \
     * @brief Initialize vector \
     * @param allocator allocator to use \
     * @return initialized vector \
     */ \
    tpl_fn_(Vec$(T), init(mem_Allocator allocator), Vec$(T)) { \
        return (Vec$(T)){ .base = { [0] = Vec_init(typeInfo$(T), allocator) } }; \
    } \
    /** \
     * @brief Push value to vector \
     * @param self vector to push to \
     * @param val value to push \
     * @return error if allocation fails \
     */ \
    $must_check \
    tpl_fn_(Vec$(T), push(Ptr$(Vec$(T)) self, T val), Err$void) { \
        return Vec_push(self->base, m_Val_from(val)); \
    } \
    /** \
     * @brief Pop value from vector \
     * @param self vector to pop from \
     * @return value popped from vector \
     */ \
    tpl_fn_(Vec$(T), pop(Ptr$(Vec$(T)) self), Opt$(T)) { \
        return m_Opt$Val_into$(Opt$(T), Vec_pop(self->base, m_Val_from((T){}))); \
    } \
    /** \
     * @brief Finalize vector \
     * @param self vector to finalize \
     * @return void \
     */ \
    tpl_fn_(Vec$(T), fini(Ptr$(Vec$(T)) self), void) { \
        Vec_fini(self->base); \
    }


#include "dh/heap/Page.h"

Vec_useT$(i32);
// TEST_fn_("Vec: push and pop" $guard) {
fn_((dh_main(Sli$Sli_const$u8 args))(Err$void) $guard) {
    let_ignore = args;

    let allocator = heap_Page_allocator(&(heap_Page){});
    var vec       = Vec$i32_init(allocator);
    defer_(Vec$i32_fini(&vec));

    try_(Vec$i32_push(&vec, 1));
    try_(Vec$i32_push(&vec, 2));
    try_(Vec$i32_push(&vec, 3));
    try_(TEST_expect(unwrap(Vec$i32_pop(&vec)) == 3));
    try_(TEST_expect(unwrap(Vec$i32_pop(&vec)) == 2));
    try_(TEST_expect(unwrap(Vec$i32_pop(&vec)) == 1));

    printf("done");

    return_ok({});
}$unguarded_(fn);
// } $unguarded_TEST_fn;

static fn_((Vec_init(TypeInfo type, mem_Allocator allocator))(Vec) $scope) {
    let init_len = 8;
    let data     = catch_((mem_Allocator_alloc(allocator, type, init_len))($ignore, claim_unreachable));
    return_({
        .allocator = allocator,
        .data      = { .ptr = { .info = data.type, .addr = data.addr }, .len = 0 },
        .cap       = init_len,
    });
} $unscoped;
static fn_((Vec_push(Vec* self, m_Val val))(Err$void) $scope) {
    if (self->data.len < self->cap) {
        bti_memcpy(
            self->data.ptr.addr + (self->data.len * self->data.ptr.info.size),
            val.ref,
            self->data.ptr.info.size
        );
        self->data.len++;
        return_ok({});
    }
    return_err(Err_Unsupported());
} $unscoped;
static fn_((Vec_pop(Vec* self, m_Val mem))(m_Opt$Val) $scope) {
    if (0 < self->data.len) {
        bti_memcpy(
            mem.ref,
            self->data.ptr.addr + ((self->data.len - 1) * self->data.ptr.info.size),
            self->data.ptr.info.size
        );
        self->data.len--;
        return_some(mem);
    }
    return_none();
} $unscoped;
static fn_((Vec_fini(Vec* self))(void)) {
    let_ignore = self;
    // mem_Allocator_free(self->allocator, anySli(self->data.ptr.addr));
}
