---
title: 빠른 시작 가이드
description: 안전한 C 프로그래밍을 위한 dasae-headers 시작하기
created: 2025-04-01
updated: 2025-04-13
version: v0.1.1
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
- Git (소스 코드 획득용)

### 소스 코득

저장소를 로컬 머신에 복제합니다:

```sh
git clone https://github.com/coding-pelican/dasae-headers.git
cd dasae-headers
```

### dh-c 빌드 도구 설치

dh-c 빌드 도구는 dasae-headers 프로젝트의 관리, 빌드 및 테스트를 간소화합니다.

#### Windows

1. 관리자 권한으로 PowerShell 프롬프트 열기
2. dasae-headers 디렉토리로 이동
3. 설치 스크립트 실행:

```ps1
.\install-dh-c.ps1
```

4. 터미널 또는 PowerShell 세션 재시작

#### Linux/macOS

1. 터미널 열기
2. dasae-headers 디렉토리로 이동
3. 설치 스크립트 실행 가능하게 만들고 실행하기:

```sh
chmod +x install-dh-c.sh
./install-dh-c.sh
```

4. 터미널 세션을 재시작하거나 프로필을 소스로 설정

## 프로젝트 설정

### 새 프로젝트 생성

dh-c 도구를 사용하여 새 프로젝트를 생성합니다:

```sh
dh-c project my-project
cd my-project
```

이 명령은 필요한 모든 설정 파일이 포함된 표준 프로젝트 구조를 생성합니다.

### 디렉토리 구조

dh-c 도구는 다음과 같은 디렉토리 구조를 생성합니다:

```txt
my-project/
├── include/          # 프로젝트별 헤더
├── src/              # 소스 파일
│   └── main.c        # 메인 진입점
├── lib/              # 외부 라이브러리
├── build/            # 빌드 출력 (빌드 중 생성됨)
├── .clangd           # 언어 서버 구성
├── .clang-format     # 코드 포맷터 구성
└── .vscode/          # VS Code 구성
    └── tasks.json    # 빌드 작업
```

## 첫 번째 프로그램

간단한 "Hello, world!" 프로그램을 만들어 보겠습니다:

1. `src/main.c`를 열고 내용을 다음으로 교체합니다:

```c
#include "dh/main.h"
#include "dh/Str.h"

fn_(dh_main(Sli$Sli_const$u8 args), Err$void $scope) {
    // 문자열 리터럴 생성
    let hello = Str_l("Hello, dasae-headers!");

    // 콘솔에 줄바꿈과 함께 출력
    Str_println(hello);

    // 성공 리턴
    return_ok({});
} $unscoped;
```

2. 프로그램 빌드 및 실행:

```sh
dh-c build dev    # 개발 모드로 빌드
dh-c run dev      # 프로그램 실행
```

### 이 예제의 주요 기능

1. `fn_` - 함수 선언
2. `Sli$Sli_const$u8` - 상수 문자열의 슬라이스 (명령줄 인수)
3. `Err$void` - void 페이로드가 있는 오류 결과 타입
7. `$scope` - 페이로드 반환 타입을 위한 확장 범위 함수
4. `let` - 변수 선언을 위한 타입 추론
5. `Str_l` - 문자열 리터럴 생성
6. `return_ok` - 페이로드가 있는 성공적인 반환
8. `$unscoped` - 함수의 확장 범위 종료

## 핵심 개념

### 변수 선언

dasae-headers는 변수를 선언할 때 기본 C 스타일과 현대적인 타입 추론 방식을 모두 제공합니다:

```c
// 기본형 (불변)
const i32 count = 10;
// 타입 추론 (불변)
let count = 10;  // const i32로 추론됨
// 타입 명시 (불변)
let_(count, i32) = 10; // const i32

// 기본형 (가변)
i32 number = 42;
// 타입 추론 (가변)
var number = 42;  // i32로 추론됨
// 타입 명시 (가변)
var_(number, i32) = 42; // i32
```

### defer를 통한 메모리 안전성

`defer`를 사용한 자동 리소스 정리:

```c
fn_(readFile(Sli_const$u8 path), Err$Str $guard) {
    let_(file, FILE*) = fopen(path.ptr, "r");
    if (file == null) {
        return_err(fileError("파일을 열 수 없습니다"));
    }

    // 함수가 종료될 때 실행됨
    defer_($ignore = fclose(file));

    // 파일 처리...

    return_ok(fileContents);
} $unguarded;
```

### 오류 처리

`try_` 패턴을 사용한 명시적 오류 처리:

```c
fn_(processData(void), Err$void $scope) {
    // 실패할 수 있는 함수 호출 및 오류 전파
    let result = try_(getData());

    // 결과 처리...

    return_ok({});
} $unscoped;
```

### 옵셔널 타입

널 값을 안전하게 처리:

```c
fn_(findUser(i32 id), Opt$User $scope) {
    if (id <= 0) {
        return_none();  // 사용자를 찾을 수 없음
    }

    User user = getUserById(id);
    return_some(user);  // 사용자를 찾음
} $unscoped;

// 사용법
if_some(findUser(42), user) {
    // 사용자가 존재함, 사용자 처리
} else_none {
    // 찾을 수 없는 경우 처리
}
```

## 빌드 및 실행

dh-c 도구는 프로젝트를 빌드하고 실행하는 간단하고 일관된 방법을 제공합니다.

### 빌드

사전 정의된 구성으로 프로젝트 빌드:

```sh
dh-c build dev         # 개발 모드 (디버그, 최적화 없음)
dh-c build test        # 테스트 모드
dh-c build release     # 릴리즈 모드 (최적화)
dh-c build performance # 최대 성능
dh-c build embedded    # 크기 최적화
dh-c build micro       # 극단적인 크기 최적화
```

### 실행

애플리케이션 실행:

```sh
dh-c run dev           # 개발 모드로 빌드하고 실행
```

프로그램에 인수 전달:

```sh
dh-c run dev --args="arg1 arg2"
```

### 테스트

테스트 실행:

```sh
dh-c test              # 모든 테스트 실행
```

### 추가 옵션

실행되는 명령 표시:

```sh
dh-c build dev --show-commands
```

빌드 구성의 출력 접미사 활성화:

```sh
dh-c build dev --use-output-suffix
```

도움말 보기:

```sh
dh-c --help
```

## 디버깅

dasae-headers는 내장 디버그 기능을 포함합니다:

### 어설션

```c
debug_assert_true(condition, "오류 메시지");
debug_assert_fmt(0 < count, "유효하지 않은 개수: %d", count);
```

## IDE 통합

dh-c 도구는 프로젝트를 위한 VSCode 설정 파일을 생성합니다:

### VSCode 작업

`Ctrl+Shift+B`(기본 단축키)를 눌러 빌드 작업에 액세스:
- `dh>build project` - 프로젝트 빌드
- `dh>run project` - 프로젝트 빌드 및 실행
- `dh>test project` - 테스트 실행
- `dh>execute project` - 재빌드 없이 실행

## 다음 단계

기본 프로젝트를 설정했으니, 다음을 탐색해 볼 수 있습니다:

- [핵심 개념](./concepts/index.md) - 심층적인 설명
- [API 참조](./api/index.md) - 모든 모듈에 대한 문서
- [예제](./examples/index.md) - 더 많은 코드 예제
- [튜토리얼](./tutorials/index.md) - 단계별 가이드

dasae-headers와 함께 즐거운 코딩 되세요!
