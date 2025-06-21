#include "py_enumerate.h"
#include "py_list.h"
#include "py_reversed.h"
#include "py_slice.h"
#include "py_zip.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <np_arr.h>
#include <optional>
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>
#include <variant>

int main() {
    try {
        NpArr<int> arr3D({2, 2, 3});
        int counter = 0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 3; ++k) {
                    arr3D.set(PyList({i, j, k}), counter++);
                }
            }
        }
        print(arr3D);
        print("orig shape:", arr3D.shape());
        print("orig size:", arr3D.size());

        NpArr<int>::NpArrView view2D = arr3D[PyList<std::variant<int, PySlice>>(
            {1, PySlice(std::nullopt), PySlice(std::nullopt)})];
        view2D.set(PyList<int>({0, 1}), 42);
        print(arr3D);
        print("view2D shape:", view2D.shape());
        print("view2D size:", view2D.size());

        NpArr<int>::NpArrView view1D =
            view2D[PyList<std::variant<int, PySlice>>(
                {0, PySlice(std::nullopt)})];
        view1D.set(PyList<int>({2}), 99);
        print(arr3D);
        print(view1D.at(PyList<int>({2})));
        print("view1D shape:", view1D.shape());
        print("view1D size:", view1D.size());

        NpArr<int>::NpArrView view1D2 =
            view2D[PyList<std::variant<int, PySlice>>(
                {PySlice(std::nullopt), 1})];
        view1D2.set(PyList<int>({0}), 100);
        print(arr3D);

    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
