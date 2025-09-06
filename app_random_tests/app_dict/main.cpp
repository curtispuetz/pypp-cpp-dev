#include "py_set.h"
#include "py_str.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <py_dict.h>
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Iterate over a pypp::PyDicts keys, values, and items
        pypp::PyDict<int, pypp::PyStr> int_dict = {{1, pypp::PyStr("a")},
                                                   {3, pypp::PyStr("b")},
                                                   {5, pypp::PyStr("c")}};
        for (const auto &key : int_dict.keys()) {
            pypp::print("key:", key);
        }
        for (const auto &value : int_dict.values()) {
            pypp::print("value:", value);
        }
        for (const auto &item : int_dict.items()) {
            pypp::print("item:", item.get<0>(), "->", item.get<1>());
        }
        int_dict[7] = pypp::PyStr("d");
        pypp::print("After adding 7->d:", int_dict);
        int a = 3;
        int_dict[std::move(a)] = pypp::PyStr("new_value");
        int_dict.dg(3);
        int_dict.dg(a);
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
