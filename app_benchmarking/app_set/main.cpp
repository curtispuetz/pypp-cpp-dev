#include "benchmark.h"
#include "py_set.h"
#include "pypp_util/create/set.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
#include <string>

int main() {
    try {
        // Test the speed of differeent ways to initialize a pypp::PySet.
        // Results: I think the results are comparable. It depends on which you
        // put first. The first one is typically slower. The average over the
        // 500000 runs is equal.
        // Conclusion: The common way is fine.
        benchmark(
            "pypp::PySet optimal",
            [&]() { pypp::PySet<int> py_set({1, 2, 3}); }, 50000);
        benchmark(
            "pypp::PySet Common",
            [&]() { pypp::PySet<int> py_set = pypp::PySet({1, 2, 3}); }, 50000);

        // Test Add speed
        // Results: The speed is identical to std::unordered_set insert.
        const int size = 50000;
        benchmark("pypp::PySet add", [&]() {
            pypp::PySet<int> py_set;
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
        benchmark("pypp::PySet with big elements", [&]() {
            pypp::PySet<std::string> py_set;
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

        // Constructing
        pypp::PySet<int> py_set_a = {1, 2, 3, 4, 5};
        pypp::PySet<int> py_set_b = pypp::set(py_set_a);
        py_set_a.add(6);
        pypp::print(py_set_a);
        pypp::print(py_set_b);

        pypp::PyList<int> py_list_a = {1, 2, 3, 4};
        pypp::PySet<int> py_set_c = pypp::set(py_list_a);
        pypp::print(py_set_c);

        pypp::PyDict<int, int> py_dict_a = {{1, 2}, {3, 4}, {5, 6}};
        pypp::PySet<int> py_set_d = pypp::set(py_dict_a);
        pypp::print(py_set_d);

        pypp::PyStr py_str = pypp::PyStr("hello");
        pypp::PySet<pypp::PyStr> py_set_e = pypp::set(py_str);
        pypp::print(py_set_e);

        // with an inline list
        pypp::PySet<int> py_set_f = pypp::set(pypp::PyList<int>({1, 2, 3}));
        pypp::print(py_set_f);

        // empty
        pypp::PySet<int> py_set_g = pypp::set<int>();
        pypp::print(py_set_g);

        // copy
        pypp::PySet<int> py_set_h = py_set_a.copy();
        py_set_a.add(7);
        pypp::print(py_set_h);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
