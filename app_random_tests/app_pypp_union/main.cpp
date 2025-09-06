#include "py_list.h"
#include "py_str.h"
#include "pypp_union.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

// function which takes Uni as an argument
void process_union(pypp::Uni<int, double> union_value) {
    if (union_value.isinst<int>()) {
        std::cout << "Processing int: " << union_value.ug<int>() << std::endl;
    }
}

// function which returns a Uni
pypp::Uni<int, double> create_union() { return pypp::Uni<int, double>(3.14); }

int main() {
    try {
        pypp::Uni<int, double> union_value(42);
        if (union_value.isinst<int>()) {
            std::cout << "The union contains an int: " << union_value.ug<int>()
                      << std::endl;
        }
        if (!union_value.isinst<double>()) {
            std::cout << "The union does not contain a double." << std::endl;
        }

        pypp::Uni<int, double, float, pypp::PyStr, pypp::PyList<int>>
            union_value2(pypp::PyList({1, 2, 3}));
        if (union_value2.isinst<pypp::PyList<int>>()) {
            std::cout << "The union contains a pypp::PyList<int>";
        }

        process_union(pypp::Uni<int, double>(100));

        pypp::Uni<int, double> union_value3(3.14);
        if (union_value3.isinst<double>()) {
            std::cout << "The union contains a double: "
                      << union_value3.ug<double>() << std::endl;
        }

        // moving
        pypp::PyList<int> list = {1, 2, 3};
        pypp::Uni<pypp::PyList<int>> union_value4(std::move(list));
        if (union_value4.isinst<pypp::PyList<int>>()) {
            std::cout
                << "The union contains a pypp::PyList<int> with elements: ";
            for (const auto &item : union_value4.ug<pypp::PyList<int>>()) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
        // copying not supported.

        // with monostate (i.e. None)
        pypp::Uni<std::monostate, int, double> union_value5(std::monostate{});
        if (union_value5.is_none()) {
            std::cout << "The union contains a monostate (i.e. None)."
                      << std::endl;
        }

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
