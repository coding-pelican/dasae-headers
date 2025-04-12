---
title: 동적 배열 리스트 (ArrList)
description: 동적으로 크기가 조정되는 배열을 제공하는 ArrList 모듈에 대한 문서
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# 동적 배열 리스트 (ArrList)

## 개요

`ArrList` 모듈은 요소가 추가되거나 제거됨에 따라 자동으로 크기가 조정되는 동적 배열 구현을 제공합니다. Zig의 `ArrayList`(C++의 `std::vector` 또는 Rust의 `Vec<T>`와 유사함)에 기반하며, 컬렉션의 요소를 관리하기 위한 메모리 안전성과 편리한 연산을 제공합니다.

## 특징

- 자동 메모리 관리 및 크기 조정
- 타입 안전 연산
- 경계 검사
- 효율적인 추가/앞쪽 추가 연산
- 임의 위치에서의 삽입 및 제거
- 메모리 용량 관리

## 타입

### 기본 구조

```c
typedef struct ArrList {
    meta_Sli      items;     // 요소를 포함하는 슬라이스
    usize         cap;       // 현재 용량
    mem_Allocator allocator; // 사용할 메모리 할당자
} ArrList;
```

### 타입화된 변형

```c
typedef union ArrList$T {
    ArrList base[1];
    struct {
        TypeInfo      type;      // 타입 정보
        Sli$T         items;     // 요소의 타입화된 슬라이스
        usize         cap;       // 현재 용량
        mem_Allocator allocator; // 메모리 할당자
    };
} ArrList$T;
```

### 선언

```c
// 한 단계로 선언 및 구현
use_ArrList$(i32);  // 정수를 위한 타입화된 배열 리스트 생성

// 선언과 구현 분리
decl_ArrList$(f32);
impl_ArrList$(f32);

// 타입 별칭
ArrList$i32 myList;  // 정수를 위한 타입화된 배열 리스트
```

## 초기화

```c
// 빈 리스트 초기화
ArrList list = ArrList_init(typeInfo$(i32), heap_allocator);

// 타입화된 초기화
ArrList$i32 typed_list = type$(ArrList$i32,
    ArrList_init(typeInfo$(i32), heap_allocator));

// 용량으로 초기화
Err$ArrList result = ArrList_initCap(typeInfo$(f32), heap_allocator, 10);
if_ok(result, list) {
    // 리스트 사용
}
```

## 기본 연산

### 요소 추가

```c
// 끝에 추가
try_(ArrList_append(list.base, &value));

// 끝에 여러 요소 추가
try_(ArrList_appendSli(list.base, slice));

// 앞에 추가
try_(ArrList_prepend(list.base, &value));

// 추가하고 새 요소에 대한 포인터 얻기
meta_Ptr item = try_(ArrList_addBackOne(list.base));
```

### 요소 제거

```c
// 끝에서 제거
ArrList_pop(list.base);

// 끝에서 요소를 제거하고 가져오기
Opt$meta_Ptr opt_item = ArrList_popOrNull(list.base);

// 앞에서 제거
ArrList_shift(list.base);

// 특정 인덱스에서 제거 (순서 유지)
meta_Ptr removed = ArrList_removeOrdered(list.base, 2);

// 특정 인덱스에서 제거 (빠름, 순서 유지하지 않음)
meta_Ptr removed = ArrList_removeSwap(list.base, 2);
```

### 용량 관리

```c
// 전체 용량 확보
try_(ArrList_ensureTotalCap(list.base, 20));

// 리스트 크기 조정
try_(ArrList_resize(list.base, 10));

// 축소하고 초과 메모리 해제
ArrList_shrinkAndFree(list.base, 5);

// 용량은 유지하면서 지우기
ArrList_clearRetainingCap(list.base);

// 모든 메모리를 지우고 해제
ArrList_clearAndFree(list.base);
```

## 리소스 관리

사용이 끝나면 리소스를 해제하는 것을 잊지 마세요:

```c
// 종료하고 리소스 해제
ArrList_fini(list.base);
```

## 예제

### 기본 사용법

```c
#include "dh/main.h"
#include "dh/ArrList.h"

fn_scope_ext(dh_main(Sli$Str_const args), Err$void) {
    // 배열 리스트 초기화
    ArrList$i32 list = type$(ArrList$i32,
        ArrList_init(typeInfo$(i32), heap_allocator));

    // 함수 종료 시 리소스 정리
    defer_(ArrList_fini(list.base));

    // 요소 추가
    i32 values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        try_(ArrList_append(list.base, &values[i]));
    }

    // 요소 출력
    printf("리스트 내용: ");
    for_slice (list.items, item) {
        printf("%d ", *item);
    }
    printf("\n");

    // 요소 제거
    ArrList_removeOrdered(list.base, 2);

    // 업데이트된 리스트 출력
    printf("제거 후: ");
    for_slice (list.items, item) {
        printf("%d ", *item);
    }
    printf("\n");

    return_ok({});
} unscoped_ext;
```

