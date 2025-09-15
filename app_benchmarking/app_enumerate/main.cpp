#include "src/benchmark.h"
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

        pypp::PyList<int> long_list(100000, 5);

        // benchmark speed to a standard for loop vs pypp::PyEnumerate
        // Results: effectively no speed difference with a small sized list
        // but for a large list, PyEnumerate can be like 2.5 times slower.
        // Conclusion: In performance critical tight loops a standard for loop
        // is prefered, but for lots of practical cases PyEnumerate is fine.
        benchmark(
            "Standard for loop speed",
            [&]() {
                for (size_t i = 0; i < long_list.len(); ++i) {
                    auto value = long_list[i];
                }
            },
            1000);
        benchmark(
            "pypp::PyEnumerate speed",
            [&]() {
                for (const auto &[i, v] : pypp::PyEnumerate(long_list)) {
                    auto value = v;
                }
            },
            1000);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
