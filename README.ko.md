# dasae-headers

<!--
***Description***
dasae-headers: Modern, Better safety and productivity to C

***Topic***
- c-programming
- memory-safety
- modern-c
- safe-c
- generic
- c17
- c-extension
- error-handling
- type-safety
- defer-pattern
- optional-type
- transpiler
- cross-platform
- rust-inspired
- zig-inspired
- testing-framework
 -->

<div align="center">
  <a href="https://github.com/coding-pelican/dasae-headers">
    <img src="./.github/logo-dasae_headers.svg" alt="dasae-headers Logo" width="150"/>
  </a>
  <h1>dasae-headers: C언어를 현대적이고 안전하게</h1>
  <p>생산성을 위한 현대적 프로그래밍 패러다임 C 언어 확장</p>

  <div>
    <a href="./README.md"><img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English"></a>
    <a href="./README.ko.md"><img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean"></a>
  </div>

  <div style="margin-top: 8px;">
    <a href="https://en.wikipedia.org/wiki/C17_(C_standard_revision)"><img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17"></a>
    <a href="./dh/include/dh/builtin/plat_cfg.h"><img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square" alt="Platform: Windows | Linux | macOS"></a>
    <a href="./LICENSE"><img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT"></a>
  </div>
</div>

<details>
<summary><strong>목차</strong></summary>

