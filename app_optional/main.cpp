#include "py_dict.h"
#include "pypp_optional.h"
#include <iostream>
#include <optional>
#include <pypp_util/main_error_handler.h>
#include <vector>

int main() {
    try {
        PyDict<int, PyList<int>> int_dict = {{0, PyList<int>({1, 2})},
                                             {1, PyList<int>({3, 4})}};
        PyppOpt<PyList<int>> d1 = int_dict.dg_opt(0);
        if (d1.has_value()) {
            d1.value().append(5);
        }
        std::cout << "Dict after dg_opt modification: " << int_dict
                  << std::endl;

        int x = 42;
        PyppOpt<int> opt1(x);
        PyppOpt<int> opt2; // Empty optional
        if (opt1) {
            std::cout << "opt1 has value: " << opt1.value() << std::endl;
        } else {
            std::cout << "opt1 is empty" << std::endl;
        }
        if (opt2) {
            std::cout << "opt2 has value: " << opt2.value() << std::endl;
        } else {
            std::cout << "opt2 is empty" << std::endl;
        }
        if (opt1.has_value()) {
            std::cout << "opt1 value: " << opt1.value() << std::endl;
        } else {
            std::cout << "opt1 has no value" << std::endl;
        }
        if (opt2.has_value()) {
            std::cout << "opt2 value: " << opt2.value() << std::endl;
        } else {
            std::cout << "opt2 has no value" << std::endl;
        }
        std::cout << opt1.value_or(0) << std::endl; // Should print 42
        std::cout << opt2.value_or(0) << std::endl; // Should print 0
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}