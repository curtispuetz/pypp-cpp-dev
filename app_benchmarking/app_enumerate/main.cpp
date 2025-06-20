#include "benchmark.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <py_enumerate.h>
#include <py_list.h>
#include <py_str.h>
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>

int main() {
    try {
        // Using PyEnumerate
        PyList<PyStr> fruits = {PyStr("apple"), PyStr("banana"),
                                PyStr("cherry")};
        for (const auto &py_tup : PyEnumerate(fruits)) {
            std::cout << "Index: " << py_tup.get<0>()
                      << ", Value: " << py_tup.get<1>() << std::endl;
        }
        std::cout << std::endl;
        for (const auto &py_tup : PyEnumerate(PyList({1, 2, 3}))) {
            std::cout << "Index: " << py_tup.get<0>()
                      << ", Value: " << py_tup.get<1>() << std::endl;
        }

        // benchmark speed to a standard for loop
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
                for (const auto &py_tup : PyEnumerate(fruits)) {
                    auto value = py_tup.get<1>();
                }
            },
            1000);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
