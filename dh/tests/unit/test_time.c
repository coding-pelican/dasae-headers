// build `clang -o test_time -xc -std=c17 test_time.c ../../src/*.c -static -g -IC:\dasae-storage\dev\source\c-cpp-workspace\projects\tests\test_terminal\dh\include`

#include "dh/time.h"
#include "dh/time/instant.h"
#include "dh/time/duration.h"
#include "dh/time/system.h"
#include <stdio.h>

int main() {
    time_Instant begin = time_Instant_now();
    printf("start: %llu\n", begin.time_.QuadPart);
    time_Instant end = time_Instant_now();
    printf("end: %llu\n", end.time_.QuadPart);
    time_Duration elapsed = time_Instant_elapsed(begin);
    printf("elapsed: %llu %u\n", elapsed.secs, elapsed.nanos);
    time_SystemTime_sleep(time_Duration_fromSecs(1));
    printf("sleep 1 sec\n");
    time_SystemTime_sleep(time_Duration_fromMillis(1000));
    printf("sleep 1,000 ms\n");
    time_SystemTime_sleep(time_Duration_fromMicros(1000000));
    printf("sleep 1,000,000 us\n");
    time_SystemTime_sleep(time_Duration_fromNanos(1000000000));
    printf("sleep 1,000,000,000 ns\n");
    time_Instant now = time_Instant_now();
    printf("now: %llu\n", now.time_.QuadPart);
    time_Duration duration = time_Instant_durationSince(now, begin);
    printf("duration: %llu %u\n", duration.secs, duration.nanos);
    time_SystemTime_sleep_s_f64(0.5);
    printf("sleep 0.5 sec\n");
    time_Instant now2 = time_Instant_now();
    printf("now: %llu\n", now2.time_.QuadPart);
    time_Duration duration2 = time_Instant_durationSince(now2, begin);
    printf("duration: %llu %u\n", duration2.secs, duration2.nanos);
    return 0;
}
