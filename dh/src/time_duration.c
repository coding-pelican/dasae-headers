// #include "dh/time/duration.h"
// #include "dh/debug/assert.h"


// time_Duration time_Duration_from(u64 secs, u32 nanos) {
//     return literal_time_Duration_from(secs, nanos);
// }

// time_Duration time_Duration_fromSecs(u64 secs) {
//     return literal_time_Duration_fromSecs(secs);
// }

// time_Duration time_Duration_fromMillis(u64 millis) {
//     return literal_time_Duration_fromMillis(millis);
// }

// time_Duration time_Duration_fromMicros(u64 micros) {
//     return literal_time_Duration_fromMicros(micros);
// }

// time_Duration time_Duration_fromNanos(u64 nanos) {
//     return literal_time_Duration_fromNanos(nanos);
// }

// u64 time_Duration_asSecs(time_Duration d) {
//     return d.secs;
// }

// u32 time_Duration_subsecMillis(time_Duration d) {
//     return d.nanos / time_NANOS_PER_MILLI;
// }

// u32 time_Duration_subsecMicros(time_Duration d) {
//     return d.nanos / time_NANOS_PER_MICRO;
// }

// u32 time_Duration_subsecNanos(time_Duration d) {
//     return d.nanos;
// }

// time_Duration time_Duration_fromSecs_f64(f64 secs) {
//     return time_Duration_from(
//         (u64)secs,
//         (u32)((secs - (f64)((u64)secs)) * (f64)time_NANOS_PER_SEC)
//     );
// }

// f64 time_Duration_asSecs_f64(time_Duration d) {
//     return (f64)d.secs + (f64)d.nanos / (f64)time_NANOS_PER_SEC;
// }

// time_Duration time_Duration_add(time_Duration lhs, time_Duration rhs) {
//     u64 total_nanos = (u64)lhs.nanos + rhs.nanos;
//     return makeWith(
//         time_Duration,
//         lhs.secs + rhs.secs + (total_nanos >= time_NANOS_PER_SEC),
//         (u32)(total_nanos >= time_NANOS_PER_SEC ? total_nanos - time_NANOS_PER_SEC : total_nanos)
//     );
// }

// time_Duration time_Duration_sub(time_Duration lhs, time_Duration rhs) {
//     return makeWith(
//         time_Duration,
//         lhs.secs - rhs.secs - (lhs.nanos < rhs.nanos),
//         lhs.nanos < rhs.nanos ? lhs.nanos + time_NANOS_PER_SEC - rhs.nanos : lhs.nanos - rhs.nanos
//     );
// }

// time_Duration time_Duration_mul(time_Duration d, u64 scalar) {
//     u64 total_nanos = d.nanos * scalar;
//     return makeWith(
//         time_Duration,
//         d.secs * scalar + total_nanos / time_NANOS_PER_SEC,
//         (u32)(total_nanos % time_NANOS_PER_SEC)
//     );
// }

// time_Duration time_Duration_div(time_Duration d, u64 scalar) {
//     u64 total_nanos = d.nanos / scalar;
//     return makeWith(
//         time_Duration,
//         d.secs / scalar + total_nanos / time_NANOS_PER_SEC,
//         (u32)(total_nanos % time_NANOS_PER_SEC)
//     );
// }

// bool time_Duration_eq(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs == rhs.secs && lhs.nanos == rhs.nanos;
// }

// bool time_Duration_ne(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs != rhs.secs || lhs.nanos != rhs.nanos;
// }

// bool time_Duration_lt(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs < rhs.secs || (lhs.secs == rhs.secs && lhs.nanos < rhs.nanos);
// }

// bool time_Duration_le(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs < rhs.secs || (lhs.secs == rhs.secs && lhs.nanos <= rhs.nanos);
// }

// bool time_Duration_gt(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs > rhs.secs || (lhs.secs == rhs.secs && lhs.nanos > rhs.nanos);
// }

// bool time_Duration_ge(time_Duration lhs, time_Duration rhs) {
//     return lhs.secs > rhs.secs || (lhs.secs == rhs.secs && lhs.nanos >= rhs.nanos);
// }

// bool time_Duration_isZero(time_Duration d) {
//     return d.secs == 0 && d.nanos == 0;
// }
