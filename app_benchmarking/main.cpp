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

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}