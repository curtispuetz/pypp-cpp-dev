#include "py_set.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <py_dict.h>
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Iterate over a PyDicts keys, values, and items
        PyDict<int, PyStr> int_dict = {
            {1, PyStr("a")}, {3, PyStr("b")}, {5, PyStr("c")}};
        for (const auto &key : int_dict.keys()) {
            print("key:", key);
        }
        for (const auto &value : int_dict.values()) {
            print("value:", value);
        }
        for (const auto &item : int_dict.items()) {
            print("item:", item.get<0>(), "->", item.get<1>());
        }
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
