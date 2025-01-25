#include <algorithm>
#include <chrono>
#include <vector>

#include "benchmark_cpp_stable_sort.hpp"

/*========== Benchmark Implementation =======================================*/

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

BenchResult benchmark_cpp_stable_sort(BenchParams params) {
    // Copy test data
    std::vector<TestElem> cpp_data(params.len);
    std::copy(params.ptr, params.ptr + params.len, cpp_data.data()); // NOLINT

    // Benchmark std::stable_sort
    auto       overhead_time = std::chrono::milliseconds(0);
    const auto begin         = std::chrono::high_resolution_clock::now();
    for (usize i = 0; i < params.iterations; ++i) {
        const auto overhead_begin = std::chrono::high_resolution_clock::now();
        std::copy(params.ptr, params.ptr + params.len, cpp_data.data()); // NOLINT
        overhead_time += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - overhead_begin);
        std::stable_sort(cpp_data.begin(), cpp_data.end());
    }
    const auto end      = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin - overhead_time);
    const f64  total_ms = static_cast<f64>(duration.count());
    const f64  avg_ms   = total_ms / (f64)params.iterations;

    // Verify stability for implementations
    bool stable = true;
    for (usize i = 1; i < params.len; ++i) {
        if (cpp_data.at(i).value == cpp_data.at(i - 1).value
            && cpp_data.at(i).seq <= cpp_data.at(i - 1).seq) {
            stable = false;
        }
    }

    return BenchResult{
        .name     = "std::stable_sort",
        .total_ms = total_ms,
        .avg_ms   = avg_ms,
        .stable   = stable
    };
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
