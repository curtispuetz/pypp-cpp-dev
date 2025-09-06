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
        // Using pypp::PyEnumerate
        pypp::PyList<pypp::PyStr> fruits = {
            pypp::PyStr("apple"), pypp::PyStr("banana"), pypp::PyStr("cherry")};
        for (const auto &[i, val] : pypp::PyEnumerate(fruits)) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }
        std::cout << std::endl;
        for (const auto &[i, val] :
             pypp::PyEnumerate(pypp::PyList({1, 2, 3}))) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }

        // benchmark speed to a standard for loop vs pypp::PyEnumerate
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
            "pypp::PyEnumerate speed",
            [&]() {
                for (const auto &[i, fruit] : pypp::PyEnumerate(fruits)) {
                    auto value = fruit;
                }
            },
            1000);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
