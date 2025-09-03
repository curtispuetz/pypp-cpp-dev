#include "py_set.h"
#include "py_str.h"
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
        int_dict[7] = PyStr("d");
        print("After adding 7->d:", int_dict);
        int a = 3;
        int_dict[std::move(a)] = PyStr("new_value");
        int_dict.dg(3);
        int_dict.dg(a);
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
