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

## 📋 목차

- [dasae-headers](#dasae-headers)
  - [📋 목차](#-목차)
  - [🌟 소개](#-소개)
  - [🛡️ 주요 특징](#️-주요-특징)
  - [🌐 플랫폼 지원](#-플랫폼-지원)
  - [🚀 시작하기](#-시작하기)
    - [💽 설치](#-설치)
  - [⚡ 코드 샘플](#-코드-샘플)
    - [👋 Hello, world!](#-hello-world)
    - [🔍 Optional Values 예제](#-optional-values-예제)
    - [🔄 Error Results 예제](#-error-results-예제)
    - [🤝 Pattern Matching 예제](#-pattern-matching-예제)
    - [🧪 Testing 예제](#-testing-예제)
  - [📚 문서](#-문서)
  - [🚧 현재 상태](#-현재-상태)
  - [🙏 기여](#-기여)
  - [📧 연락처](#-연락처)
  - [📜 라이센스](#-라이센스)

## 🌟 소개

> "Make C Great Again"

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

### 💽 설치

> 자세한 설치 및 빌드 가이드는 준비 중에 있습니다. 조금만 기다려주세요!

1. 이 저장소를 클론합니다:

```bash
git clone https://github.com/coding-pelican/dasae-headers.git
```

2. 필요한 헤더를 포함시킵니다:

```c
#include "dh/main.h"
#include "dh/core.h"
#include "dh/opt.h"
#include "dh/err_res.h"
// 필요한 기능에 따라 추가 헤더 포함
```

## ⚡ 코드 샘플

### 👋 Hello, world!

```c
// 프로그램 진입점을 제공하는 메인 헤더 포함
#include "dh/main.h"
// 텍스트 작업을 위한 문자열 유틸리티 포함
#include "dh/Str.h"

// 확장 범위와 오류 처리를 갖는 메인 함수 정의
// 명령줄 인수를 받고 void 페이로드가 있는 오류 결과 반환
fn_ext_scope(dh_main(Sli$Str_const args), Err$void) {
    // Str_l로 문자열 리터럴 생성
    let hello_world = Str_l("Hello, world!");

    // 문자열을 콘솔에 줄바꿈과 함께 출력
    Str_println(hello_world);

    // 성공 반환 (오류 없는 void 값)
    return_ok({});
} ext_unscoped; // 확장 범위 블록 종료
```

### 🔍 Optional Values 예제

```c
fn_ext_scope(findValueIndex(i32 value, Sli_const$i32 items), Opt$i32) {
    for_slice_indexed (items, item, index) {
        if (*item != value) { continue; }
        return_some(index); // 값이 있음을 반환
    }
    return_none(); // 값이 없음을 반환
} ext_unscoped;

fn_(example(void), void) {
    Arr$$(5, i32) nums = Arr_init({ 10, 20, 30, 40, 50 });

    // Optional 값 생성
    let opt_value = some$(Opt$i32, 42);
    let opt_empty = none$(Opt$i32);

    // 배열에서 값 찾기
    let found = findValueIndex(30, Sli_arr$(Sli_const$i32, nums));

    // Optional 값 확인
    if_some(found, index) {
        printf("찾은 위치: %d\n", index);
    } else_none {
        printf("찾지 못함\n");
    }

    // 기본값 설정
    let value = orelse(found, -1); // 찾지 못한 경우 -1 사용

    // 안전하지 않은 추출 (옵션이 none인 경우 assertion 발생)
    let unsafe_value = unwrap(opt_value);
}
```

### 🔄 Error Results 예제

```c
config_ErrSet(math_Err,
    DivisionByZero,
    Overflow,
    Underflow
);

use_ErrSet$(math_Err, i32); // 또는 일반적으로 `use_Err$(i32)`
fn_ext_scope(safeDivide(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero()); // 오류를 반환
    }
    return_ok(lhs / rhs); // 값을 반환
} ext_unscoped;

fn_ext_scope(example(void), Err$void) {
    // 리소스 할당
    var buffer = meta_cast$(Sli$i32,
        try_(mem_Allocator_alloc(allocator, typeInfo$(i32), 100))
    );
    // 함수가 반환될 때 항상 정리됨
    defer_(mem_Allocator_free(allocator, anySli(buffer)));
    // 오류가 발생하고 전파될 때만 정리됨
    errdefer_(log_error("오류 발생!"));

    // 오류 전파 (조기 반환)
    let result_invalid = try_(safeDivide(10, 0));

    // 기본값으로 오류 처리
    let result_default = catch_(safeDivide(10, 0), 1);

    // 오류 페이로드 캡처를 통한 오류 처리
    let result_handling = catch_from(safeDivide(10, 0), err, eval({
        Err_print(err);   // 오류 출력
        ErrTrace_print(); // 오류 추적 출력
        return_err(err);  // 오류를 반환
    }));

    // 정상 반환
    return_ok({});
} ext_unscoped;
```

### 🤝 Pattern Matching 예제

```c
config_UnionEnum(InputEvent,
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
);
use_Opt$(InputEvent);
fn_(pullInputEvent(void), Opt$InputEvent);

fn_(example(void), void) {
    if_some(pullInputEvent(), event) {
        match_(event) {
        pattern_(InputEvent_press_key, on_pressed) {
            debug_assert_true_fmt(
                -1 < on_pressed->key && on_pressed->key <= 255,
                "key is out of range"
            );
        } break;
        pattern_(InputEvent_release_button, on_released) {
            debug_assert_true_fmt(
                -1 < on_released->button && on_released->button <= 5,
                "button is out of range"
            );
        } break;
        fallback_()
            claim_unreachable;
        }
    }
}
```

### 🧪 Testing 예제

dasae-headers는 간편하고 강력한 내장 테스트 프레임워크를 제공합니다. `TEST.h` 헤더를 통해 단위 테스트를 쉽게 작성하고 실행할 수 있습니다.

```c
#include "dh/main.h"
#include "dh/TEST.h"

// 테스트 대상 함수 정의
fn_(mathAdd(i32 a, i32 b), i32) {
    return a + b;
}

fn_(mathMultiply(i32 a, i32 b), i32) {
    return a * b;
}

// 테스트 케이스 정의
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

- [위키 홈](./dh/docs/ko/home.md) - 메인 문서 포탈
- [빠른 시작 가이드](./dh/docs/ko/quick-start.md) - 시작을 위한 상세 가이드
- [API 참조](./dh/docs/ko/api/index.md) - 모듈 문서

추가 자료:
- [헤더 파일](https://github.com/coding-pelican/dasae-headers/tree/main/dh/include)
- [소스 파일](https://github.com/coding-pelican/dasae-headers/tree/main/dh/src)
- [예제 코드](https://github.com/coding-pelican/dasae-headers/tree/main/dh/samples)
- [테스트](https://github.com/coding-pelican/dasae-headers/tree/main/dh/tests)

## 🚧 현재 상태

이 프로젝트는 적극적으로 개발 중이며 API는 아직 안정화되지 않았습니다. 실험적인 프로젝트에서 사용하고 피드백을 제공해 주시면 감사하겠습니다.
프로젝트 개선에 관심이 있으시다면 [contribution.md](./dh/docs/ko/contributing.md)를 참조해 주세요.

## 🙏 기여

이슈, 풀 리퀘스트, 그리고 피드백은 언제나 환영합니다!

1. 이슈 보고: 버그 리포트 / 기능 요청
2. 코드 기여: 기능 추가 / 버그 수정
3. 문서화: 문서 개선 / 예제 코드 추가

## 📧 연락처

문의사항이 있으시다면 프로젝트 작성자에게 연락해 주세요:
- 김경태(dev-dasae) <<codingpelican@gmail.com>>

## 📜 라이센스

이 프로젝트는 MIT 라이센스를 따릅니다 - 자세한 내용은 [LICENSE](./LICENSE) 파일을 참조하세요.

Copyright © 2024-2025 Gyeongtae Kim. All rights reserved.
