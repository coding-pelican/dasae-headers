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
  - [주요 특징](#주요-특징)
    - [메모리 안전성 및 리소스 관리](#메모리-안전성-및-리소스-관리)
    - [강화된 타입 시스템](#강화된-타입-시스템)
    - [오류 처리 및 디버깅](#오류-처리-및-디버깅)
    - [현대적 프로그래밍 구문](#현대적-프로그래밍-구문)
    - [개발 도구](#개발-도구)
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

### 왜 dasae-headers인가?

dasae-headers는 기존 C 환경의 제약을 극복하고 현대적인 개발 경험을 제공하기 위해 아래와 같은 설계 원칙을 고수합니다.

- **기존 C 생태계와의 공존:** 기존 C 라이브러리 및 레거시 코드베이스를 수정하지 않고도 현대적 문법과 안전 장치를 즉시 도입할 수 있습니다.
- **제로 코스트 추상화:** 인라인화와 전처리 단계 평가, constant-folding 유도 등의 최적화를 통해 고수준 기능을 제공하면서도 런타임 오버헤드를 최소화합니다.
- **점진적 도입 가능:** 프로젝트 전체를 전환할 필요 없이, 필요한 모듈(예: 에러 처리, 할당자 등)만 선택적으로 연결하여 사용할 수 있습니다.
- **Freestanding 및 베어메탈 지원:** 임베디드 환경이나 커널 개발 등 프리스텐딩 환경에서의 개발 가능함을 최우선 로드맵으로 두고 있어, 시스템의 가장 낮은 곳에서도 유효합니다.
- **사용자 정의 매크로 최소화:** 일반적인 활용에 있어 핵심 기능을 구현하기 위해 사용자가 별도의 복잡한 매크로를 작성할 필요가 없습니다. 제네릭 인스턴스화 역시 정립된 패턴을 따르기만 하면 전처리 단계에서 자동으로 처리됩니다.
- **디버깅 친화적 설계:** 매크로가 런타임 디버깅(Call Stack 추적, Step-by-step 실행 등)을 방해하지 않도록 정교하게 디자인되어 개발 생산성을 유지합니다.
- **일관된 컨벤션:** 엄격하고 일관된 코드 컨벤션과 명명법을 통해 대규모 코드베이스에서도 가독성과 유지보수성을 보장합니다.

---

## 주요 특징

본 프로젝트는 Zig와 Rust의 문법 구조 및 표준 라이브러리 설계 방식을 참고하여 개발되었습니다.

### 메모리 안전성 및 리소스 관리
- **커스텀 할당자(Allocator):** 메모리 할당 방식을 선택적으로 제어할 수 있는 추상화 계층
- **메모리 추적:** 런타임 중 메모리 누수 및 관련 버그를 탐지하는 내장된 디버깅 기능
- **경계 검사(Boundary Check):** 배열 접근 시 유효 범위를 검사하는 슬라이스 타입
- **자동 리소스 관리:** `defer` 및 `errdefer`를 통한 스코프 종료시의 직관적인 자원 해제 로직

### 강화된 타입 시스템
- **컴파일 타임 검사:** 전처리 단계의 타입 추론 및 검사를 통해 런타임 오류 가능성 축소
- **대수적 데이터 타입(Variant):** `match` 구문과 연동되는 패턴 매칭 및 가변타입
- **옵셔널 타입:** `some`/`none` 키워드와 `unwrap`/`orelse` 패턴을 사용하는 옵셔널 타입

### 오류 처리 및 디버깅
- **명시적 에러 핸들링:** `ok`/`err` 키워드를 활용한 반환 값 중심의 오류 처리
- **구조적 전파:** `try`, `catch` 패턴을 통한 가독성 높은 오류 전파 로직
- **에러 추적:** 오류 발생 지점의 호출 스택을 포함하는 포괄적인 추적 정보

### 현대적 프로그래밍 구문
- **타입 추론:** `let`과 `var` 키워드를 사용한 변수 선언 방식
- **함수 정의:** `fn` 선언 형식을 통한 함수 시그니처 구성
- **확장 기능:** `la` 람다 표현식, 일급 객체 `Callable` 타입, 일관된 명명 규칙을 적용한 매크로

### 개발 도구
- **테스트 프레임워크:** 별도의 외부 라이브러리 없이 내장된 단위 및 통합 테스트를 수행할 수 있는 도구
- **컴파일러 호환성:** 주요 C 컴파일러(Clang, GCC) 및 멀티 플랫폼 환경 지원

---

## 플랫폼 지원

| 구분             | 지원 범위                                                    |
| :--------------- | :----------------------------------------------------------- |
| **OS**           | Windows, Unix, Linux, macOS                                  |
| **Architecture** | x86 (32-bit), x64 (64-bit)                                   |
| **Clang**        | 19.1.0+ (권장) / 16.0.0+ (지원) / 9.0.0+ (-std=gnu11 필수)   |
| **GCC**          | 15.1.0+ (권장) / 13.1.0+ (지원) / N/A(TBU) (-std=gnu11 필수) |
| **MSVC**         | 지원 예정 (TBD)                                              |

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

모든 자료구조와 함수 설계에 있어 동적 할당이 필요한 경우, 할당자(Allocator) 혹은 메모리 버퍼를 주입받는 구조로 설계되어 메모리 레이아웃을 완전히 제어할 수 있습니다.
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
- **Email:** <codingpelican@gmail.com>

## 라이선스

본 프로젝트는 [MIT License](./LICENSE) 하에 배포됩니다.

Copyright © 2024-2025 Gyeongtae Kim.
