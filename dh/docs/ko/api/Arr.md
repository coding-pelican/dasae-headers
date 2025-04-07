---
title: 고정 크기 배열 (Arr)
description: 경계 검사를 갖춘 타입 안전 고정 크기 배열을 제공하는 Arr 모듈에 대한 문서
created: 2025-04-01
updated: 2025-04-07
version: v0.1.1
target_version: pre-alpha
---

# 고정 크기 배열 (Arr)

## 개요

`Arr` 모듈은 경계 검사와 편리한 연산을 갖춘 타입 안전 고정 크기 배열을 제공합니다. 표준 C 배열과 달리 `Arr` 배열은 안전성 보장과 더 표현력 있는 API를 제공합니다.

## 특징

- 컴파일 타임 크기 및 요소 타입 검사
- 디버그 모드에서 자동 경계 검사
- 풍부한 슬라이싱 연산
- 편리한 반복 패턴
- 릴리스 빌드에서 제로 오버헤드 추상화

## 타입

### 선언

배열은 여러 가지 방법으로 선언하고 구현할 수 있습니다:

```c
// 한 단계로 선언 및 구현
use_Arr$(3, i32);  // 3개 정수의 고정 크기 배열 생성

// 구현 없이 선언
decl_Arr$(4, f32);

// 이전에 선언된 타입 구현
impl_Arr$(4, f32);
```

### 타입 별칭

```c
// 배열 타입 생성
Arr$(5, f32) arr;  // [5]f32 배열 타입

// 타입정의용 (익명 배열 타입)
typedef Arr$$(4, u8) ByteQuad;  // 4바이트 배열을 위한 타입 정의
```

## 초기화

배열은 여러 가지 방법으로 초기화할 수 있습니다:

```c
// 0으로 초기화
Arr$(3, i32) arr1 = Arr_zero();  // {0, 0, 0}

// 값으로 초기화
Arr$(3, i32) arr2 = Arr_init({ 1, 2, 3 });  // {1, 2, 3}

// 익명 배열 초기화
var anon1 = Arr_zero$(Arr$$(3, i32));  // 0으로 초기화된 익명 배열
var anon2 = Arr_init$(Arr$$(3, i32), { 1, 2, 3 });  // 값으로 초기화

// 타입 변환
var typed1 = Arr_anonCast$(Arr$3$i32, anon1);
var typed2 = Arr_anonCast$(Arr$(3, i32), anon2);
```

## 기본 연산

### 배열 정보

```c
// 배열 길이 가져오기
usize len = Arr_len(arr);  // 요소 수 반환
```

### 요소 접근

```c
// 요소 포인터 가져오기 (경계 검사)
i32* ptr = Arr_at(arr, 1);

// 요소 값 가져오기 (경계 검사)
i32 val = Arr_getAt(arr, 1);

// 요소 값 설정 (경계 검사)
Arr_setAt(arr, 1, 5);  // arr = {1, 5, 3}
```

## 반복

배열은 여러 가지 방법으로 반복할 수 있습니다:

```c
// 순방향 반복
for_array (arr, item) {
    printf("%d ", *item);  // 출력: 1 2 3
}

// 인덱스를 포함한 순방향 반복
for_array_indexed (arr, item, idx) {
    printf("[%zu]=%d ", idx, *item);  // 출력: [0]=1 [1]=2 [2]=3
}

// 역방향 반복
for_array_rev (arr, item) {
    printf("%d ", *item);  // 출력: 3 2 1
}

// 인덱스를 포함한 역방향 반복
for_array_rev_indexed (arr, item, idx) {
    printf("[%zu]=%d ", idx, *item);  // 출력: [2]=3 [1]=2 [0]=1
}
```

## 슬라이싱 연산

배열은 슬라이스하여 배열의 일부에 대한 뷰를 생성할 수 있습니다:

```c
// 시작부터 끝까지 슬라이스 가져오기
Sli$i32 slice = Arr_slice(arr, (1, 2));  // 인덱스 1부터 2까지(배타적) 슬라이스

// 타입 특정 슬라이스
Sli$i32 typed_slice = Arr_slice$(Sli$i32, arr, (1, 2));

// 처음부터 인덱스까지 슬라이스 가져오기
Sli$i32 prefix = Arr_prefix(arr, 2);  // 처음 2개 요소

// 인덱스부터 끝까지 슬라이스 가져오기
Sli$i32 suffix = Arr_suffix(arr, 1);  // 마지막 2개 요소
```

## 전체 예제

```c
#include "dh/main.h"
#include "dh/Arr.h"

fn_scope(dh_main(Sli$Str_const args), Err$void) {
    // 고정 크기 배열 선언 및 초기화
    Arr$(3, i32) numbers = Arr_init({ 10, 20, 30 });

    // 길이 가져오기
    printf("배열 길이: %zu\n", Arr_len(numbers));

    // 요소 접근
    printf("두 번째 요소: %d\n", Arr_getAt(numbers, 1));

    // 요소 수정
    Arr_setAt(numbers, 1, 25);

    // 반복
    printf("배열 요소: ");
    for_array (numbers, item) {
        printf("%d ", *item);
    }
    printf("\n");

    // 슬라이스 생성
    Sli$i32 slice = Arr_slice(numbers, (0, 2));
    printf("슬라이스 요소: ");
    for_slice (slice, item) {
        printf("%d ", *item);
    }
    printf("\n");

    return_ok({});
} unscoped;
```

## API 참조

### 타입 선언

- `use_Arr$(N, T)` - 한 단계로 배열 타입 선언 및 구현
- `decl_Arr$(N, T)` - 구현 없이 배열 타입 선언
- `impl_Arr$(N, T)` - 이전에 선언된 배열 타입 구현

### 타입 별칭

- `Arr$(N, T)` - T 타입의 N개 요소를 가진 배열 타입 생성
- `Arr$$(N, T)` - T 타입의 N개 요소를 가진 익명 배열 타입 생성
- `Arr_anonCast$(T_Arr, var_anon...)` - 익명 배열을 특정 타입으로 변환

### 초기화

- `Arr_zero()` - 배열을 0으로 초기화
- `Arr_zero$(T_Arr)` - 특정 배열 타입을 0으로 초기화
- `Arr_init(_Initial...)` - 지정된 값으로 배열 초기화
- `Arr_init$(T_Arr, _Initial...)` - 특정 배열 타입을 값으로 초기화

### 연산

- `Arr_len(var_self...)` - 배열의 요소 수 가져오기
- `Arr_at(var_self, usize_index...)` - 인덱스의 요소 포인터 가져오기 (경계 검사)
- `Arr_getAt(var_self, usize_index...)` - 인덱스의 요소 값 가져오기 (경계 검사)
- `Arr_setAt(var_self, usize_index, val_item...)` - 인덱스의 값 설정 (경계 검사)

### 반복

- `for_array(var_arr, _Iter_item)` - 요소를 순방향으로 반복
- `for_array_indexed(var_arr, _Iter_item, _Iter_index)` - 인덱스를 포함한 순방향 반복
- `for_array_rev(var_arr, _Iter_item)` - 요소를 역방향으로 반복
- `for_array_rev_indexed(var_arr, _Iter_item, _Iter_index)` - 인덱스를 포함한 역방향 반복
