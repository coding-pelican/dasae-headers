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

// Core types
typedef struct {
    anyptr raw_;
    usize  size_;
} Ptr;

typedef struct {
    usize offset_;
    usize size_;
} PtrDiff;

typedef enum Error {
    Error_expt
} Error;

// Result type for operations that can fail
typedef struct {
    bool  success;
    Error error; // Assuming Error type exists
} Result;

// Option type for operations that might return null
typedef struct {
    bool has_value;
    Ptr  value;
} Option_Ptr;

// Constants
#define PTR_NULL ((Ptr){ NULL, 0 })

// Construction and validation
Ptr  Ptr_from(anyptr raw, usize size); // More explicit than "make"
Ptr  Ptr_withAlloc(usize size);        // Allocates new memory
Ptr  Ptr_nullWithSize(usize size);     // Explicit null with size
bool Ptr_isValid(Ptr p);               // Validates pointer state
bool Ptr_isNull(Ptr p);
bool Ptr_hasMinSize(Ptr p, usize required_size);

// Safe type casting
#define Ptr_as(ptr, T) \
    ((ptr).size_ >= sizeof(T) ? (T*)(ptr).raw_ : NULL)

#define Ptr_asArray(ptr, T, count) \
    ((ptr).size_ >= sizeof(T) * (count) ? (T*)(ptr).raw_ : NULL)

#define Ptr_asBytes(ptr) \
    Ptr_as(ptr, u8)

// Bounds checking and safety
bool  Ptr_canRead(Ptr p, usize offset, usize count);
bool  Ptr_canWrite(Ptr p, usize offset, usize count);
bool  Ptr_isAligned(Ptr p, usize alignment);
usize Ptr_remainingBytes(Ptr p, usize offset);
bool  Ptr_containsRange(Ptr container, Ptr inner);

// Safe memory operations
Result Ptr_copyTo(Ptr dest, Ptr src, usize count);
Result Ptr_copyFromRaw(Ptr dest, const anyptr src, usize count);
Result Ptr_copyToRaw(anyptr dest, Ptr src, usize count);
Result Ptr_writeBytes(Ptr dest, u8 value, usize count);
Result Ptr_zero(Ptr dest);

// Safe pointer arithmetic
Option_Ptr Ptr_advance(Ptr p, usize count);
Option_Ptr Ptr_retreat(Ptr p, usize count);
Option_Ptr Ptr_offset(Ptr p, isize offset);
Option_Ptr Ptr_slice(Ptr p, usize offset, usize size);

// Explicit comparison operations
bool  Ptr_eq(Ptr lhs, Ptr rhs);        // Compares both ptr and size
bool  Ptr_eqPtr(Ptr lhs, Ptr rhs);     // Compares only ptr values
isize Ptr_cmp(Ptr lhs, Ptr rhs);       // Three-way comparison
bool  Ptr_contentEq(Ptr lhs, Ptr rhs); // Deep comparison

// Memory manipulation with explicit safety
Result Ptr_swap(Ptr a, Ptr b, usize count);
Result Ptr_fill(Ptr dest, u8 value, usize count);
Result Ptr_move(Ptr dest, Ptr src, usize count);

// Alignment operations
usize      Ptr_alignmentOffset(Ptr p, usize alignment);
Option_Ptr Ptr_alignUp(Ptr p, usize alignment);
Option_Ptr Ptr_alignDown(Ptr p, usize alignment);

// Metadata access
usize  Ptr_size(Ptr p);
anyptr Ptr_raw(Ptr p);
bool   Ptr_hasData(Ptr p);

// PtrDiff operations
PtrDiff    PtrDiff_new(usize offset, usize size);
PtrDiff    PtrDiff_fromPtrs(Ptr end, Ptr start);
Option_Ptr PtrDiff_apply(PtrDiff diff, Ptr base);
