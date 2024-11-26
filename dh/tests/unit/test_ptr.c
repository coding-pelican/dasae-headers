/**
 * @file    test_ptr.c
 * @brief   Test suite for Ptr bit operations
 */

#include "dh/core/Ptr.h"
#include "dh/debug/assert.h"

#include <stdio.h>
#include <inttypes.h>

void TEST_PrintSection(const char* section_name) {
    printf("\n=== Testing %s ===\n", section_name);
}

void TEST_PrintResult(const char* test_name, bool success) {
    printf("%s: %s\n", test_name, success ? "PASSED" : "FAILED");
}

void TEST_Ptr_metadata_masks() {
    TEST_PrintSection("Ptr Metadata Masks");

    // Verify mask calculations
    bool masks_correct = Ptr_mask_bits_size == 0xFFFull && Ptr_mask_bits_align == 0x7ull && Ptr_mask_bits_flags == 0x1ull;

    TEST_PrintResult("Metadata masks correctly calculated", masks_correct);
}

void TEST_Ptr_bit_ranges() {
    TEST_PrintSection("Ptr Bit Ranges");

    // Test maximum values
    i32 max_size  = 4095; // 12 bits
    i32 max_align = 7;    // 3 bits
    i32 max_flags = 1;    // 1 bit

    // Create a pointer with maximum values
    i32 value = 42;
    Ptr ptr   = Ptr_fromRaw(&value, max_size);
    ptr       = Ptr_withFlags(ptr, true);

    bool ranges_correct = Ptr_size(ptr) == (usize)max_size && (Ptr_align(ptr) & Ptr_mask_bits_align) <= (1ull << max_align) && Ptr_flags(ptr) == (bool)max_flags;

    TEST_PrintResult("Metadata ranges within bounds", ranges_correct);
}

void TEST_Ptr_address_bits() {
    TEST_PrintSection("Ptr Address Bits");

    // Test address masking
    usize test_addr = 0x0000123456789000ull;
    Ptr   ptr       = Ptr_fromRaw(intToRaw(anyptr, test_addr), 42); // NOLINT(performance-no-int-to-ptr)

    // The address bits should be preserved after raw extraction
    usize masked_addr    = (usize)Ptr_raw(ptr);
    bool  addr_preserved = (masked_addr & ((1ull << Ptr_bits_addr_len) - 1)) == (test_addr & ((1ull << Ptr_bits_addr_len) - 1));

    TEST_PrintResult("Address bits preserved", addr_preserved);

    // Test high bits are properly cleared
    bool high_bits_cleared = (masked_addr & ~((1ull << Ptr_bits_addr_len) - 1)) == 0;
    TEST_PrintResult("High bits properly cleared", high_bits_cleared);
}

// void TEST_Ptr_alignment_order() {
//     TEST_PrintSection("Ptr Alignment Order");

//     struct alignas(64) AlignedStruct {
//         int x;
//     } value;

//     Ptr ptr = Ptr_fromRaw(&value, sizeof(value));

//     // For 64-byte alignment, alignment order should be 6 (2^6 = 64)
//     usize expected_order = 6;
//     usize actual_align   = Ptr_align(ptr);
//     bool  align_correct  = actual_align == 64;

//     TEST_PrintResult("Alignment order calculation", align_correct);
//     printf("Expected alignment: 64, Got: %zu\n", actual_align);
// }

void TEST_Ptr_bit_preservation() {
    TEST_PrintSection("Ptr Bit Preservation");

    // Create a pointer with known values
    i32   value = 42;
    usize size  = 32;
    Ptr   ptr   = Ptr_fromRaw(&value, size);
    ptr         = Ptr_withFlags(ptr, true);

    // Read back values
    bool values_preserved = Ptr_raw(ptr) == &value && Ptr_size(ptr) == size && Ptr_flags(ptr) == true;

    TEST_PrintResult("Metadata values preserved", values_preserved);
}

void TEST_Ptr_null_handling() {
    TEST_PrintSection("Ptr Null Handling");

    // Test null pointer creation
    Ptr null_ptr = Ptr_fromRaw(NULL, 0);

    bool null_correct = Ptr_isNull(null_ptr) && Ptr_raw(null_ptr) == NULL && Ptr_size(null_ptr) == 0;

    TEST_PrintResult("Null pointer handling", null_correct);
}

int main(void) {
    printf("Starting Ptr Bit Operations Tests\n");

    TEST_Ptr_metadata_masks();
    TEST_Ptr_bit_ranges();
    TEST_Ptr_address_bits();
    // TEST_Ptr_alignment_order();
    TEST_Ptr_bit_preservation();
    TEST_Ptr_null_handling();

    printf("\nAll tests completed.\n");
    return 0;
}
