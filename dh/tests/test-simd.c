/**
 * @file    Vec_example.c
 * @brief   Example usage of dasae-headers SIMD vector operations
 * @author  Gyeongtae Kim (dev-dasae)
 */

#include <stdio.h>
#include <math.h>
#include "dh/Vec.h"
#include "dh/prl/types/Arr.h"

/* Example 1: Basic vector arithmetic */
void example_basic_arithmetic(void) {
    printf("=== Example 1: Basic Vector Arithmetic ===\n");

    /* Create two 4-element float vectors */
    let a = Vec_splat$((Vec$(4, f32))(1.0f));
    let b = Vec_from$((f32){ 2.0f, 2.0f, 2.0f, 2.0f });
    let c = Vec_fromA(A_from$((f32){ 3.0f, 3.0f, 3.0f, 3.0f }));
    let_ignore = Vec_toA(c);

    var cnt = 0;
    printf("cnt = %d\n", cnt);
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("cnt = %d\n", cnt);
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("cnt = %d\n", cnt);
    printf("%f\n", *Vec_at((a)[cnt++]));
    printf("cnt = %d\n", cnt);

    /* Perform operations */
    let sum = Vec_add(a, b);
    let diff = Vec_sub(a, b);
    let prod = Vec_mul(a, b);
    let quot = Vec_div(a, b);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("sum  = [%.2f, %.2f, %.2f, %.2f]\n", sum[0], sum[1], sum[2], sum[3]);
    printf("diff = [%.2f, %.2f, %.2f, %.2f]\n", diff[0], diff[1], diff[2], diff[3]);
    printf("prod = [%.2f, %.2f, %.2f, %.2f]\n", prod[0], prod[1], prod[2], prod[3]);
    printf("quot = [%.2f, %.2f, %.2f, %.2f]\n\n", quot[0], quot[1], quot[2], quot[3]);
}

/* Example 2: Vector reduction operations */
void example_reductions(void) {
    printf("=== Example 2: Vector Reductions ===\n");

    /* Create vector from array */
    var data = A_from$((f32){ 1.0f, 2.0f, 3.0f, 4.0f });
    let vec = Vec_fromA(data);

    /* Reduce to scalars */
    let sum = Vec_reduceAdd(vec);
    let prod = Vec_reduceMul(vec);
    let min = Vec_reduceMin(vec);
    let max = Vec_reduceMax(vec);

    printf("vec = [%.2f, %.2f, %.2f, %.2f]\n", *Vec_at((vec)[0]), *Vec_at((vec)[1]), *Vec_at((vec)[2]), *Vec_at((vec)[3]));
    printf("sum  = %.2f\n", sum);
    printf("prod = %.2f\n", prod);
    printf("min  = %.2f\n", min);
    printf("max  = %.2f\n\n", max);
}

// /* Example 3: Vector comparisons and masking */
// void example_comparisons(void) {
//     printf("=== Example 3: Vector Comparisons ===\n");

//     let a = Vec_from(4, f32, 1.0f, 2.0f, 3.0f, 4.0f);
//     let b = Vec_from(4, f32, 2.0f, 2.0f, 2.0f, 2.0f);

//     /* Compare operations return mask vectors */
//     let lt_mask = Vec_cmpLt(a, b); /* a < b */
//     let eq_mask = Vec_cmpEq(a, b); /* a == b */
//     let gt_mask = Vec_cmpGt(a, b); /* a > b */

//     /* Convert to bitmasks */
//     usize lt_bits = Vec_toBitMask(lt_mask);
//     usize eq_bits = Vec_toBitMask(eq_mask);
//     usize gt_bits = Vec_toBitMask(gt_mask);

//     printf("a  = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
//     printf("b  = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
//     printf("a < b: bitmask = 0x%lx (%s)\n", lt_bits, Vec_anyTrue(lt_mask) ? "some true" : "none true");
//     printf("a == b: bitmask = 0x%lx (%s)\n", eq_bits, Vec_anyTrue(eq_mask) ? "some true" : "none true");
//     printf("a > b: bitmask = 0x%lx (%s)\n\n", gt_bits, Vec_anyTrue(gt_mask) ? "some true" : "none true");
// }

// /* Example 4: Mathematical functions */
// void example_math_functions(void) {
//     printf("=== Example 4: Mathematical Functions ===\n");

//     let vec = Vec_from(4, f32, 4.0f, 9.0f, 16.0f, 25.0f);

//     let sqrt_vec = Vec_sqrt(vec);
//     let floor_vec = Vec_floor(vec);
//     let ceil_vec = Vec_ceil(vec);

//     printf("vec   = [%.2f, %.2f, %.2f, %.2f]\n", vec[0], vec[1], vec[2], vec[3]);
//     printf("sqrt  = [%.2f, %.2f, %.2f, %.2f]\n", sqrt_vec[0], sqrt_vec[1], sqrt_vec[2], sqrt_vec[3]);
//     printf("floor = [%.2f, %.2f, %.2f, %.2f]\n", floor_vec[0], floor_vec[1], floor_vec[2], floor_vec[3]);
//     printf("ceil  = [%.2f, %.2f, %.2f, %.2f]\n\n", ceil_vec[0], ceil_vec[1], ceil_vec[2], ceil_vec[3]);
// }

