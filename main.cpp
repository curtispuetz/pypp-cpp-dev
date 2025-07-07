#include "exceptions/exception.h"
#include "py_dict.h"
#include "py_enumerate.h"
#include "py_list.h"
#include "py_range.h"
#include "py_set.h"
#include "py_str.h"
#include "py_tuple.h"
#include "pypp_util/main_error_handler.h"
#include "pypp_util/print.h"
#include "pypp_util/to_py_str.h"
#include "slice/creators.h"
#include "slice/py_slice.h"
#include <format>
#include <iostream>

// function that returns by reference an argument that is passed by reference
PyList<int> &get_list(PyList<int> &list) { return list; }

int main() {
    try {
        std::cout << "Hello, World!" << std::endl;
        // Use an initializer list directly
        PyStr s = PyStr("This is a test string.");
        std::cout << "string size: " << s.len() << std::endl;
        PyList<PyStr> parts = s.split(PyStr(" "));
        print(parts);
        std::cout << "Parts lens: " << parts.len() << std::endl;
        PyTup<int, double, PyStr> tup =
            PyTup<int, double, PyStr>(42, 3.14, std::move(s));
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
        print(parts[py_slice(0, std::nullopt, 2)]);

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
                          "{}, PyList: {}, PyDict: {}",
                          PyStr("Hello"), PyTup(1, 2), PySet({1, 2, 3}),
                          PyList({1, 2, 3}), PyDict<int, int>({{0, 1}})));

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
        print("slice printing");
        print(py_slice(5, 1, -1).indices(10));
        print(py_slice(10));
        print(py_slice(5, 10));
        print("Slice length 5: ", py_slice(5, 10).calc_slice_length(100));
        print("Slice length 2: ", py_slice(1000).calc_slice_length(2));
        print(py_slice(5, 10, -5));
        print(py_slice(1, std::nullopt));
        // range printing
        print(PyRange(10));
        print(PyRange(5, 10));
        print(PyRange(5, 10, -1));
        print(PyRange(5, 10, 1));
        // formatting slice and range
        print(std::format("Formatted PySlice: {}", py_slice(1, 10, 2)));
        print(std::format("Formatted PyRange: {}", PyRange(1, 10, 2)));

        // Using slice and range in sets (for hashing)
        PySet<PySlice> slice_set = PySet({py_slice(1)});
        print(slice_set);
        PySet<PyRange> range_set = PySet({PyRange(1)});
        print(range_set);

        // I need to think about all these references and moves for not just
        // PyTup, but also PyList, PySet, and PyDict, etc.. In Python, if you
        // have 2 list variables, and then you add them to a second list of size
        // 2, what happens? And what happens if you do the same here in the C++?

        // It might be ok to just accept that there is a lot of copying for now
        // and then later I can optimize it. Even with the copying, I might
        // still get big performance improvements over Python for the vast
        // majority of cases

        PyList<int> numbers = {10, 20, 30, 40, 50};
        PyTup<PyStr, PyList<int>> py_tup(std::move(PyStr("Numbers")),
                                         std::move(numbers));
        py_tup.get<1>().append(60);
        print(numbers);

        // Tuple structured binding
        PyTup<int, int, PyStr, PyList<int>> tup2(1, 2, PyStr("Hello"),
                                                 PyList<int>({1, 2, 3}));
        auto &[first, second, third, fourht] = tup2;    // references
        auto [first2, second2, third2, fourth2] = tup2; // copies

        for (const auto &[i, val] : PyEnumerate(PyList({10, 20, 30}))) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }

        // Testing function that returns by reference
        PyList<int> numbers2 = {10, 20, 30, 40, 50};
        PyList<int> &list_returned_by_ref = get_list(numbers2);
        list_returned_by_ref.append(70);
        print(numbers2);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}