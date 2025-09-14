#include "py_list.h"
#include "slice/creators.h"
#include <benchmark/benchmark.h>
#include <vector>

constexpr std::size_t N = 1'000'000;
constexpr std::size_t K = 10000;
constexpr std::size_t M = 4000;

static void BM_std_vector_iterate(benchmark::State &state) {
    std::vector<int> v(N);
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = static_cast<int>(i);
    }
    for (const auto _ : state) {
        int sum = 0;
        for (auto item : v) {
            sum += item;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_std_vector_iterate);

static void BM_py_list_iterate(benchmark::State &state) {
    pypp::PyList<int> v(N);
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = static_cast<int>(i);
    }
    for (const auto _ : state) {
        int sum = 0;
        for (auto item : v) {
            sum += item;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_py_list_iterate);

// Results: same speed
static void BM_std_vector_set(benchmark::State &state) {
    std::vector<int> v(N);
    for (auto _ : state) {
        for (std::size_t i = 0; i < v.size(); ++i) {
            v[i] = static_cast<int>(i);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_set);

static void BM_py_list_set(benchmark::State &state) {
    pypp::PyList<int> v(N);
    for (auto _ : state) {
        for (std::size_t i = 0; i < v.len(); ++i) {
            v[i] = static_cast<int>(i);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_set);

// Results: same speed
static void BM_std_vector_push_back(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) {
            v.push_back(static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_push_back);

static void BM_py_list_append(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N; ++i) {
            v.append(static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_append);

static void BM_std_vector_pop_back(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v(N);
        for (std::size_t i = 0; i < N; ++i) {
            v.pop_back();
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_pop_back);

static void BM_py_list_pop(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v(N);
        for (std::size_t i = 0; i < N; ++i) {
            v.pop();
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_pop);

static void BM_std_vector_erase_begin(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.erase(v.begin());
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_erase_begin);

static void BM_py_list_pop_begin(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.pop(0);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_pop_begin);

static void BM_std_vector_erase_middle(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.erase(v.begin() + v.size() / 2);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_erase_middle);

static void BM_py_list_pop_middle(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.pop(v.len() / 2);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_pop_middle);

static void BM_std_vector_insert_begin(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.insert(v.begin(), static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_insert_begin);

static void BM_py_list_insert_begin(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v;
        v.reserve(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.insert(0, static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_insert_begin);

static void BM_std_vector_insert_middle(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.insert(v.begin() + v.size() / 2, static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_insert_middle);

static void BM_py_list_insert_middle(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v;
        v.reserve(M);
        for (std::size_t i = 0; i < M; ++i) {
            v.insert(v.len() / 2, static_cast<int>(i));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_insert_middle);

static void BM_std_vector_extend(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N / K; ++i) {
            std::vector<int> to_add(K);
            for (std::size_t i = 0; i < K; ++i) {
                to_add[i] = static_cast<int>(i);
            }
            v.insert(v.end(), std::make_move_iterator(to_add.begin()),
                     std::make_move_iterator(to_add.end()));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_extend);

static void BM_py_list_extend(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v;
        v.reserve(N);
        for (std::size_t i = 0; i < N / K; ++i) {
            pypp::PyList<int> to_add(K);
            for (std::size_t i = 0; i < K; ++i) {
                to_add[i] = static_cast<int>(i);
            }
            v.extend(std::move(to_add));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_extend);

static void BM_std_vector_slice(benchmark::State &state) {
    std::vector<int> v(N);
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = static_cast<int>(i);
    }
    for (auto _ : state) {
        std::vector<int> slice;
        slice.reserve(N / 10);
        for (std::size_t i = 0; i < N; i += 10) {
            slice.push_back(v[i]);
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_slice);

// Note: This is a little less efficient than the manual approach with the
// for loop
static void BM_py_list_slice(benchmark::State &state) {
    pypp::PyList<int> v(N);
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = static_cast<int>(i);
    }
    for (auto _ : state) {
        pypp::PyList<int> slice = v[pypp::py_slice(0, std::nullopt, 10)];
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_slice);

static void BM_std_vector_slice_manual_move(benchmark::State &state) {
    for (auto _ : state) {
        std::vector<int> v(N);
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<int>(i);
        }
        std::vector<int> slice;
        slice.reserve(N / 10);
        for (std::size_t i = 0; i < N; i += 10) {
            slice.push_back(std::move(v[i]));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_std_vector_slice_manual_move);

static void BM_py_list_manual_slice(benchmark::State &state) {
    for (auto _ : state) {
        pypp::PyList<int> v(N);
        for (std::size_t i = 0; i < N; ++i) {
            v[i] = static_cast<int>(i);
        }
        pypp::PyList<int> slice;
        slice.reserve(N / 10);
        for (std::size_t i = 0; i < N; i += 10) {
            slice.append(std::move(v[i]));
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_py_list_manual_slice);

BENCHMARK_MAIN();
