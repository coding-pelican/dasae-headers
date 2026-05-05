/**
 * test_meta_ptr.c - Test cases for meta_ptr.h
 * 
 * Demonstrates usage with inline TypeInfo literals (no globals).
 * 
 * Compile: gcc -O2 -Wall -Wextra -o test_meta_ptr test_meta_ptr.c
 * Assembly: gcc -O2 -S -o test_meta_ptr.s test_meta_ptr.c
 */

#include "meta_ptr.h"

/* ============================================================================
 * Test Structures
 * ============================================================================ */

struct Foo {
    u32  a;
    i64  b;
    bool c;
    f64  d;
};

struct Bar {
    u8  x;
    u64 y;
    u8  z;
};

/* ============================================================================
 * Inline TypeInfo Slice Macros (no globals needed)
 * ============================================================================ */

/* For Foo: { u32, i64, bool, f64 } */
#define FOO_FIELDS() tiSlice$( \
    typeInfo$(u32),  \
    typeInfo$(i64),  \
    typeInfo$(bool), \
    typeInfo$(f64)   \
)

/* For Bar: { u8, u64, u8 } */
#define BAR_FIELDS() tiSlice$( \
    typeInfo$(u8),  \
    typeInfo$(u64), \
    typeInfo$(u8)   \
)

/* Packed TypeInfo for compile-time macros */
#define TIP_u8   TI_PACKED$(u8)
#define TIP_u32  TI_PACKED$(u32)
#define TIP_i64  TI_PACKED$(i64)
#define TIP_u64  TI_PACKED$(u64)
#define TIP_bool TI_PACKED$(bool)
#define TIP_f64  TI_PACKED$(f64)

/* ============================================================================
 * Compile-Time Verification
 * ============================================================================ */

_Static_assert(sizeof(struct Foo) == 32, "Foo size");
_Static_assert(sizeof(struct Bar) == 24, "Bar size");

_Static_assert(CT_OFFSET_0$(TIP_u32) == __builtin_offsetof(struct Foo, a), "Foo.a");
_Static_assert(CT_OFFSET_1$(TIP_u32, TIP_i64) == __builtin_offsetof(struct Foo, b), "Foo.b");
_Static_assert(CT_OFFSET_2$(TIP_u32, TIP_i64, TIP_bool) == __builtin_offsetof(struct Foo, c), "Foo.c");
_Static_assert(CT_OFFSET_3$(TIP_u32, TIP_i64, TIP_bool, TIP_f64) == __builtin_offsetof(struct Foo, d), "Foo.d");

/* ============================================================================
 * Test 1: metaPtr_fromBytes with inline literals
 * ============================================================================ */

static int test_fromBytes(void) {
    u8 buffer[sizeof(struct Foo)];
    const S$u8 bytes = sliceFrom$(buffer, sizeof(buffer));
    
    /* Create meta pointer using inline literal (no global) */
    const u_P$T foo_mp = metaPtr_fromBytes(bytes, FOO_FIELDS());
    
    /* Verify TypeInfo */
    if (foo_mp.ti.size != sizeof(struct Foo)) return 0;
    if (alignVal$(foo_mp.ti.align) != _Alignof(struct Foo)) return 0;
    if ((u8*)foo_mp.ptr != buffer) return 0;
    
    return 1;
}

/* ============================================================================
 * Test 2: metaPtr_fields returns S$u_P$T
 * ============================================================================ */

static int test_fields(void) {
    u8 buffer[sizeof(struct Foo)];
    struct Foo* const foo = (struct Foo*)buffer;
    
    foo->a = 0xDEADBEEF;
    foo->b = -12345678901234LL;
    foo->c = true;
    foo->d = 2.71828;
    
    const S$u8 bytes = sliceFrom$(buffer, sizeof(buffer));
    const u_P$T foo_mp = metaPtr_fromBytes(bytes, FOO_FIELDS());
    
    /* Output array */
    u_P$T field_arr[4];
    
    /* Get all fields - returns slice */
    const S$u_P$T field_slice = metaPtr_fields(
        foo_mp, 
        FOO_FIELDS(), 
        metaPtrSlice$(field_arr, 4)
    );
    
    /* Verify returned slice */
    if (field_slice.len != 4) return 0;
    if (field_slice.ptr != field_arr) return 0;
    
    /* Verify field values through returned slice */
    if (metaPtr_read$(u32, field_slice.ptr[0]) != 0xDEADBEEF) return 0;
    if (metaPtr_read$(i64, field_slice.ptr[1]) != -12345678901234LL) return 0;
    if (metaPtr_read$(bool, field_slice.ptr[2]) != true) return 0;
    
    const f64 d_val = metaPtr_read$(f64, field_slice.ptr[3]);
    if (d_val < 2.71 || d_val > 2.72) return 0;
    
    return 1;
}

