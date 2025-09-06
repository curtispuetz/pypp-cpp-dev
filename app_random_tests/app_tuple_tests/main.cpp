#include "py_str.h"
#include "py_tuple.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

pypp::PyTup<int, double, pypp::PyStr> get_tuple() {
    return pypp::PyTup<int, double, pypp::PyStr>(100, 2.718,
                                                 pypp::PyStr("World"));
}

void unpack_tuple(int a, int b) {
    std::cout << "Unpacked values: a = " << a << ", b = " << b << std::endl;
}

// Creating a new tuple from a tuple. So, in Py++ you would have to do:
// def get_new_tup(tup: valu(tuple[int, float]) -> tuple[int, float]:
//    a: int = pypp_tg(tup, 0)
//    return (a, pypp_tg(tup, 1) + 1.0)
pypp::PyTup<int, double> get_new_tup(pypp::PyTup<int, double> tup) {
    int a = tup.get<0>();
    return pypp::PyTup<int, double>(std::move(a), tup.get<1>() + 1.0);
}

int main() {
    try {
        // getting elements from a tuple
        pypp::PyTup<int, double, pypp::PyStr> tup =
            pypp::PyTup(42, 3.14, pypp::PyStr("Hello"));
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

        pypp::PyTup<int, double> new_tup =
            pypp::PyTup(std::move(1), std::move(2.0));
        std::cout << "New tuple after move:\n";
        std::cout << "First: " << new_tup.get<0>() << "\n";
        std::cout << "Second: " << new_tup.get<1>() << "\n";

        // unpacking a tuple in function call
        pypp::PyTup<int, int> py_tuple(10, 20);
        std::apply(unpack_tuple, py_tuple.raw());

        // Moving tuple elements
        pypp::PyTup<int, int> move_tuple =
            pypp::PyTup(std::move(30), std::move(40));
        std::cout << "Moved tuple elements:\n";
        std::cout << "First: " << move_tuple.get<0>() << "\n";
        std::cout << "Second: " << move_tuple.get<1>() << "\n";

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