/* Example 5: Fused Multiply-Add (FMA) */
void example_fma(void) {
    printf("=== Example 5: Fused Multiply-Add ===\n");

    let a = Vec_from$((f32){ 1.0f, 2.0f, 3.0f, 4.0f });
    let b = Vec_from$((f32){ 2.0f, 3.0f, 4.0f, 5.0f });
    let c = Vec_from$((f32){ 1.0f, 1.0f, 1.0f, 1.0f });

    /* result = (a * b) + c */
    let result = Vec_fma(a, b, c);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("c = [%.2f, %.2f, %.2f, %.2f]\n", c[0], c[1], c[2], c[3]);
    printf("fma(a, b, c) = [%.2f, %.2f, %.2f, %.2f]\n\n", result[0], result[1], result[2], result[3]);
}

/* Example 6: Shuffle operations */
void example_shuffle(void) {
    printf("=== Example 6: Shuffle Operations ===\n");

    let a = Vec_from$((f32){ 1.0f, 2.0f, 3.0f, 4.0f });
    let b = Vec_from$((f32){ 5.0f, 6.0f, 7.0f, 8.0f });

    /* Shuffle: take elements from a and b based on indices
     * Indices 0-3 come from a, 4-7 come from b
     * Result: [a[0], b[1], a[2], b[3]] = [1, 6, 3, 8]
     */
    let shuffled = Vec_shuffle(a, b, 0, 5, 2, 7);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("shuffle(a, b, 0, 5, 2, 7) = [%.2f, %.2f, %.2f, %.2f]\n\n", shuffled[0], shuffled[1], shuffled[2], shuffled[3]);
}

/* Example 7: Select (blend) operations */
void example_select(void) {
    printf("=== Example 7: Select/Blend Operations ===\n");

    let a = Vec_from$((f32){ 1.0f, 2.0f, 3.0f, 4.0f });
    let b = Vec_from$((f32){ 10.0f, 20.0f, 30.0f, 40.0f });

    /* Create a mask (true selects from b, false selects from a) */
    let mask = Vec_from$((i32){ 0, -1, 0, -1 }); /* false, true, false, true */

    /* Select: mask ? b : a */
    let selected = Vec_select(mask, a, b);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("mask = [%d, %d, %d, %d]\n", mask[0] != 0, mask[1] != 0, mask[2] != 0, mask[3] != 0);
    printf("select = [%.2f, %.2f, %.2f, %.2f]\n\n", selected[0], selected[1], selected[2], selected[3]);
}

/* Example 8: Practical use case - Dot product */
f32 vec4_dot(Vec$4$f32 a, Vec$4$f32 b) {
    let prod = Vec_mul(a, b);
    return Vec_reduceAdd(prod);
}

/* Example 9: Practical use case - Vector length */
f32 vec4_length(Vec$4$f32 v) {
    let dot = vec4_dot(v, v);
    return sqrtf(dot);
}

/* Example 10: Practical use case - Vector normalize */
Vec$4$f32 vec4_normalize(Vec$4$f32 v) {
    let len = vec4_length(v);
    let len_vec = Vec_splat$((Vec$(4, f32))(len));
    return Vec_div(v, len_vec);
}

void example_practical_vector_ops(void) {
    printf("=== Example 8-10: Practical Vector Operations ===\n");

    let v1 = Vec_from$((f32){ 1.0f, 2.0f, 3.0f, 0.0f });
    let v2 = Vec_from$((f32){ 4.0f, 5.0f, 6.0f, 0.0f });

    f32 dot = vec4_dot(v1, v2);
    f32 len = vec4_length(v1);
    let normalized = vec4_normalize(v1);

    printf("v1 = [%.2f, %.2f, %.2f, %.2f]\n", v1[0], v1[1], v1[2], v1[3]);
    printf("v2 = [%.2f, %.2f, %.2f, %.2f]\n", v2[0], v2[1], v2[2], v2[3]);
    printf("dot(v1, v2) = %.2f\n", dot);
    printf("length(v1) = %.2f\n", len);
    printf("normalize(v1) = [%.4f, %.4f, %.4f, %.4f]\n\n", normalized[0], normalized[1], normalized[2], normalized[3]);
}

