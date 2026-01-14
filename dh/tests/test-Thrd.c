#include "dh/main.h"
#include "dh/Thrd/common.h"
#include "dh/time/Duration.h"
#include "dh/io/stream.h"

/* $static Thrd_fn_(((timesTwo)(i32 input))(i32) $scope($ignore, args)) {
    let input = args->input;
    Thrd_sleep(time_Duration_fromMillis(10));
    return_(input * 2);
} $unscoped_(Thrd_fn); */
$static Thrd_fn_(timesTwo, ({ i32 input; }, i32), ($ignore, args)$scope) {
    let input = args->input;
    Thrd_sleep(time_Duration_fromMillis(10));
    return_(input * 2);
} $unscoped_(Thrd_fn);

TEST_fn_("Thrd: Basic Lifetime (Stack Memory)" $scope) {
    var worker_ctx = Thrd_FnCtx_from$((timesTwo)(42));
    let worker = try_(Thrd_spawn(Thrd_SpawnCfg_default, worker_ctx.as_raw));
    let result_ctx = Thrd_join(worker);
    let result = Thrd_FnCtx_ret$((timesTwo)(result_ctx));

    try_(TEST_expect(result == 84));
    try_(TEST_expect(result_ctx == worker_ctx.as_raw));
    io_stream_println(u8_l("Thrd: Basic Lifetime (Stack Memory) passed"));
} $unscoped_(TEST_fn);

typedef A$$(100, i32) ArrForTest;

/* $static Thrd_fn_(((sumValues)(ArrForTest values))(i32) $scope($ignore, args)) {
    i32 sum = 0;
    for_(($a(args->values))(value) {
        sum += *value;
    });
    return_(sum);
} $unscoped_(Thrd_fn); */
$static Thrd_fn_(sumValues, ({ ArrForTest values; }, i32), ($ignore, args)$scope) {
    i32 sum = 0;
    for_(($a(args->values))(value) {
        sum += *value;
    });
    return_(sum);
} $unscoped_(Thrd_fn);

TEST_fn_("Thrd: Args Synchronization" $scope) {
    ArrForTest values = A_zero();
    for_(($rf(0), $s(A_ref(values)))(i, value) {
        *value = intCast$((i32)(i));
    });

    var worker_ctx = Thrd_FnCtx_from$((sumValues)(values));
    let worker = try_(Thrd_spawn(Thrd_SpawnCfg_default, worker_ctx.as_raw));
    let result_ctx = Thrd_join(worker);
    let result = Thrd_FnCtx_ret$((sumValues)(result_ctx));

    let expected_sum = (99 * 100) / 2; // 0+1+2+...+99 = 4950
    try_(TEST_expect(result == expected_sum));
    try_(TEST_expect(result_ctx == worker_ctx.as_raw));
    io_stream_println(u8_l("Thrd: Args Synchronization passed"));
} $unscoped_(TEST_fn);

// ============================================================================
// 테스트 4: 여러 스레드 동시 실행
// ============================================================================

/* $static Thrd_fn_(((sleepSecsForIdAndSq)(i32 id))(i32) $scope($ignore, args)) {
    let id = args->id;
    Thrd_sleep(time_Duration_fromMillis(1000ull * (id % 10)));
    return_(id * id);
} $unscoped_(Thrd_fn); */
$static Thrd_fn_(sleepSecsForIdAndSq, ({ i32 id; }, i32), ($ignore, args)$scope) {
    let id = args->id;
    Thrd_sleep(time_Duration_fromMillis(1000ull * (as$(usize)(id) % 10)));
    return_(id * id);
} $unscoped_(Thrd_fn);

TEST_fn_("Thrd: Multiple Threads" $scope) {
    A$$(10, O$$(Thrd_FnCtx$(sleepSecsForIdAndSq))) workers = A_zero();
    A$$(10, Thrd) threads = A_zero();
    for_(($rf(0), $s(A_ref(workers)), $s(A_ref(threads)))(i, worker, thread) {
        asg_lit((worker)(some(Thrd_FnCtx_from$((sleepSecsForIdAndSq)(intCast$((i32)(i)))))));
        *thread = try_(Thrd_spawn(Thrd_SpawnCfg_default, unwrap_(O_asP(worker))->as_raw));
    });
    for_(($s(A_ref(threads)))(thread) {
        Thrd_join(*thread);
    });
    for_(($rf(0), $s(A_ref(workers)))(i, worker) {
        let result = Thrd_FnCtx_ret$((sleepSecsForIdAndSq)(unwrap_(O_asP(worker))));
        try_(TEST_expect(result == as$(i32)(i * i)));
    });
    io_stream_println(u8_l("Thrd: Multiple Threads passed"));
} $unscoped_(TEST_fn);

