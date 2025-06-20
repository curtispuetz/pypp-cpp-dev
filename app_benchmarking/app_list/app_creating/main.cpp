#include "benchmark.h"
#include "py_list.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
int main() {
    try {
        // Different ways to create a PyList
        // Results: The fastest way was found to create a PyList given a size
        // and the results were comparable to std::vector.
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

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
