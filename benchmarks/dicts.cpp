#include "py_dict.h"
#include <benchmark/benchmark.h>
#include <random>
#include <unordered_map>

constexpr std::size_t LRG_N = 100'000;
constexpr std::size_t N = 10'000;
constexpr std::size_t LOOKUP_N = 100'000;

// Results: all same
static void BM_unordered_map_insert(benchmark::State &state) {
    for (auto _ : state) {
        std::unordered_map<int, int> map;
        for (int i = 0; i < N; ++i) {
            map[i] = i;
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_unordered_map_insert);

static void BM_py_dict_insert(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyDict<int, int> map;
        for (int i = 0; i < N; ++i) {
            map[i] = i;
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_dict_insert);

static void BM_unordered_map_lookup(benchmark::State &state) {
    std::unordered_map<int, int> map;
    for (int i = 0; i < LOOKUP_N; ++i) {
        map[i] = i;
    }
    for (auto _ : state) {
        for (int i = 0; i < LOOKUP_N; ++i) {
            benchmark::DoNotOptimize(map.find(i)->second);
        }
    }
}
BENCHMARK(BM_unordered_map_lookup);

// dg lookup is the same.
static void BM_py_dict_lookup(benchmark::State &state) {
    pypp::PyDict<int, int> map;
    for (int i = 0; i < LOOKUP_N; ++i) {
        map[i] = i;
    }
    for (auto _ : state) {
        for (int i = 0; i < LOOKUP_N; ++i) {
            benchmark::DoNotOptimize(map.dg(i));
        }
    }
}
BENCHMARK(BM_py_dict_lookup);

static void BM_unordered_map_update(benchmark::State &state) {
    std::unordered_map<int, int> other_original;
    for (int i = 0; i < N; ++i) {
        other_original[i] = i;
    }
    for (auto _ : state) {
        auto other = other_original;
        std::unordered_map<int, int> map;
        for (auto &[k, v] : other) {
            map.insert_or_assign(std::move(k), std::move(v));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_unordered_map_update);

static void BM_py_dict_update(benchmark::State &state) {
    pypp::PyDict<int, int> other_original;
    for (int i = 0; i < N; ++i) {
        other_original[i] = i;
    }
    for (auto _ : state) {
        auto other = other_original;
        pypp::PyDict<int, int> map;
        map.update(std::move(other));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_dict_update);

static void BM_unordered_map_iteration(benchmark::State &state) {
    std::unordered_map<int, std::vector<int>> map;
    for (int i = 0; i < N; ++i) {
        map[i] = {i,     i + 1, i + 2, i + 3, i + 4,
                  i + 5, i + 6, i + 7, i + 8, i + 9};
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto &[key, val] : map) {
            sum += key;
            for (const auto &v : val) {
                sum += v;
            }
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_unordered_map_iteration);

static void BM_py_dict_iteration(benchmark::State &state) {
    pypp::PyDict<int, std::vector<int>> map;
    for (int i = 0; i < N; ++i) {
        map[i] = {i,     i + 1, i + 2, i + 3, i + 4,
                  i + 5, i + 6, i + 7, i + 8, i + 9};
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto &[key, val] : map.items()) {
            sum += key;
            for (const auto &v : val) {
                sum += v;
            }
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_dict_iteration);

static void BM_unordered_map_iteration_normal(benchmark::State &state) {
    std::unordered_map<int, int> map;
    for (int i = 0; i < N; ++i) {
        map[i] = i;
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto &item : map) {
            sum += item.first;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_unordered_map_iteration_normal);

static void BM_py_dict_iteration_keys(benchmark::State &state) {
    pypp::PyDict<int, int> map;
    for (int i = 0; i < N; ++i) {
        map[i] = i;
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto &item : map.keys()) {
            sum += item;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_dict_iteration_keys);

static void BM_py_dict_iteration_values(benchmark::State &state) {
    pypp::PyDict<int, int> map;
    for (int i = 0; i < N; ++i) {
        map[i] = i;
    }
    for (auto _ : state) {
        int sum = 0;
        for (const auto &val : map.values()) {
            sum += val;
        }
        benchmark::DoNotOptimize(sum);
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_dict_iteration_values);

BENCHMARK_MAIN();