/* // ============================================================================
// 테스트 5: Compound Literal 생명주기 (실제 패턴 테스트)
// ============================================================================

typedef struct {
    int value;
    char message[64];
} TestContext5;

void* worker_compound(void* arg) {
    TestContext5* ctx = arg;
    usleep(50000); // 50ms - 충분히 긴 시간

    // 메모리가 여전히 유효한지 확인
    ctx->value *= 2;
    strcat(ctx->message, " processed");

    return ctx;
}

void test_compound_literal_lifetime() {
    printf("\n=== Test 5: Compound Literal Lifetime ===\n");

    pthread_t thread;

    // Compound literal 사용 (당신의 코드 패턴)
    TestContext5* ctx = &(TestContext5){
        .value = 100,
        .message = "test"
    };

    pthread_create(&thread, NULL, worker_compound, ctx);
    pthread_join(thread, NULL);

    TEST_ASSERT(ctx->value == 200, "Value should be 200");
    TEST_ASSERT(strstr(ctx->message, "processed") != NULL, "Message should contain 'processed'");

    TEST_PASS("Compound literal lifetime test");
}

// ============================================================================
// 테스트 6: 스트레스 테스트 (생명주기 + 동기화)
// ============================================================================

typedef struct {
    int iteration;
    int computed;
} TestContext6;

void* worker_stress(void* arg) {
    TestContext6* ctx = arg;

    // 간단한 계산
    int sum = 0;
    for (int i = 0; i < 1000; i++) {
        sum += i;
    }

    ctx->computed = ctx->iteration + sum;
    return ctx;
}

void test_stress() {
    printf("\n=== Test 6: Stress Test (%d iterations) ===\n", TEST_ITERATIONS);

    for (int iter = 0; iter < TEST_ITERATIONS; iter++) {
        TestContext6 ctx = { .iteration = iter, .computed = 0 };
        pthread_t thread;

        pthread_create(&thread, NULL, worker_stress, &ctx);
        pthread_join(thread, NULL);

        int expected = iter + (999 * 1000) / 2;
        if (ctx.computed != expected) {
            printf("Iteration %d failed: expected %d, got %d\n", iter, expected, ctx.computed);
            TEST_ASSERT(0, "Stress test failed");
        }

        if (iter % 100 == 0 && iter > 0) {
            printf("  Progress: %d/%d iterations\n", iter, TEST_ITERATIONS);
        }
    }

    TEST_PASS("Stress test");
}

// ============================================================================
// 테스트 7: 빠른 생성/소멸 (생명주기 경계 테스트)
// ============================================================================

typedef struct {
    volatile int ready;
    int value;
} TestContext7;

void* worker_fast(void* arg) {
    TestContext7* ctx = arg;
    ctx->ready = 1;
    ctx->value = 999;
    return ctx;
}

void test_fast_spawn_join() {
    printf("\n=== Test 7: Fast Spawn/Join Test ===\n");

    for (int i = 0; i < 1000; i++) {
        TestContext7 ctx = { .ready = 0, .value = 0 };
        pthread_t thread;

        pthread_create(&thread, NULL, worker_fast, &ctx);
        pthread_join(thread, NULL);

        TEST_ASSERT(ctx.ready == 1, "Ready flag not set");
        TEST_ASSERT(ctx.value == 999, "Value not set correctly");
    }

    TEST_PASS("Fast spawn/join test");
}

// ============================================================================
// 테스트 8: 복잡한 구조체 (Flexible Array Member 시뮬레이션)
// ============================================================================

typedef struct {
    size_t data_size;
    int checksum;
    char data[256]; // FAM 시뮬레이션
} TestContext8;

void* worker_complex(void* arg) {
    TestContext8* ctx = arg;

    // 데이터 처리
    int sum = 0;
    for (size_t i = 0; i < ctx->data_size; i++) {
        sum += ctx->data[i];
    }
    ctx->checksum = sum;

    return ctx;
}

void test_complex_structure() {
    printf("\n=== Test 8: Complex Structure Test ===\n");

    TestContext8 ctx = { .data_size = 100, .checksum = 0 };

    // 데이터 준비
    for (size_t i = 0; i < ctx.data_size; i++) {
        ctx.data[i] = (char)(i % 256);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, worker_complex, &ctx);
    pthread_join(thread, NULL);

    // Checksum 검증
    int expected_sum = 0;
    for (size_t i = 0; i < ctx.data_size; i++) {
        expected_sum += (i % 256);
    }

    TEST_ASSERT(ctx.checksum == expected_sum, "Checksum mismatch");

    TEST_PASS("Complex structure test");
}

// ============================================================================
// 테스트 9: 다중 스레드 스트레스 (동시성)
// ============================================================================

void test_concurrent_stress() {
    printf("\n=== Test 9: Concurrent Stress (%d threads) ===\n", STRESS_THREAD_COUNT);

    TestContext4 contexts[STRESS_THREAD_COUNT];
    pthread_t threads[STRESS_THREAD_COUNT];

    // 많은 스레드 동시 생성
    for (int i = 0; i < STRESS_THREAD_COUNT; i++) {
        contexts[i].thread_id = i;
        contexts[i].result = 0;
        pthread_create(&threads[i], NULL, worker_multiple, &contexts[i]);
    }

    // 모두 join
    for (int i = 0; i < STRESS_THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // 검증
    int failed = 0;
    for (int i = 0; i < STRESS_THREAD_COUNT; i++) {
        int expected = i * i;
        if (contexts[i].result != expected) {
            printf("  Thread %d: expected %d, got %d\n", i, expected, contexts[i].result);
            failed++;
        }
    }

    TEST_ASSERT(failed == 0, "Some threads failed");
    TEST_PASS("Concurrent stress test");
}
 */
