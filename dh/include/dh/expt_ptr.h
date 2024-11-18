#include "core.h"
#include <string.h>


typedef struct Bytes {
    u8*   data;
    usize size;
} Bytes;

static const Bytes Bytes_null = (Bytes){ .data = null, .size = 0 };

static inline Bytes Bytes_make(u8* data, usize size) {
    return (Bytes){ .data = data, .size = size };
}

static inline usize Bytes_size(Bytes b) {
    return b.size;
}

static inline const u8* Bytes_asRaw(const Bytes* self) {
    return self->data;
}

static inline u8* Bytes_mut_asRaw(Bytes* self) {
    return self->data;
}

static inline u8* Bytes_raw(Bytes b) {
    return b.data;
}

static inline Bytes Bytes_offset(Bytes b, usize offset) {
    if (b.size <= offset) { return Bytes_null; }
    return Bytes_make(b.data + offset, b.size - offset);
}

static inline bool Bytes_copy(const Bytes* src, Bytes* dest) {
    if (dest->size < src->size) { return false; }
    memcpy(dest->data, src->data, src->size);
    return true;
}

static inline bool Bytes_move(Bytes* src, Bytes* dest) {
    if (dest->size < src->size) { return false; }
    memmove(dest->data, src->data, src->size);
    return *src = Bytes_null, true;
}


static inline isize Bytes_cmp(const Bytes* self, const Bytes* other) {
    if (self->size < other->size) { return -1; }
    if (self->size > other->size) { return 1; }
    return memcmp(self->data, other->data, self->size);
}

static inline bool Bytes_eq(const Bytes* self, const Bytes* other) {
    return Bytes_cmp(self, other) == 0;
}

static inline bool Bytes_ne(const Bytes* self, const Bytes* other) {
    return !Bytes_eq(self, other);
}

static inline bool Bytes_lt(const Bytes* self, const Bytes* other) {
    return Bytes_cmp(self, other) < 0;
}

static inline bool Bytes_gt(const Bytes* self, const Bytes* other) {
    return Bytes_cmp(self, other) > 0;
}

static inline bool Bytes_le(const Bytes* self, const Bytes* other) {
    return !Bytes_gt(self, other);
}

static inline bool Bytes_ge(const Bytes* self, const Bytes* other) {
    return !Bytes_lt(self, other);
}

static inline bool Bytes_isNull(const Bytes* self) {
    return Bytes_eq(self, &Bytes_null);
}


typedef union SizedPtr {
    Bytes Bytes[1];
    struct {
        anyptr data;
        usize  size;
    };
} SizedPtr;

static const SizedPtr SizedPtr_null = (SizedPtr){ .data = null, .size = 0 };

// Core ptr operations
static inline SizedPtr SizedPtr_make(anyptr p, usize size) {
    return (SizedPtr){ .data = p, .size = size };
}

static inline SizedPtr SizedPtr_fromBytes(Bytes other) {
    return (SizedPtr){ .Bytes = { other } };
}

static inline Bytes Bytes_into(Bytes* other) {
    Bytes value   = *other;
    return *other = Bytes_null, value;
}

static inline SizedPtr* SizedPtr_takeBytes(SizedPtr* self, Bytes b) {
    return *self = SizedPtr_fromBytes(b), self;
}

static inline usize SizedPtr_size(SizedPtr p) {
    return p.size;
}

static inline const anyptr SizedPtr_asRaw(const SizedPtr* self) {
    return self->data;
}

static inline anyptr SizedPtr_mut_asRaw(SizedPtr* self) {
    return self->data;
}

static inline anyptr SizedPtr_raw(SizedPtr p) {
    return p.data;
}

static inline const Bytes* SizedPtr_asBytes(const SizedPtr* self) {
    return self->Bytes;
}

static inline Bytes* SizedPtr_mut_asBytes(SizedPtr* self) {
    return self->Bytes;
}

static inline Bytes SizedPtr_bytes(SizedPtr p) {
    return p.Bytes[0];
}

static inline SizedPtr SizedPtr_offset(SizedPtr p, usize offset) {
    if (p.size <= offset) { return SizedPtr_null; }
    return SizedPtr_fromBytes(Bytes_offset(SizedPtr_bytes(p), offset));
}

static inline bool SizedPtr_copy(SizedPtr* src, SizedPtr* dest) {
    return Bytes_copy((Bytes*)src, (Bytes*)dest);
}

static inline bool SizedPtr_move(SizedPtr* src, SizedPtr* dest) {
    return Bytes_move((Bytes*)src, (Bytes*)dest);
}

static inline Bytes Bytes_fromSizedPtr(SizedPtr p) {
    return SizedPtr_bytes(p);
}

static inline SizedPtr SizedPtr_into(SizedPtr* self) {
    SizedPtr value = *self;
    return *self   = SizedPtr_null, value;
}

static inline Bytes* Bytes_takeSizedPtr(Bytes* self, SizedPtr p) {
    return *self = SizedPtr_bytes(p), self;
}


static inline isize SizedPtr_cmp(const SizedPtr* self, const SizedPtr* other) {
    return Bytes_cmp(SizedPtr_asBytes(self), SizedPtr_asBytes(other));
}

static inline bool SizedPtr_eq(const SizedPtr* self, const SizedPtr* other) {
    return SizedPtr_cmp(self, other) == 0;
}

static inline bool SizedPtr_ne(const SizedPtr* self, const SizedPtr* other) {
    return !SizedPtr_eq(self, other);
}

static inline bool SizedPtr_lt(const SizedPtr* self, const SizedPtr* other) {
    return SizedPtr_cmp(self, other) < 0;
}

static inline bool SizedPtr_gt(const SizedPtr* self, const SizedPtr* other) {
    return SizedPtr_cmp(self, other) > 0;
}

static inline bool SizedPtr_le(const SizedPtr* self, const SizedPtr* other) {
    return !SizedPtr_gt(self, other);
}

static inline bool SizedPtr_ge(const SizedPtr* self, const SizedPtr* other) {
    return !SizedPtr_lt(self, other);
}

static inline bool SizedPtr_isNull(const SizedPtr* self) {
    return SizedPtr_eq(self, &SizedPtr_null);
}

/* TODO: Implement mem_alloc<->free, create<->destroy,
realloc, set, copy, move, cmp */
