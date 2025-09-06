#include "py_dict_default.h"
#include "py_list.h"
#include "py_set.h"
#include "py_str.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // int
        auto d = pypp::PyDefaultDict<int, int>([] { return 0; });
        d[1] += 5;
        pypp::print(d[1]);

        // double
        auto d_double = pypp::PyDefaultDict<int, double>([] { return 0.0; });
        d_double[1] += 3.14;
        pypp::print(d_double[1]);

        // pypp::PyStr
        auto d_str = pypp::PyDefaultDict<int, pypp::PyStr>(
            [] { return pypp::PyStr(""); });
        d_str[1] += pypp::PyStr("one");
        pypp::print(d_str[1]);

        // pypp::PyList
        auto d_list = pypp::PyDefaultDict<int, pypp::PyList<int>>(
            [] { return pypp::PyList<int>(); });
        d_list[1].append(1);
        pypp::print(d_list[1]);

        // pypp::PySet
        auto d_set = pypp::PyDefaultDict<int, pypp::PySet<int>>(
            [] { return pypp::PySet<int>(); });
        d_set[1].add(1);
        pypp::print(d_set[1]);

        // pypp::PyDict
        auto d_dict = pypp::PyDefaultDict<int, pypp::PyDict<int, int>>(
            [] { return pypp::PyDict<int, int>(); });
        d_dict[1][2] = 3;
        pypp::print(d_dict[1]);

        // default factory for int
        auto d_int_default = pypp::PyDefaultDict<int, int>::int_factory();
        int a = d_int_default[1];
        pypp::print(d_int_default);

        // default factory for double
        auto d_double_default =
            pypp::PyDefaultDict<int, double>::float_factory();
        double c = d_double_default[1];
        pypp::print(d_double_default);

        // default factory for pypp::PyStr
        auto d_str_default =
            pypp::PyDefaultDict<int, pypp::PyStr>::str_factory();
        pypp::PyStr s = d_str_default[1];
        pypp::print(d_str_default);

        // default factory for pypp::PySet
        auto d_set_default =
            pypp::PyDefaultDict<int, pypp::PySet<int>>::set_factory();
        pypp::PySet<int> a_set = d_set_default[1];
        pypp::print(d_set_default);

        // default factory for pypp::PyDict
        auto d_dict_default =
            pypp::PyDefaultDict<int, pypp::PyDict<int, int>>::dict_factory();
        pypp::PyDict<int, int> a_dict = d_dict_default[1];
        pypp::print(d_dict_default);

        // default factory for pypp::PyList
        auto d_list_default =
            pypp::PyDefaultDict<int, pypp::PyList<int>>::list_factory();
        pypp::PyList<int> b = d_list_default[1];
        pypp::print(d_list_default);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
