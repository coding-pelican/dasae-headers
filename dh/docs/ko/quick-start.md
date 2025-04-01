---
title: 빠른 시작 가이드
description: 안전한 C 프로그래밍을 위한 dasae-headers 시작하기
created: 2025-04-01
updated: 2025-04-01
version: v0.1
target_version: pre-alpha
---

# 빠른 시작 가이드

이 가이드는 더 안전하고 생산적인 C 프로그래밍을 위한 현대적인 확장 언어인 dasae-headers를 시작하는 데 도움이 됩니다.

## 목차

- [설치](#설치)
- [프로젝트 설정](#프로젝트-설정)
- [첫 번째 프로그램](#첫-번째-프로그램)
- [핵심 개념](#핵심-개념)
- [빌드 및 실행](#빌드-및-실행)
- [디버깅](#디버깅)
- [다음 단계](#다음-단계)

## 설치

### 전제 조건

- C17 호환 컴파일러 (clang 11+, gcc 8+, 또는 MSVC 19.20+)
- Git (소스 코득용)
- CMake 3.15+ (빌드 설정용)

### 소스 코득

저장소를 로컬 머신에 복제합니다:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
cd dasae-headers
```

### 설치 옵션

#### 방법 1: 직접 포함 (기존 프로젝트에 권장)

1. `dh` 디렉토리를 프로젝트 소스에 복사합니다
2. C 파일에 필요한 헤더를 포함합니다:

```c
#include "dh/main.h"   // 프로그램 진입점
#include "dh/core.h"   // 핵심 기능
// 필요에 따라 다른 헤더 추가
```

#### 방법 2: CMake 통합

<!-- 1. 저장소를 서브모듈로 추가:

```bash
git submodule add https://github.com/coding-pelican/dasae-headers.git external/dasae-headers
```

2. CMakeLists.txt 업데이트:

```cmake
add_subdirectory(external/dasae-headers)
target_link_libraries(your_target PRIVATE dasae-headers)
``` -->

## 프로젝트 설정

### 디렉토리 구조

일반적인 dasae-headers 프로젝트는 다음과 같은 구조를 따릅니다:

```
my-project/
├── src/
│   ├── main.c       # 메인 진입점
│   └── ...          # 기타 소스 파일
├── include/         # 프로젝트별 헤더
├── dh/              # dasae-headers 디렉토리
├── tests/           # 테스트 파일
└── README.md        # 프로젝트 문서
```

### 메인 진입점

표준 C와 달리 dasae-headers는 내장된 오류 처리 기능을 갖춘 구조화된 진입점을 제공합니다:

```c
#include "dh/main.h"

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // 코드 작성
    return_ok({});  // 성공 리턴
} ext_unscoped;
```

## 첫 번째 프로그램

dasae-headers를 사용한 간단한 "Hello, world!" 프로그램을 만들어 보겠습니다:

```c
#include "dh/main.h"
#include "dh/Str.h"

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // 문자열 리터럴 생성
    let hello = Str_l("Hello, dasae-headers!");

    // 콘솔에 줄바꿈과 함께 출력
    Str_println(hello);

    // 성공 리턴
    return_ok({});
} ext_unscoped;
```

### 이 예제의 주요 기능

1. `fn_ext_scope` - 리소스 정리를 위한 확장 범위 함수
2. `Sli$Str_const` - 상수 문자열의 슬라이스 (명령줄 인수)
3. `Err$void` - void 페이로드가 있는 오류 결과 타입
4. `let` - 변수 선언을 위한 타입 추론
5. `Str_l` - 문자열 리터럴 생성
6. `return_ok` - 페이로드가 있는 성공적인 반환

## 핵심 개념

### 변수 선언

dasae-headers는 변수를 선언하는 두 가지 방법을 제공합니다:

```c
// 타입 추론 (기본적으로 불변)
let count = 10;  // i32로 추론됨

// 명시적 타입 (가변)
var number = 42;  // 명시적으로 가변
```

### defer를 통한 메모리 안전성

`defer`를 사용한 자동 리소스 정리:

```c
fn_ext_scope(readFile(Str_const path), Err$Str) {
    let_(file, FILE*) = fopen(path.ptr, "r");
    if (file == null) {
        return_err(fileError("파일을 열 수 없습니다"));
    }

    // 함수가 종료될 때 실행됨
    defer_($ignore fclose(file));

    // 파일 처리...

    return_ok(fileContents);
} ext_unscoped;
```

### 오류 처리

`try_` 패턴을 사용한 명시적 오류 처리:

```c
fn_ext_scope(processData(void), Err$void) {
    // 실패할 수 있는 함수 호출 및 오류 전파
    let result = try_(getData());

    // 결과 처리...

    return_ok({});
} ext_unscoped;
```

### 옵셔널 타입

널 값을 안전하게 처리:

```c
fn_ext_scope(findUser(i32 id), Opt$User) {
    if (id <= 0) {
        return_none();  // 사용자를 찾을 수 없음
    }

    User user = getUserById(id);
    return_some(user);  // 사용자를 찾음
} ext_unscoped;

// 사용법
if_some(findUser(42), user) {
    // 사용자가 존재함, 사용자 처리
} else_none {
    // 찾을 수 없는 경우 처리
}
```

## 빌드 및 실행

<!-- ### CMake 사용

1. `CMakeLists.txt` 파일 생성:

```cmake
cmake_minimum_required(VERSION 3.15)
project(my_dasae_project C)

set(CMAKE_C_STANDARD 17)

# 소스 파일
add_executable(my_program
    src/main.c
    # 다른 소스 파일 추가
)

# 인클루드 경로
target_include_directories(my_program PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/dh
)
```

2. 프로젝트 빌드:

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

3. 프로그램 실행:

```bash
./my_program
```

### Make 또는 다른 빌드 시스템 사용

CMake가 아닌 프로젝트의 경우 다음을 확인하세요:

1. C 표준을 C17 이상으로 설정
2. 인클루드 경로에 dasae-headers 디렉토리 포함
3. 필요한 모든 소스 파일 컴파일 -->

## 디버깅

dasae-headers는 내장 디버그 기능을 포함합니다:

### 어설션

```c
debug_assert_true(condition, "오류 메시지");
debug_assert_fmt(0 < count, "유효하지 않은 개수: %d", count);
```

## 다음 단계

기본 프로젝트를 설정했으니, 다음을 탐색해 볼 수 있습니다:

- [핵심 개념](./concepts/index.md) - 심층적인 설명
- [API 참조](./api/index.md) - 모든 모듈에 대한 문서
- [예제](./examples/index.md) - 더 많은 코드 예제
- [튜토리얼](./tutorials/index.md) - 단계별 가이드

dasae-headers와 함께 즐거운 코딩 되세요!
