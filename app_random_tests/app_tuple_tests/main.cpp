#include "py_str.h"
#include "py_tuple.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

PyTup<int, double, PyStr> get_tuple() {
    return PyTup<int, double, PyStr>(100, 2.718, PyStr("World"));
}

void unpack_tuple(int a, int b) {
    std::cout << "Unpacked values: a = " << a << ", b = " << b << std::endl;
}

int main() {
    try {
        // getting elements from a tuple
        PyTup<int, double, PyStr> tup = PyTup(42, 3.14, PyStr("Hello"));
        const auto &[a, b, c] = tup;
        std::cout << "Tuple elements:\n";
        std::cout << "a: " << a << "\n";
        std::cout << "b: " << b << "\n";
        std::cout << "c: " << c.str() << "\n";

        // getting elements from a tuple returned by a function
        const auto &[x, y, z] = get_tuple();
        std::cout << "Tuple from function:\n";
        std::cout << "x: " << x << "\n";
        std::cout << "y: " << y << "\n";
        std::cout << "z: " << z.str() << "\n";

        // unpacking a tuple in function call
        PyTup<int, int> py_tuple(10, 20);
        std::apply(unpack_tuple, py_tuple.raw());

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
