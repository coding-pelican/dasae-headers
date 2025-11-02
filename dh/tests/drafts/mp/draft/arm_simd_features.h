// ============================================
// ARM SIMD Feature Detection & Extended Support
// NEON, FP16, SVE, SVE2 감지 및 활용
// ============================================

#ifndef ARM_SIMD_FEATURES_H
#define ARM_SIMD_FEATURES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif

// ============================================
// 1. Feature Flags
// ============================================

typedef struct {
    bool neon;           // 기본 NEON (128-bit, 4x float32)
    bool neon_fp16;      // FP16 지원 (128-bit, 8x float16)
    bool sve;            // SVE (Scalable Vector Extension)
    bool sve2;           // SVE2
    bool dotprod;        // Dot Product 지원
    bool crypto;         // 암호화 확장
    int sve_vector_bits; // SVE 벡터 길이 (0 if not supported)
} arm_simd_features_t;

static arm_simd_features_t g_arm_features = {0};
static bool g_features_detected = false;

// ============================================
// 2. 컴파일 타임 Feature Detection
// ============================================

static inline void detect_compile_time_features(arm_simd_features_t* features) {
    // NEON
    #ifdef __ARM_NEON
        features->neon = true;
    #endif
    
    // FP16
    #ifdef __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
        features->neon_fp16 = true;
    #endif
    
    // SVE
    #ifdef __ARM_FEATURE_SVE
        features->sve = true;
    #endif
    
    // SVE2
    #ifdef __ARM_FEATURE_SVE2
        features->sve2 = true;
    #endif
    
    // Dot Product
    #ifdef __ARM_FEATURE_DOTPROD
        features->dotprod = true;
    #endif
    
    // Crypto
    #if defined(__ARM_FEATURE_CRYPTO) || defined(__ARM_FEATURE_AES)
        features->crypto = true;
    #endif
    
    // SVE 벡터 길이
    #ifdef __ARM_FEATURE_SVE_BITS
        features->sve_vector_bits = __ARM_FEATURE_SVE_BITS;
    #endif
}

// ============================================
// 3. 런타임 Feature Detection (Linux/Android)
// ============================================

#if defined(__linux__) || defined(__ANDROID__)

#include <sys/auxv.h>

#ifndef HWCAP_NEON
#define HWCAP_NEON (1 << 12)
#endif

#ifndef HWCAP_FP
#define HWCAP_FP (1 << 0)
#endif

#ifndef HWCAP_ASIMD
#define HWCAP_ASIMD (1 << 1)
#endif

#ifndef HWCAP2_SVE
#define HWCAP2_SVE (1 << 1)
#endif

#ifndef HWCAP2_SVE2
#define HWCAP2_SVE2 (1 << 2)
#endif

static inline void detect_runtime_features(arm_simd_features_t* features) {
    // getauxval을 사용한 감지
    unsigned long hwcaps = getauxval(AT_HWCAP);
    unsigned long hwcaps2 = getauxval(AT_HWCAP2);
    
    // NEON (ASIMD)
    if (hwcaps & HWCAP_ASIMD) {
        features->neon = true;
    }
    
    // SVE
    if (hwcaps2 & HWCAP2_SVE) {
        features->sve = true;
    }
    
    // SVE2
    if (hwcaps2 & HWCAP2_SVE2) {
        features->sve2 = true;
    }
    
    // /proc/cpuinfo 파싱 (보조)
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (fp) {
        char line[1024];
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "Features") || strstr(line, "features")) {
                if (strstr(line, "neon") || strstr(line, "asimd")) {
                    features->neon = true;
                }
                if (strstr(line, "fp16") || strstr(line, "fphp")) {
                    features->neon_fp16 = true;
                }
                if (strstr(line, "sve")) {
                    features->sve = true;
                }
                if (strstr(line, "sve2")) {
                    features->sve2 = true;
                }
                if (strstr(line, "dotprod") || strstr(line, "asimddp")) {
                    features->dotprod = true;
                }
            }
        }
        fclose(fp);
    }
}

#else
// 다른 OS용 fallback
static inline void detect_runtime_features(arm_simd_features_t* features) {
    // 컴파일 타임 정보만 사용
}
#endif

// ============================================
// 4. 초기화
// ============================================

