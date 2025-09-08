#include "benchmark.h"
#include "py_dict.h"
#include "py_str.h"
#include "pypp_util/create/dict.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>
#include <unordered_map>

int main() {
    try {
        // Benchmark speed for a standard for loop over std::unordered_map vs.
        // pypp::PyDict keys, values, and items methods.
        // Results: for keys(), values(), and items() all speeds are virtually
        // identical. As usual, the very first call seems to be a bit slower, no
        // matter which benchmark is placed first. Conclusion: it is fine to use
        // keys(), values(), and items() to iterate.
        const int size = 100000;
        std::unordered_map<int, int> bigMap;
        for (int i = 0; i < size; ++i) {
            bigMap[i] = i * 2;
        }
        pypp::PyDict<int, int> big_py_dict;
        for (int i = 0; i < size; ++i) {
            big_py_dict[std::move(i)] = i * 2;
        }
        benchmark("Standard for loop over std::unordered_map keys", [&]() {
            for (const auto &[key, _] : bigMap) {
                auto value = key * 2;
            }
        });
        benchmark("pypp::PyDict keys method", [&]() {
            for (const auto &key : big_py_dict.keys()) {
                auto value = key * 2;
            }
        });
        benchmark("Standard for loop over std::unordered_map values", [&]() {
            for (const auto &[_, value] : bigMap) {
                auto result = value + 1;
            }
        });
        benchmark("pypp::PyDict values method", [&]() {
            for (const auto &value : big_py_dict.values()) {
                auto result = value + 1;
            }
        });
        benchmark("Standard for loop over std::unordered_map items", [&]() {
            for (const auto &[key, value] : bigMap) {
                auto result = key + value;
            }
        });
        benchmark("pypp::PyDict items method", [&]() {
            for (const auto &item : big_py_dict.items()) {
                auto result = item.get<0>() + item.get<1>();
            }
        });

        // Constructing
        pypp::PyDict<int, int> py_dict_a = {{1, 2}, {3, 4}, {5, 6}};
        pypp::PyDict<int, int> py_dict_b = pypp::dict(py_dict_a);
        py_dict_a[7] = 8;
        pypp::print(py_dict_a);
        pypp::print(py_dict_b);

        pypp::PyList<pypp::PyTup<int, int>> lst_of_tuples(
            {pypp::PyTup(1, 2), pypp::PyTup(3, 4)});
        pypp::PyDict<int, int> py_dict_c = pypp::dict(lst_of_tuples);
        pypp::print(py_dict_c);

        // inline list of tuples
        pypp::PyDict<int, int> py_dict_d =
            pypp::dict(pypp::PyList<pypp::PyTup<int, int>>(
                {pypp::PyTup(1, 2), pypp::PyTup(3, 4)}));
        pypp::print(py_dict_d);

        // empty
        pypp::PyDict<int, pypp::PyStr> py_dict_e =
            pypp::dict<int, pypp::PyStr>();
        pypp::print(py_dict_e);

        // copy
        pypp::PyDict<int, int> py_dict_f = py_dict_a.copy();
        py_dict_a[9] = 9;
        pypp::print(py_dict_f);

        // get with default value
        int value = py_dict_a.get(100, -1);
        pypp::print(value); // should print -1
        pypp::PyStr str_value = py_dict_e.get(200, pypp::PyStr("default"));
        pypp::print(str_value); // should print "default"

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
