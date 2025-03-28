# dasae-headers

<div align="center">
  <h1>dasae-headers: C언어를 현대적이고 안전하게</h1>
  <p><strong>C언어를 보다 안전하고 생산적으로 사용하기 위한 현대적인 확장 언어</strong></p>

  <div>
    <a href="./README.md"><img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English"></a>
    <a href="./README.ko.md"><img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean"></a>
  </div>

  <div style="margin-top: 8px;">
    <img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17">
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square" alt="Platform: Windows | Linux | macOS">
    <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT">
  </div>
</div>

## 목차

- [dasae-headers](#dasae-headers)
  - [목차](#목차)
  - [🌟 소개](#-소개)
  - [🛡️ 주요 특징](#️-주요-특징)
  - [🌐 플랫폼 지원](#-플랫폼-지원)
  - [🚀 시작하기](#-시작하기)
    - [설치](#설치)
  - [⚡ 코드 샘플](#-코드-샘플)
    - [기본 예제](#기본-예제)
    - [오류 처리 예제](#오류-처리-예제)
    - [🧪 테스트 코드 예제](#-테스트-코드-예제)
  - [📚 문서](#-문서)
  - [🚧 현재 상태](#-현재-상태)
  - [📜 라이센스](#-라이센스)

## 🌟 소개

dasae-headers는 C언어에 현대적인 프로그래밍 패러다임을 적용하여 안전성, 표현력 및 생산성을 크게 향상시키는 것을 목표로 합니다. 메모리 안전성, 타입 안전성, 향상된 오류 처리 기능을 제공하면서도 C언어의 간결함을 유지합니다.

현재 개발 초기 단계인 dasae-headers는 C의 전처리기를 트랜스파일러로서 사용하며, C 표준 라이브러리의 고질적인 문제들을 해결하는 동시에 점진적으로 독립적인 언어로 발전시키는 과정에 있습니다.

## 🛡️ 주요 특징

dasae-headers는 Zig와 Rust의 문법, 표준 라이브러리에서 영감을 받아 개발되었습니다:

**안전성 강화**
- `defer` 문을 통한 메모리 관리 자동화
- `Optional` 타입을 통한 null 안전성
- 컴파일 타임 검사를 통한 향상된 타입 안전성
- 메모리 누수 및 관련 버그 감지를 위한 내장 메모리 추적 기능

**현대적인 프로그래밍 구조**
- `let`과 `var` 키워드를 통한 타입 추론
- 안전한 메모리 조작을 위한 경계 검사된 슬라이스
- `try_`, `catch_` 패턴이 있는 명시적 오류 처리를 위한 결과(Result) 타입
- 패턴 매칭이 가능한 대수적 데이터 타입(union enum)
- 검사된 접근 메서드를 통한 배열 안전성

**개발 생산성**
- 코드 명확성 향상을 위한 표현력 있는 매크로
- `fn_` 선언을 사용한 현대적인 함수 구문
- 다양한 아키텍처에 대한 크로스 플랫폼 지원
- 주요 컴파일러 호환성(clang, gcc, msvc)
- 일관된 코드 스타일 및 명명 규칙

## 🌐 플랫폼 지원

- **운영체제**: Windows, Unix, Linux, macOS, Android 등
- **CPU 아키텍처**: 64/32비트 아키텍처 고려
- **컴파일러 호환성**: clang(우선), gcc, msvc 등

## 🚀 시작하기

### 설치

1. 이 저장소를 클론합니다:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
```

2. 필요한 헤더를 포함시킵니다:

```c
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
// 필요한 기능에 따라 추가 헤더 포함
```

## ⚡ 코드 샘플

### 기본 예제

```c
#include "dh/main.h"
#include "dh/core.h"
#include "dh/opt.h"

// 옵셔널 값을 반환하는 함수
fn_ext_scope(findValue(i32 id), Opt$i32) {
    if (id > 0) {
        return_some(id * 10);
    }
    return_none();
} ext_unscoped;

fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // 옵셔널 값 사용
    let result = findValue(5);

    if_some(result, value) {
        printf("값을 찾았습니다: %d\n", value);
    } else_none {
        printf("값을 찾지 못했습니다.\n");
    }

    return_void();
} ext_unscoped;
```

### 오류 처리 예제

```c
// safe division - 0으로 나눔 오류 처리
fn_ext_scope(safeDivide(i32 a, i32 b), math_Err$i32) {
    if (b == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(a / b);
} ext_unscoped;

// Optional 값 처리
fn_(processNumber(Opt$i32 maybe_num), void) {
    if_some(maybe_num, num) {
        printf("Got number: %d\n", num);
    } else_none {
        printf("No number provided\n");
    }
}

// 경계 검사된 슬라이스로 메모리 안전성 보장
fn_(sumSlice(Sli_const$i32 numbers), i32) {
    i32 sum = 0;
    for_slice(numbers, num) {
        sum += deref(num);
    }
    return sum;
}
```

### 🧪 테스트 코드 예제

dasae-headers는 간편하고 강력한 내장 테스트 프레임워크를 제공합니다. `TEST.h` 헤더를 통해 단위 테스트를 쉽게 작성하고 실행할 수 있습니다.

```c
#include "dh/main.h"
#include "dh/TEST.h"

// 테스트할 함수 정의
fn_(mathAdd(i32 a, i32 b), i32) {
    return a + b;
}

fn_(mathMultiply(i32 a, i32 b), i32) {
    return a * b;
}

// 테스트 모듈 정의
fn_TEST_scope("기본 수학 연산 테스트") {
    // 덧셈 테스트
    let a = 5;
    let b = 7;
    let sum = mathAdd(a, b);

    // 결과 검증
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(sum > 10, "합계는 10보다 커야 합니다"));

    // 곱셈 테스트
    let product = mathMultiply(a, b);
    try_(TEST_expect(product == 35));

    // 실패하는 테스트 (의도적인 오류 발생)
    // try_(TEST_expect(product == 30)); // 실패: 35 != 30
} TEST_unscoped;
```

## 📚 문서

자세한 문서는 다음 위치에서 확인할 수 있습니다:
- [소스 코드 및 예제](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)
- [API 문서](https://github.com/coding-pelican/dasae-headers/tree/main/dh/docs)

## 🚧 현재 상태

이 프로젝트는 적극적으로 개발 중이며 API는 아직 안정화되지 않았습니다. 실험적인 프로젝트에서 사용하고 피드백을 제공해 주시면 감사하겠습니다.
<!-- 프로젝트 개선에 관심이 있으시다면 contribution.md를 참조해 주세요. -->

1. 이슈 보고: 버그 리포트나 기능 요청
2. 코드 기여: Pull Request로 기능 추가나 버그 수정
3. 문서화: 문서 개선이나 예제 코드 추가

## 📜 라이센스

Copyright © 2024-2025. Gyeongtae Kim. All rights reserved.
