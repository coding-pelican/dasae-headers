#define main_no_args (1)
#include "dh/main.h"

#include "dh/core.h"
#include "dh/sli.h"


static fn_(slice_example(void), void);
fn_ext_scope(dh_main(void), Err$void) {
    slice_example();
    return_ok({});
} ext_unscoped;


decl_Sli$(u8);
decl_Sli$(i32);
fn_(slice_example(void), void) {
    // Create a slice from an array
    i32 numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    let slice     = Sli_from$(Sli$i32, numbers, 10);

    // Basic operations
    let third_ptr = Sli_at(slice, 2);    // Get pointer to the third element
    let value     = Sli_getAt(slice, 3); // Get value of the fourth element
    Sli_setAt(slice, 4, 50);             // Set the fifth element to 50
    let length = Sli_len(slice);         // Get the length (10)

    // Range-based operations
    let middle      = Sli_slice(slice, (2, 7));
    let first_half  = Sli_prefix(slice, 5); // First 5 elements
    let second_half = Sli_suffix(slice, 5); // Last 5 elements

    let all           = Sli_prefixZ(slice);
    let terminated_10 = Sli_prefixS(slice, 10);
    for (var iter = SliS_at(terminated_10, 0); deref(iter) != terminated_10.sentinel; ++iter) {
        printf("%d ", deref(iter));
    }
    printf("\n");

    // Create a null-terminated string
    u8 str[] = "Hello, world";

    // Working with null-terminated slices (SliZ)
    let str_z = SliZ_from$(SliZ$u8, str);
    let ch    = SliZ_getAt(str_z, 1); // 'e'
    // let hello_sli = SliZ_toSli$(Sli$u8, str_z); // Convert to regular slice
    printf("%s\n", str_z.ptr);

    // Working with sentinel-terminated slices (SliS)
    i32 numbers_with_sentinel[] = { 1, 2, 3, 4, 5, -1, 6, 7 }; // -1 is sentinel
    let nums_s                  = SliS_from$(SliS$i32, numbers_with_sentinel, -1);
    // let s_len                   = SliS_len(nums_s);             // 5 (up to but not including -1)
    let num                     = SliS_getAt(nums_s, 2); // 3
    // let nums_sli                = SliS_toSli$(Sli$i32, nums_s); // Convert to regular slice
    for (var iter = SliS_at(nums_s, 0); deref(iter) != nums_s.sentinel; ++iter) {
        printf("%d ", deref(iter));
    }
    printf("\n");

    // Type casting example
    // Sli$u32 bytes = Sli_castS$(Sli$u8, slice);  // View as bytes (i32 -> u8)
    // Sli$i32 ints  = Sli_castL$(Sli$i32, bytes); // Convert back to ints

    ignore third_ptr;
    ignore value;
    ignore length;
    ignore middle;
    ignore first_half;
    ignore second_half;
    ignore all;
    ignore ch;
    ignore num;
}