- [dasae-headers](#dasae-headers)
  - [소개](#소개)
    - [왜 dasae-headers인가?](#왜-dasae-headers인가)
    - [어떻게 기존의 C 사용과 달라지는가?](#어떻게-기존의-c-사용과-달라지는가)
      - [1. 코드 패턴 및 플랫폼 추상화 (Code Pattern \& Platform Abstraction)](#1-코드-패턴-및-플랫폼-추상화-code-pattern--platform-abstraction)
      - [2. 현대적 자원 관리 및 인자 전달 (Memory \& Argument Patterns)](#2-현대적-자원-관리-및-인자-전달-memory--argument-patterns)
      - [3. 동시성 모델 (Concurrency)](#3-동시성-모델-concurrency)
      - [4. 메타 타입 및 제네릭 시스템 (Meta Type \& Generic System)](#4-메타-타입-및-제네릭-시스템-meta-type--generic-system)
      - [5. 제로 오버헤드 안전 보장 (Numerical \& Safety)](#5-제로-오버헤드-안전-보장-numerical--safety)
      - [6. 개발 생태계 통합 (Ecosystem \& Infrastructure)](#6-개발-생태계-통합-ecosystem--infrastructure)
    - [무엇이 dasae-headers를 특별하게 만드는가?](#무엇이-dasae-headers를-특별하게-만드는가)
      - [1. 정적 분석 및 IDE 호환성 극대화](#1-정적-분석-및-ide-호환성-극대화)
      - [2. 메타 시스템 및 익명 타입 호환성](#2-메타-시스템-및-익명-타입-호환성)
      - [3. 제로 오버헤드 안전 보장 (Zero-overhead Safety)](#3-제로-오버헤드-안전-보장-zero-overhead-safety)
  - [주요 기능](#주요-기능)
  - [플랫폼 지원](#플랫폼-지원)
  - [🚀 시작하기](#-시작하기)
    - [💽 설치](#-설치)
    - [🔨 빌드 및 실행](#-빌드-및-실행)
  - [코드 샘플](#코드-샘플)
    - [Hello, world!](#hello-world)
    - [Optional Values](#optional-values)
    - [Error Results \& Defer](#error-results--defer)
    - [Pattern Matching](#pattern-matching)
    - [Chaining - Filter, Map, Fold, Reduce](#chaining---filter-map-fold-reduce)
    - [Threads vs Stackless-Coroutines](#threads-vs-stackless-coroutines)
    - [Atomic Values](#atomic-values)
    - [SIMD Vectors](#simd-vectors)
    - [Meta System](#meta-system)
    - [Data Structures \& Algorithms](#data-structures--algorithms)
    - [Testing](#testing)
  - [문서](#문서)
  - [현재 상태 및 명칭에 관하여](#현재-상태-및-명칭에-관하여)
  - [기여 및 연락처](#기여-및-연락처)
  - [라이선스](#라이선스)

</details>

---

## 소개

**dasae-headers**는 C언어에 현대적인 프로그래밍 패러다임을 도입하여 안전성, 표현력, 생산성을 개선하는 것을 목적으로 합니다.
C언어의 설계 원칙인 간결함을 유지하면서, 메모리 안전성 및 타입 안전성을 강화하고 구조화된 에러 처리 메커니즘을 제공합니다.

현재 개발 초기 단계로, C 전처리기를 활용하여 언어적 확장을 구현하는 트랜스파일러 환경을 제공합니다.
이는 표준 C 라이브러리의 구조적 한계를 보완하며 점진적으로 독립적인 개발 생태계를 구축하는 과정에 있습니다.

---

### 왜 dasae-headers인가?

dasae-headers는 기존 C 환경의 제약을 극복하고 현대적인 개발 경험을 제공하기 위해 아래와 같은 설계 원칙을 고수합니다.

- **기존 C 생태계와의 공존:** 기존 C 라이브러리 및 레거시 코드베이스를 수정하지 않고도 현대적 문법과 안전 장치를 즉시 도입할 수 있는 유연성
- **제로 코스트 추상화:** 인라인화, 전처리 단계 평가, 상수 폴딩/전파 유도 등의 최적화를 통해 고수준 기능을 제공하면서도 런타임 오버헤드를 이론적 최소치로 수렴시킴
- **점진적 도입 가능:** 프로젝트 전체를 전환할 필요 없이, 필요한 모듈(에러 처리, 할당자 등)만 선택적으로 연결하여 사용할 수 있는 모듈화된 설계
- **Freestanding 및 베어메탈 지원:** libc의 사용이 선택적. 임베디드 환경이나 커널 개발 등 프리스텐딩 환경과 같은 시스템의 최저 수준에서도 기능함을 최우선적으로 설계
- **사용자 정의 매크로 최소화:** 일반적인 활용 및 핵심 비즈니스 로직, 기능 구현을 위해 사용자가 별도의 복잡한 매크로를 작성할 필요가 없음. 필요시 정립된 메타 시스템 기반 제네릭 인스턴스화 패턴/전처리 시스템을 따라 쉬운 인스턴스화 및 확장을 지원
- **디버깅 친화적 설계:** 매크로가 런타임 디버깅(Call Stack 추적, Step-by-step 실행 등)을 방해하지 않도록 정교하게 디자인됨. 복잡한 추상화 도입 시 발생하는 개발 생산성 저하를 원천적으로 방지
- **일관된 컨벤션:** 엄격하고 일관된 코드 컨벤션과 명명법을 통해 대규모 코드베이스에서도 가독성과 유지보수성을 보장

---

### 어떻게 기존의 C 사용과 달라지는가?

C의 유연함을 유지하면서도, 현대적인 언어들이 제공하는 안전 장치와 생산성 도구들을 시스템 계층에 최적화된 형태로 이식합니다.
단순히 기능을 추가하는 것을 넘어, 기존 C의 고질적인 설계 결함과 파편화된 컨벤션을 구조적으로 해결하는 데 집중합니다.

#### 1. 코드 패턴 및 플랫폼 추상화 (Code Pattern & Platform Abstraction)

표준 C의 파편화된 언어/아키텍처/운영체제/컴파일러별 API와 복잡한 구문을 단일 인터페이스로 통합합니다.

| 항목             | 전통적인 C (Standard C)                               | dasae-headers                                                              |
| ---------------- | ----------------------------------------------------- | -------------------------------------------------------------------------- |
| **변수 및 함수** | 명시적 타입 선언 및 반복적 시그니처                   | `let` (constant), `var` (mutable) 타입 추론 및 `fn_` 현대적 구문           |
| **람다/클로저**  | GCC nested function 또는 Clang blocks, 플랫폼 의존    | `la_` 구문으로 양 컴파일러 확장 통합, `fn_`과 `la_`를 다룰 `Callable` 타입 |
| **플랫폼 대응**  | `#ifdef` 기반의 파편화된 분기 작성                    | **통합 API** (`{lang/arch/plat/comp}_cfg.h`, `os.h`, `posix.h`) 제공       |
| **전처리 분기**  | 단순한 수치값/대상 변경에도 `#ifdef` 분기별 정의 작성 | `pp_if_`, `pp_switch_`를 통해 단일 정의에 전처리 분기 가능                 |

#### 2. 현대적 자원 관리 및 인자 전달 (Memory & Argument Patterns)

성공 여부를 `bool`로 반환하고 결과값을 `out` 파라미터로 받는 불안전한 패턴을 지양합니다.
`Optional` 및 `Error Result`를 통해 값이 없거나 에러가 발생한 상황을 타입 시스템 수준에서 강제 검증합니다.

| 항목               | 전통적인 C (Standard C)                  | dasae-headers                                               |
| ------------------ | ---------------------------------------- | ----------------------------------------------------------- |
| **자원 해제**      | `goto cleanup` 또는 수동 해제 코드 산재  | `defer_`, `errdefer_` 기반 스코프 자동 해제                 |
| **결과 반환**      | `bool`/에러코드 반환 + `out` 포인터 인자 | **Optional** (`O$`) / **Error Result** (`E$`) 반환          |
| **결과 분기 처리** | 반환값 수동 검증 `if (err)` 분기         | `orelse_`, `unwrap_` / `try_`, `catch_` 구문 기반 결과 제어 |
| **데이터 전달**    | 포인터와 길이(`len`)를 별도로 전달       | **슬라이스** (`S$`) 또는 **값으로서의 배열** (`A$`) 전달    |
| **문자열/배열**    | `0(NULL)` 센티널 기반의 암시적 길이 확인 | 명시적 길이 필드 기반의 **경계 검사(Boundary Check)**       |

#### 3. 동시성 모델 (Concurrency)

OS 스레드 오버헤드 없이 수만 개의 태스크를 동시 처리할 수 있는 초경량 비동기 환경을 제공합니다.
상태 머신 기반의 제어 흐름을 `async_/await_` 패턴으로 간결하게 표현합니다.

| 항목            | 전통적인 C (Standard C)                   | dasae-headers                                                   |
| --------------- | ----------------------------------------- | --------------------------------------------------------------- |
| **비동기 모델** | OS 네이티브 스레드 중심 구성              | OS 네이티브 스레드 + **상태 머신 기반 Stackless 코루틴**        |
| **동기화 도구** | 원시적 `mutex`, `cond` 의존               | `WaitGroup`, `RwLock`, `ResetEvent` 등                          |
| **제어 흐름**   | 콜백 지옥 또는 복잡한 상태 머신 수동 구현 | `async_`, `await_`, `suspend_`, `resume_` 구문 기반 코루틴 제어 |

#### 4. 메타 타입 및 제네릭 시스템 (Meta Type & Generic System)

매크로 지옥을 탈피하고 메타 타입 시스템을 통해 타입 안전성을 컴파일 타임에 검증합니다.
특히 C 표준에서 허용되지 않는 익명 사용자 타입 간의 논리적 호환성을 정적으로 보장하는 차별화된 레이어를 제공합니다.

| 항목          | 전통적인 C (Standard C)                       | dasae-headers                                                                |
| ------------- | --------------------------------------------- | ---------------------------------------------------------------------------- |
| **구현 방식** | `void*` 캐스팅 (타입 정보 소실)               | **Meta System** (`u_*`) 기반 고수준 추상화                                   |
| **익명 타입** | 구조가 동일해도 익명성으로 인해 대입 불가     | 필드의 메모리 레이아웃 및 논리적 구조(Size/Align/Offset) 동일 시 호환성 보장 |
| **API 노출**  | 자료구조 API 구현이 매크로 내부에 직접 포함됨 | **Macro-less API**: 표준 함수 정의 형식 준수                                 |

#### 5. 제로 오버헤드 안전 보장 (Numerical & Safety)

하드웨어 아키텍처별 SIMD 명령어를 추상화하여 제공하며,
산술 연산의 오버플로우와 부적절한 타입 캐스팅을 정적으로 감지합니다.
모든 안전 장치는 릴리즈 모드에서 최적화되어 런타임 오버헤드를 발생시키지 않습니다.

| 항목            | 전통적인 C (Standard C)                                 | dasae-headers                                                           |
| --------------- | ------------------------------------------------------- | ----------------------------------------------------------------------- |
| **벡터 연산**   | 수동 루프 기반 연산 또는 플랫폼 의존 API                | `simd.h` 기반 아키텍처 독립적 가속 연산                                 |
| **산술 안전**   | `Overflow`, `DivisionByZero`, `NaN` 등 런타임 예외 취약 | 컴파일 타임 **Overflow/NaN 정적 감지** 레이어                           |
| **타입 캐스팅** | 암시적 형변환으로 인한 데이터 손실 위험                 | `Signed`/`Unsigned` 및 크기 변환, `int`<->`float` 변환 시 미스매치 추적 |

#### 6. 개발 생태계 통합 (Ecosystem & Infrastructure)

별도의 의존성 없이 내장 도구만으로 프로젝트 생성부터 빌드, 테스트까지 전 과정을 관리합니다.
에러 발생 시 단순 반환 값을 넘어 호출 스택 정보를 보존하여 원인을 즉각적으로 특정합니다.

| 항목                  | 전통적인 C (Standard C)                 | dasae-headers                                                    |
| --------------------- | --------------------------------------- | ---------------------------------------------------------------- |
| **자료구조/알고리즘** | 수동 구현 혹은 파편화된 외부 라이브러리 | `ArrList`,`ArrPDeq`, `HashMap` 등 표준 컨테이너 및 알고리즘 제공 |
| **메모리 제어**       | 고정된 전역 할당 방식 의존              | 모든 API에 **할당자(Allocator) 또는 메모리버퍼 주입** 가능       |
| **테스트/분석**       | 외부 프레임워크 연동 필수               | 내장 `TEST.h` 및 `ErrTrace.h` 기반 정교한 추적                   |

---

### 무엇이 dasae-headers를 특별하게 만드는가?

단순한 문법 확장을 넘어, 컴파일러와 정적 분석기의 동작 방식을 깊이 있게 활용하여
타 프로젝트와는 차별화된 사용자 중심의 개발 경험을 제공합니다.

#### 1. 정적 분석 및 IDE 호환성 극대화

**정적 분석기 투명성 보장:**

`clang-tidy` 등 주요 분석 도구가 일반적인 C 코드와 동일하게 소스를 파악할 수 있도록 설계되었습니다.
특히 `_Generic` 활용 시 발생하는 정적 분석기의 오작동(모든 분기 검사로 인한 타입 불일치 등)을 차단하여
"인식 오류"가 아닌 "실존하는" 경고와 에러에만 집중할 수 있게 합니다.

**투명한 API 노출 (Macro-less API):**

자료구조를 포함한 핵심 API를 복잡한 매크로 함수로 래핑하지 않습니다.
매크로는 특수 기호를 대체하는 연산자 역할에 집중하며, 실제 로직은 표준 함수 정의 형식을 따라 작성됩니다.
따라서 사용자가 라이브러리 호출을 위해 별도의 매크로를 작성하거나 복잡한 매크로 구조를 파악해야 하는 부담 자체를 제거하며,
코어 로직에서 "연산자로서의 매크로"와 "실제 함수 사용"의 구분을 예상 가능한 구준으로 정의합니다.

#### 2. 메타 시스템 및 익명 타입 호환성

**Meta Type 기반 Generic System:**

제네릭 로직을 정적 분석의 사각지대인 매크로가 아닌 메타 타입 시스템(`u_*`)으로 구현합니다.
LTO, 상수 폴딩과 상수 전파에 의해 매크로 인라인화 코드와 동일하게 최적화되는 제로 코스트 추상화 레이어를 제공합니다.

**구조적 익명 타입 호환성 검증:**

반드시 `typedef` 에일리어싱을 거쳐야만 제네릭을 허용하는 타 라이브러리와 달리,
익명 사용자 타입을 효과적으로 수용합니다. 필드의 메모리 레이아웃 및 논리적 구조(Size, Align, Offset)가 동일한 경우에 한해 안전성을 컴파일 타임에 정적으로 검증하며, 익명 타입 간의 안전한 상호작용을 보장합니다.

#### 3. 제로 오버헤드 안전 보장 (Zero-overhead Safety)

**컴파일 타임 평가 우선 연산 및 캐스팅 검증:**

산술 연산의 `Overflow`, `DivisionByZero`, `NaN` 감지 및 부적절한 타입 캐스팅을 정적으로 감지합니다.
모든 안전 검증은 컴파일 타임 평가를 우선하며, 런타임에서는 `assertion`으로서 기능합니다.
릴리즈 모드에서는 이러한 검증 항목들이 분기 최적화 대상이 되어 바이너리 오버헤드 없이 안전성을 확보하며,
개발 단계에서는 기존 C 사용보다 더 빠르게 문제 원인을 정확히 특정할 수 있습니다.

---

## 주요 기능

본 프로젝트는 Zig와 Rust의 문법 구조 및 표준 라이브러리 설계 방식을 참고하여 개발되었습니다.

- **메모리 안전성:** 커스텀 할당자, 메모리 추적, 경계 검사, `defer_`/`errdefer_` 기반 자동 리소스 관리
- **강화된 타입 시스템:** 컴파일 타임 검사, 메타 타입 시스템, 대수적 데이터 타입(Variant), 옵셔널 타입
- **명시적 오류 처리:** `ok`/`err` 키워드, `try_`/`catch_` 패턴, 호출 스택을 포함한 에러 추적
- **현대적 구문:** 타입 추론(`let`/`var`), 함수 정의(`fn_`), 람다 표현식(`la_`), 일급 객체 `Callable` 타입
- **개발 도구:** 내장 테스트 프레임워크, 주요 C 컴파일러(Clang, GCC) 및 멀티 플랫폼 지원

---

## 플랫폼 지원

| 구분             | 지원 범위                                                     |
| :--------------- | :------------------------------------------------------------ |
| **OS**           | Windows, Unix, Linux, macOS                                   |
| **Architecture** | x86 (32-bit), x64 (64-bit)                                    |
| **Clang**        | 19.1.0+ (권장) / 16.0.0+ (지원) / 9.0.0+ (-std=gnu11 필수)    |
| **GCC**          | 15.1.0+ (권장) / 13.1.0+ (지원) / N/A (TBU) (-std=gnu11 필수) |
| **MSVC**         | 지원 예정 (TBD)                                               |

---

## 🚀 시작하기

<!-- Interactive Quick Start -->
<!--
> **"10초 안에 dasae-headers 경험하기"**
> ```sh
> curl -sSL https://raw.githubusercontent.com/coding-pelican/dasae-headers/main/try.sh | bash
> ```
-->

### 💽 설치

1. 레포지토리 클론:
```sh
git clone https://github.com/coding-pelican/dasae-headers
```

2. 전용 빌드 도구(dh-c) 설치:
```sh
cd dasae-headers
chmod +x install.sh
./install.sh
```

3. 신규 프로젝트 생성:
```sh
dh-c project myproject
cd myproject
```

### 🔨 빌드 및 실행

- **빌드:** `dh-c build dev`
- **실행:** `dh-c run dev`
- **테스트:** `dh-c test`

자세한 절차는 [빠른 시작 가이드](./dh/docs/ko/quick-start.md)를 참고하시기 바랍니다.

---

## 코드 샘플

### Hello, world!

```c
#include "dh/main.h"
#include "dh/io/stream.h"

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let message = u8_l("Hello");
    io_stream_println(u8_l("{:s}, world!"), message);
    return_ok({});
} $unscoped_(fn);
```

### Optional Values

```c
fn_((findValueIndex(i32 value, S_const$i32 items))(O$i32) $scope) {
    for_(($s(items), $rf(0))(item, index) {
        if (*item != value) return_some(index);
    });
    return_none();
} $unscoped_(fn);

fn_((example(void))(void)) {
    var nums = A_from$((i32){ 10, 20, 30, 40, 50 });
    let found = findValueIndex(30, A_ref$((S$i32)(nums)).as_const);

    io_stream_println(u8_("found = {:?d}"), found);
    if_some((found)(index)) {
        io_stream_println(u8_("- Found at: {:d}"), index);
    } else_none {
        io_stream_println(u8_("- Not found"));
    }

    let value = orelse_((found)(-1));
    let value_assumed = unwrap_(found);
}
```

### Error Results & Defer

```c
errset_((math_Err)(
    DivisionByZero,
    Overflow,
    Underflow
));

T_use_E$($set(math_Err)(i32));
$attr($must_check)
fn_((safeDivide(i32 num, i32 denom))(math_Err$i32) $scope) {
    if (denom == 0) return_err(math_Err_DivisionByZero());
    return_ok(num / denom);
} $unscoped_(fn);

$attr($must_check)
fn_((example(mem_Allocator gpa))(E$void) $guard) {
    // Allocate resources
    var buffer = u_castS$((S$i32)(try_(mem_Allocator_alloc(gpa, typeInfo$(i32), 100))));
    defer_(mem_Allocator_free(gpa, u_anyS(buffer)));

    // Only executed when an error occurs and propagates
    errdefer_(err, io_stream_eprintln(u8_l("Occurred error!: {:e}"), err));

    // Error propagation (try_) and handling (catch_)
    let divided = try_(safeDivide(10, 0));
    let divided_handled = catch_((safeDivide(10, 0))($ignore, 1)); // Use default value 1 when error occurs

    return_ok({});
} $unguarded_(fn);
```

<details>
<summary><strong>더 많은 코드 샘플 보기</strong></summary>

### Pattern Matching

```c
typedef variant_((InputEvent $bits(8))(
    (InputEvent_press_key,      struct { i32 key; }),
    (InputEvent_release_button, struct { i8 button; })
)) InputEvent;
T_use_O$(InputEvent);
fn_((pullInputEvent(void))(O$InputEvent));

fn_((example(void))(void)) {
    if_some((pullInputEvent())(event)) match_(event) {
    pattern_((InputEvent_press_key)(on_pressed)) {
        debug_assert_true_fmt(
            -1 < on_pressed->key && on_pressed->key <= 255,
            "key is out of range"
        );
        break;
    } $end(pattern);
    pattern_((InputEvent_release_button)(on_released)) {
        debug_assert_true_fmt(
            -1 < on_released->button && on_released->button <= 5,
            "button is out of range"
        );
        break;
    } $end(pattern);
    fallback_(claim_unreachable);
} $end(match);
} $unscoped_(fn);
```

### Chaining - Filter, Map, Fold, Reduce

```c
T_use$((i32)(
    ArrList,
    ArrList_init,
    ArrList_fini,
    ArrList_appendWithin
));

fn_((collectEvenSq(S_const$i32 items, mem_Allocator gpa))(mem_Err$ArrList$i32) $scope) {
    let init = ArrList_init$i32;
    let appendWithin = ArrList_appendWithin$i32;
    return_ok(chain$((ArrList$i32)(items)(
        filter_((x)(int_isEven(*x))),
        map$((i32)(x)(int_sq(*x))),
        fold_(try_(init(gpa, items.len)), (collect, x)(appendWithin(&collect, *x), collect))
    )));
} $unscoped_(fn);

fn_((reduceSumEvenSq(S_const$i32 items))(O$i32)) {
    return chain$((O$i32)(items)(
        filter_((x)(int_isEven(*x))),
        map$((i32)(x)(int_sq(*x))),
        reduce_((acc, x)(acc + *x))
    ));
};

$attr($must_check)
fn_((example(void))(E$void) $guard) {
    var page = (heap_Page){};
    let gpa = heap_Page_allocator(&page);
    let nums = A_ref$((S$i32)(A_from$((i32){ 1, 2, 3, 4, 5, 6, 7, 8 }))).as_const;

    let even_sqs = try_(collectEvenSq(nums, gpa));
    defer_(ArrList_fini$i32(&even_sqs, gpa));
    let sum = chain$((i32)(even_sqs.items)(fold_((0), (acc, item)(acc + *item))));
    let sum_even_sqs = orelse_((reduceSumEvenSq(nums))(0));
    claim_assert(sum == sum_even_sqs);

    return_ok({});
} $unguarded_(fn);
```

### Threads vs Stackless-Coroutines

전통적인 OS 스레드 외에도, 초경량 비동기 처리를 위한 상태 머신 기반 코루틴을 제공합니다.

```c
Thrd_fn_(((timesTwoThread)(i32 input))(i32) $scope($ignore, args)) {
    time_sleep(time_Duration_fromMillis(10));
    return_(args->input * 2);
} $unscoped_(Thrd_fn);

fn_((mainThread(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    var task = try_(Thrd_spawn(Thrd_SpawnConfig_default, Thrd_FnCtx_from$((timesTwoThread)(10)).as_raw));
    let result = Thrd_FnCtx_ret$((timesTwoThread)(Thrd_join(task)));
    io_stream_println(u8_l("result: {:d}"), result);
    return_ok({});
} $unscoped_(fn);

async_fn_(((timesTwoAsync)(O$$(Co_Ctx*) caller, i32 input))(i32) $scope({
    var_(sleep_ctx, Co_CtxFn$(exec_sleep));
})(self_ctx, args, locals)) {
    callAsync((locals->sleep_ctx)((exec_sleep)(
        some(orelse_((caller)(self_ctx->anyraw))), time_Duration_fromMillis(10)
    )));
    areturn_(args->input * 2);
} $unscoped_(async_fn);

async_fn_(((mainAsync)(S$S_const$u8 args))(Void) $scope({
    var_(task, Co_CtxFn$(timesTwoAsync));
})($ignore, $ignore, $ignore)) {
    locals->task = async_ctx((timesTwoAsync)(none(), 10));
    await_(resume_(locals->task));
    io_stream_println(u8_l("result: {:d}"), Co_Ctx_returned(task));
    areturn_({});
} $unscoped_(async_fn);
```

### Atomic Values

C11 Atomics를 래핑하여 load, store, CAS 연산을 타입 안전하고 직관적인 API로 제공합니다.
> *TODO: document*
<!-- TODO: document -->

### SIMD Vectors

CPU 아키텍처(AVX, NEON 등)에 독립적인 통합 인터페이스를 통해 벡터 병렬 연산을 가속합니다.
> *TODO: document*
<!-- TODO: document -->

### Meta System

컴파일 타임 타입 정보(typeInfo$)와 리플렉션을 활용하여 제네릭한 자료구조 처리 및 직렬화의 기반을 제공합니다.
> *TODO: document*
<!-- TODO: document -->

### Data Structures & Algorithms

모든 자료구조와 함수 설계에 있어 동적 할당이 필요한 경우,
할당자(Allocator) 혹은 메모리 버퍼를 주입받는 구조로 설계되어 메모리 레이아웃을 완전히 제어할 수 있습니다.
> *TODO: document*
<!-- TODO: document -->

### Testing

```c
#include "dh/main.h"
#include "dh/TEST.h"

// Define functions to test
fn_((mathAdd(i32 a, i32 b))(i32)) { return a + b; }
fn_((mathMul(i32 a, i32 b))(i32)) { return a * b; }

TEST_fn_("Basic Math Operations Test" $scope) {
    // Addition test
    let_(a, i32)   = 5;
    let_(b, i32)   = 7;
    let_(sum, i32) = math_addI32(a, b);

    // Validate results
    try_(TEST_expect(sum == 12));
    try_(TEST_expectMsg(sum > 10, "Sum should be greater than 10"));

    // Multiplication test
    let product = mathMul(a, b);
    try_(TEST_expect(product == 35));

    // Failing test (intentional error)
    let should_fail = TEST_expect(product == 30); // Fails: 35 != 30
    try_(TEST_expect(isErr(should_fail)));
} $unscoped_(TEST_fn);
```

</details>

---

## 문서

- [위키 홈](./dh/docs/ko/home.md)
- [빠른 시작 가이드](./dh/docs/ko/quick-start.md)
- [API 참조](./dh/docs/ko/api/index.md)

---

## 현재 상태 및 명칭에 관하여

본 프로젝트는 현재 개발 초기 단계에 있으며 API 사양은 실험적입니다.

**"dasae-headers"**라는 명칭은 프로젝트 초기,
자주 사용하는 C 유틸리티 코드를 모아둔 헤더 전용 라이브러리(Header-only Library)로 시작된 것에서 유래했습니다.

지속적인 변화와 기능 확장에 따라, 현재는 전용 빌드 도구와 소스 파일이 포함된 구조를 갖추게 되어
단순히 '헤더 전용'이라 부르기에는 더 넓은 범위를 다루는 구조가 되었습니다.
아직까지는 프로젝트의 고도화 및 최적화 과정과 함께 사용자 경험 개선을 위해 구조적 유연성을 유지하고 있습니다.

따라서 현재의 이름이 곧 프로젝트의 정체성을 완벽하게 확정 짓는 것은 아닙니다.
dasae-headers가 지향하는 철학을 더 잘 드러낼 수 있는 좋은 이름, 유니크한 이름 제안이 있으시다면 알려주세요 :D

---

## 기여 및 연락처

버그 보고, 기능 제안 및 풀 리퀘스트를 통한 기여를 환영합니다.
[기여 가이드](./dh/docs/ko/contributing.md)를 참고하실 수 있습니다.
많은 피드백 부탁드립니다!

- **Author:** Gyeongtae Kim (dev-dasae)
- **Email:** [codingpelican@gmail.com](mailto:codingpelican@gmail.com)

## 라이선스

본 프로젝트는 [MIT License](./LICENSE) 하에 배포됩니다.

Copyright © 2024-2025 Gyeongtae Kim.
