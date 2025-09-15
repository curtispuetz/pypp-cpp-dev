#include "py_list.h"
#include "py_zip.h"
#include "src/benchmark.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Benchmark speed for a standard for loop vs. pypp::PyZip
        // Results: For large lists, zip is about 5 times slower. For small
        // lists, there is no noticable difference.
        // Conclusion: for many
        // practical purposes, zip is fine, but for high performant tight loops,
        // it is better to use the standard for loop.
        benchmark(
            "Standard for loop speed",
            [&]() {
                pypp::PyList<int> list1 = {1, 2, 3, 4, 5};
                pypp::PyList<int> list2 = {6, 7, 8, 9, 10};
                for (size_t i = 0; i < list1.len(); ++i) {
                    auto value1 = list1[i];
                    auto value2 = list2[i];
                }
            },
            1000);
        benchmark(
            "pypp::PyZip speed",
            [&]() {
                pypp::PyList<int> list1 = {1, 2, 3, 4, 5};
                pypp::PyList<int> list2 = {6, 7, 8, 9, 10};
                for (const auto &[val1, val2] : pypp::PyZip(list1, list2)) {
                    auto value1 = val1;
                    auto value2 = val2;
                }
            },
            1000);

        // For large lists

        pypp::PyList<int> large_list1(100000, 1);
        pypp::PyList<int> large_list2(100000, 2);

        benchmark(
            "Standard for loop speed with large lists",
            [&]() {
                for (size_t i = 0; i < large_list1.len(); ++i) {
                    auto value1 = large_list1[i];
                    auto value2 = large_list2[i];
                }
            },
            1000);
        benchmark(
            "pypp::PyZip speed with large lists",
            [&]() {
                for (const auto &[val1, val2] :
                     pypp::PyZip(large_list1, large_list2)) {
                    auto value1 = val1;
                    auto value2 = val2;
                }
            },
            1000);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
