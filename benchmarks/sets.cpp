#include "py_set.h"
#include <benchmark/benchmark.h>
#include <random>
#include <unordered_set>
#include <vector>

constexpr std::size_t N = 10'000;

// iterate
static void BM_unordered_set_iterate(benchmark::State &state) {
    std::unordered_set<int> s;
    for (int i = 0; i < N; ++i) {
        s.insert(i);
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto item : s) {
            sum += item;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_unordered_set_iterate);

static void BM_py_set_iterate(benchmark::State &state) {
    pypp::PySet<int> s;
    for (int i = 0; i < N; ++i) {
        s.add(std::move(i));
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto item : s) {
            sum += item;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_py_set_iterate);

// Helper to generate random numbers
std::vector<int> generate_numbers() {
    std::vector<int> numbers(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, 1000000);
    for (auto &num : numbers)
        num = dist(rng);
    return numbers;
}

// Benchmark std::unordered_set insert
static void BM_unordered_set_insert(benchmark::State &state) {
    auto original_numbers = generate_numbers();
    for (auto _ : state) {
        auto numbers = original_numbers;
        std::unordered_set<int> s;
        for (auto num : numbers) {
            s.insert(num);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_unordered_set_insert);

// Benchmark MySetWrapper insert
static void BM_py_set_add(benchmark::State &state) {
    auto original_numbers = generate_numbers();
    for (auto _ : state) {
        auto numbers = original_numbers;
        pypp::PySet<int> s;
        for (auto num : numbers) {
            s.add(std::move(num));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_set_add);

BENCHMARK_MAIN();
