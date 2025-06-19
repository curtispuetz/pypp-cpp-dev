#include "exceptions/exception.h"
#include "np_arr.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_range.h"
#include "py_set.h"
#include "py_slice.h"
#include "py_str.h"
#include "py_tuple.h"
#include "pypp_util/main_error_handler.h"
#include "pypp_util/to_py_str.h"
#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

void benchmark(const std::string &label, const std::function<void()> &func,
               int runs = 10) {
    // Warm up cache and JIT
    auto start = std::chrono::high_resolution_clock::now();
    func();
    std::cout << label << ":" << std::endl;
    std::cout << std::chrono::duration<double, std::milli>(
                     std::chrono::high_resolution_clock::now() - start)
                     .count()
              << "ms (first call)" << std::endl;

    std::vector<double> timings;

    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        timings.push_back(std::chrono::duration<double, std::milli>(
                              std::chrono::high_resolution_clock::now() - start)
                              .count());
    }

    double avg = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
    std::cout << avg << " ms (average over " << runs << " runs)" << std::endl;
}

int main() {
    try {
        const int size = 50000;

        benchmark("std::vector initialization",
                  [=]() { std::vector<int> vec(size); });

        std::vector<int> vec2(size);
        benchmark("std::vector iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                vec2[i] = i;
            }
        });
        benchmark("std::vector range-based iteration", [&]() {
            for (auto &v : vec2) {
                vec2[0] = v; // just to use the variable
            }
        });
        benchmark("std::vector push_back", [&]() {
            std::vector<int> vec3;
            for (int i = 0; i < size; ++i) {
                vec3.push_back(i);
            }
        });
        // PyList append speed is almost identical to std::vector push_back
        benchmark("PyList append", [&]() {
            PyList<int> py_list;
            for (int i = 0; i < size; ++i) {
                py_list.append(i);
            }
        });

        benchmark("std::array initialization", [&]() {
            std::array<int, size> arr; // fine in main, watch for stack
        });

        std::array<int, size> arr2;
        benchmark("std::array iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                arr2[i] = i;
            }
        });

        // This one is longer. But it adds at most 0.01ms over 50,000
        // iterations. Which would be 0.2ms for 1 million iterations. So, this
        // very rarely would matter and therefore is fine to use.
        benchmark("std::array iteration with PyRange", [&]() {
            for (const auto &i : PyRange(size)) {
                arr2[i] = i;
            }
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}