#include "benchmark.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "py_str.h"
#include "py_zip.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <py_reversed.h>
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Show usage of pypp::PyReversed (using it with pypp::PyZip here too!)
        pypp::PyList<int> numbers = pypp::PyList<int>({10, 20, 30});
        pypp::PyList<pypp::PyStr> words = pypp::PyList<pypp::PyStr>(
            {pypp::PyStr("one"), pypp::PyStr("two"), pypp::PyStr("three")});
        std::cout << "Reversing a vector of integers:" << std::endl;
        for (const auto &[word, number] :
             pypp::PyZip(pypp::PyReversed(words), pypp::PyReversed(numbers))) {
            std::cout << "Word: " << word << ", Number: " << number
                      << std::endl;
        }
        std::cout << "\n\n";

        // Benchmark speed for a standard for loop vs. pypp::PyReversed
        // Results: effectively no speed difference.
        // Conclusion: usage of reversed is fine.
        benchmark(
            "Standard for loop speed",
            [&]() {
                pypp::PyList<int> list1 = {1, 2, 3, 4, 5};
                for (size_t i = 0; i < list1.len(); ++i) {
                    auto value = list1[i];
                    auto v = value * 2;
                }
            },
            1000);
        benchmark(
            "pypp::PyReversed speed",
            [&]() {
                pypp::PyList<int> list1 = {1, 2, 3, 4, 5};
                for (const auto &value : pypp::PyReversed(list1)) {
                    auto v = value * 2;
                }
            },
            1000);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