static inline const arm_simd_features_t* arm_detect_features(void) {
    if (!g_features_detected) {
        memset(&g_arm_features, 0, sizeof(g_arm_features));
        
        // 컴파일 타임 감지
        detect_compile_time_features(&g_arm_features);
        
        // 런타임 감지
        detect_runtime_features(&g_arm_features);
        
        g_features_detected = true;
    }
    
    return &g_arm_features;
}

// ============================================
// 5. Feature 출력
// ============================================

static inline void arm_print_features(void) {
    const arm_simd_features_t* f = arm_detect_features();
    
    printf("ARM SIMD Features:\n");
    printf("  NEON (128-bit):     %s\n", f->neon ? "✓" : "✗");
    printf("  NEON FP16:          %s\n", f->neon_fp16 ? "✓" : "✗");
    printf("  SVE:                %s", f->sve ? "✓" : "✗");
    if (f->sve && f->sve_vector_bits > 0) {
        printf(" (%d bits)\n", f->sve_vector_bits);
    } else {
        printf("\n");
    }
    printf("  SVE2:               %s\n", f->sve2 ? "✓" : "✗");
    printf("  Dot Product:        %s\n", f->dotprod ? "✓" : "✗");
    printf("  Crypto:             %s\n", f->crypto ? "✓" : "✗");
}

// ============================================
// 6. 확장 SIMD 연산
// ============================================

// FP16 벡터 덧셈 (8 x float16)
#ifdef __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
static inline void arm_fp16_add(const __fp16* a, const __fp16* b, 
                                __fp16* c, int n) {
    int i = 0;
    
    // 8개씩 처리
    for (; i + 8 <= n; i += 8) {
        float16x8_t va = vld1q_f16(&a[i]);
        float16x8_t vb = vld1q_f16(&b[i]);
        float16x8_t vc = vaddq_f16(va, vb);
        vst1q_f16(&c[i], vc);
    }
    
    // 나머지
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}
#endif

// Dot Product 활용 (4개씩 내적)
#ifdef __ARM_FEATURE_DOTPROD
static inline float arm_dot_product_accel(const float* a, const float* b, int n) {
    float32x4_t sum = vdupq_n_f32(0.0f);
    int i = 0;
    
    // 4개씩 FMA
    for (; i + 4 <= n; i += 4) {
        float32x4_t va = vld1q_f32(&a[i]);
        float32x4_t vb = vld1q_f32(&b[i]);
        sum = vmlaq_f32(sum, va, vb);  // sum += a * b
    }
    
    // 수평 합계
    float result = vaddvq_f32(sum);
    
    // 나머지
    for (; i < n; i++) {
        result += a[i] * b[i];
    }
    
    return result;
}
#endif

// ============================================
// 7. 적응형 벡터화 (폭 자동 선택)
// ============================================

typedef enum {
    SIMD_WIDTH_NONE = 0,
    SIMD_WIDTH_128 = 4,   // 4x float32
    SIMD_WIDTH_256 = 8,   // 8x float16 (FP16) or 8x float32 (future)
    SIMD_WIDTH_512 = 16,  // SVE (if available)
    SIMD_WIDTH_1024 = 32, // SVE
    SIMD_WIDTH_2048 = 64  // SVE
} simd_width_t;

static inline simd_width_t arm_get_optimal_width(void) {
    const arm_simd_features_t* f = arm_detect_features();
    
    // SVE가 있으면 가장 넓은 폭 사용
    if (f->sve && f->sve_vector_bits >= 512) {
        return f->sve_vector_bits / 32;  // float32 기준
    }
    
    // FP16 있으면 8개 (하지만 float32 연산에는 4개)
    if (f->neon_fp16) {
        return SIMD_WIDTH_128;  // float32는 여전히 4개
    }
    
    // 기본 NEON
    if (f->neon) {
        return SIMD_WIDTH_128;
    }
    
    return SIMD_WIDTH_NONE;
}

// ============================================
// 8. 최적화된 벡터 연산 (자동 선택)
// ============================================

