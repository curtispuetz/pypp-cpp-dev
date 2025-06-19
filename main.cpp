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
#include <format>
#include <iostream>

// TODO: after the above, add support for printing, hasing, and formatting range
// and slice.
int main() {
    try {
        std::cout << "Hello, World!" << std::endl;
        // Use an initializer list directly
        auto arr_zeros = pypp_np::zeros<int>({2, 3, 4});

        std::cout << "Created a zero-initialized array with shape: ";
        for (size_t dim : arr_zeros.shape()) {
            std::cout << dim << " ";
        }
        std::cout << std::endl;
        PyStr s = PyStr("This is a test string.");
        std::cout << "string size: " << s.len() << std::endl;
        PyList<PyStr> parts = s.split(PyStr(" "));
        print(parts);
        std::cout << "Parts lens: " << parts.len() << std::endl;
        PyTup<int, double, PyStr> tup = PyTup<int, double, PyStr>(42, 3.14, s);
        print(tup);
        std::cout << tup.count(42) << std::endl;
        std::cout << tup.index(3.14) << std::endl;
        std::cout << "Tuple size: " << tup.len() << std::endl;
        PySet<int> my_set = {1, 2, 3, 4, 5};
        print(my_set);
        std::cout << "Set size: " << my_set.len() << std::endl;
        PyDict<int, PyStr> my_dict = {
            {1, PyStr("one")},
            {2, PyStr("two")},
            {3, PyStr("three")},
        };
        print(my_dict);
        std::cout << "Dict size: " << my_dict.len() << std::endl;
        PyDict<PyStr, int> my_dict2 = {
            {PyStr("one"), 1},
            {PyStr("two"), 2},
            {PyStr("three"), 3},
        };
        print(my_dict2);
        PySlice sl(1, 5, 2);
        PyList<PyStr> sliced_parts = parts[sl];
        print(sliced_parts);
        print(parts[PySlice(0, std::nullopt, 2)]);
        NpArr<int> dyn_arr = pypp_np::ones<int>(PyList({2, 3, 4, 2}));
        std::cout << dyn_arr(0, 0, 0, 0) << std::endl;
        print(dyn_arr);
        print(dyn_arr.shape());
        std::cout << "Dynamic array size: " << dyn_arr.size() << std::endl;
        NpArr<int> dyn_arr2 = pypp_np::full<int>(PyList({2, 3, 4}), 7);
        print(dyn_arr2);

        PyList<PyList<PyList<PyList<float>>>> nested_list = {
            {{{1.1, 2}, {4, 5}}, {{10, 11}, {13, 14}}},
            {{{1, 2}, {1, 2}}, {{1, 2}, {1, 2}}}};

        NpArr<float> arr_from_nested = pypp_np::array<float>(nested_list);
        print(arr_from_nested);
        NpArr<int> arr_from_nested2 = pypp_np::array<int>(PyList({1, 2, 3}));
        print(arr_from_nested2);

        NpArr<double> d =
            pypp_np::array<double>(PyList({PyList({1, 2}), PyList({3, 4})}));

        // accessing elements
        std::cout << "Element at (0, 1): " << d(0, 1) << std::endl;
        // setting elements
        d(0, 1) = 5.5; // This one won't be used in pypp
        std::cout << "After setting element at (0, 1) to 5.5: " << d(0, 1)
                  << std::endl;
        d.set(PyTup(0, 1), 6.5); // This one will be used in pypp
        std::cout << "After setting element at (0, 1) to 6.5: " << d(0, 1)
                  << std::endl;

        PyDict<int, PyDict<int, int>> nested_dict(
            {{0, {{0, 1}}}, {1, {{0, 1}}}});
        print(nested_dict);
        // dict assignment
        PyDict<int, int> int_dict = PyDict<int, int>({{0, 1}, {1, 2}});
        print(int_dict);

        // for loops over lists
        PyList<int> my_py_list = {1, 2, 3};
        for (const auto &val : my_py_list) {
            std::cout << val;
        }
        // for loops over sets
        PySet<PyStr> my_py_set = {PyStr("a"), PyStr("b"), PyStr("c")};
        for (const auto &val : my_py_set) {
            std::cout << val;
        }
        // for loops over dict items
        for (const auto &pypp_it_tup : int_dict.items()) {
            auto &k = pypp_it_tup.get<0>();
            auto &v = pypp_it_tup.get<1>();
            std::cout << k << ": " << v << std::endl;
        }
        // for loops over list of tuples
        PyList<PyTup<int, int>> list_of_tuples =
            PyList({PyTup(1, 2), PyTup(3, 4)});
        for (const auto &pypp_it_tup : list_of_tuples) {
            auto &first = pypp_it_tup.get<0>();
            auto &second = pypp_it_tup.get<1>();
            std::cout << first << ", " << second << std::endl;
        }
        print(list_of_tuples);
        // basic while loops
        int i = 0;
        while (i < 3) {
            std::cout << i << " ";
            i += 1;
        }
        // to_pystr
        print(to_pystr(1));
        print(to_pystr(3.14));
        print(to_pystr("Hello"));

        // print
        print(PyStr("This is a test of the print function."));

        print(std::format("Formatted string: {}", 42));
        print(std::format("Formatted string with PyStr: {}, PyTup: {}, PySet: "
                          "{}, PyList: {}, PyDict: {}, NpArr: {}",
                          PyStr("Hello"), PyTup(1, 2), PySet({1, 2, 3}),
                          PyList({1, 2, 3}), PyDict<int, int>({{0, 1}}),
                          pypp_np::array<int>(PyList({1, 2, 3}))));

        // Testing that hashing works
        PyDict<PyTup<int, int>, PyStr> dict_of_tups = {
            {PyTup(1, 2), PyStr("one two")},
            {PyTup(3, 4), PyStr("three four")},
        };
        PyDict<PyList<int>, PyStr> dict_of_lists = {
            {PyList<int>({1, 2}), PyStr("one two")},
            {PyList<int>({3, 4}), PyStr("three four")},
        };
        PyDict<PyStr, PyStr> dict_of_strs = {
            {PyStr("one"), PyStr("1")},
            {PyStr("two"), PyStr("2")},
        };
        print("Are hashable: PyTup, PyList, PyStr");

        // PyRange
        for (const auto &i : PyRange(3)) {
            print(i); // 0, 1, 2
        }
        for (const auto &i : PyRange(1, 4)) {
            print(i); // 1, 2, 3
        }
        for (const auto &i : PyRange(10, 4, -2)) {
            print(i); // 10, 8, 6
        }
        for (const auto &i : PyRange(100, 98)) {
            print(i); // Should print nothing
        }
        for (const auto &i : PyRange(50, 100, -1)) {
            print(i); // Should print nothing
        }

        // slice printing
        print(PySlice(10));
        print(PySlice(5, 10));
        print(PySlice(5, 10, -5));
        print(PySlice(1, std::nullopt));
        // range printing
        print(PyRange(10));
        print(PyRange(5, 10));
        print(PyRange(5, 10, -1));
        print(PyRange(5, 10, 1));
        // formatting slice and range
        print(std::format("Formatted PySlice: {}", PySlice(1, 10, 2)));
        print(std::format("Formatted PyRange: {}", PyRange(1, 10, 2)));

        // Using slice and range in sets (for hashing)
        PySet<PySlice> slice_set = PySet({PySlice(1)});
        print(slice_set);
        PySet<PyRange> range_set = PySet({PyRange(1)});
        print(range_set);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}