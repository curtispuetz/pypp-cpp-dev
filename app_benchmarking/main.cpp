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
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

void benchmark(const std::string &label, const std::function<void()> &func,
               int runs = 10) {
    // Warm up cache and JIT
    auto start = std::chrono::high_resolution_clock::now();
    func();
    std::cout << label << ":" << std::endl;
    std::cout << std::chrono::duration<double, std::milli>(
                     std::chrono::high_resolution_clock::now() - start)
                     .count()
              << "ms (first call)" << std::endl;

    std::vector<double> timings;

    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        timings.push_back(std::chrono::duration<double, std::milli>(
                              std::chrono::high_resolution_clock::now() - start)
                              .count());
    }

    double avg = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
    std::cout << avg << " ms (average over " << runs << " runs)" << std::endl;
}

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