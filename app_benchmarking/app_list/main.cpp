#include "benchmark.h"
#include "py_list.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
int main() {
    try {
        // Test moving
        PyList<int> py_list1 = {1, 2, 3};
        PyList<PyList<int>> py_list2 = {std::move(py_list1), {4, 5, 6}};
        py_list1.append(7);
        print(py_list2[0]); // Should print [1, 2, 3]
        // Should print [1, 2, 3, 7] (or something else if it was moved
        // properly)
        print(py_list1);

        // Speed test vs std::vector. If a big difference is noticed, then
        // it means the move is likely not working properly.
        // Results: The speed is comparable to std::vector, and the last one
        // that does not use std::move is about 2 times slower on the first
        // call and many times slower on subsequent calls. I think the
        // subsequent calls its not a good marker for this one because it
        // probably does nothing on subsequent calls. The subsequent calls mean
        // nothing in this case. Because the speed of PyList is slower without
        // std::move, I should use std::move when creating a PyList.
        std::vector<int> big_list1(100000, 1);
        PyList<int> py_big_list1(100000, 1);
        std::vector<int> big_list2(100000, 1);
        PyList<int> py_big_list2(100000, 1);
        PyList<int> py_big_list3(100000, 1);
        benchmark("Created nested pylist by moving another big std::vector",
                  [&]() {
                      for (int i = 0; i < 5; ++i) {
                          PyList<std::vector<int>> small_list = {
                              std::move(big_list1), std::move(big_list1)};
                      }
                  });
        benchmark(
            "Created nested std::vector by moving another big std::vector",
            [&]() {
                for (int i = 0; i < 5; ++i) {
                    std::vector<std::vector<int>> small_list = {
                        std::move(big_list2), std::move(big_list2)};
                }
            });
        benchmark("Created nested pylist by moving another big pylist", [&]() {
            for (int i = 0; i < 5; ++i) {
                PyList<PyList<int>> small_list = {std::move(py_big_list1),
                                                  std::move(py_big_list1)};
            }
        });
        benchmark("Created nested std::vector by moving another big pylist",
                  [&]() {
                      for (int i = 0; i < 5; ++i) {
                          std::vector<PyList<int>> small_list = {
                              std::move(py_big_list2), std::move(py_big_list2)};
                      }
                  });
        benchmark(
            "Created nested pylist by moving another big pylist without moving",
            [&]() {
                for (int i = 0; i < 5; ++i) {
                    PyList<PyList<int>> small_list = {py_big_list3,
                                                      py_big_list3};
                }
            });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
