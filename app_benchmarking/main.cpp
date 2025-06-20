#include "benchmark.h"
#include "exceptions/exception.h"
#include "np_arr.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_range.h"
#include "py_set.h"
#include "py_slice.h"
#include "py_str.h"
#include "py_tuple.h"
#include "pypp_util/main_error_handler.h"
#include "pypp_util/print.h"
#include "pypp_util/to_py_str.h"
#include <array>
#include <iostream>
#include <vector>

int main() {
    try {
        const int size = 50000;

        benchmark("std::vector initialization",
                  [=]() { std::vector<int> vec(size); });

        std::vector<int> vec2(size);
        benchmark("std::vector iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                vec2[i] = i;
            }
        });
        benchmark("std::vector range-based iteration", [&]() {
            for (auto &v : vec2) {
                vec2[0] = v; // just to use the variable
            }
        });
        benchmark("std::vector push_back", [&]() {
            std::vector<int> vec3;
            for (int i = 0; i < size; ++i) {
                vec3.push_back(i);
            }
        });
        // PyList append speed is almost identical to std::vector push_back
        benchmark("PyList append", [&]() {
            PyList<int> py_list;
            for (int i = 0; i < size; ++i) {
                py_list.append(i);
            }
        });

        benchmark("std::array initialization", [&]() {
            std::array<int, size> arr; // fine in main, watch for stack
        });

        std::array<int, size> arr2;
        benchmark("std::array iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                arr2[i] = i;
            }
        });

        // This one is longer. But it adds at most 0.01ms over 50,000
        // iterations. Which would be 0.2ms for 1 million iterations. So, this
        // very rarely would matter and therefore is fine to use.
        benchmark("std::array iteration with PyRange", [&]() {
            for (const auto &i : PyRange(size)) {
                arr2[i] = i;
            }
        });

        // hard_situations_for_pypp.py
        int list_size = 50000;
        int iter_length = 10;
        PyList<int> larger_list = PyList({1}) * list_size;
        benchmark("create list of the same list", [&]() {
            PyList<PyList<int>> list;
            for (int i = 0; i < iter_length; ++i) {
                list.append(larger_list);
            }
        });
        benchmark("create list of many creadted lists", [&]() {
            PyList<PyList<int>> list;
            for (int i = 0; i < iter_length; ++i) {
                PyList<int> inner = PyList<int>({1}) * list_size;
                list.append(inner);
            }
        });
        benchmark("create list of many creadted lists directly", [&]() {
            PyList<PyList<int>> list;
            for (int i = 0; i < iter_length; ++i) {
                list.append(PyList<int>({1}) * list_size);
            }
        });
        benchmark("create list of many creadted lists with move constructor",
                  [&]() {
                      PyList<PyList<int>> list;
                      for (int i = 0; i < iter_length; ++i) {
                          PyList<int> inner = PyList<int>({1}) * list_size;
                          list.append(std::move(inner));
                      }
                  });
        benchmark("create list of many creadted lists with move constructor, "
                  "different way",
                  [&]() {
                      // no different to above.
                      PyList<PyList<int>> list;
                      for (int i = 0; i < iter_length; ++i) {
                          list.append(std::move(PyList<int>({1}) * list_size));
                      }
                  });
        benchmark("create list of many creadted lists with move constructor, "
                  "different second way",
                  [&]() {
                      // This one is slower than above.
                      PyList<PyList<int>> list;
                      for (int i = 0; i < iter_length; ++i) {
                          PyList<int> inner = PyList<int>({1});
                          inner *= list_size;
                          list.append(std::move(PyList<int>({1}) * list_size));
                      }
                  });
        benchmark("create list of many creadted lists with move constructor, "
                  "different new third way",
                  [&]() {
                      // This is equivalent to the speed of the std::vector
                      // version below.
                      PyList<PyList<int>> list;
                      for (int i = 0; i < iter_length; ++i) {
                          list.append(
                              std::move(create_list_full(list_size, 1)));
                      }
                  });
        benchmark("std::vector equavlanet to above", [&]() {
            std::vector<std::vector<int>> list;
            list.reserve(iter_length);
            for (int i = 0; i < iter_length; ++i) {
                std::vector<int> inner;
                inner.reserve(list_size);
                inner.assign(list_size, 1);
                list.push_back(std::move(inner));
            }
        });
        benchmark("std::vector equavlanet to above, different way", [&]() {
            // no different to above.
            std::vector<std::vector<int>> list;
            list.reserve(iter_length);
            for (int i = 0; i < iter_length; ++i) {
                std::vector<int> inner(list_size, 1);
                list.push_back(std::move(inner));
            }
        });

        std::vector<std::vector<int>> vec_of_vecs(10, std::vector<int>(10, 1));
        vec_of_vecs[0][0] = 2; // just to use the variable
        print(vec_of_vecs[0][0]);
        print(vec_of_vecs[1][0]);
        PyList<PyList<int>> py_list_of_lists(10, PyList<int>(10, 1));
        py_list_of_lists[0][0] = 2; // just to use the variable
        print(py_list_of_lists[0][0]);
        print(py_list_of_lists[1][0]);

        std::vector<int> vec_of_ints(10, 1);
        int insert_val = 5;
        vec_of_ints.push_back(std::move(insert_val));
        // Note: primitive types like int are moved by value no matter what,
        // so the std::move does not change the behavior here.
        print(insert_val); // should be 5

        // Test the speed of different PyList initialization methods.
        // Results: equal to each other.
        benchmark(
            "PyList Common",
            [&]() { PyList<int> py_list = PyList({1, 2, 3, 4, 5}); }, 50000);
        benchmark(
            "PyList optimal", [&]() { PyList<int> py_list({1, 2, 3, 4, 5}); },
            50000);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}