static inline void arm_vector_add_auto(const float* a, const float* b, 
                                       float* c, int n) {
    #ifdef __ARM_NEON
    const arm_simd_features_t* f = arm_detect_features();
    
    if (f->neon) {
        // NEON 사용 (4 floats)
        int i = 0;
        for (; i + 4 <= n; i += 4) {
            float32x4_t va = vld1q_f32(&a[i]);
            float32x4_t vb = vld1q_f32(&b[i]);
            float32x4_t vc = vaddq_f32(va, vb);
            vst1q_f32(&c[i], vc);
        }
        
        // 나머지
        for (; i < n; i++) {
            c[i] = a[i] + b[i];
        }
        return;
    }
    #endif
    
    // 스칼라 fallback
    for (int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

// ============================================
// 9. 언롤링을 통한 추가 성능 (2x 처리)
// ============================================

// NEON으로 8개 float 동시 처리 (2개 레지스터 사용)
#ifdef __ARM_NEON
static inline void arm_vector_add_unrolled(const float* a, const float* b,
                                           float* c, int n) {
    int i = 0;
    
    // 8개씩 처리 (2x unroll)
    for (; i + 8 <= n; i += 8) {
        float32x4_t va1 = vld1q_f32(&a[i]);
        float32x4_t va2 = vld1q_f32(&a[i+4]);
        float32x4_t vb1 = vld1q_f32(&b[i]);
        float32x4_t vb2 = vld1q_f32(&b[i+4]);
        
        float32x4_t vc1 = vaddq_f32(va1, vb1);
        float32x4_t vc2 = vaddq_f32(va2, vb2);
        
        vst1q_f32(&c[i], vc1);
        vst1q_f32(&c[i+4], vc2);
    }
    
    // 4개 처리
    if (i + 4 <= n) {
        float32x4_t va = vld1q_f32(&a[i]);
        float32x4_t vb = vld1q_f32(&b[i]);
        float32x4_t vc = vaddq_f32(va, vb);
        vst1q_f32(&c[i], vc);
        i += 4;
    }
    
    // 나머지
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

// 4x 언롤링 (16개 동시 처리)
static inline void arm_vector_add_unrolled4x(const float* a, const float* b,
                                             float* c, int n) {
    int i = 0;
    
    // 16개씩 처리 (4x unroll)
    for (; i + 16 <= n; i += 16) {
        float32x4_t va1 = vld1q_f32(&a[i]);
        float32x4_t va2 = vld1q_f32(&a[i+4]);
        float32x4_t va3 = vld1q_f32(&a[i+8]);
        float32x4_t va4 = vld1q_f32(&a[i+12]);
        
        float32x4_t vb1 = vld1q_f32(&b[i]);
        float32x4_t vb2 = vld1q_f32(&b[i+4]);
        float32x4_t vb3 = vld1q_f32(&b[i+8]);
        float32x4_t vb4 = vld1q_f32(&b[i+12]);
        
        float32x4_t vc1 = vaddq_f32(va1, vb1);
        float32x4_t vc2 = vaddq_f32(va2, vb2);
        float32x4_t vc3 = vaddq_f32(va3, vb3);
        float32x4_t vc4 = vaddq_f32(va4, vb4);
        
        vst1q_f32(&c[i], vc1);
        vst1q_f32(&c[i+4], vc2);
        vst1q_f32(&c[i+8], vc3);
        vst1q_f32(&c[i+12], vc4);
    }
    
    // 4개씩 처리
    for (; i + 4 <= n; i += 4) {
        float32x4_t va = vld1q_f32(&a[i]);
        float32x4_t vb = vld1q_f32(&b[i]);
        float32x4_t vc = vaddq_f32(va, vb);
        vst1q_f32(&c[i], vc);
    }
    
    // 나머지
    for (; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}
#endif

// ============================================
// 10. 정보 조회 함수
// ============================================

static inline bool arm_has_neon(void) {
    return arm_detect_features()->neon;
}

static inline bool arm_has_fp16(void) {
    return arm_detect_features()->neon_fp16;
}

static inline bool arm_has_sve(void) {
    return arm_detect_features()->sve;
}

static inline bool arm_has_dotprod(void) {
    return arm_detect_features()->dotprod;
}

static inline int arm_get_sve_bits(void) {
    return arm_detect_features()->sve_vector_bits;
}

#endif // ARM_SIMD_FEATURES_H
