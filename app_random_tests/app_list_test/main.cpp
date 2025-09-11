#include "py_list.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Create a list of integers
        int a = 1;
        pypp::PyList<int> numbers = pypp::PyList({std::move(a), 2, 3, 4, 5});
        pypp::print(a); // still prints 1, because move doesn't change basic
                        // types like an int.

        // Create a list of lists
        pypp::PyList<int> inner_list1 = pypp::PyList({1, 2});
        pypp::PyList<pypp::PyList<int>> list_of_lists = {
            std::move(inner_list1), pypp::PyList<int>({3, 4}),
            pypp::PyList<int>({5, 6})};
        pypp::print(list_of_lists);
        pypp::print(inner_list1); // prints [] because it was moved

        // assignment with move

        pypp::PyList<pypp::PyList<int>> b =
            pypp::PyList({pypp::PyList({1, 2}), pypp::PyList({3, 4})});
        pypp::print(b);

        pypp::PyList<int> inner_list2 = pypp::PyList({5, 6});
        b[0] = inner_list2;            // copy
        b[1] = std::move(inner_list2); // move
        pypp::print(b);
        pypp::print(inner_list2);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
