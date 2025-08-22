#include "benchmark.h"
#include "py_set.h"
#include <compy_util/main_error_handler.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <string>

int main() {
    try {
        // Test the speed of differeent ways to initialize a PySet.
        // Results: I think the results are comparable. It depends on which you
        // put first. The first one is typically slower. The average over the
        // 500000 runs is equal.
        // Conclusion: The common way is fine.
        benchmark(
            "PySet optimal", [&]() { PySet<int> py_set({1, 2, 3}); }, 50000);
        benchmark(
            "PySet Common", [&]() { PySet<int> py_set = PySet({1, 2, 3}); },
            50000);

        // Test Add speed
        // Results: The speed is identical to std::unordered_set insert.
        const int size = 50000;
        benchmark("PySet add", [&]() {
            PySet<int> py_set;
            for (int i = 0; i < size; ++i) {
                py_set.add(std::move(i));
            }
        });
        benchmark("std::unordered_set insert (i.e. add)", [&]() {
            std::unordered_set<int> std_set;
            for (int i = 0; i < size; ++i) {
                std_set.insert(i);
            }
        });

        // Test creation speed with big elements vs. std::unordered_set.
        // Results: same speed as std::unordered_set.
        // Conclusion: I am thinking about just not using the std::move
        // and worry about that latter. Because I think for my programs
        // I am going to be fine without it.
        benchmark("PySet with big elements", [&]() {
            PySet<std::string> py_set;
            for (int i = 0; i < 10; ++i) {
                std::string big_string(10000, 'a');
                py_set.add(std::move(big_string));
            }
        });
        benchmark("std::unordered_set with big elements", [&]() {
            std::unordered_set<std::string> std_set;
            for (int i = 0; i < 10; ++i) {
                std::string big_string(10000, 'a');
                std_set.insert(big_string);
            }
        });
        benchmark("std::unordered_set with big elements and std::move", [&]() {
            std::unordered_set<std::string> std_set;
            for (int i = 0; i < 10; ++i) {
                std::string big_string(10000, 'a');
                std_set.insert(std::move(big_string));
            }
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
