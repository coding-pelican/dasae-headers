# DH-C 표준 코딩 컨벤션

## 머리말

안전하고 현대적인 C 프로그래밍을 위한 가이드라인이다. 가독성과 안전성을 최우선으로 삼는다.

## 기본 원칙

1. 코드 가독성을 최우선으로 한다. 코드 자체가 문서 역할을 해야 한다.
2. 문제가 발생하면 최대한 빨리 오류를 감지하고 실패하도록 코드를 작성한다.
3. 잠재적인 메모리 관련 오류를 방지하는 패턴을 우선시한다.
4. 모든 경고를 오류로 처리하고 수정한다.

## I. 명명 규칙

### 1. 타입 이름

모든 타입 이름은 파스칼 표기법(PascalCase)을 사용한다.

```c
typedef struct Rectangle {
    i32 width;
    i32 height;
} Rectangle;

typedef struct Circle {
    f32 radius;
} Circle;
```

### 2. 변수 이름

지역 변수와 함수 매개변수는 스네이크 표기법(snake_case)을 사용한다.

```c
fn_(calculateArea(const Rectangle* rectangle), i32) {
    let width = rectangle->width;
    let height = rectangle->height;
    return width * height;
}
```

#### 2.1. 상수

상수는 스네이크 표기법(snake_case)을 사용한다.

```c
#define max_buffer_size (1024)
const i32 max_retry_count = 5;
```

### 3. 함수 이름

함수 이름은 동사로 시작하고 카멜 표기법(camelCase)을 사용한다.

```c
fn_(calculateSum(i32 a, i32 b), i32);
fn_(validateInput(Str_const input), bool);
fn_(initializeSystem(void), void);
```

불리언 값을 반환하는 함수는 `is`, `has`, `can`, `should` 등의 접두사를 사용한다.

```c
fn_(isValidInput(Str_const input), bool);
fn_(hasPermission(const User* user), bool);
```

### 4. 구조체 필드

구조체 필드는 스네이크 표기법을 사용한다.

```c
typedef struct User {
    Sli$u8 user_name;
    u64    user_id;
    i32    access_level;
} User;
```

### 5. 열거형

열거형 이름은 파스칼 표기법을, 열거형 값은 스네이크 표기법을 사용한다.

```c
// enum 구성 예시
typedef enum Direction {
    Direction_north = 0,
    Direction_south = 1,
    Direction_east = 2,
    Direction_west = 3,
    Direction_count
} Direction;
```

#### 5.1. 오류 집합

오류 집합 이름은 파스칼 표기법을, 오류집합 값은 예외적으로 파스칼 표기법을 사용한다.

```c
// ErrSet 구성 예시
config_ErrSet(FileErr
    FileNotFound,
    AccessDenied,
    InvalidFormat,
);
```

#### 5.2. 유니온 열거형

```c
// UnionEnum 구성 예시
config_UnionEnum(Value
    (Value_integer_number, struct { i32 value; }),
    (Value_floating_point, struct { f64 value; }),
    (Value_string_literal, Str_const)
);
```

## II. 코드 구조 및 형식

### 1. 들여쓰기와 중괄호

들여쓰기는 공백 4칸을 사용한다.

```c
if (condition) {
    doSomething();
}
```

조건문이나 반복문에서 단일 명령문이라도 중괄호를 사용한다.

```c
// 잘못된 예시
if (condition)
    doSomething();

// 올바른 예시
if (condition) {
    doSomething();
}
```

### 2. 함수 정의

함수 정의는 `fn_`를 사용한다.
- `$scope`/`$unscoped` - 페이로드 반환을 위한 확장 범위 함수
- `$guard`/`$unguarded` - defer, errdefer 등을 위한 보호 범위 함수

```c
// 기본 함수 정의
fn_(calculateSum(i32 a, i32 b), i32) {
    return a + b;
}

// 확장된 스코프가 필요한 함수 정의 (Optional 반환)
use_Opt$(i32);
fn_(divideNumbers(i32 lhs, i32 rhs), Opt$i32, $scope) {
    if (rhs == 0) {
        return_none();
    }
    return_some(lhs / rhs);
} $unscoped;

// 오류 처리가 필요한 함수
use_Err$(Str_const);
fn_(readFile(Str_const path), Err$Str_const) $must_check;
fn_(readFile(Str_const path), Err$Str_const, $scope) {
    if (Str_isEmpty(path)) {
        return_err(Err_EmptyPathProvided());
    }
    // 처리...
    return_ok(content);
} $unscoped;
```

