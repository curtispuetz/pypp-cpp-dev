#include "py_dict.h"
#include <iostream>
#include <optional>
#include <pypp_util/main_error_handler.h>
#include <vector>

int main() {
    try {
        int x = 42;
        std::optional<int> opt1(x);
        std::optional<int> opt2; // Empty optional
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