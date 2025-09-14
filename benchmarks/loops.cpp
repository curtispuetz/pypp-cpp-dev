#include "py_enumerate.h"
#include "py_list.h"
#include "py_reversed.h"
#include "py_zip.h"
#include <benchmark/benchmark.h>
#include <random>
#include <unordered_set>

constexpr std::size_t N = 1'000'000;

pypp::PyList<int> create_random_list() {
    pypp::PyList<int> vec(N);
    for (int i = 0; i < N; ++i) {
        vec[i] = i;
    }
    // Shuffle the vector to avoid any caching effects
    std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});
    return vec;
}

static void BM_standard_for_loop(benchmark::State &state) {
    pypp::PyList<int> vec = create_random_list();
    for (auto _ : state) {
        int sum = 0;
        for (std::size_t i = 0; i < N; ++i) {
            sum += vec[i];
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_standard_for_loop);

static void BM_py_enumerate(benchmark::State &state) {
    pypp::PyList<int> vec = create_random_list();
    for (auto _ : state) {
        int sum = 0;
        for (const auto &[i, val] : pypp::PyEnumerate(vec)) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_py_enumerate);

static void BM_py_reversed(benchmark::State &state) {
    pypp::PyList<int> vec = create_random_list();
    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : pypp::PyReversed(vec)) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_py_reversed);

static void BM_standard_zip(benchmark::State &state) {
    pypp::PyList<int> vec1 = create_random_list();
    pypp::PyList<int> vec2 = create_random_list();
    for (auto _ : state) {
        int sum = 0;
        for (std::size_t i = 0; i < N; ++i) {
            sum += vec1[i] + vec2[i];
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_standard_zip);

static void BM_py_zip(benchmark::State &state) {
    pypp::PyList<int> vec1 = create_random_list();
    pypp::PyList<int> vec2 = create_random_list();
    for (auto _ : state) {
        int sum = 0;
        for (const auto &[v1, v2] : pypp::PyZip(vec1, vec2)) {
            sum += v1 + v2;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_py_zip);

BENCHMARK_MAIN();