/* Example 11: Min/Max and Clamp */
void example_minmax_clamp(void) {
    printf("=== Example 11: Min/Max and Clamp ===\n");

    let a = Vec_from$((f32){ 1.0f, 5.0f, 3.0f, 8.0f });
    let b = Vec_from$((f32){ 3.0f, 2.0f, 4.0f, 6.0f });

    let min_vec = Vec_min(a, b);
    let max_vec = Vec_max(a, b);

    let min_bound = Vec_splat$((Vec$(4, f32))(2.0f));
    let max_bound = Vec_splat$((Vec$(4, f32))(6.0f));
    let clamped = Vec_clamp(a, min_bound, max_bound);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("min(a, b) = [%.2f, %.2f, %.2f, %.2f]\n", min_vec[0], min_vec[1], min_vec[2], min_vec[3]);
    printf("max(a, b) = [%.2f, %.2f, %.2f, %.2f]\n", max_vec[0], max_vec[1], max_vec[2], max_vec[3]);
    printf("clamp(a, 2.0, 6.0) = [%.2f, %.2f, %.2f, %.2f]\n\n", clamped[0], clamped[1], clamped[2], clamped[3]);
}

/* Example 12: Integer vector operations */
void example_integer_vectors(void) {
    printf("=== Example 12: Integer Vector Operations ===\n");

    let a = Vec_from$((i32){ 10, 20, 30, 40 });
    let b = Vec_from$((i32){ 5, 10, 15, 20 });

    let sum = Vec_add(a, b);
    let diff = Vec_sub(a, b);
    let prod = Vec_mul(a, b);

    /* Bitwise operations */
    let and_result = Vec_and(a, b);
    let or_result = Vec_or(a, b);
    let xor_result = Vec_xor(a, b);

    printf("a = [%d, %d, %d, %d]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%d, %d, %d, %d]\n", b[0], b[1], b[2], b[3]);
    printf("sum  = [%d, %d, %d, %d]\n", sum[0], sum[1], sum[2], sum[3]);
    printf("diff = [%d, %d, %d, %d]\n", diff[0], diff[1], diff[2], diff[3]);
    printf("prod = [%d, %d, %d, %d]\n", prod[0], prod[1], prod[2], prod[3]);
    printf("and  = [%d, %d, %d, %d]\n", and_result[0], and_result[1], and_result[2], and_result[3]);
    printf("or   = [%d, %d, %d, %d]\n", or_result[0], or_result[1], or_result[2], or_result[3]);
    printf("xor  = [%d, %d, %d, %d]\n\n", xor_result[0], xor_result[1], xor_result[2], xor_result[3]);
}

/* Example 13: Horizontal operations */
void example_horizontal_ops(void) {
    printf("=== Example 13: Horizontal Operations ===\n");

    let a = Vec_from$((f32){ 1.0f, 2.0f, 3.0f, 4.0f });
    let b = Vec_from$((f32){ 5.0f, 6.0f, 7.0f, 8.0f });

    /* Horizontal add: sum adjacent pairs
     * hadd(a, b) = [a[0]+a[1], a[2]+a[3], b[0]+b[1], b[2]+b[3]]
     *            = [1+2, 3+4, 5+6, 7+8]
     *            = [3, 7, 11, 15]
     */
    let hadd_result = Vec_hAdd(a, b);

    printf("a = [%.2f, %.2f, %.2f, %.2f]\n", a[0], a[1], a[2], a[3]);
    printf("b = [%.2f, %.2f, %.2f, %.2f]\n", b[0], b[1], b[2], b[3]);
    printf("hadd(a, b) = [%.2f, %.2f, %.2f, %.2f]\n\n", hadd_result[0], hadd_result[1], hadd_result[2], hadd_result[3]);
}

/* Example 14: Type conversion */
void example_type_conversion(void) {
    printf("=== Example 14: Type Conversion ===\n");

    let int_vec = Vec_from$((i32){ 1, 2, 3, 4 });
    let float_vec = Vec_cast$(Vec$4$f32, int_vec);

    printf("int_vec = [%d, %d, %d, %d]\n", int_vec[0], int_vec[1], int_vec[2], int_vec[3]);
    printf("float_vec (casted) = [%.2f, %.2f, %.2f, %.2f]\n\n", float_vec[0], float_vec[1], float_vec[2], float_vec[3]);
}

/* Example 15: Performance comparison - Array vs SIMD */
void example_performance_comparison(void) {
    printf("=== Example 15: Performance Comparison (Conceptual) ===\n");
    printf("Regular array processing:\n");
    printf("  for (i = 0; i < n; i++) result[i] = a[i] + b[i];\n");
    printf("  -> Processes 1 element per iteration\n\n");

    printf("SIMD vector processing:\n");
    printf("  for (i = 0; i < n; i += 4) {\n");
    printf("    vec_result = Vec_add(vec_a, vec_b);\n");
    printf("  }\n");
    printf("  -> Processes 4 elements per iteration\n");
    printf("  -> Theoretical 4x speedup for aligned data\n\n");
}

int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║  dasae-headers SIMD Vector Operations Example             ║\n");
    printf("║  Zig-style vector operations in C                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    example_basic_arithmetic();
    example_reductions();
    // example_comparisons();
    // example_math_functions();
    example_fma();
    example_shuffle();
    example_select();
    example_practical_vector_ops();
    example_minmax_clamp();
    example_integer_vectors();
    example_horizontal_ops();
    example_type_conversion();
    example_performance_comparison();

    printf("All examples completed successfully!\n\n");
    return 0;
}
