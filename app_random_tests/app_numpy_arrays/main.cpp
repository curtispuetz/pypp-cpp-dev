#include "numpy/creators/arr.h"
#include "numpy/creators/fulls.h"
#include "numpy/np_arr.h"
#include "numpy/np_arr_sanity.h"
#include "py_enumerate.h"
#include "py_list.h"
#include "py_reversed.h"
#include "py_zip.h"
#include "slice/creators.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <optional>
#include <pypp_util/main_error_handler.h>
#include <pypp_util/print.h>
#include <variant>

int main() {
    try {
        print("1st");
        NpArr<int> arr3D = np::zeros(PyList({2, 2, 3}));
        print("2nd");
        int counter = 0;
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 3; ++k) {
                    arr3D[{i, j, k}] = counter++;
                }
            }
        }
        print("3rd");
        print(arr3D.to_string());
        print("4th");
        print("orig shape:", arr3D.shape());
        print("orig size:", arr3D.size());

        arr3D.fill(100);
        print("fill: ");
        print(arr3D.to_string());

        arr3D *= 2;
        print("inplace mult:");
        print(arr3D.to_string());

        arr3D /= 2;
        print("inplace divide: ");
        print(arr3D.to_string());

        arr3D += 5;
        print("inplace add:");
        print(arr3D.to_string());

        arr3D -= 5;
        print("inplace sub:");
        print(arr3D.to_string());

        print("add: ");
        print((arr3D + 5).to_string());

        print("sub: ");
        print((arr3D - 5).to_string());

        print("mult: ");
        print((arr3D * 2).to_string());

        print("div: ");
        print((arr3D / 2).to_string());

        print("right add: ");
        print((5 + arr3D).to_string());

        print("right sub: ");
        print((5 - arr3D).to_string());

        print("right mult: ");
        print((2 * arr3D).to_string());

        print("right div: ");
        print((400 / arr3D).to_string());

        // Test view now
        print("creating view 2D slice of 3D array");
        NpArr<int> view2D =
            arr3D.view(ViewSAndI({0, py_slice_empty(), py_slice_empty()}));

        print("view2D:");
        print(view2D.to_string());
        view2D *= 5;
        print("view2D after *= 5:");
        print(view2D.to_string());
        print("arr3D after view2D *= 5:");
        print(arr3D.to_string());

        print("view2D shape:", view2D.shape());
        print("view2D size:", view2D.size());

        NpArr<int> view1D = view2D.view(ViewSAndI({py_slice_empty(), 1}));
        view1D += 10;
        print("view1D:");
        print(view1D.to_string());
        print("arr3D after view1D += 10:");
        print(arr3D.to_string());

        // test np::array creation function
        print("creating 1D array with np::array: ");
        std::vector<int> data = {1, 2, 3, 4, 5};
        NpArr<int> arr1D = np::array(std::move(data));
        print(arr1D.to_string());
        std::vector data2d = {std::vector<int>{1, 2, 3},
                              std::vector<int>{4, 5, 6},
                              std::vector<int>{7, 8, 9}};
        print("creating 2D array with np::array: ");
        NpArr<int> arr2D = np::array(std::move(data2d));
        print(arr2D.to_string());
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
