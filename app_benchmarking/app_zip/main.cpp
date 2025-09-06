#include "benchmark.h"
#include "py_list.h"
#include "py_zip.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Benchmark speed for a standard for loop vs. pypp::PyZip
        // Results: effectively no speed difference. The first
        // call is a little bit slower, like usual.
        // Conclusion: usage of zip is fine.
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

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