/* ============================================================================
 * Test 3: metaPtr_field with inline literals
 * ============================================================================ */

static int test_field(void) {
    u8 buffer[sizeof(struct Bar)];
    struct Bar* const bar = (struct Bar*)buffer;
    
    bar->x = 0xAA;
    bar->y = 0x123456789ABCDEF0ULL;
    bar->z = 0xBB;
    
    const S$u8 bytes = sliceFrom$(buffer, sizeof(buffer));
    const u_P$T bar_mp = metaPtr_fromBytes(bytes, BAR_FIELDS());
    
    /* Get each field using inline literal */
    const u_P$T x_mp = metaPtr_field(bar_mp, BAR_FIELDS(), 0);
    const u_P$T y_mp = metaPtr_field(bar_mp, BAR_FIELDS(), 1);
    const u_P$T z_mp = metaPtr_field(bar_mp, BAR_FIELDS(), 2);
    
    /* Verify */
    if (metaPtr_read$(u8, x_mp) != 0xAA) return 0;
    if (metaPtr_read$(u64, y_mp) != 0x123456789ABCDEF0ULL) return 0;
    if (metaPtr_read$(u8, z_mp) != 0xBB) return 0;
    
    return 1;
}

/* ============================================================================
 * Test 4: metaPtr_parent with inline literals
 * ============================================================================ */

static int test_parent(void) {
    u8 buffer[sizeof(struct Foo)];
    struct Foo* const foo = (struct Foo*)buffer;
    
    foo->a = 111;
    foo->b = 222;
    foo->c = false;
    foo->d = 333.0;
    
    const S$u8 bytes = sliceFrom$(buffer, sizeof(buffer));
    const u_P$T foo_mp = metaPtr_fromBytes(bytes, FOO_FIELDS());
    
    /* Get field b */
    const u_P$T b_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 1);
    
    /* Recover parent from field b */
    const u_P$T parent_mp = metaPtr_parent(b_mp, FOO_FIELDS(), 1);
    
    /* Verify parent matches original */
    if ((u8*)parent_mp.ptr != (u8*)foo_mp.ptr) return 0;
    if (parent_mp.ti.size != foo_mp.ti.size) return 0;
    
    /* Test from other fields */
    const u_P$T d_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 3);
    const u_P$T parent_from_d = metaPtr_parent(d_mp, FOO_FIELDS(), 3);
    if ((u8*)parent_from_d.ptr != buffer) return 0;
    
    return 1;
}

/* ============================================================================
 * Test: Write through meta pointers
 * ============================================================================ */

static int test_write(void) {
    u8 buffer[sizeof(struct Foo)] = {0};
    const S$u8 bytes = sliceFrom$(buffer, sizeof(buffer));
    
    const u_P$T foo_mp = metaPtr_fromBytes(bytes, FOO_FIELDS());
    
    /* Get fields */
    const u_P$T a_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 0);
    const u_P$T b_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 1);
    const u_P$T c_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 2);
    const u_P$T d_mp = metaPtr_field(foo_mp, FOO_FIELDS(), 3);
    
    /* Write values */
    metaPtr_write$(a_mp, (u32)0xCAFEBABE);
    metaPtr_write$(b_mp, (i64)-9999999999LL);
    metaPtr_write$(c_mp, (bool)true);
    metaPtr_write$(d_mp, (f64)1.41421356);
    
    /* Verify via struct cast */
    const struct Foo* const foo = (const struct Foo*)buffer;
    if (foo->a != 0xCAFEBABE) return 0;
    if (foo->b != -9999999999LL) return 0;
    if (foo->c != true) return 0;
    if (foo->d < 1.41 || foo->d > 1.42) return 0;
    
    return 1;
}

/* ============================================================================
 * Test: Compile-time field access macros
 * ============================================================================ */

