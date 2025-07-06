#include "py_list.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Create a list of integers
        int a = 1;
        PyList<int> numbers = PyList({std::move(a), 2, 3, 4, 5});
        print(a); // still prints 1, because move doesn't change basic types
                  // like an int.

        // Create a list of lists
        PyList<int> inner_list1 = PyList({1, 2});
        PyList<PyList<int>> list_of_lists = {
            std::move(inner_list1), PyList<int>({3, 4}), PyList<int>({5, 6})};
        print(list_of_lists);
        print(inner_list1); // prints [] because it was moved

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
