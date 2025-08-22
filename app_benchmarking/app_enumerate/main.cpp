#include "benchmark.h"
#include <compy_util/main_error_handler.h>
#include <compy_util/print.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <py_enumerate.h>
#include <py_list.h>
#include <py_str.h>

int main() {
    try {
        // Using PyEnumerate
        PyList<PyStr> fruits = {PyStr("apple"), PyStr("banana"),
                                PyStr("cherry")};
        for (const auto &[i, val] : PyEnumerate(fruits)) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }
        std::cout << std::endl;
        for (const auto &[i, val] : PyEnumerate(PyList({1, 2, 3}))) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }

        // benchmark speed to a standard for loop vs PyEnumerate
        // Results: effectively no speed difference.
        benchmark(
            "Standard for loop speed",
            [&]() {
                for (size_t i = 0; i < fruits.len(); ++i) {
                    auto value = fruits[i];
                }
            },
            1000);
        benchmark(
            "PyEnumerate speed",
            [&]() {
                for (const auto &[i, fruit] : PyEnumerate(fruits)) {
                    auto value = fruit;
                }
            },
            1000);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
