#include "np_arr.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "py_slice.h"
#include "py_str.h"
#include "py_tuple.h"
#include <iostream>

int main() {
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
    parts.print();
    std::cout << "Parts lens: " << parts.len() << std::endl;
    PyTup<int, double, PyStr> tup = PyTup<int, double, PyStr>(42, 3.14, s);
    tup.print();
    std::cout << tup.count(42) << std::endl;
    std::cout << tup.index(3.14) << std::endl;
    std::cout << "Tuple size: " << tup.len() << std::endl;
    PySet<int> my_set = {1, 2, 3, 4, 5};
    my_set.print();
    std::cout << "Set size: " << my_set.len() << std::endl;
    PyDict<int, PyStr> my_dict = {
        {1, PyStr("one")},
        {2, PyStr("two")},
        {3, PyStr("three")},
    };
    my_dict.print();
    std::cout << "Dict size: " << my_dict.len() << std::endl;
    PyDict<PyStr, int> my_dict2 = {
        {PyStr("one"), 1},
        {PyStr("two"), 2},
        {PyStr("three"), 3},
    };
    my_dict2.print();
    PySlice sl(1, 5, 2);
    PyList<PyStr> sliced_parts = parts[sl];
    sliced_parts.print();
    parts[PySlice(0, std::nullopt, 2)].print();
    NpArr<int> dyn_arr = pypp_np::ones<int>(PyList<size_t>({2, 3, 4, 2}));
    std::cout << dyn_arr(0, 0, 0, 0) << std::endl;
    dyn_arr.print();
    dyn_arr.shape().print();
    std::cout << "Dynamic array size: " << dyn_arr.size() << std::endl;
    NpArr<int> dyn_arr2 = pypp_np::full<int>({2, 3, 4}, 7);
    dyn_arr2.print();

    PyList<PyList<PyList<PyList<float>>>> nested_list = {
        {{{1.1, 2}, {4, 5}}, {{10, 11}, {13, 14}}},
        {{{1, 2}, {1, 2}}, {{1, 2}, {1, 2}}}};

    NpArr<float> arr_from_nested = pypp_np::array<float>(nested_list);
    arr_from_nested.print();
    NpArr<int> arr_from_nested2 = pypp_np::array<int>(PyList({1, 2, 3}));
    arr_from_nested2.print();

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
    return 0;
}