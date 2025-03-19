// #include "dh/core.h"
// #include "dh/simd.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <time.h>

// // Function to process array using SIMD operations
// void process_array_simd(const i32* input, i32* const output, usize length) {
//     // Get the suggested vector length for i32 on current architecture
//     const usize vec_len = simd_suggestLen$(i32);

//     if (vec_len == 0) {
//         printf("SIMD operations not recommended for this platform/type\n");
//         return;
//     }

//     // Process elements in chunks of vector_length
//     usize i = 0;
//     for (; i + vec_len <= length; i += vec_len) {
//         // Create initial sequence for indexing: {0,1,2,3...}
//         i32 indices[vec_len];
//         for (usize j = 0; j < vec_len; j++) {
//             indices[j] = j;
//         }

//         // Extract current chunk
//         i32 chunk[vec_len];
//         for (usize j = 0; j < vec_len; j++) {
//             chunk[j] = input[i + j];
//         }

//         // Demonstrate various SIMD operations

//         // 1. Rotate elements left by 1
//         i32 rotated[vec_len];
//         simd_rotateLeft(chunk, 1);

//         // 2. Find elements equal to a specific value (e.g., 5)
//         i32   found_indices[vec_len];
//         usize count = simd_countElementsWithValue(chunk, 5);

//         // 3. Reverse the chunk
//         i32 reversed[vec_len];
//         simd_reverse(chunk);

//         // Store processed results
//         for (usize j = 0; j < vec_len; j++) {
//             output[i + j] = reversed[j];
//         }

//         // Optional: Use spin loop hint while waiting for any memory operations
//         simd_spinLoopHint();
//     }

//     // Handle remaining elements traditionally
//     for (; i < length; i++) {
//         output[i] = input[i];
//     }
// }

// // Function to process array without SIMD (for comparison)
// void process_array_traditional(const i32* input, i32* const output, usize length) {
//     for (usize i = 0; i < length; i++) {
//         output[i] = input[i];
//         // Perform the same operations as SIMD version
//         if (i > 0) {
//             // Rotate left by swapping
//             i32 temp      = output[i - 1];
//             output[i - 1] = output[i];
//             output[i]     = temp;
//         }
//     }

//     // Reverse the array
//     for (usize i = 0; i < length / 2; i++) {
//         i32 temp               = output[i];
//         output[i]              = output[length - 1 - i];
//         output[length - 1 - i] = temp;
//     }
// }

// int main() {
//     const usize array_size  = 1024;
//     i32*        input       = malloc(array_size * sizeof(i32));
//     i32*        output_simd = malloc(array_size * sizeof(i32));
//     i32*        output_trad = malloc(array_size * sizeof(i32));

//     // Initialize random input
//     srand(time(null)); // NOLINT
//     for (usize i = 0; i < array_size; i++) {
//         input[i] = rand() % 100;
//     }

//     // Process using SIMD
//     var start = clock();
//     process_array_simd(input, output_simd, array_size);
//     var simd_time = clock() - start;

//     // Process traditionally
//     start = clock();
//     process_array_traditional(input, output_trad, array_size);
//     var trad_time = clock() - start;

//     // Compare results and timing
//     printf("SIMD Processing time: %f seconds\n", ((double)simd_time) / CLOCKS_PER_SEC);
//     printf("Traditional Processing time: %f seconds\n", ((double)trad_time) / CLOCKS_PER_SEC);

//     // Verify results match
//     i32 mismatch = 0;
//     for (usize i = 0; i < array_size; i++) {
//         if (output_simd[i] != output_trad[i]) {
//             mismatch++;
//             printf("Mismatch at index %zu: SIMD=%d, Traditional=%d\n", i, output_simd[i], output_trad[i]);
//         }
//     }
//     printf("Total mismatches: %d\n", mismatch);

//     // Cleanup
//     free(input);
//     free(output_simd);
//     free(output_trad);

//     return 0;
// }
