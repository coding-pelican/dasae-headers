// ptr.h
#ifndef PTR_INCLUDED
#define PTR_INCLUDED

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/core/prim/ptr.h"

#include <assert.h>
#include <string.h>

// Core pointer with metadata

typedef struct cptr {
    union {
        anyptr raw;
        usize  bits;
        struct {
            usize addr  : 48; // 256TB address space
            usize size  : 12; // Up to 4096 bytes
            usize align : 3;  // Up to 128-byte alignment
            usize flags : 1;  // Flags
        } meta;
    };
} cptr;

// Derived pointer types

typedef struct sptr {
    cptr core;
} sptr; // single-item pointer

typedef struct mptr {
    cptr core;
} mptr; // many-item pointer

typedef struct nptr {
    cptr   core;
    size_t len;
} nptr; // N-item pointer

/*========== Core Functions =================================================*/

#define sptr(THint) sptr
#define mptr(THint) mptr
#define nptr(THint) nptr

force_inline cptr cptr_init(anyptr raw, size_t size, size_t align);
force_inline cptr cptr_null(void);

// Conversion
force_inline anyptr cptr_raw(cptr self);
force_inline usize  cptr_bits(cptr self);
force_inline size_t cptr_size(cptr self);
force_inline size_t cptr_align(cptr self);
force_inline bool   cptr_flags(cptr self);

// Single-item pointer operations
force_inline sptr      sptr_init(anyptr raw, size_t size, size_t align);
force_inline anyptr    sptr_deref(sptr self);
force_inline nptr      sptr_slice(sptr self, size_t start, size_t end);
force_inline ptrdiff_t sptr_diff(sptr self, sptr other);

// Many-item pointer operations
force_inline mptr      mptr_init(anyptr raw, size_t size, size_t align);
force_inline anyptr    mptr_index(mptr self, size_t index);
force_inline nptr      mptr_slice(mptr self, size_t start, size_t end);
force_inline mptr      mptr_add(mptr self, ptrdiff_t offset);
force_inline mptr      mptr_sub(mptr self, ptrdiff_t offset);
force_inline ptrdiff_t mptr_diff(mptr self, mptr other);

// N-item pointer operations
force_inline nptr      nptr_init(anyptr raw, size_t len, size_t size, size_t align);
force_inline anyptr    nptr_index(nptr self, size_t index);
force_inline nptr      nptr_slice(nptr self, size_t start, size_t end);
force_inline ptrdiff_t nptr_diff(nptr self, nptr other);

/*========== Implementation Macros ==========================================*/

#define IMPL_cptr_init(raw, size, align) \
    ((cptr){ .raw        = (raw),        \
             .meta.size  = (size),       \
             .meta.align = get_alignment_order(align) })

#define IMPL_sptr_init(raw, size, align) \
    ((sptr){ .core = cptr_init((raw), (size), (align)) })

#define IMPL_mptr_init(raw, size, align) \
    ((mptr){ .core = cptr_init((raw), (size), (align)) })

#define IMPL_nptr_init(raw, len, size, align) \
    ((nptr){ .core = cptr_init((raw), (size), (align)), .len = (len) })

/*========== Convenience Macros =============================================*/

#define ptr_new(T, raw) \
    sptr_init((raw), sizeof(T), _Alignof(T))

#define ptr_many_new(T, raw) \
    mptr_init((raw), sizeof(T), _Alignof(T))

#define ptr_array_new(T, raw, count) \
    nptr_init((raw), (count), sizeof(T), _Alignof(T))

/*========== Implementation =================================================*/

// ptr_impl.h
#ifndef PTR_IMPL_INCLUDED
#define PTR_IMPL_INCLUDED

/*========== Core Pointer Functions ===================================*/

force_inline cptr cptr_init(anyptr raw, size_t size, size_t align) {
    return IMPL_cptr_init(raw, size, align);
}

force_inline cptr cptr_null(void) {
    return (cptr){ .raw = NULL };
}

force_inline anyptr cptr_raw(cptr self) {
    return self.raw;
}

force_inline usize cptr_bits(cptr self) {
    return self.bits;
}

force_inline size_t cptr_size(cptr self) {
    return self.meta.size;
}

force_inline size_t cptr_align(cptr self) {
    return 1ULL << self.meta.align;
}

force_inline bool cptr_flags(cptr self) {
    return self.meta.flags;
}

/*========== Single-item Pointer Functions ============================*/

force_inline sptr sptr_init(anyptr raw, size_t size, size_t align) {
    return IMPL_sptr_init(raw, size, align);
}

force_inline anyptr sptr_deref(sptr self) {
    return cptr_raw(self.core);
}

force_inline nptr sptr_slice(sptr self, size_t start, size_t end) {
    anyptr base = cptr_raw(self.core);
    size_t size = cptr_size(self.core);
    return nptr_init(
        (char*)base + start * size,
        end - start,
        size,
        cptr_align(self.core)
    );
}

force_inline ptrdiff_t sptr_diff(sptr self, sptr other) {
    return ((char*)cptr_raw(self.core) - (char*)cptr_raw(other.core))
         / cptr_size(self.core);
}

/*========== Many-item Pointer Functions ==============================*/

force_inline mptr mptr_init(anyptr raw, size_t size, size_t align) {
    return IMPL_mptr_init(raw, size, align);
}

force_inline anyptr mptr_index(mptr self, size_t index) {
    return (char*)cptr_raw(self.core) + index * cptr_size(self.core);
}

force_inline nptr mptr_slice(mptr self, size_t start, size_t end) {
    anyptr base = cptr_raw(self.core);
    size_t size = cptr_size(self.core);
    return nptr_init(
        (char*)base + start * size,
        end - start,
        size,
        cptr_align(self.core)
    );
}

force_inline mptr mptr_add(mptr self, ptrdiff_t offset) {
    anyptr base = cptr_raw(self.core);
    size_t size = cptr_size(self.core);
    return mptr_init(
        (char*)base + offset * size,
        size,
        cptr_align(self.core)
    );
}

force_inline mptr mptr_sub(mptr self, ptrdiff_t offset) {
    return mptr_add(self, -offset);
}

force_inline ptrdiff_t mptr_diff(mptr self, mptr other) {
    return ((char*)cptr_raw(self.core) - (char*)cptr_raw(other.core))
         / cptr_size(self.core);
}

/*========== N-item Pointer Functions ================================*/

force_inline nptr nptr_init(anyptr raw, size_t len, size_t size, size_t align) {
    return IMPL_nptr_init(raw, len, size, align);
}

force_inline anyptr nptr_index(nptr self, size_t index) {
    assert(index < self.len);
    return (char*)cptr_raw(self.core) + index * cptr_size(self.core);
}

force_inline nptr nptr_slice(nptr self, size_t start, size_t end) {
    assert(start <= end && end <= self.len);
    anyptr base = cptr_raw(self.core);
    size_t size = cptr_size(self.core);
    return nptr_init(
        (char*)base + start * size,
        end - start,
        size,
        cptr_align(self.core)
    );
}

force_inline ptrdiff_t nptr_diff(nptr self, nptr other) {
    return ((char*)cptr_raw(self.core) - (char*)cptr_raw(other.core))
         / cptr_size(self.core);
}

#endif // PTR_IMPL_INCLUDED

#endif // PTR_INCLUDED
