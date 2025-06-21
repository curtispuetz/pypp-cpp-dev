#include "py_enumerate.h"
#include "py_list.h"
#include "py_reversed.h"
#include "py_zip.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <np_arr.h>
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>

int main() {
    try {
        NpArr<int> arr({3, 5, 6});
        int counter = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                for (int k = 0; k < 6; ++k) {
                    arr[PyList({i, j, k})] = counter++;
                }
            }
        }
        print(arr);

    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