### 3. 포인터 변수 선언

포인터 기호(*)는 타입에 붙여 작성한다.
혹은 `Ptr$(T)`를 사용한다.

```c
// 잘못된 예시
i32 *data;

// 올바른 예시
i32* data;
// 혹은
Ptr$(i32) data;
```

### 4. 변수 초기화

변수는 선언과 동시에 반드시 초기화한다. 제로 초기화가 필요하면 cleared()를 사용한다.

```c
// 잘못된 예시
i32 counter;
counter = 0;

// 올바른 예시
let counter = 0;

// 구조체 초기화 - 제로 초기화
User user = cleared();

// 컴파운드 리터럴 사용
let rect = make$(Rectangle, .width = 10, .height = 20);
```

## III. 안전한 프로그래밍 관행

### 1. 메모리 관리

리소스 할당 후에는 defer를 사용하여 정리 작업을 명시한다.

```c
// 잘못된 예시
fn_(processData(void), Err$void, $scope) {
    let buffer = meta_cast$(Sli$i8, try_(mam_Allocator_alloc(typeInfo$(i8), buffer_size)));

    // 처리 로직...

    mem_Allocator_free(anySli(buffer)); // 중간에 return이 있으면 메모리 누수 발생
} $unscoped;

// 올바른 예시
fn_(processData(void), Err$void, $guard) {
    let buffer = meta_cast$(Sli$i8, try_(mam_Allocator_alloc(typeInfo$(i8), buffer_size)));
    defer_(mem_Allocator_free(anySli(buffer)));

    // 처리 로직...
    // 어디서 return하더라도 buffer는 항상 해제됨
} $unguarded;
```

### 2. 파일 처리

파일 열기 후에는 defer를 사용하여 닫기 작업을 명시한다.

```c
fn_(readFileContents(Str_const filename), io_Err$Str_const, $guard) {
    let file = fopen(filename.ptr, "r");
    if (file == null) {
        return_err(io_Err_OpenFailed());
    }
    defer_($ignore = fclose(file));

    // 파일 처리...

    return_ok(content); // 함수 종료 시 file은 자동으로 닫힘
} $unguarded;
```

### 3. Optional 값 처리

out 매개변수 대신 Optional 반환 값을 사용한다.

```c
// 잘못된 예시
fn_(findUser(i32 user_id, User** out_user), bool) {
    if (user_id <= 0 || out_user == null) {
        return false;
    }

    *out_user = lookupUserById(user_id);
    return (*out_user != null);
}

// 올바른 예시
fn_(findUser(i32 user_id), Opt$Ptr$User, $scope) {
    if (user_id <= 0) {
        return_none();
    }

    const User* user = lookupUserById(user_id);
    if (user == null) {
        return_none();
    }

    return_some(user);
} $unscoped;

// 사용 예시
fn_(processUserData(i32 user_id), bool) {
    if_some(findUser(user_id), user) {
        processData(user);
        return true;
    }
    log_error("User not found");
    return false;
}
```

### 4. 안전한 배열 접근

원시 배열 및 원시 포인터 인덱스 접근 대신 안전한 배열 기법을 사용한다.

```c
// 잘못된 예시
fn_(processArray(i32* data, usize size), void) {
    for (usize i = 0; i < size; ++i) {
        process(data[i]); // 잠재적인 경계 오류
    }
}

// 올바른 예시
fn_(processArray(Sli$i32 data), void) {
    // 배열 순회 방식 사용
    for_slice (data, item) {
        process(deref(item));
    }

    // 또는 직접 접근 시 안전한 접근 함수 사용
    for (usize i = 0; i < data.len; ++i) {
        process(Arr_getAt(data, i)); // 잘못된 접근시 assertion 트리거
    }
}
```
