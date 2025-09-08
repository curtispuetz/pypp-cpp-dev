#include "benchmark.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_str.h"
#include "pypp_util/create/list.h"
#include "pypp_util/main_error_handler.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE

int main() {
    try {
        // Test moving before benchmarking to show it works as expected
        pypp::PyList<int> py_list1 = {1, 2, 3};
        pypp::PyList<pypp::PyList<int>> py_list2 = {std::move(py_list1),
                                                    {4, 5, 6}};
        py_list1.append(7);
        pypp::print(py_list2[0]); // Should print [1, 2, 3]
        // Should print [1, 2, 3, 7] (or something else if it was moved
        // properly)
        pypp::print(py_list1);

        // Test creation speed with big elements vs std::vector.
        // Results: The speed is comparable to std::vector, and the last one
        // that does not use std::move is about 2 times slower on the first
        // call and many times slower on subsequent calls. I think the
        // subsequent calls its not a good marker for this one because it
        // probably does nothing on subsequent calls. The subsequent calls mean
        // nothing in this case. Because the speed of pypp::PyList is slower
        // without std::move. Conclusion: I should use std::move when creating a
        // pypp::PyList. It seems to be working properly.
        std::vector<int> big_list1(100000, 1);
        pypp::PyList<int> py_big_list1(100000, 1);
        std::vector<int> big_list2(100000, 1);
        pypp::PyList<int> py_big_list2(100000, 1);
        pypp::PyList<int> py_big_list3(100000, 1);
        benchmark("Created nested pylist by moving another big std::vector",
                  [&]() {
                      pypp::PyList<std::vector<int>> small_list = {
                          std::move(big_list1), std::move(big_list1)};
                  });
        benchmark(
            "Created nested std::vector by moving another big std::vector",
            [&]() {
                std::vector<std::vector<int>> small_list = {
                    std::move(big_list2), std::move(big_list2)};
            });
        benchmark("Created nested pylist by moving another big pylist", [&]() {
            pypp::PyList<pypp::PyList<int>> small_list = {
                std::move(py_big_list1), std::move(py_big_list1)};
        });
        benchmark("Created nested std::vector by moving another big pylist",
                  [&]() {
                      std::vector<pypp::PyList<int>> small_list = {
                          std::move(py_big_list2), std::move(py_big_list2)};
                  });
        benchmark("Created nested pylist by copying another big pylist", [&]() {
            pypp::PyList<pypp::PyList<int>> small_list = {py_big_list3,
                                                          py_big_list3};
        });

        // Do above test but with a different type (pypp::PyDict) to be sure
        // Results: The speed is identical.
        pypp::PyDict<int, pypp::PyStr> big_py_dict1;
        for (int i = 0; i < 100000; ++i) {
            big_py_dict1[std::move(i)] = pypp::PyStr("test");
        }
        pypp::PyDict<int, pypp::PyStr> big_py_dict2;
        for (int i = 0; i < 100000; ++i) {
            big_py_dict2[std::move(i)] = pypp::PyStr("test");
        }
        benchmark("Created pylist by moving a big pypp::PyDict", [&]() {
            pypp::PyList<pypp::PyDict<int, pypp::PyStr>> small_list = {
                std::move(big_py_dict1), std::move(big_py_dict1)};
        });
        benchmark("Created std::vector by moving a big pypp::PyDict", [&]() {
            std::vector<pypp::PyDict<int, pypp::PyStr>> small_list = {
                std::move(big_py_dict2), std::move(big_py_dict2)};
        });

        // Test the speed of different ways to initialize.
        // Results: equal to each other.
        benchmark(
            "pypp::PyList Common",
            [&]() {
                pypp::PyList<int> py_list = pypp::PyList({1, 2, 3, 4, 5});
            },
            50000);
        benchmark(
            "pypp::PyList optimal",
            [&]() { pypp::PyList<int> py_list({1, 2, 3, 4, 5}); }, 50000);

        // Test pypp::PyList appending vs std::vector push_back for integers
        // Results: The speed is almost identical.
        const int size = 50000;
        benchmark("std::vector push_back", [&]() {
            std::vector<int> vec3;
            for (int i = 0; i < size; ++i) {
                vec3.push_back(i);
            }
        });
        // pypp::PyList append speed is almost identical to std::vector
        // push_back
        benchmark("pypp::PyList append", [&]() {
            pypp::PyList<int> py_list;
            for (int i = 0; i < size; ++i) {
                py_list.append(std::move(i));
            }
        });

        benchmark(
            "pypp::PyList repeating",
            [&]() {
                pypp::PyList<int> py_list;
                py_list = pypp::PyList({1}) * 100000;
            },
            1000);
        benchmark(
            "pypp::PyList constructor with size",
            [&]() { pypp::PyList<double> py_list(100000, 1.0); }, 1000);
        benchmark(
            "pypp::PyList constructor with size no value",
            [&]() { pypp::PyList<int> py_list(100000); }, 1000);

        // Constructing
        pypp::PyList<int> py_list_a = {1, 2, 3, 4, 5};
        pypp::PyList<int> py_list_b = pypp::list(py_list_a);
        py_list_a.append(6);
        pypp::print(py_list_a);
        pypp::print(py_list_b);

        pypp::PySet<int> py_set_a = {1, 2, 3, 4};
        pypp::PyList<int> py_list_c = pypp::list(py_set_a);
        print(py_list_c);

        pypp::PyDict<int, int> py_dict_a = {{1, 2}, {3, 4}, {5, 6}};
        pypp::PyList<int> py_list_d = pypp::list(py_dict_a);
        print(py_list_d);

        pypp::PyStr py_str = pypp::PyStr("hello");
        pypp::PyList<pypp::PyStr> py_list_e = pypp::list(py_str);
        print(py_list_e);

        // empty list
        pypp::PyList<int> py_list_f = pypp::list<int>();
        pypp::print(py_list_f);

        // copy
        pypp::PyList<int> py_list_g = py_list_a.copy();
        py_list_a.append(7);
        pypp::print(py_list_g);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
