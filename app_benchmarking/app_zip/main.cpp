#include "benchmark.h"
#include "py_list.h"
#include "py_zip.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // benchmark speed for a standard for loop vs. PyZip
        // Results: effectively no speed difference. The first
        // call is a little bit slower, like usual.
        // Conclusion: usage of zip is fine.
        benchmark(
            "Standard for loop speed",
            [&]() {
                PyList<int> list1 = {1, 2, 3, 4, 5};
                PyList<int> list2 = {6, 7, 8, 9, 10};
                for (size_t i = 0; i < list1.len(); ++i) {
                    auto value1 = list1[i];
                    auto value2 = list2[i];
                }
            },
            1000);
        benchmark(
            "PyZip speed",
            [&]() {
                PyList<int> list1 = {1, 2, 3, 4, 5};
                PyList<int> list2 = {6, 7, 8, 9, 10};
                for (const auto &py_tup : PyZip(list1, list2)) {
                    auto value1 = py_tup.get<0>();
                    auto value2 = py_tup.get<1>();
                }
            },
            1000);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
