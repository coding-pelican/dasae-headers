#include <stdio.h>

// Clang/GCC 벡터 타입 (4개의 float)
typedef float vec4f __attribute__((vector_size(4 * 4)));

// C-스타일 배열 (4개의 float)
typedef float arr4f[4];

// --- 함수 정의 ---

// 1. 벡터 타입: Pass-by-Value (복사)
// 'v'는 main의 'v1'의 복사본입니다.
void modify_vector(vec4f v) {
    printf("  Inside func (before): v[0] = %.1f\n", v[0]);
    v[0] = 99.0f; // 복사본을 수정
    printf("  Inside func (after):  v[0] = %.1f\n", v[0]);
}

// 2. C 배열: Pass-by-Reference (포인터 감쇠)
// 'a'는 실제로는 'float*'이며 main의 'a1'을 가리킵니다.
void modify_array(arr4f a) { // 'float a[4]'는 'float* a'와 동일
    printf("  Inside func (before): a[0] = %.1f\n", a[0]);
    a[0] = 99.0f; // 원본을 수정
    printf("  Inside func (after):  a[0] = %.1f\n", a[0]);
}

int main() {
    printf("--- 1. Vector Type (Pass-by-Value) ---\n");
    vec4f v1 = { 1.0f, 2.0f, 3.0f, 4.0f };
    printf("Main (before): v1[0] = %.1f\n", v1[0]);
    modify_vector(v1);                                          // v1의 모든 내용이 '복사'되어 전달
    printf("Main (after):  v1[0] = %.1f (원본 불변)\n", v1[0]); // 원본 변경 없음

    printf("\n--- 2. C Array Type (Pointer Decay) ---\n");
    arr4f a1 = { 1.0f, 2.0f, 3.0f, 4.0f };
    printf("Main (before): a1[0] = %.1f\n", a1[0]);
    modify_array(a1);                                             // a1의 '주소'가 전달
    printf("Main (after):  a1[0] = %.1f (원본 변경됨)\n", a1[0]); // 원본 변경됨

    // 할당 연산 비교
    vec4f v2;
    v2 = v1; // (O) 가능: 벡터 값 복사
    printf("\nVector assignment: v2[0] = %.1f\n", v2[0]);

    // arr4f a2;
    // a2 = a1; // (X) 컴파일 오류: 배열은 할당 불가능

    return 0;
}
