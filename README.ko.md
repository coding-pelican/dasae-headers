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
    <a href="./README.md">
      <img src="https://img.shields.io/badge/readme-en-red?style=flat-square" alt="Language: English">
    </a>
    <a href="./README.ko.md">
      <img src="https://img.shields.io/badge/readme-ko-orange?style=flat-square" alt="Language: Korean">
    </a>
  </div>

  <div style="margin-top: 8px;">
    <a href="https://en.wikipedia.org/wiki/C17_(C_standard_revision)">
      <img src="https://img.shields.io/badge/language-C17-blue?style=flat-square" alt="Language: C17">
    </a>
    <a href="./dh/include/dh/builtin/plat_cfg.h">
      <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-brightgreen?style=flat-square"
           alt="Platform: Windows | Linux | macOS">
    </a>
    <a href="./LICENSE">
      <img src="https://img.shields.io/badge/license-MIT-lightgrey?style=flat-square" alt="License: MIT">
    </a>
  </div>
</div>

<details>
<summary><strong>목차</strong></summary>

- [dasae-headers](#dasae-headers)
  - [🚀 시작하기](#-시작하기)
    - [💽 설치](#-설치)
      - [사전 요구 사항](#사전-요구-사항)
      - [Step 1: 레포지토리 클론](#step-1-레포지토리-클론)
      - [Step 2: `dh-c` 도구 빌드](#step-2-dh-c-도구-빌드)
      - [Step 3: 환경 변수 설정](#step-3-환경-변수-설정)
      - [Step 4: 설치 확인](#step-4-설치-확인)
      - [Step 5: 새 프로젝트 생성](#step-5-새-프로젝트-생성)
      - [Hello, world!](#hello-world)
    - [🔨 빌드 및 실행](#-빌드-및-실행)
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
  - [Modules Reference](#modules-reference)
      - [`builtin` — Compiler \& Platform Abstraction](#builtin--compiler--platform-abstraction)
      - [`core` — Language Primitives \& Syntax Extensions](#core--language-primitives--syntax-extensions)
      - [`prl` — Prelude Types](#prl--prelude-types)
      - [`simd` — SIMD Vector Operations](#simd--simd-vector-operations)
      - [`cmp` — Comparison Utilities](#cmp--comparison-utilities)
      - [`math` — Mathematical Functions](#math--mathematical-functions)
      - [`Rand` — Random Number Generation](#rand--random-number-generation)
      - [`search` — Searching Algorithms](#search--searching-algorithms)
      - [`sort` — Sorting Algorithms](#sort--sorting-algorithms)
      - [Linked Lists](#linked-lists)
      - [Tree Structures *(planned)*](#tree-structures-planned)
      - [Array-Based Containers](#array-based-containers)
      - [Hash-Based Containers](#hash-based-containers)
      - [`mem` — Memory Utilities](#mem--memory-utilities)
      - [`heap` — Heap Allocators](#heap--heap-allocators)
      - [`meta` — Runtime Record/Type Reflection](#meta--runtime-recordtype-reflection)
      - [`atom` — Atomic Operations](#atom--atomic-operations)
      - [`Thrd` — Threading](#thrd--threading)
      - [`Co` / `async` — Stackless Coroutines](#co--async--stackless-coroutines)
      - [`mp` — Multi-Processing *(planned)*](#mp--multi-processing-planned)
      - [`ascii` — ASCII character utilities](#ascii--ascii-character-utilities)
      - [`utf8` — UTF-8 encoding/decoding](#utf8--utf-8-encodingdecoding)
      - [`utf16` — UTF-16 encoding/decoding](#utf16--utf-16-encodingdecoding)
      - [`wtf8` — WTF-8 (UTF-8 superset for Windows)](#wtf8--wtf-8-utf-8-superset-for-windows)
      - [`wtf16` — WTF-16 encoding](#wtf16--wtf-16-encoding)
      - [`unicode` — Unicode conversion hub](#unicode--unicode-conversion-hub)
      - [`os` — OS-Specific APIs](#os--os-specific-apis)
      - [`posix` — POSIX Compatibility](#posix--posix-compatibility)
      - [`proc` — Process Management *(planned)*](#proc--process-management-planned)
      - [`time` — Time \& Duration](#time--time--duration)
      - [`io` — Input/Output](#io--inputoutput)
      - [`fmt` — Formatting](#fmt--formatting)
      - [`log` — Logging](#log--logging)
      - [`fs` — File System](#fs--file-system)
      - [`net` — Networking *(planned)*](#net--networking-planned)
      - [`http` — HTTP *(planned)*](#http--http-planned)
      - [`TEST` — Testing Framework](#test--testing-framework)
      - [`main` — Entry Point](#main--entry-point)
  - [Meta System](#meta-system)
  - [플랫폼 지원](#플랫폼-지원)
  - [코드 샘플](#코드-샘플)
    - [Optional Values](#optional-values)
    - [Error Results \& Defer](#error-results--defer)
    - [Pattern Matching](#pattern-matching)
    - [Chaining - Filter, Map, Fold, Reduce](#chaining---filter-map-fold-reduce)
    - [Threads vs Stackless-Coroutines](#threads-vs-stackless-coroutines)
    - [Atomic Values](#atomic-values)
    - [SIMD Vectors](#simd-vectors)
    - [Meta System](#meta-system-1)
    - [Data Structures \& Algorithms](#data-structures--algorithms)
    - [Testing](#testing)
  - [Documentation](#documentation)
  - [명칭에 관하여](#명칭에-관하여)
  - [기여 및 연락처](#기여-및-연락처)
  - [라이선스](#라이선스)

</details>

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

> **참고:** 현재는 **Clang**만 컴파일러로 지원합니다. GCC 지원은 예정되어 있으며 아직 제공되지 않습니다.

#### 사전 요구 사항

- **Clang** 16.0.0+ (권장: 19.1.0+)
- **Make** (GNU Make 또는 호환)

#### Step 1: 레포지토리 클론

```sh
git clone https://github.com/coding-pelican/dasae-headers
cd dasae-headers
```

#### Step 2: `dh-c` 도구 빌드

```sh
cd dh-c
./gen-makefile.sh
make
```

`dh-c` 빌드 도구는 `dh-c/build/dh-c`(Windows에서는 `dh-c/build/dh-c.exe`)에 생성됩니다.

#### Step 3: 환경 변수 설정

다음 두 환경 변수를 설정해야 합니다.

| 변수      | 설명                           | 예시 값                             |
| --------- | ------------------------------ | ----------------------------------- |
| `DH_HOME` | 헤더가 있는 `dh` 디렉터리 경로 | `/path/to/dasae-headers/dh`         |
| `PATH`    | `dh-c` 실행 파일 경로 추가     | `/path/to/dasae-headers/dh-c/build` |

**Linux/macOS (bash/zsh):**

```sh
# ~/.bashrc 또는 ~/.zshrc에 추가
export DH_HOME="/path/to/dasae-headers/dh"
export PATH="/path/to/dasae-headers/dh-c/build:$PATH"
```

이후 셸 설정을 다시 로드하세요.

```sh
source ~/.bashrc  # 또는 source ~/.zshrc
```

**Windows (MSYS2/MinGW):**

```sh
# ~/.bashrc에 추가
export DH_HOME="/c/path/to/dasae-headers/dh"
export PATH="/c/path/to/dasae-headers/dh-c/build:$PATH"
```

**Windows (PowerShell):**

```powershell
# PowerShell 프로필에 추가하거나 시스템 환경 변수로 설정
$env:DH_HOME = "C:\path\to\dasae-headers\dh"
$env:PATH = "C:\path\to\dasae-headers\dh-c\build;$env:PATH"
```

#### Step 4: 설치 확인

```sh
dh-c --version
dh-c --help
```

#### Step 5: 새 프로젝트 생성

```sh
dh-c project myproject
cd myproject
dh-c run
```

#### Hello, world!

```c
#include <dh/main.h>
#include <dh/io/stream.h>

fn_((main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    let message = u8_l("Hello");
    io_stream_println(u8_l("{:s}, world!"), message);
    return_ok({});
} $unscoped_(fn);
```

### 🔨 빌드 및 실행

```sh
# Run directly
dh-c run

# Build executable
dh-c build

# Run tests
dh-c test

# Clean build artifacts
dh-c clean
```

---

## 소개

**dasae-headers**는 C언어에 현대적인 프로그래밍 패러다임을 도입하여
안전성, 표현력, 생산성을 개선하는 것을 목적으로 합니다.
C언어의 설계 원칙인 간결함을 유지하면서, 메모리 안전성 및 타입 안전성을 강화하고
구조화된 에러 처리 메커니즘을 제공합니다.

현재 개발 초기 단계로, C 전처리기를 활용하여 언어적 확장을 구현하는 트랜스파일러 환경을 제공합니다.
이는 표준 C 라이브러리의 구조적 한계를 보완하며 점진적으로 독립적인 개발 생태계를 구축하는 과정에 있습니다.

### 왜 dasae-headers인가?

dasae-headers는 기존 C 환경의 제약을 극복하고 현대적인 개발 경험을 제공하기 위해 아래와 같은 설계 원칙을 고수합니다.

<details>
<summary><strong>설계 원칙</strong></summary>

- **기존 C 생태계와의 공존:**
  기존 C 라이브러리 및 레거시 코드베이스를 수정하지 않고도 현대적 문법과 안전 장치를 즉시 도입할 수 있는 유연성
- **제로 코스트 추상화:**
  인라인화, 전처리 단계 평가, 상수 폴딩/전파 유도 등의 최적화를 통해 고수준 기능을 제공하면서도
  런타임 오버헤드를 이론적 최소치로 수렴
- **점진적 도입 가능:**
  프로젝트 전체를 전환할 필요 없이, 필요한 모듈(에러 처리, 할당자 등)만 선택적으로 연결하여 사용가능
- **Freestanding 및 베어메탈 지원:**
  libc의 사용이 선택적. 임베디드 환경이나 커널 개발 등 프리스텐딩 환경과 같은 시스템의 최저 수준에서도 기능함을 최우선
- **사용자 정의 매크로 최소화:**
  일반적인 활용 및 핵심 비즈니스 로직, 기능 구현을 위해 사용자가 별도의 복잡한 매크로를 작성할 필요가 없음.
  필요시 정립된 메타 시스템 기반 제네릭 인스턴스화 패턴/전처리 시스템을 따라 쉬운 인스턴스화 및 확장을 지원
- **디버깅 친화적 설계:**
  매크로가 런타임 디버깅(Call Stack 추적, Step-by-step 실행 등)을 방해하지 않도록 정교하게 디자인됨.
  복잡한 추상화 도입 시 발생하는 개발 생산성 저하를 원천적으로 방지
- **일관된 컨벤션:**
  엄격하고 일관된 코드 컨벤션과 명명법을 통해 대규모 코드베이스에서도 가독성과 유지보수성을 보장

</details>

---

### 어떻게 기존의 C 사용과 달라지는가?

<details>
<summary><strong>상세 보기</strong></summary>

C의 유연함을 유지하면서도, 현대적인 언어들이 제공하는 안전 장치와 생산성 도구들을
시스템 계층에 최적화된 형태로 이식합니다.
단순히 기능을 추가하는 것을 넘어, 기존 C의 고질적인 설계 결함과 파편화된 컨벤션을
구조적으로 해결하는 데 집중합니다.

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
| **동기화 도구** | 원시적 `mutex`, `cond` 의존               | `RWLock`, `ResetEvent`, `WaitGroup` 등                          |
| **제어 흐름**   | 콜백 지옥 또는 복잡한 상태 머신 수동 구현 | `async_`, `await_`, `suspend_`, `resume_` 구문 기반 코루틴 제어 |

#### 4. 메타 타입 및 제네릭 시스템 (Meta Type & Generic System)

매크로 지옥을 탈피하고 메타 타입 시스템을 통해 타입 안전성을 컴파일 타임에 검증합니다.
특히 C 표준에서 허용되지 않는 익명 사용자 타입 간의 논리적 호환성을 정적으로 보장하는 차별화된 레이어를 제공합니다.
메타 타입 시스템과 `meta` 모듈(레코드 리플렉션)의 동작 및 관계는 [Meta System](#meta-system)을 참고하세요.

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

</details>

---

### 무엇이 dasae-headers를 특별하게 만드는가?

<details>
<summary><strong>상세 보기</strong></summary>

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
따라서 사용자가 라이브러리 호출을 위해 별도의 매크로를 작성하거나
복잡한 매크로 구조를 파악해야 하는 부담 자체를 제거하며,
코어 로직에서 "연산자로서의 매크로"와 "실제 함수 사용"의 구분을 예상 가능한 기준으로 정의합니다.

#### 2. 메타 시스템 및 익명 타입 호환성

**Meta Type 기반 Generic System:**

제네릭 로직을 정적 분석의 사각지대인 매크로가 아닌 메타 타입 시스템(`u_*`)으로 구현합니다.
LTO, 상수 폴딩과 상수 전파에 의해 매크로 인라인화 코드와 동일하게 최적화되는 제로 코스트 추상화 레이어를 제공합니다.
전체 내용(메타 타입 시스템 + `meta` 모듈 및 관계)은 [Meta System](#meta-system)을 참고하세요.

**구조적 익명 타입 호환성 검증:**

반드시 `typedef` 에일리어싱을 거쳐야만 제네릭을 허용하는 타 라이브러리와 달리,
익명 사용자 타입을 효과적으로 수용합니다.
필드의 메모리 레이아웃 및 논리적 구조(Size, Align, Offset)가 동일한 경우에 한해
안전성을 컴파일 타임에 정적으로 검증하며, 익명 타입 간의 안전한 상호작용을 보장합니다.

#### 3. 제로 오버헤드 안전 보장 (Zero-overhead Safety)

**컴파일 타임 평가 우선 연산 및 캐스팅 검증:**

산술 연산의 `Overflow`, `DivisionByZero`, `NaN` 감지 및 부적절한 타입 캐스팅을 정적으로 감지합니다.
모든 안전 검증은 컴파일 타임 평가를 우선하며, 런타임에서는 `assertion`으로서 기능합니다.
릴리즈 모드에서는 이러한 검증 항목들이 분기 최적화 대상이 되어 바이너리 오버헤드 없이 안전성을 확보하며,
개발 단계에서는 기존 C 사용보다 더 빠르게 문제 원인을 정확히 특정할 수 있습니다.

</details>

---

## 주요 기능

본 프로젝트는 Zig와 Rust의 문법 구조 및 표준 라이브러리 설계 방식을 참고하여 개발되었습니다.

- **메모리 안전성:**
  커스텀 할당자, 메모리 추적, 경계 검사, `defer_`/`errdefer_` 기반 자동 리소스 관리
- **강화된 타입 시스템:**
  컴파일 타임 검사, 메타 타입 시스템, 대수적 데이터 타입(Variant), 옵셔널 타입
- **명시적 오류 처리:**
  `ok`/`err` 키워드, `try_`/`catch_` 패턴, 호출 스택을 포함한 에러 추적
- **현대적 구문:**
  타입 추론(`let`/`var`), 함수 정의(`fn_`), 람다 표현식(`la_`), 일급 객체 `Callable` 타입
- **개발 도구:**
  내장 테스트 프레임워크, 주요 C 컴파일러(Clang, GCC) 및 멀티 플랫폼 지원

---

## Modules Reference

> Legend: Items marked with *(planned)* are under development and not yet publicly available.

<details>
<summary><strong>Core Language Extensions</strong></summary>

#### `builtin` — Compiler & Platform Abstraction

Preprocessor utilities, platform/compiler detection, source location tracking,
type and container introspection.

- **Submodules:**
  `pp`, `lang_cfg`, `arch_cfg`, `plat_cfg`, `comp_cfg`, `comp`, `src_loc`,
  `static_assert`, `auto`, `lambda`, `type_info`, `container_info`, `mem`, `atom`
- **`pp` (preprocessor):** Implements compile-time metaprogramming as preprocessor macros:
  - **Token comparison:**
    `pp_eq`, `pp_ne`, `pp_and`, `pp_or`, `pp_xor`, `pp_not`, `pp_Tok_eq`
    — token equality and logic
  - **Control flow:**
    `pp_if_` / `pp_then_` / `pp_else_` — conditional;
    `pp_switch_` / `pp_case_` / `pp_default_` — dispatch
  - **Optionals:**
    `pp_some`, `pp_none`, `pp_isSome`, `pp_isNone`, `pp_orelse_`
    — preprocessor optional values
  - **Utilities:**
    `pp_strfy`, `pp_cat`, `pp_join`, `pp_join2`/`pp_join3`/`pp_join4`, `pp_uniqTok`,
    `pp_overload`, `pp_foreach`, `pp_countArg`, `pp_defer`, `pp_expand`, `nameOf`
- **Platform/compiler:**
  `lang_cfg`, `arch_cfg`, `plat_cfg`, `comp_cfg` — detection macros
- **Source location:**
  `srcLoc$` — compile-time source location capture
- **Type introspection (builtin):**
  `typeInfo$`, `sizeOf$`, `alignOf$`, `TypeOf`, `TypeOfUnqual`, `Type_eq$`,
  `RefType$`, `DerefType$`
- **Container/field introspection:**
  `FieldType$`, `FieldTypeUnqual$`, `FieldType_eq$`, `offsetTo`, `fieldPtr`
  (in `container_info`)

#### `core` — Language Primitives & Syntax Extensions

Primitives, function syntax, assertions, debugging, scoped resource management,
and safe arithmetic.

- **Submodules:**
  `prim`, `fn`, `Callable`, `claim`, `debug`, `range`, `op`, `cmp`, `pipe`, `chain`,
  `blk`, `scope`, `src_loc`, `type_info`, `struct_layout`
- **Primitive types:**
  `bool`, `i8`..`i64`, `u8`..`u64`, `isize`, `usize`, `f32`, `f64`
- **Syntax:**
  `fn_`, `la_` — function and lambda; `Callable` — first-class callable
- **Assertions:**
  `claim_assert`, `claim_unreachable`; `debug_assert`, `debug_only`
- **Control/scope:**
  `R`, `for_` — range iteration; `defer_`, `errdefer_` — scoped cleanup
- **Chaining:**
  `pipe_`, `chain$`, `each_`, `filter_`, `map$`, `fold_`, `reduce_`, `all_`, `any_`
- **Core primitives (arithmetic, bitwise, comparison):**
  `prim_add`, `prim_sub`, `prim_mul`, `prim_div`, `prim_rem`, `prim_neg`, `prim_abs`,
  `prim_sgn`, `prim_not`, `prim_shl`, `prim_shr`, `prim_and`, `prim_xor`, `prim_or`,
  `prim_eql`, `prim_neq`, `prim_ord`, `prim_min`, `prim_max`, `prim_clamp`,
  `prim_bitSet`, `prim_bitReset`, `prim_maskLo$`, `prim_maskHi$`,
  `prim_memcpy`, `prim_memset`, `prim_memmove`, `prim_memeql`, `bitCast$`
- **Safe integer ops (overflow-checked, in `prim`):**
  `int_add`, `int_sub`, `int_mul`, `int_div`, `int_rem`, `int_divTrunc`, `int_divRound`,
  `int_divCeil`, `iint_divFloor`, `iint_divEuclid`, `iint_mod`, `intCast$`
- **Integer/float division variants:**
  `int_remRound`, `int_modCeil`; `flt_divTrunc`, `flt_divFloor`, `flt_rem`, `flt_mod`
- **Type predicates & limits:**
  `isBool$`, `isUInt$`, `isIInt$`, `isFlt$`, `int_bits$`, `uint_limit$`,
  `int_limit_min$`, `int_limit_max$`, `flt_limit_min$`, `flt_limit_max$`,
  `flt_nan$`, `flt_inf$`
- **Type info (core):**
  `typeInfo$` — runtime `TypeInfo` (size, align) for a type

#### `prl` — Prelude Types

Core algebraic types: Optional, Error Result, Slice, Array, Variant.
(Safe arithmetic lives in `core`/`prim`.)

- **Submodules:**
  `types` (`raw`, `Val`, `Ptr`, `Arr`, `Sli`, `Opt`, `ErrRes`, `Err`, `variant`, `meta`),
  `common`, `ErrTrace`, `int`, `flt`
- **Key Types:**
  - `O$(T)` (Optional) — `some(v)`, `none()`, `if_some((opt)(capture))`, `orelse_((opt)(default))`, `unwrap_(opt)`
  - `E$(T)` (Error Result) — `ok(v)`, `err(e)`, `try_(expr)`, `catch_((expr)(err, block))`, `return_ok`, `return_err`
  - `S$(T)` (Slice) — `S_deref$`, `S_at`, `S_slice`, `S_prefix`, `S_suffix`, `S_len`
  - `A$(N, T)` (Array) — `A_zero`, `A_init$`, `A_from$`, `A_ref$`, `A_len`
  - `variant_` — Tagged union; create with `union_of$`, match with `match_`, `pattern_`
  - `ErrTrace` — Error tracing with call stack information
- **prl/int, prl/flt:**
  Per-type safe wrappers (e.g. `u8_add`, `u32_div`, `i64_mod`) with debug overflow checks;
  see `core`/`prim` for generic `int_add`, `intCast$`, etc.
- **Zero-cost meta type system** (`prl/types/meta`):
  Type-erased generic layer over PRL types so algorithms can work on values
  without knowing the concrete type at compile time.
  How it works and how it relates to the `meta` module (record/type reflection)
  are described in [Meta System](#meta-system).

</details>

<details>
<summary><strong>SIMD & Math</strong></summary>

#### `simd` — SIMD Vector Operations

Architecture-independent SIMD operations (AVX, SSE, NEON) using compiler vector extensions.

- **Types:**
  `Vec$(N, T)` — e.g., `Vec$(4, f32)`, `Vec$(8, f32)`, `Vec$(4, i32)`, `Vec$(2, f64)`
- **Initialization:**
  `Vec_init$`, `Vec_splat$`, `Vec_from$`, `Vec_fromA$`, `Vec_toA$`, `Vec_cat$`
- **Arithmetic:**
  `Vec_add`, `Vec_sub`, `Vec_mul`, `Vec_div`, `Vec_neg`, `Vec_abs`, `Vec_fma`
- **Comparison:**
  `Vec_eq`, `Vec_ne`, `Vec_lt`, `Vec_le`, `Vec_gt`, `Vec_ge`
- **Min/Max:**
  `Vec_min`, `Vec_max`, `Vec_clamp`
- **Reduction:**
  `Vec_reduceAdd`, `Vec_reduceMul`, `Vec_reduceMin`, `Vec_reduceMax`
- **Shuffle:**
  `Vec_shuffle$`, `Vec_select`
- **Math:**
  `Vec_sqrt`, `Vec_floor`, `Vec_ceil`, `Vec_round`, `Vec_trunc`

#### `cmp` — Comparison Utilities

- **`cmp_Ord`** — Three-way ordering type:
  `cmp_Ord_lt` (−1), `cmp_Ord_eq` (0), `cmp_Ord_gt` (1).
  Represents the mathematical sign of (lhs − rhs): strictly less, equal, or strictly greater.
  Used when a total (or partial) order exists;
  all ordering-derived predicates are defined in terms of `cmp_ord$(_T)(lhs, rhs)`.
- **`cmp_Eql`** — Equality-only comparison (no order).
  Used when only equivalence is defined:
  `cmp_eql$(_T)(lhs, rhs)` and `cmp_neq$(_T)(lhs, rhs)`.
- **Derived names and defaults:**
  - **ord** — Three-way comparison; default direction is *ascending* (smaller-first).
    `cmp_OrdFn_defaultAsc` / `cmp_OrdFn_defaultDesc` (in `cmp.h`)
    select ascending or descending.
  - **eql** / **neq** — Equality and its negation; *not* derived from order.
    Implement `cmp_eql$(_T)` (and optionally `cmp_neq$(_T)`;
    the other is defaulted as its negation).
  - **eq** / **ne** — Equality/inequality *derived from* `cmp_ord$`:
    `eq` ⇔ (ord == cmp_Ord_eq), `ne` ⇔ (ord != cmp_Ord_eq).
    So for ordered types, `cmp_eq$`/`cmp_ne$` are defaulted from `cmp_ord$`;
    for equality-only types you implement `cmp_eql$`/`cmp_neq$` only.
- **Defining ordering (ord vs lt):**
  You can supply ordering for a type in either of two ways.
  **(1) ord defined:** Implement `cmp_ord$(_T)(lhs, rhs)` returning `cmp_Ord` (three-way).
  Then `cmp_eq$`, `cmp_ne$`, `cmp_lt$`, `cmp_gt$`, `cmp_le$`, `cmp_ge$` are defaulted from it
  via the `cmp_fn_*_default$` macros.
  **(2) lt only:** Implement only `cmp_lt$(_T)(lhs, rhs)` (strict less-than, returns bool).
  Use `cmp_fn_ord_default$(_T)` to derive ord from lt
  (ord = lt(lhs,rhs) ? lt : lt(rhs,lhs) ? gt : eq);
  then eq/ne/lt/gt/le/ge are defaulted from that ord as in (1).
  The same pattern applies to context and approximate variants: `ordCtx`/`ltCtx`, `ordApx`/`ltApx`.
- **Distinction:**
  **eql**/**neq** are the *primitive* equality interface
  (reflexive, symmetric, and for consistent types transitive).
  **eq**/**ne** are the *order-derived* equality:
  they coincide with eql/neq when the order is total and consistent with equality,
  but eq/ne are defined only when `cmp_ord$` exists.
  Use eql/neq for types that have equality but no order (e.g. slices by content);
  use eq/ne when you have an order and want (ord == cmp_Ord_eq) as equality.
- **Predicates on `cmp_Ord`:**
  `cmp_Ord_isLt`, `cmp_Ord_isEq`, `cmp_Ord_isGt`, `cmp_Ord_isLe`, `cmp_Ord_isGe`, `cmp_Ord_inv`

#### `math` — Mathematical Functions

- **Submodules:**
  `common`, `vec`/`vec_types`, `mat`/`mat_types`, `quat`/`quat_types`
- **Common (prefix `math_`):**
  `math_abs`, `math_min`, `math_max`, `math_clamp`, `math_sign`, `math_wrap`,
  `math_floor`, `math_ceil`, `math_round`, `math_trunc`, `math_sqrt`, `math_pow`, `math_rsqrt`,
  `math_sin`, `math_cos`, `math_tan`, `math_asin`, `math_acos`, `math_atan`, `math_atan2`;
  constants e.g. `math_pi`, `math_f32_pi`, `math_limit_min$`, `math_limit_max$`.
- **Vectors (types `m_V2f32`, `m_V3f32`, `m_V4f32`, and f64/i32/i64 variants; prefix `m_V*`):**
  `m_V*_of`, `m_V*_splat`, `m_V*_add`, `m_V*_sub`, `m_V*_mul`, `m_V*_div`, `m_V*_dot`, `m_V*_cross`,
  `m_V*_norm`, `m_V*_len`, `m_V*_lenSq`, `m_V*_dist`, `m_V*_min`, `m_V*_max`, `m_V*_clamp`, `m_V*_rotate`
- **Matrices (types `m_M2f32`, `m_M3f32`, `m_M4f32`; prefix `m_M*`):**
  `m_M*_identity`, `m_M*_ofCols`, `m_M*_ofRows`, `m_M*_mulM`, `m_M*_mulV`, `m_M*_transpose`,
  `m_M*_det`, `m_M*_inv`, `m_M*_rotate`, `m_M*_scale`, `m_M*_scaleUniform`
- **Quaternions (type `m_Q4f32`; prefix `m_Q4f32_`):**
  `m_Q4f32_identity`, `m_Q4f32_of`, `m_Q4f32_mul`, `m_Q4f32_mulQ`, `m_Q4f32_conj`, `m_Q4f32_invQ`,
  `m_Q4f32_norm`, `m_Q4f32_slerp`, `m_Q4f32_fromAxisAngle`, `m_Q4f32_fromM3`/`fromM4`, `m_Q4f32_toM3`/`toM4`

#### `Rand` — Random Number Generation

Random number generator (struct `Rand`, prefix `Rand_`).

- **Init/seed:**
  `Rand_init`, `Rand_initSeed`, `Rand_withSeed`, `Rand_setSeed`
- **Next:**
  `Rand_nextUInt`, `Rand_next$usize`, `Rand_next$u64`, `Rand_next$u32`,
  `Rand_next$u16`, `Rand_next$u8`, `Rand_nextIInt`, `Rand_next$isize`,
  `Rand_next$i64`/…, `Rand_nextFlt`, `Rand_next$f64`, `Rand_next$f32`
- **Ranges:**
  `Rand_rangeUInt`, `Rand_rangeIInt`, `Rand_rangeFlt`

</details>

<details>
<summary><strong>Algorithms</strong></summary>

#### `search` — Searching Algorithms

| Function             | Time     | Description                              |
| -------------------- | -------- | ---------------------------------------- |
| `search_linearFirst` | O(N)     | Find first matching index (forward scan) |
| `search_linearLast`  | O(N)     | Find last matching index (backward scan) |
| `search_binary`      | O(log N) | Binary search on partitioned sequence    |
| `search_lowerBound`  | O(log N) | First index where element ≥ target       |
| `search_upperBound`  | O(log N) | First index where element > target       |
| `search_partPoint`   | O(log N) | Partition point where predicate changes  |
| `search_eqRange`     | O(log N) | Range of indices equal to target         |

#### `sort` — Sorting Algorithms

Optimal stable and unstable sorting functions isolated by auxiliary memory constraints.

| Function          | Stability | Time       | Space    | Description                                      |
| ----------------- | --------- | ---------- | -------- | ------------------------------------------------ |
| `sort_insert`     | Stable    | O(N²)      | O(1)     | Insertion sort for small/partially sorted arrays |
| `sort_heap`       | Unstable  | O(N log N) | O(1)     | Heapsort for strict O(1) memory constraints      |
| `sort_pdq`        | Unstable  | O(N log N) | O(log N) | Pattern-Defeating Quicksort, adaptive            |
| `sort_block`      | Stable    | O(N log N) | O(1)     | Block sort (WikiSort) with internal buffers      |
| `sort_blockCache` | Stable    | O(N log N) | O(K)     | Block sort with provided cache buffer            |
| `sort_blockAlloc` | Stable    | O(N log N) | O(K)     | Block sort with allocator-provided buffer        |

- **Context variants:**
  `sort_insertCtx`, `sort_heapCtx`, `sort_pdqCtx`, `sort_blockCtx`
- **Index-based variants:**
  `sort_insertIdx`, `sort_heapIdx`, `sort_pdqIdx` (for non-contiguous layouts)
- **Utilities:**
  `sort_inOrdd` (check if sorted)

</details>

<details>
<summary><strong>Data Structures</strong></summary>

#### Linked Lists

| Module        | Description        | Key Functions                                          |
| ------------- | ------------------ | ------------------------------------------------------ |
| **`ListSgl`** | Singly linked list | `empty`, `prepend`, `remove`, `shift`                  |
| **`ListDbl`** | Doubly linked list | `empty`, `prepend`, `append`, `remove`, `shift`, `pop` |

#### Tree Structures *(planned)*

| Module         | Description                          | Key Functions |
| -------------- | ------------------------------------ | ------------- |
| **`BTree`**    | B-tree *(planned)*                   | —             |
| **`BTreeMap`** | B-tree based ordered map *(planned)* | —             |
| **`BTreeSet`** | B-tree based ordered set *(planned)* | —             |
| **`SegTree`**  | Segment tree *(planned)*             | —             |

#### Array-Based Containers

| Module        | Description                                   | Key Functions                                                                                           |
| ------------- | --------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| **`ArrList`** | Dynamic array list with amortized O(1) append | `init`, `fini`, `append`, `appendS`, `pop`, `insert`, `removeOrdd`, `removeSwap`, `resize`, `ensureCap` |
| **`ArrStk`**  | Array-based stack (LIFO)                      | `init`, `fini`, `push`, `pop`, `top`, `isEmpty`                                                         |
| **`ArrDeq`**  | Array-based double-ended queue                | `init`, `fini`, `append`, `prepend`, `pop`, `shift`                                                     |
| **`ArrQue`**  | Array-based queue (FIFO)                      | `init`, `fini`, `enque`, `deque`, `front`                                                               |
| **`ArrPDeq`** | Array-based priority double-ended queue       | `init`, `fini`, `enque`, `popMin`, `popMax`, `peekMin`, `peekMax`                                       |
| **`ArrPQue`** | Array-based priority queue (binary heap)      | `init`, `fini`, `enque`, `deque`, `peek`, `update`                                                      |

#### Hash-Based Containers

| Module           | Description                                             | Key Functions                                                                      |
| ---------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| **`Hash`**       | Hash utilities *(planned)*                              | —                                                                                  |
| **`HashMap`**    | Hash map with open addressing                           | `init`, `fini`, `put`, `by`, `ptrBy`, `for`, `entry`, `contains`, `remove`, `iter` |
| **`HashSet`**    | Hash set with open addressing                           | `init`, `fini`, `put`, `ensure`, `contains`, `remove`, `iter`                      |
| **`HashMapSeq`** | Ordered hash map preserving insertion order *(planned)* | —                                                                                  |
| **`HashSetSeq`** | Ordered hash set preserving insertion order *(planned)* | —                                                                                  |

</details>

<details>
<summary><strong>Memory Management</strong></summary>

#### `mem` — Memory Utilities

Low-level memory operations with type-safe wrappers.

- **Submodules:** `common`, `Allocator`, `Tracker`
- **Bit Operations:**
  `mem_trailingZeros{8,16,32,64,Size}`, `mem_leadingZeros{8,16,32,64,Size}`
- **Byte Swap:**
  `mem_byteSwap{16,32,64,Size}`
- **Endian Conversion:**
  `mem_littleToNative*`, `mem_bigToNative*`, `mem_nativeToLittle*`, `mem_nativeToBig*`
- **Alignment:**
  `mem_isValidAlign`, `mem_isAligned`, `mem_alignFwd`, `mem_alignBwd`, `mem_alignToLog2`
- **Memory Operations:**
  - `mem_copy`, `mem_move`, `mem_set`, `mem_set0` — Copy, move, fill
  - `mem_eql`, `mem_ord`, `mem_eq`, `mem_ne`, `mem_lt`, `mem_gt`, `mem_le`, `mem_ge`
    — Comparison
  - `mem_swap`, `mem_reverse`, `mem_rotate` — Manipulation
  - `mem_startsWith`, `mem_endsWith` — Pattern matching
- **Iterators:**
  - `mem_TokenIter` — Tokenizer with value/pattern/choice delimiters
  - `mem_tokenizeValue`, `mem_tokenizePattern`, `mem_tokenizeChoice`
  - `mem_SplitIter`, `mem_WindowIter` *(planned)*
- **Allocator Interface:**
  `mem_Allocator` — Unified allocator interface
- **Tracker:**
  `mem_Tracker` — Memory leak and double-free detection

#### `heap` — Heap Allocators

| Allocator      | Description                                              |
| -------------- | -------------------------------------------------------- |
| **`Classic`**  | Traditional heap allocation (C runtime `malloc`/`free`)  |
| **`Page`**     | Page-aligned allocation for OS-mapped memory blocks      |
| **`Sbrk`**     | Sbrk-based allocation for linear memory growth and reuse |
| **`Sys`**      | System-dependent allocation (Page or Sbrk)               |
| **`Fixed`**    | Fixed-size block allocator for bulk operations           |
| **`ThrdSafe`** | Thread-safe wrapper for any allocator                    |
| **`Smp`**      | SMP-aware allocation with per-core caching               |
| **`Arena`**    | Region-based allocation for bulk operations              |
| **`Pool`**     | Pool-based allocation for object reuse                   |

#### `meta` — Runtime Record/Type Reflection

Record layout and field access from `TypeInfo`, operating on the **meta type system**
(`u_P$raw`, `u_S$raw`).
For the relationship between this module and the meta type system (prl/types/meta),
see [Meta System](#meta-system).
Compile-time type info is in `builtin`/`core` (`typeInfo$`, `sizeOf$`, `alignOf$`).

- **Record from fields:**
  `u_typeInfoRecord`, `u_sizeOfRecord`, `u_alignOfRecord`
- **Field offsets:**
  `u_offsetTo`, `u_offsets`
- **Field/record pointers (meta):**
  `u_fieldPtr`, `u_fieldPtrMut`, `u_fieldPtrs`, `u_fieldPtrsMut`,
  `u_recordPtr`, `u_recordPtrMut`
- **Array type info:**
  `u_typeInfoA`, `u_sizeOfA`, `u_alignOfA`
- **N-replicated records:**
  `u_typeInfoRecordN`, `u_offsetToN`, `u_offsetsN`, `u_fieldSli`, `u_fieldSliMut`,
  `u_fieldSlis`, `u_fieldSlisMut`, `u_recordNPtr`, `u_recordNPtrMut`

</details>

<details>
<summary><strong>Concurrency</strong></summary>

#### `atom` — Atomic Operations

C11 atomics and type-safe atomic value wrappers (prefix `atom_`).

- **Ordering:**
  `atom_MemOrd` — `unordered`, `monotonic`, `acquire`, `release`, `acq_rel`, `seq_cst`
- **Operations:**
  `atom_fence`, `atom_load`, `atom_store`, `atom_fetchXchg`,
  `atom_cmpXchgWeak$`, `atom_cmpXchgStrong$`,
  `atom_fetchAdd`, `atom_fetchSub`, `atom_fetchAnd`, `atom_fetchOr`,
  `atom_fetchXor`, `atom_fetchNand`
- **Atomic value wrapper:**
  `atom_V$(_T)`, `atom_V_zero$`, `atom_V_init$`, `atom_V_from`
  — generic atomic variable type

#### `Thrd` — Threading

OS thread management and synchronization primitives.

| Primitive        | Description                          |
| ---------------- | ------------------------------------ |
| **`Thrd`**       | Thread creation, spawn, join, detach |
| **`Ftx`**        | Futex (fast userspace mutex)         |
| **`Mtx`**        | Mutex                                |
| **`Sem`**        | Semaphore                            |
| **`Cond`**       | Condition variable                   |
| **`RWLock`**     | Read-write lock                      |
| **`ResetEvent`** | Manual/auto reset event              |
| **`WaitGroup`**  | Wait for multiple tasks              |

- **Key Functions:**
  `Thrd_spawn`, `Thrd_join`, `Thrd_detach`, `Thrd_current`, `Thrd_yield`, `Thrd_sleep`

#### `Co` / `async` — Stackless Coroutines

State-machine-based coroutines for ultra-lightweight async processing.

- **Syntax:**
  `async_fn_`, `await_`, `suspend_`, `resume_`, `areturn_`
- **Context:**
  `Co_Ctx`, `Co_CtxFn$`

#### `mp` — Multi-Processing *(planned)*

Multi-processing utilities for parallel workloads.

</details>

<details>
<summary><strong>Text & Encoding</strong></summary>

#### `ascii` — ASCII character utilities

`ascii_isAlpha`, `ascii_isDigit`, `ascii_isSpace`, `ascii_toLower`, `ascii_toUpper`

#### `utf8` — UTF-8 encoding/decoding

`utf8_encode`, `utf8_decode`, `utf8_isValid`, `utf8_validate`, `utf8_count`, `utf8_view`, `utf8_iter`

#### `utf16` — UTF-16 encoding/decoding

`utf16_encode`, `utf16_decode`, `utf16_codepointSeqLen`, `utf16_isHighSurrogate`, `utf16_isLowSurrogate`

#### `wtf8` — WTF-8 (UTF-8 superset for Windows)

`wtf8_encode`, `wtf8_decode`, `wtf8_view`, `wtf8_iter`

#### `wtf16` — WTF-16 encoding

`wtf16_iter`, `wtf16_Iter_next`

#### `unicode` — Unicode conversion hub

`unicode_utf8ToUTF16`, `unicode_utf16ToUTF8`, `unicode_wtf8ToWTF16`, `unicode_wtf16ToWTF8`

</details>

<details>
<summary><strong>System & OS</strong></summary>

#### `os` — OS-Specific APIs

- **Windows:**
  `cfg`, `base`, `handle`, `debug`, `nls`, `sysinfo`, `mem`, `file`, `io`, `dll`,
  `console`, `proc`, `thrd`, `sync`, `nt`, `auth`, `crypt`, `sock`

#### `posix` — POSIX Compatibility

POSIX API compatibility layer for cross-platform code.

#### `proc` — Process Management *(planned)*

Process management utilities for cross-platform code.

#### `time` — Time & Duration

- **Submodules:**
  `cfg`, `common`, `Duration`, `Instant`, `SysTime`
- **Duration:**
  `time_Duration_fromSecs`, `time_Duration_fromMillis`, `time_Duration_fromNanos`,
  `time_Duration_add`, `time_Duration_sub`
- **Instant:**
  `time_Instant_now`, `time_Instant_elapsed`, `time_Instant_durationSince`
- **SysTime:**
  `time_SysTime_now`
- **Sleep:**
  `time_sleep`

</details>

<details>
<summary><strong>I/O & Formatting, Filesystem</strong></summary>

#### `io` — Input/Output

- **Submodules:** `common`, `stream`, `Reader`, `Writer`, `Fixed`, `Buf`
- **Stream:**
  `io_stream_print`, `io_stream_println`, `io_stream_eprint`, `io_stream_eprintln`, `io_stream_nl`
- **Reader:**
  `io_Reader_read`, `io_Reader_readExact`, `io_Reader_readByte`, `io_Reader_skip`
- **Writer:**
  `io_Writer_write`, `io_Writer_writeBytes`, `io_Writer_writeByte`,
  `io_Writer_print`, `io_Writer_println`, `io_Writer_nl`
- **Fixed (in-memory fixed buffer):**
  `io_Fixed_reading`, `io_Fixed_writing`, `io_Fixed_written`, `io_Fixed_reset`,
  `io_Fixed_Reader_init`, `io_Fixed_reader`, `io_Fixed_Writer_init`, `io_Fixed_writer`
- **Buf (buffered Reader/Writer):**
  `io_Buf_Reader_init`, `io_Buf_Reader_fill`, `io_Buf_Reader_peekByte`, `io_Buf_Reader_readUntilByte`,
  `io_Buf_Reader_skipUntilByte`, `io_Buf_Reader_skip`, `io_Buf_reader`,
  `io_Buf_Writer_init`, `io_Buf_Writer_flush`, `io_Buf_writer`

#### `fmt` — Formatting

String formatting and parsing with a spec system (prefix `fmt_`).

- **Submodules:** `common`, `cfg`
- **Format API:**
  `fmt_format`, `fmt_formatVaArgs` — format to writer;
  `fmt_Spec`, `fmt_Type`, `fmt_Size`, `fmt_Align`, `fmt_Sign`
- **Format by type:**
  `fmt_formatBool`, `fmt_format$bool`,
  `fmt_formatUInt`, `fmt_format$usize`/`$u64`/`$u32`/…,
  `fmt_formatIInt`, `fmt_format$isize`/…,
  `fmt_formatFlt`, `fmt_formatPtr`, `fmt_formatErr`,
  `fmt_formatStr`, `fmt_formatStrZ`, `fmt_formatASCII`, `fmt_formatUTF8`
- **Parse:**
  `fmt_parseBool`, `fmt_parse$bool`,
  `fmt_parseUInt`, `fmt_parse$usize`/…,
  `fmt_parseIInt`, `fmt_parseFlt`, …
- **Type specifiers (fmt_Type):**
  `{:x}`/`{:X}` (hex), `{:o}` (octal), `{:b}` (binary), `{:B}` (boolean),
  `{:u}` (unsigned), `{:di}` (signed), `{:f}`/`{:F}` (float), `{:p}`/`{:P}` (pointer),
  `{:e}` (error), `{:0}` (void), `{:c}` (ASCII code), `{:C}` (UTF-8 codepoint),
  `{:z}` (null-terminated string), `{:s}` (slice string)
- **Float conversion:**
  Ryu-based fast float-to-string (configurable via `fmt_cfg`)

#### `log` — Logging

- **Levels:** `log_debug`, `log_info`, `log_warn`, `log_error`

#### `fs` — File System

- **Submodules:** `common`, `path`, `File`, `Dir`
- **File:**
  `fs_File_close`, `fs_File_reader`, `fs_File_writer`, `fs_File_Handle_promote`
- **Dir:**
  `fs_Dir_create`, `fs_Dir_close`, `fs_Dir_openDir`, `fs_Dir_openFile`,
  `fs_Dir_createFile`, `fs_Dir_deleteFile`, `fs_Dir_deleteDir`,
  `fs_Dir_readFile`, `fs_Dir_readFileAlloc`
- **Path:**
  `fs_path_join`, `fs_path_dirname`, `fs_path_basename`
  *(partial — `fs_path_extension` planned)*

#### `net` — Networking *(planned)*

Networking utilities for cross-platform code.

#### `http` — HTTP *(planned)*

HTTP client/server utilities for cross-platform code.

</details>

<details>
<summary><strong>Testing & Entry Point</strong></summary>

#### `TEST` — Testing Framework

- **Macros:**
  `TEST_fn_`, `TEST_expect`, `TEST_expectMsg`, `TEST_expectEq`, `TEST_expectNe`
- **Usage:**
  Define tests with `TEST_fn_`, run with `dh-c test`

#### `main` — Entry Point

- **Macro:**
  `fn_((main(S$S_const$u8 args))(E$void) $scope)`
  — Standard entry point with argument parsing and error handling

</details>

---

## Meta System

The meta system has two related parts: the **meta type system** (type-erased generics)
and the **meta module** (record/type reflection).
Both use the same `u_` prefix and share `TypeInfo` from `core`/`type_info.h`.

---

**1. Meta type system** (`prl/types/meta.h`, included via `prl/types.h`)

A **zero-cost generic layer** over PRL types so algorithms can operate on values
without knowing the concrete type at compile time.

- **What it is:**
  Type-erased wrappers that carry a raw representation (pointer, slice, array, optional, or result)
  plus runtime **type information** (`TypeInfo`: size, align).
  A single implementation can handle any type that fits the same shape
  (e.g. any slice `S$T` as `u_S$raw`).

- **How it works:**
  - **Unified meta types:**
    `u_V$raw` (value), `u_P$raw` / `u_P_const$raw` (pointer),
    `u_S$raw` / `u_S_const$raw` (slice), `u_A$raw` (array), `u_O$raw` (optional),
    `u_E$raw` (error result).
    Each stores the same underlying pointer/slice/layout as the typed form plus `TypeInfo`,
    so there is no extra indirection.
  - **Conversion:**
    Typed → meta: `u_retV$(_T)` etc. from a type;
    `u_anyP(_p)`, `u_anyS(_s)`, `u_anyV(_v)`, `u_anyA(_a)`, `u_anyO(_o)`, `u_anyE(_e)` from a value.
    Meta → typed: `u_castP$((_T)(meta))`, `u_castV$((_T)(meta))`, `u_castS$((_T)(meta))`,
    `u_castA$((_N,_T)(meta))`, `u_castO$((_OT)(meta))`, `u_castE$((_ET)(meta))`.
  - **Generic operations:**
    Slicing/indexing use `TypeInfo` for stride and length
    (`u_atS`, `u_sliceP`, `u_sliceS`, `u_prefixP`, `u_suffixS`, …).
    Memory and comparison use size/align and optional custom eql/ord
    (`u_memcpy`, `u_memeql`, `u_memord`, `u_eql`, `u_ord`, `u_lt`, …;
    `u_*By` and `u_*Ctx` for custom comparators).
    `TypeInfo.align` is log2-encoded; use `mem_log2ToAlign()` when you need byte alignment.

- **Zero-cost:**
  No vtables or runtime dispatch; only `TypeInfo` and inlined logic.
  With LTO and constant folding, meta-based code optimizes like hand-written typed code.
  In release builds, assertions become compiler optimization hints.
  **Anonymous type compatibility:** when layout (size, align, offsets) matches,
  anonymous types interoperate with meta without a typedef.

- **Allocation, type safety, and value semantics:**

  **1. Dynamic allocation**

  Stack allocation is built in: `u_allocV(_type)`, `u_make(_type)`, `u_create(_type)`
  allocate a single value with `alloca`;
  `u_allocA(_len, _type)` allocates an array of values on the stack.
  All return meta wrappers (e.g. `u_V$raw`, `u_P$raw`, `u_A$raw`)
  whose referenced memory lives in the current stack frame.
  For heap allocation, the meta system does not allocate itself:
  you obtain a pointer (or slice) from your allocator,
  then wrap it with the meta layer using `u_init$S`, `u_anyP`, `u_anyS`, etc.
  So stack vs heap is determined by where the underlying pointer comes from;
  the meta layer only attaches `TypeInfo` and the same operations apply.

  **2. Type safety**

  The meta system is **not fully type-safe** in the static sense:
  you can cast to a wrong type with `u_cast*` and misuse memory.
  It provides **weak type safety** in that it
  **preserves the memory range and boundaries of a type**:
  `TypeInfo` (size, align) is carried with every pointer and slice,
  and operations (copy, compare, index, slice) use that information
  so that accesses stay within the described size and alignment.
  Unlike `void*`, meta types carry layout information, enabling bounds-checked operations.
  The responsibility to use the correct target type when casting remains with the caller.

  **3. Values vs pointers/slices; avoiding dangling pointers; lifetime**

  Elements allocated on the stack (via `u_allocV`, `u_make`, etc.) are exposed as pointers
  in the meta representation (e.g. `u_V$raw` holds a pointer `inner` plus `TypeInfo`).
  To prevent dangling pointers and keep lifetime clear across stack frames,

  **meta values are treated as copy semantics**.

  - **Values are copied.**
    When a value is passed to another function,
    the *content* referred to by the meta value is copied:
    the callee receives (or constructs) a meta value that refers to **its own** memory—
    e.g. the callee uses `u_load(_v)` to get a new `u_V$raw`
    whose `inner` points to a buffer in the callee's frame
    (or the caller allocates and copies before the call).
    So although the meta representation is "pointer + type",
    each logical copy has its own memory.
    That gives **immutability between original and copies**:
    the caller's and callee's storage are independent.

    When the callee returns a value,
    the referenced memory is copied back to the caller's side for use
    (e.g. into the caller's stack or a buffer the caller owns),
    so the caller again has its own copy.
    The **lifetime of the referenced pointer is therefore clear**:
    the callee's meta value refers to storage that lives in the callee's scope
    (or storage the callee owns); the caller's refers to the caller's.
    No shared reference to the other frame's stack.

  - **Why meta-value types when we have pointer and slice types?**
    Pointers (`u_P$raw`) and slices (`u_S$raw`) are **by-reference**:
    they share the same underlying memory.
    They are used when the lifetime of the referent is known to outlive the use
    (e.g. caller-owned buffer, or heap).
    The **value type** (`u_V$raw`) exists to express **by-value** semantics:
    "this is a value that, when passed across boundaries, is copied."
    So for passing data across stack frames or to callees
    that should not alias the caller's storage,
    you use the value type and the copy convention
    (e.g. `u_load`, or copy-in/copy-out at call boundaries).
    That way you avoid dangling references and keep ownership and lifetime explicit.

    **Reference vs copy:**
    `u_deref(p)` gives reference access to the pointee without copying;
    `u_load(v)` creates an independent stack copy for safe cross-boundary passing.
    For indirection patterns (e.g. storing indices or handles with context-based comparison),
    stack usage is bounded by the handle size, not the referenced data.

---

**2. Meta module** (`dh/meta.h`)

**Record/type reflection** built on top of the meta type system.
The module includes `prl.h` (and thus gets `u_P$raw`, `u_S$raw`, etc.)
and provides layout and field access **in terms of meta pointers and slices**.

- **Record layout from fields:**
  Given a slice of field `TypeInfo`s, it computes record `TypeInfo`, size, alignment,
  and field offsets: `u_typeInfoRecord`, `u_sizeOfRecord`, `u_alignOfRecord`,
  `u_offsetTo`, `u_offsets`.
  Array-shaped records: `u_typeInfoA`, `u_sizeOfA`, `u_alignOfA`;
  N-replicated records: `u_typeInfoRecordN`, `u_offsetToN`, `u_offsetsN`.
- **Field access as meta:**
  `u_fieldPtr` / `u_fieldPtrMut` take a **meta pointer** to a record
  (`u_P_const$raw` / `u_P$raw`) and a slice of field `TypeInfo`s,
  and return a meta pointer to the chosen field.
  `u_fieldPtrs` / `u_fieldPtrsMut` fill a slice of meta pointers for all fields.
  `u_recordPtr` / `u_recordPtrMut` go from a field meta pointer back to the record meta pointer.
  So the meta module is the layer that lets you describe structs by their field types
  and then read/write fields through the same type-erased `u_*` representation
  that the meta type system uses.
- **N-replicated (SoA-style) access:**
  `u_fieldSli`, `u_fieldSliMut`, `u_fieldSlis`, `u_fieldSlisMut` expose a field across N records
  as `u_S_const$raw` / `u_S$raw`;
  `u_recordNPtr` / `u_recordNPtrMut` go from a field slice back to the record meta pointer.

**Relationship:**
The meta **type system** defines the generic representation
(pointer/slice/array/option/result + `TypeInfo`).
The meta **module** uses that representation for reflection:
it computes layout from field `TypeInfo`s
and gives you field and record access as `u_P$raw` / `u_S$raw`,
so generic code can walk records without knowing the concrete struct type.
Both rely on `TypeInfo` from `core`/`type_info.h`
(and compile-time `typeInfo$(_T)` from `builtin`/`core`).

---

## 플랫폼 지원

| 구분             | 지원 범위                                                     |
| :--------------- | :------------------------------------------------------------ |
| **OS**           | Windows, Unix, Linux, macOS                                   |
| **Architecture** | x86 (32-bit), x64 (64-bit)                                    |
| **Clang**        | 19.1.0+ (권장) / 16.0.0+ (지원) / 9.0.0+ (-std=gnu11 필수)    |
| **GCC**          | 15.1.0+ (권장) / 13.1.0+ (지원) / N/A (TBU) (-std=gnu11 필수) |
| **MSVC**         | 지원 예정 (TBD)                                               |

> **참고:** `dh-c` 빌드 도구의 지원 범위는 [사전 요구 사항](#사전-요구-사항)을 참고하세요.

---

## 코드 샘플

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

<details>
<summary><strong>더 많은 코드 샘플 보기</strong></summary>

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

</details>

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
    let_(sum, i32) = mathAdd(a, b);

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

---

## Documentation

> **Note:** Comprehensive documentation is a work in progress. For now, please refer to:
> - The code samples in this README
> - Header files in `dh/include/dh/` which contain detailed comments and API declarations
> - Example projects in the repository

---

## 명칭에 관하여

<!--
본 프로젝트는 현재 개발 초기 단계에 있으며 API 사양은 실험적입니다.
 -->

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

Copyright © 2024-2026 Gyeongtae Kim.
