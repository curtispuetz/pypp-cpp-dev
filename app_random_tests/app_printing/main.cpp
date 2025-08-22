#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "py_str.h"
#include "py_tuple.h"
#include <compy_util/main_error_handler.h>
#include <compy_util/print.h>
#include <cstdlib> // Required for EXIT_FAILURE

int main() {
    try {
        // Below shows the printing of PyStr inside collectiosn where the single
        // quotes are correctly printed around the strings.
        // list
        PyList<PyStr> str_list = {PyStr("one"), PyStr("two"), PyStr("three")};
        print("String List:", str_list);
        // set
        PySet<PyStr> str_set = {PyStr("apple"), PyStr("banana"),
                                PyStr("cherry")};
        print("String Set:", str_set);
        // dict
        PyDict<PyStr, PyStr> str_dict = {{PyStr("onek"), PyStr("one")},
                                         {PyStr("twok"), PyStr("two")},
                                         {PyStr("threek"), PyStr("three")}};
        print("String Dict:", str_dict);
        // dict keys and values
        print("String Keys:", str_dict.keys());
        print("String Values:", str_dict.values());
        print("String Items:", str_dict.items());
        // tuple
        PyTup<int, PyStr> int_str_tup(1, PyStr("one"));
        print("Int-String Tuple:", int_str_tup);

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
