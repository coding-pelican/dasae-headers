#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

int main(void) {
    let base    = time_Instant_now();
    let point_a = time_Instant_addDuration(base, time_Duration_fromSecs(1));
    let point_b = time_Instant_addDuration(base, time_Duration_fromSecs(2));
    let dur_a   = time_Instant_durationSince(point_a, base);
    let dur_b   = time_Instant_durationSince(point_b, base);
    printf("%.4lf %.4lf\n", time_Duration_asSecs$f64(dur_a), time_Duration_asSecs$f64(dur_b));

    time_sleepSecs(3);

    let elapsed = time_Instant_elapsed(base);
    printf("%.4lf\n", time_Duration_asSecs$f64(elapsed));
    let elapsed_1 = time_Instant_elapsed(point_a);
    printf("%.4lf\n", time_Duration_asSecs$f64(elapsed_1));
    let elapsed_2 = time_Instant_elapsed(point_b);
    printf("%.4lf\n", time_Duration_asSecs$f64(elapsed_2));
    return 0;
}
