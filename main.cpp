#include "exceptions/exception.h"
#include "lib_test/t.h"
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
pypp::PyList<int> &get_list(pypp::PyList<int> &list) { return list; }

int main() {
    try {
        std::cout << "Hello, World!" << std::endl;
        // Use an initializer list directly
        pypp::PyStr s = pypp::PyStr("This is a test string.");
        std::cout << "string size: " << s.len() << std::endl;
        pypp::PyList<pypp::PyStr> parts = s.split(pypp::PyStr(" "));
        pypp::print(parts);
        std::cout << "Parts lens: " << parts.len() << std::endl;
        pypp::PyTup<int, double, pypp::PyStr> tup =
            pypp::PyTup<int, double, pypp::PyStr>(42, 3.14, std::move(s));
        pypp::print(tup);
        std::cout << tup.count(42) << std::endl;
        std::cout << tup.index(3.14) << std::endl;
        std::cout << "Tuple size: " << tup.len() << std::endl;
        pypp::PySet<int> my_set = {1, 2, 3, 4, 5};
        pypp::print(my_set);
        std::cout << "Set size: " << my_set.len() << std::endl;
        pypp::PyDict<int, pypp::PyStr> my_dict = {
            {1, pypp::PyStr("one")},
            {2, pypp::PyStr("two")},
            {3, pypp::PyStr("three")},
        };
        pypp::print(my_dict);
        std::cout << "Dict size: " << my_dict.len() << std::endl;
        pypp::PyDict<pypp::PyStr, int> my_dict2 = {
            {pypp::PyStr("one"), 1},
            {pypp::PyStr("two"), 2},
            {pypp::PyStr("three"), 3},
        };
        pypp::print(my_dict2);
        pypp::PySlice sl(1, 5, 2);
        pypp::PyList<pypp::PyStr> sliced_parts = parts[sl];
        pypp::print(sliced_parts);
        pypp::print(parts[pypp::py_slice(0, std::nullopt, 2)]);

        pypp::PyDict<int, pypp::PyDict<int, int>> nested_dict(
            {{0, {{0, 1}}}, {1, {{0, 1}}}});
        pypp::print(nested_dict);
        // dict assignment
        pypp::PyDict<int, int> int_dict =
            pypp::PyDict<int, int>({{0, 1}, {1, 2}});
        pypp::print(int_dict);

        // for loops over lists
        pypp::PyList<int> my_py_list = {1, 2, 3};
        for (const auto &val : my_py_list) {
            std::cout << val;
        }
        // for loops over sets
        pypp::PySet<pypp::PyStr> my_py_set = {
            pypp::PyStr("a"), pypp::PyStr("b"), pypp::PyStr("c")};
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
        pypp::PyList<pypp::PyTup<int, int>> list_of_tuples =
            pypp::PyList({pypp::PyTup(1, 2), pypp::PyTup(3, 4)});
        for (const auto &pypp_it_tup : list_of_tuples) {
            auto &first = pypp_it_tup.get<0>();
            auto &second = pypp_it_tup.get<1>();
            std::cout << first << ", " << second << std::endl;
        }
        pypp::print(list_of_tuples);
        // basic while loops
        int i = 0;
        while (i < 3) {
            std::cout << i << " ";
            i += 1;
        }
        // pypp::to_pystr
        pypp::print(pypp::to_pystr(1));
        pypp::print(pypp::to_pystr(3.14));
        pypp::print(pypp::to_pystr("Hello"));

        // print
        pypp::print(pypp::PyStr("This is a test of the print function."));

        pypp::print(std::format("Formatted string: {}", 42));
        pypp::print(std::format(
            "Formatted string with pypp::PyStr: {}, pypp::PyTup: "
            "{}, pypp::PySet: "
            "{}, pypp::PyList: {}, pypp::PyDict: {}",
            pypp::PyStr("Hello"), pypp::PyTup(1, 2), pypp::PySet({1, 2, 3}),
            pypp::PyList({1, 2, 3}), pypp::PyDict<int, int>({{0, 1}})));

        // Testing that hashing works
        pypp::PyDict<pypp::PyTup<int, int>, pypp::PyStr> dict_of_tups = {
            {pypp::PyTup(1, 2), pypp::PyStr("one two")},
            {pypp::PyTup(3, 4), pypp::PyStr("three four")},
        };
        pypp::PyDict<pypp::PyList<int>, pypp::PyStr> dict_of_lists = {
            {pypp::PyList<int>({1, 2}), pypp::PyStr("one two")},
            {pypp::PyList<int>({3, 4}), pypp::PyStr("three four")},
        };
        pypp::PyDict<pypp::PyStr, pypp::PyStr> dict_of_strs = {
            {pypp::PyStr("one"), pypp::PyStr("1")},
            {pypp::PyStr("two"), pypp::PyStr("2")},
        };
        pypp::print("Are hashable: pypp::PyTup, pypp::PyList, pypp::PyStr");

        // pypp::PyRange
        for (const auto &i : pypp::PyRange(3)) {
            pypp::print(i); // 0, 1, 2
        }
        for (const auto &i : pypp::PyRange(1, 4)) {
            pypp::print(i); // 1, 2, 3
        }
        for (const auto &i : pypp::PyRange(10, 4, -2)) {
            pypp::print(i); // 10, 8, 6
        }
        for (const auto &i : pypp::PyRange(100, 98)) {
            pypp::print(i); // Should print nothing
        }
        for (const auto &i : pypp::PyRange(50, 100, -1)) {
            pypp::print(i); // Should print nothing
        }

        // slice printing
        pypp::print("slice printing");
        pypp::print(pypp::py_slice(5, 1, -1).indices(10));
        pypp::print(pypp::py_slice(10));
        pypp::print(pypp::py_slice(5, 10));
        pypp::print("Slice length 5: ",
                    pypp::py_slice(5, 10).calc_slice_length(100));
        pypp::print("Slice length 2: ",
                    pypp::py_slice(1000).calc_slice_length(2));
        pypp::print(pypp::py_slice(5, 10, -5));
        pypp::print(pypp::py_slice(1, std::nullopt));
        // range printing
        pypp::print(pypp::PyRange(10));
        pypp::print(pypp::PyRange(5, 10));
        pypp::print(pypp::PyRange(5, 10, -1));
        pypp::print(pypp::PyRange(5, 10, 1));
        // formatting slice and range
        pypp::print(std::format("Formatted pypp::PySlice: {}",
                                pypp::py_slice(1, 10, 2)));
        pypp::print(std::format("Formatted pypp::PyRange: {}",
                                pypp::PyRange(1, 10, 2)));

        // Using slice and range in sets (for hashing)
        pypp::PySet<pypp::PySlice> slice_set = pypp::PySet({pypp::py_slice(1)});
        pypp::print(slice_set);
        pypp::PySet<pypp::PyRange> range_set = pypp::PySet({pypp::PyRange(1)});
        pypp::print(range_set);

        // I need to think about all these references and moves for not just
        // pypp::PyTup, but also pypp::PyList, pypp::PySet, and pypp::PyDict,
        // etc.. In Python, if you have 2 list variables, and then you add them
        // to a second list of size 2, what happens? And what happens if you do
        // the same here in the C++?

        // It might be ok to just accept that there is a lot of copying for now
        // and then later I can optimize it. Even with the copying, I might
        // still get big performance improvements over Python for the vast
        // majority of cases

        pypp::PyList<int> numbers = {10, 20, 30, 40, 50};
        pypp::PyTup<pypp::PyStr, pypp::PyList<int>> py_tup(
            std::move(pypp::PyStr("Numbers")), std::move(numbers));
        py_tup.get<1>().append(60);
        pypp::print(numbers);

        // Tuple structured binding
        pypp::PyTup<int, int, pypp::PyStr, pypp::PyList<int>> tup2(
            1, 2, pypp::PyStr("Hello"), pypp::PyList<int>({1, 2, 3}));
        auto &[first, second, third, fourht] = tup2;    // references
        auto [first2, second2, third2, fourth2] = tup2; // copies

        for (const auto &[i, val] :
             pypp::PyEnumerate(pypp::PyList({10, 20, 30}))) {
            std::cout << "Index: " << i << ", Value: " << val << std::endl;
        }

        // Testing function that returns by reference
        pypp::PyList<int> numbers2 = {10, 20, 30, 40, 50};
        pypp::PyList<int> &list_returned_by_ref = get_list(numbers2);
        list_returned_by_ref.append(70);
        pypp::print(numbers2);
        PseudoCustomTypeCpp pct(42);
        std::cout << "pct.a_: " << pct.get_a() << std::endl;

        // String min and max
        pypp::PyStr abc = pypp::PyStr("abc");
        pypp::print(pypp::PyStr("abc min: "), abc.min());
        pypp::print(pypp::PyStr("abc max: "), abc.max());

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}