#include "py_list.h"
#include "py_str.h"
#include "py_tuple.h"
#include "pypp_util/print.h"
#include "src/benchmark.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
    // Test moving before benchmarking to show it works as expected
    pypp:
        pypp::PyStr str1("Hello");
        pypp::PyList<int> list1 = {1, 2, 3};
        pypp::PyTup<pypp::PyStr, pypp::PyList<int>> py_tup1 =
            pypp::PyTup(std::move(str1), std::move(list1));
        list1.append(4);
        pypp::print(py_tup1.get<0>()); // Should print "Hello"
        pypp::print(py_tup1.get<1>()); // Should print [1, 2, 3]
        // Should print [1, 2, 3, 4] (or something else if it was moved
        // properly)
        pypp::print(list1);

        // Test creation speed with big elements vs. std::tuple.
        // Results: They depend on which order you run the benchmark tests. The
        // first one is typically slower. After trying different orders, it
        // looks like they are comparable speeds.
        // Conclusion: I should use
        // std::move when creating a pypp::PyTup. It seems to be working
        // properly.
        std::vector<int> big_list1(100000, 1);
        std::vector<int> big_list2(100000, 1);
        pypp::PyList<int> py_big_list1(100000, 1);
        benchmark("Create std::tuple by moving a big std::vector", [&]() {
            std::tuple<std::vector<int>, std::vector<int>> small_tuple(
                std::move(big_list2), std::move(big_list2));
        });
        benchmark("Created py_tuple by moving a big pypp::PyList", [&]() {
            pypp::PyTup<pypp::PyList<int>, pypp::PyList<int>> small_tuple(
                std::move(py_big_list1), std::move(py_big_list1));
        });
        benchmark("Create py_tuple by moving a big std::vector", [&]() {
            pypp::PyTup<std::vector<int>, std::vector<int>> small_tuple(
                std::move(big_list1), std::move(big_list1));
        });

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
