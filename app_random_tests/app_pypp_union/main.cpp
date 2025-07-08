#include "py_list.h"
#include "py_str.h"
#include "pypp_union.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

// function which takes Uni as an argument
void process_union(Uni<int, double> union_value) {
    if (union_value.isinst<int>()) {
        std::cout << "Processing int: " << union_value.ug<int>() << std::endl;
    }
}

// function which returns a Uni
Uni<int, double> create_union() { return Uni<int, double>(3.14); }

int main() {
    try {
        Uni<int, double> union_value(42);
        if (union_value.isinst<int>()) {
            std::cout << "The union contains an int: " << union_value.ug<int>()
                      << std::endl;
        }
        if (!union_value.isinst<double>()) {
            std::cout << "The union does not contain a double." << std::endl;
        }

        Uni<int, double, float, PyStr, PyList<int>> union_value2(
            PyList({1, 2, 3}));
        if (union_value2.isinst<PyList<int>>()) {
            std::cout << "The union contains a PyList<int>";
        }

        process_union(Uni<int, double>(100));

        Uni<int, double> union_value3(3.14);
        if (union_value3.isinst<double>()) {
            std::cout << "The union contains a double: "
                      << union_value3.ug<double>() << std::endl;
        }

        // moving
        PyList<int> list = {1, 2, 3};
        Uni<PyList<int>> union_value4(std::move(list));
        if (union_value4.isinst<PyList<int>>()) {
            std::cout << "The union contains a PyList<int> with elements: ";
            for (const auto &item : union_value4.ug<PyList<int>>()) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
        // copying not supported.

        // with monostate (i.e. None)
        Uni<std::monostate, int, double> union_value5(std::monostate{});
        if (union_value5.is_none()) {
            std::cout << "The union contains a monostate (i.e. None)."
                      << std::endl;
        }

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
