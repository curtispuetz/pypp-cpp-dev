#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "py_str.h"
#include "py_tuple.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>

int main() {
    try {
        // Below shows the printing of pypp::PyStr inside collectiosn where the
        // single quotes are correctly printed around the strings. list
        pypp::PyList<pypp::PyStr> str_list = {
            pypp::PyStr("one"), pypp::PyStr("two"), pypp::PyStr("three")};
        pypp::print("String List:", str_list);
        // set
        pypp::PySet<pypp::PyStr> str_set = {
            pypp::PyStr("apple"), pypp::PyStr("banana"), pypp::PyStr("cherry")};
        pypp::print("String Set:", str_set);
        // dict
        pypp::PyDict<pypp::PyStr, pypp::PyStr> str_dict = {
            {pypp::PyStr("onek"), pypp::PyStr("one")},
            {pypp::PyStr("twok"), pypp::PyStr("two")},
            {pypp::PyStr("threek"), pypp::PyStr("three")}};
        pypp::print("String Dict:", str_dict);
        // dict keys and values
        pypp::print("String Keys:", str_dict.keys());
        pypp::print("String Values:", str_dict.values());
        pypp::print("String Items:", str_dict.items());
        // tuple
        pypp::PyTup<int, pypp::PyStr> int_str_tup(1, pypp::PyStr("one"));
        pypp::print("Int-String Tuple:", int_str_tup);

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
