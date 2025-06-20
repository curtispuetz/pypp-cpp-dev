#include "benchmark.h"
#include "py_list.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        const int size = 50000;
        // Test initializing a vector with a size vs initializing as empty
        benchmark("std::vector with size", [&]() {
            // This is 2 times as fast as the empty vector on the first call
            // and 10 times as fast on subsequent calls.
            std::vector<int> vec_with_size(size);
            for (int i = 0; i < size; ++i) {
                vec_with_size[i] = i;
            }
        });
        benchmark("std::vector empty then push_back", [&]() {
            std::vector<int> vec_empty;
            for (int i = 0; i < size; ++i) {
                vec_empty.push_back(i);
            }
        });
        // Test the same with PyList
        // The results are equivalent to std::vector
        benchmark("PyList with size", [&]() {
            // The results are the same here as std::vector. 2 times as fast on
            // the first call and 10 times as fast on subsequent calls.
            // Very Nice!
            PyList<int> py_list_with_size(size);
            for (int i = 0; i < size; ++i) {
                py_list_with_size[i] = i;
            }
        });
        benchmark("PyList empty then append", [&]() {
            PyList<int> py_list_empty;
            for (int i = 0; i < size; ++i) {
                py_list_empty.append(i);
            }
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