static int test_compile_time_field(void) {
    u8 buffer[sizeof(struct Foo)];
    struct Foo* const foo = (struct Foo*)buffer;
    
    foo->a = 100;
    foo->b = 200;
    foo->c = true;
    foo->d = 300.0;
    
    /* Create meta pointer */
    const u_P$T foo_mp = metaPtr$((u8*)buffer, TypeInfo_struct(FOO_FIELDS()));
    
    /* Use compile-time field macros (constant offsets) */
    const u_P$T a_mp = metaPtr_field0$(foo_mp, TIP_u32);
    const u_P$T b_mp = metaPtr_field1$(foo_mp, TIP_u32, TIP_i64);
    const u_P$T c_mp = metaPtr_field2$(foo_mp, TIP_u32, TIP_i64, TIP_bool);
    const u_P$T d_mp = metaPtr_field3$(foo_mp, TIP_u32, TIP_i64, TIP_bool, TIP_f64);
    
    /* Verify */
    if (metaPtr_read$(u32, a_mp) != 100) return 0;
    if (metaPtr_read$(i64, b_mp) != 200) return 0;
    if (metaPtr_read$(bool, c_mp) != true) return 0;
    
    const f64 d_val = metaPtr_read$(f64, d_mp);
    if (d_val < 299.0 || d_val > 301.0) return 0;
    
    return 1;
}

/* ============================================================================
 * Test: Offset-only operations (null-safe)
 * ============================================================================ */

static int test_offset_only(void) {
    /* These operations don't touch any pointers, inherently safe */
    const usize off_a = metaPtr_offsetOf(FOO_FIELDS(), 0);
    const usize off_b = metaPtr_offsetOf(FOO_FIELDS(), 1);
    const usize off_c = metaPtr_offsetOf(FOO_FIELDS(), 2);
    const usize off_d = metaPtr_offsetOf(FOO_FIELDS(), 3);
    
    if (off_a != 0) return 0;
    if (off_b != 8) return 0;
    if (off_c != 16) return 0;
    if (off_d != 24) return 0;
    
    const usize foo_size = metaPtr_sizeOf(FOO_FIELDS());
    if (foo_size != 32) return 0;
    
    return 1;
}

/* ============================================================================
 * Constant Folding Verification
 * 
 * These should compile to simple constant returns or minimal pointer arithmetic
 * ============================================================================ */

/* Should fold completely: return 8 */
usize get_offset_b_inline(void) {
    return metaPtr_offsetOf(FOO_FIELDS(), 1);
}

/* Should fold: return 32 */
usize get_struct_size_inline(void) {
    return metaPtr_sizeOf(FOO_FIELDS());
}

/* Should compile to: lea 8(%rdi), %rax with constant TypeInfo */
u_P$T get_field_b_ct(const u_P$T parent) {
    return metaPtr_field1$(parent, TIP_u32, TIP_i64);
}

/* Should compile to: lea 24(%rdi), %rax with constant TypeInfo */  
u_P$T get_field_d_ct(const u_P$T parent) {
    return metaPtr_field3$(parent, TIP_u32, TIP_i64, TIP_bool, TIP_f64);
}

/* ============================================================================
 * Main
 * ============================================================================ */

static void write_str(const char* const s, const usize len) {
    __asm__ volatile (
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "mov %0, %%rsi\n"
        "mov %1, %%rdx\n"
        "syscall\n"
        :
        : "r"(s), "r"(len)
        : "rax", "rdi", "rsi", "rdx", "rcx", "r11", "memory"
    );
}

static void print_result(const char* const name, const int pass) {
    usize len = 0;
    while (name[len]) len++;
    write_str(name, len);
    write_str(pass ? " PASS\n" : " FAIL\n", 6);
}

int main(void) {
    int all_pass = 1;
    int r;
    
    r = test_fromBytes();
    print_result("test_fromBytes:", r);
    all_pass &= r;
    
    r = test_fields();
    print_result("test_fields:", r);
    all_pass &= r;
    
    r = test_field();
    print_result("test_field:", r);
    all_pass &= r;
    
    r = test_parent();
    print_result("test_parent:", r);
    all_pass &= r;
    
    r = test_write();
    print_result("test_write:", r);
    all_pass &= r;
    
    r = test_compile_time_field();
    print_result("test_ct_field:", r);
    all_pass &= r;
    
    r = test_offset_only();
    print_result("test_offset_only:", r);
    all_pass &= r;
    
    /* Verify constant folding */
    r = (get_offset_b_inline() == 8);
    print_result("inline_offset_b=8:", r);
    all_pass &= r;
    
    r = (get_struct_size_inline() == 32);
    print_result("inline_size=32:", r);
    all_pass &= r;
    
    return all_pass ? 0 : 1;
}
