#include "benchmark.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <py_dict.h>
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>
#include <unordered_map>

int main() {
    try {
        // Benchmark speed for a standard for loop over std::unordered_map vs.
        // PyDict keys, values, and items methods.
        // Results: for keys(), values(), and items() all speeds are virtually
        // identical. As usual, the very first call seems to be a bit slower, no
        // matter which benchmark is placed first. Conclusion: it is fine to use
        // keys(), values(), and items() to iterate.
        const int size = 100000;
        std::unordered_map<int, int> bigMap;
        for (int i = 0; i < size; ++i) {
            bigMap[i] = i * 2;
        }
        PyDict<int, int> big_py_dict;
        for (int i = 0; i < size; ++i) {
            big_py_dict[std::move(i)] = i * 2;
        }
        benchmark("Standard for loop over std::unordered_map keys", [&]() {
            for (const auto &[key, _] : bigMap) {
                auto value = key * 2;
            }
        });
        benchmark("PyDict keys method", [&]() {
            for (const auto &key : big_py_dict.keys()) {
                auto value = key * 2;
            }
        });
        benchmark("Standard for loop over std::unordered_map values", [&]() {
            for (const auto &[_, value] : bigMap) {
                auto result = value + 1;
            }
        });
        benchmark("PyDict values method", [&]() {
            for (const auto &value : big_py_dict.values()) {
                auto result = value + 1;
            }
        });
        benchmark("Standard for loop over std::unordered_map items", [&]() {
            for (const auto &[key, value] : bigMap) {
                auto result = key + value;
            }
        });
        benchmark("PyDict items method", [&]() {
            for (const auto &item : big_py_dict.items()) {
                auto result = item.get<0>() + item.get<1>();
            }
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
