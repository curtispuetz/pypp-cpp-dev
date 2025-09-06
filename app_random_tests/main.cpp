#include "py_list.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
int main() {
    try {
        // Just showing that std::vector and pypp::PyList behavior is the same
        // here.
        std::vector<std::vector<int>> vec_of_vecs(10, std::vector<int>(10, 1));
        vec_of_vecs[0][0] = 2; // just to use the variable
        pypp::print(vec_of_vecs[0][0]);
        pypp::print(vec_of_vecs[1][0]);
        pypp::PyList<pypp::PyList<int>> py_list_of_lists(
            10, pypp::PyList<int>(10, 1));
        py_list_of_lists[0][0] = 2; // just to use the variable
        pypp::print(py_list_of_lists[0][0]);
        pypp::print(py_list_of_lists[1][0]);

        // Just showing that using std::move with a primitive type like int
        // does not change anything and I can still use that value. Because
        // primitive types are moved by value no matter what.
        std::vector<int> vec_of_ints(10, 1);
        int insert_val = 5;
        vec_of_ints.push_back(std::move(insert_val));
        pypp::print(insert_val); // should be 5
        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
