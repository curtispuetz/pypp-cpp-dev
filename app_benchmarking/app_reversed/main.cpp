#include "benchmark.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "py_str.h"
#include "py_zip.h"
#include <compy_util/main_error_handler.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <py_reversed.h>

int main() {
    try {
        // Show usage of PyReversed (using it with PyZip here too!)
        PyList<int> numbers = PyList<int>({10, 20, 30});
        PyList<PyStr> words =
            PyList<PyStr>({PyStr("one"), PyStr("two"), PyStr("three")});
        std::cout << "Reversing a vector of integers:" << std::endl;
        for (const auto &[word, number] :
             PyZip(PyReversed(words), PyReversed(numbers))) {
            std::cout << "Word: " << word << ", Number: " << number
                      << std::endl;
        }
        std::cout << "\n\n";

        // Benchmark speed for a standard for loop vs. PyReversed
        // Results: effectively no speed difference.
        // Conclusion: usage of reversed is fine.
        benchmark(
            "Standard for loop speed",
            [&]() {
                PyList<int> list1 = {1, 2, 3, 4, 5};
                for (size_t i = 0; i < list1.len(); ++i) {
                    auto value = list1[i];
                    auto v = value * 2;
                }
            },
            1000);
        benchmark(
            "PyReversed speed",
            [&]() {
                PyList<int> list1 = {1, 2, 3, 4, 5};
                for (const auto &value : PyReversed(list1)) {
                    auto v = value * 2;
                }
            },
            1000);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