### 동적 컬렉션

```c
fn_scope_ext(processDynamicData(Sli$i32 input), Err$Sli$i32) {
    // 할당자로 초기화
    ArrList$i32 results = type$(ArrList$i32,
        ArrList_init(typeInfo$(i32), heap_allocator));

    // 함수 종료 시 리소스 정리
    defer_(ArrList_fini(results.base));

    // 입력 데이터 처리
    for_slice (input, item) {
        if (*item % 2 == 0) {
            // 짝수 처리
            i32 processed = *item * 2;
            try_(ArrList_append(results.base, &processed));
        }
    }

    // 소유권 있는 슬라이스로 변환하고 반환 (호출자가 소유권 가짐)
    meta_Sli owned = try_(ArrList_toOwnedSli(results.base));
    return_ok(meta_cast$(Sli$i32, owned));
} unscoped_ext;
```

## API 참조

### 타입 관리

- `use_ArrList$(T)` - 타입화된 배열 리스트 선언 및 구현
- `decl_ArrList$(T)` - 타입화된 배열 리스트 선언
- `impl_ArrList$(T)` - 타입화된 배열 리스트 구현
- `ArrList$(T)` - 배열 리스트 타입 생성
- `ArrList$$(T)` - 익명 배열 리스트 생성
- `ArrList_anonCast$(T_ArrList, var_anon...)` - 익명 배열 리스트를 특정 타입으로 변환

### 초기화/종료

- `ArrList_init(TypeInfo type, mem_Allocator allocator)` - 배열 리스트 초기화
- `ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap)` - 용량으로 초기화
- `ArrList_fini(ArrList* self)` - 배열 리스트 종료

### 변환

- `ArrList_toOwnedSli(ArrList* self)` - 소유권 있는 슬라이스로 변환
- `ArrList_fromOwnedSli(mem_Allocator allocator, meta_Sli slice)` - 소유권 있는 슬라이스에서 생성
- `ArrList_clone(const ArrList* self)` - 배열 리스트 복제

### 용량 관리

- `ArrList_ensureTotalCap(ArrList* self, usize new_cap)` - 전체 용량 확보
- `ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap)` - 정확한 용량 확보
- `ArrList_ensureUnusedCap(ArrList* self, usize additional)` - 사용되지 않은 용량 확보

### 크기 연산

- `ArrList_resize(ArrList* self, usize new_len)` - 새 길이로 크기 조정
- `ArrList_shrinkAndFree(ArrList* self, usize new_len)` - 축소하고 초과 메모리 해제
- `ArrList_shrinkRetainingCap(ArrList* self, usize new_len)` - 해제하지 않고 축소
- `ArrList_expandToCap(ArrList* self)` - 전체 용량으로 확장

### 요소 추가

- `ArrList_append(ArrList* self, meta_Ptr item)` - 끝에 추가
- `ArrList_appendSli(ArrList* self, meta_Sli items)` - 끝에 슬라이스 추가
- `ArrList_prepend(ArrList* self, meta_Ptr item)` - 앞에 추가
- `ArrList_prependSli(ArrList* self, meta_Sli items)` - 앞에 슬라이스 추가
- `ArrList_addBackOne(ArrList* self)` - 끝에 하나 추가하고 포인터 가져오기
- `ArrList_addFrontOne(ArrList* self)` - 앞에 하나 추가하고 포인터 가져오기

### 요소 제거

- `ArrList_pop(ArrList* self)` - 끝에서 제거
- `ArrList_popOrNull(ArrList* self)` - 끝에서 제거하고 반환
- `ArrList_shift(ArrList* self)` - 앞에서 제거
- `ArrList_shiftOrNull(ArrList* self)` - 앞에서 제거하고 반환
- `ArrList_removeOrdered(ArrList* self, usize index)` - 순서를 유지하며 인덱스에서 제거
- `ArrList_removeSwap(ArrList* self, usize index)` - 교체를 통해 인덱스에서 제거

### 지우기

- `ArrList_clearRetainingCap(ArrList* self)` - 용량은 유지하면서 지우기
- `ArrList_clearAndFree(ArrList* self)` - 모든 메모리를 지우고 해제
