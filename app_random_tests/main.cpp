#include "compy_util/print.h"
#include "py_list.h"
#include <compy_util/main_error_handler.h>
#include <cstdlib> // Required for EXIT_FAILURE
int main() {
    try {
        // Just showing that std::vector and PyList behavior is the same here.
        std::vector<std::vector<int>> vec_of_vecs(10, std::vector<int>(10, 1));
        vec_of_vecs[0][0] = 2; // just to use the variable
        print(vec_of_vecs[0][0]);
        print(vec_of_vecs[1][0]);
        PyList<PyList<int>> py_list_of_lists(10, PyList<int>(10, 1));
        py_list_of_lists[0][0] = 2; // just to use the variable
        print(py_list_of_lists[0][0]);
        print(py_list_of_lists[1][0]);

        // Just showing that using std::move with a primitive type like int
        // does not change anything and I can still use that value. Because
        // primitive types are moved by value no matter what.
        std::vector<int> vec_of_ints(10, 1);
        int insert_val = 5;
        vec_of_ints.push_back(std::move(insert_val));
        print(insert_val); // should be 5
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
