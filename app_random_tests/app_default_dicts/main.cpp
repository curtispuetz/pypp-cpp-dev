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
        PyDefaultDict<int, int> d([] { return 0; });
        d[1] += 5;
        print(d[1]);

        // double
        PyDefaultDict<int, double> d_double([] { return 0.0; });
        d_double[1] += 3.14;
        print(d_double[1]);

        // PyStr
        PyDefaultDict<int, PyStr> d_str([] { return PyStr(""); });
        d_str[1] += PyStr("one");
        print(d_str[1]);

        // PyList
        PyDefaultDict<int, PyList<int>> d_list([] { return PyList<int>(); });
        d_list[1].append(1);
        print(d_list[1]);

        // PySet
        PyDefaultDict<int, PySet<int>> d_set([] { return PySet<int>(); });
        d_set[1].add(1);
        print(d_set[1]);

        // PyDict
        PyDefaultDict<int, PyDict<int, int>> d_dict(
            [] { return PyDict<int, int>(); });
        d_dict[1][2] = 3;
        print(d_dict[1]);

        // default factory for int
        auto d_int_default = PyDefaultDict<int, int>::int_factory();
        int a = d_int_default[1];
        print(d_int_default);

        // default factory for double
        auto d_double_default = PyDefaultDict<int, double>::float_factory();
        double c = d_double_default[1];
        print(d_double_default);

        // default factory for PyStr
        auto d_str_default = PyDefaultDict<int, PyStr>::str_factory();
        PyStr s = d_str_default[1];
        print(d_str_default);

        // default factory for PySet
        auto d_set_default = PyDefaultDict<int, PySet<int>>::set_factory();
        PySet<int> a_set = d_set_default[1];
        print(d_set_default);

        // default factory for PyDict
        auto d_dict_default =
            PyDefaultDict<int, PyDict<int, int>>::dict_factory();
        PyDict<int, int> a_dict = d_dict_default[1];
        print(d_dict_default);

        // default factory for PyList
        auto d_list_default = PyDefaultDict<int, PyList<int>>::list_factory();
        PyList<int> b = d_list_default[1];
        print(d_list_default);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
