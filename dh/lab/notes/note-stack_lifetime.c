#ifdef UNUSED_CODE
#include <string.h>
#include <stdio.h>

void dangerous(int len) {
    char* p = ({
        char* tmp = __builtin_alloca(len);
        strcpy(tmp, "Important data");
        tmp;
    });
    /* char* p = ({
        char tmp[len] = {};
        strcpy(tmp, "Important data");
        tmp;
    }); */
    /* char* p = ({
        char* tmp = (char[len]){};
        strcpy(tmp, "Important data");
        tmp;
    }); */

    // 위험! 새로운 VLA나 큰 배열이 alloca 영역을 덮어쓸 수 있음
    int n = 50;
    char buffer[n]; // VLA - 스택에서 할당
    prim_memset(buffer, 'X', n);

    printf("%s\n", p); // "Important data"가 손상될 수 있음
    printf("%s\n", buffer);
}
#endif // UNUSED_CODE

#include <stdio.h>

typedef struct A {
    int foo;
    struct {
        float bar;
    };
} A;

typedef struct B {
    int foo;
    struct {
        float* bar;
    };
} B;

A getA(void) {
    return (A){ .foo = 123, .bar = 456.0f };
}

#define someOp(_a...) ({ \
    A* __p = (A[1]){ [0] = _a }; \
    (B){ .foo = __p->foo, .bar = &__p->bar }; \
})

void test(void) {
    // 함수 호출 테스트
    B b1 = someOp(getA());
    printf("b1: foo=%d, bar=%f\n", b1.foo, *b1.bar);

    // 증감 연산 테스트
    A a = { .foo = 10, .bar = 20.0f };
    int i = 0;
    B b2 = someOp((a.foo = ++i, a)); // 복잡한 표현식
    printf("b2: foo=%d, bar=%f\n", b2.foo, *b2.bar);
    printf("i=%d\n", i); // 1 (한 번만 증가)

    // 여러 B 생성 - 모두 안전
    B b3 = someOp({ .foo = 30, .bar = 40.0f });
    B b4 = someOp({ .foo = 50, .bar = 60.0f });

    // 모든 포인터가 유효함
    printf("All valid: %f, %f, %f, %f\n", *b1.bar, *b2.bar, *b3.bar, *b4.bar);
}

typedef struct C {
    A* a;
} C;
C C_withA(A* a) {
    return (C){ .a = a };
}

#include "dh/prl.h"

int main(void) {
    test();
    C my_c = C_withA(&copy(getA()));
    A* a = move(&my_c.a);
    let_ignore = a;
    return 0;
}

#ifdef UNUSED_CODE
// C언어에서 아래와 같은 코드가 있다
// Clang 컴파일러 환경이며, C17 표준을 따르고, 컴파일러 확장을 사용함
typedef struct A {
    int foo;
    struct {
        float bar;
    };
} A;
typedef struct B {
    int foo;
    struct {
        float* bar;
    };
} B;
// A와 B의 차이점은 멤버변수 bar이 값이냐 포인터이냐

void someFn(void) {
    A a = (A){ .foo = 123, .bar = 456.0f };
    B b = (B){ .foo = a.foo, .bar = &a.bar };
    // A는 값으로서의 타입이며, B는 FatPointer로서의 타입
    // A와 B의 공통값인 foo는 복사해야하고, bar는 A의 bar를 참조해야함

    // 원하는 동작
    extern A getA(void);
    B b = someOp(getA());

// 이를 매크로로 표현해보자
// Case 1:
#define someOp(_a) ((B){ .foo = (_a).foo, .bar = &(_a).bar })
    // 이 방식의 문제는 인자로 전달된 _a가 텍스트 대치되며 인자로 함수 호출, 증감 연산을 사용하는 경우 값이 평가되어버려 다른 객체가 되어버림
    // e.g. 함수 전달 경우
    B b = someOp(getA());
    B b = ((B){ .foo = (getA()).foo, .bar = &(getA()).bar });
    // e.g. 증감 연산 경우
    B b = someOp(a++);
    B b = ((B){ .foo = (a++).foo, .bar = &(a++).bar });

// Case 2: ({}) 사용
#define someOp(_a) ({ \
    A __a = (_a); \
    (B){ .foo = __a.foo, .bar = &__a.bar }; \
})
    // 이 방법의 경우, 완벽해보이지만, __a는 스택에 할당된 개체이므로, 스코프를 벗어나는 순간 소멸되어버림
    B b = someOp(getA());
    B b = ({
        A __a = (getA());
        (B){ .foo = __a.foo /* 값 */, .bar = &__a.bar /* 포인터 */ };
    }); // 스코프를 벗어날 때 __a가 소멸됨
    // foo는 값을 복사하였지만, bar는 댕글링 포인터가 되어버림

    // 나는 절대적으로 동적할당을 사용하고 싶지 않으며, static 또한 사용하고 싶지 않음
}
#endif // UNUSED_CODE
