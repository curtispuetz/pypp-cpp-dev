#include "benchmark.h"
#include "numpy/creators/fulls.h"
#include "pypp_util/print.h"
#include "slice/creators.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        // Test creating zeros with different dimensions
        // Results: 200 times slower than Python numpy.zeros(). The speed does
        // not depend on the number of dimensions of the array.
        // Conclusion: It is still very fast, so maybe its not a big deal
        //  but a little concerning. So, I should learn what numpy is doing
        //  differently than what I am.
        // Hint: The np::ones function is the same speed as np.ones in Python.
        // So, there must be something special that Python is doing for
        // numpy.zeros()
        // ChatGPT says: that for zeros numpy can use special low level
        // operations to make it faster, but it can not do it for ones. For ones
        // it has to loop over and set the values just like I do here, and that
        // is why they are the same speed.
        // Final Conclusion: I should not worry about this. Optimizing the
        // initialization to zeros case could be done later if needed. That
        // optimzation might be nice for other types, such as PyList as well.
        const int size = 1000000;
        const int size2d = 1000;
        const int size3d = 100;
        const int size4d = 10;
        benchmark("creating zeros",
                  []() { auto z = np::zeros<double>({size}); });
        benchmark("creating 2D zeros",
                  []() { auto z = np::zeros<double>({size2d, size2d}); });
        benchmark("creating 3D zeros", []() {
            auto z = np::zeros<double>({size3d, size3d, size3d});
        });
        benchmark("creating 4D zeros", []() {
            auto z = np::zeros<double>({size4d, size4d, size3d, size3d});
        });
        benchmark("creating std::vector zeros",
                  []() { std::vector<double> z(size, 0.0); });

        // Test creating ones with different dimensions
        // Results: the same speed as Python numpy.ones()
        // Conclusion: This is fine to use.
        benchmark("creating ones", []() { auto o = np::ones<double>({size}); });

        benchmark("creating fulls",
                  []() { auto f = np::full<double>({size}, 5.0); });

        // Test accessing elements in a 1D numpy array.
        // Results: about 30-50 times faster than Python numpy, and even more
        // for the additional iterations.
        // Conclusion: This is great. But the difference in speed could just be
        // due to the for loop overhead in Python.
        const int accessing_size = 100000;
        benchmark("accessing", []() {
            auto arr = np::ones<double>({accessing_size});
            for (int i = 0; i < accessing_size; ++i) {
                auto z = arr[i];
            }
        });

        // Test accessing elements in a 2D numpy array.
        // Results: Identical speed to Python numpy.
        // Conclusion: The reason why this one is the same speed as numpy and
        // the above one was much faster than numpy is I guess because the list
        // created each time here. This is fine to use.
        const int accessing_size2d_1 = 1000;
        const int accessing_size2d_2 = 100;
        benchmark("accessing 2D", []() {
            auto arr =
                np::ones<double>({accessing_size2d_1, accessing_size2d_2});
            for (int i = 0; i < accessing_size2d_1; ++i) {
                for (int j = 0; j < accessing_size2d_2; ++j) {
                    auto z = arr[PyList({i, j})];
                }
            }
        });
        // Test slicing a 1D array
        // Results: The same speed as Python numpy.
        // Conclusion: Good to use.
        const int slicing_size = 500000;
        benchmark("slicing 1D", []() {
            auto arr = np::ones<double>({size});
            auto z = arr.view(ViewSAndI({py_slice_stop(slicing_size)}));
        });

        // Test slicing a 2D array
        // Results: about 1.2-1.3 time slower than Python numpy.
        // Conclusion: Its good enough. I could look at optimizing later.
        const int slicing_size2d = 500;
        benchmark("slicing 2D", []() {
            auto arr = np::ones<double>({size2d, size2d});
            auto z = arr.view(ViewSAndI({py_slice_stop(slicing_size2d),
                                         py_slice_stop(slicing_size2d)}));
        });

        // Test slicing a 3D array only in the middle element (I do this in my
        // game engine)
        // Results: The same speed as Python numpy.
        // Conclusion: Good to use.
        benchmark("slicing middle 3D", []() {
            auto arr = np::ones<double>({size3d, size3d, size3d});
            auto z = arr.view(ViewSAndI(
                {py_slice_empty(), py_slice_stop(50), py_slice_empty()}));
        });
        // Test filling
        // Results: maybe 1-1.2 times slower than Python numpy.
        // Conclusion: This is fine to use. Could check why it is maybe slower
        // and optimize later. I believe it is slower because of those .g()
        // calls. It could also be because of the generator that I use
        // i.e. 'iter_shape'. NOTE: adding new object members so that I don't
        // have to use .g() slows down initialization of the NpArr by 2 times.
        // So that doesn't seem like a good solution.
        benchmark("filling 1D", []() {
            auto arr = np::ones<double>({size});
            arr.fill(3.14);
        });

        // Test inplace operations
        // Results: Same as above. Maybe 1-1.2 times slower than Python numpy.
        // Conclusion: This is fine to use. I could optimize maybe the .g()
        // later.
        benchmark("in-place multiplication 1D", []() {
            auto arr = np::ones<double>({size});
            arr *= 2.5;
        });
        benchmark("in-place division 1D", []() {
            auto arr = np::ones<double>({size});
            arr /= 2.5;
        });
        benchmark("in-place addition 1D", []() {
            auto arr = np::ones<double>({size});
            arr += 2.5;
        });
        benchmark("in-place subtraction 1D", []() {
            auto arr = np::ones<double>({size});
            arr -= 2.5;
        });

        // Test filling 2D
        // Results: Looks about the same speed as Python numpy.
        // Conclusion: This is fine to use.
        benchmark("filling 2D", []() {
            auto arr = np::ones<double>({size2d, size2d});
            arr.fill(3.14);
        });
        // Note: I don't need to test the in-place operations for 2D because it
        // should be the same as fill.

        // Test operations by a scalar
        // Results: about 1-1.2 times slower than Python numpy. Due to the same
        // reasons mentioned above for the other ones that could be 1-1.2 times
        // slower.
        // Conclusion: Fine to use.
        benchmark("multiplication", []() {
            auto arr = np::ones<double>({size});
            auto res = arr * 2.5;
        });
        benchmark("division", []() {
            auto arr = np::ones<double>({size});
            auto res = arr / 2.5;
        });
        benchmark("addition", []() {
            auto arr = np::ones<double>({size});
            auto res = arr + 2.5;
        });
        benchmark("subtraction", []() {
            auto arr = np::ones<double>({size});
            auto res = arr - 2.5;
        });
        benchmark("right multiplcation", []() {
            auto arr = np::ones<double>({size});
            auto res = 2.5 * arr;
        });
        benchmark("right division", []() {
            auto arr = np::ones<double>({size});
            auto res = 2.5 / arr;
        });
        benchmark("right addition", []() {
            auto arr = np::ones<double>({size});
            auto res = 2.5 + arr;
        });
        benchmark("right subtraction", []() {
            auto arr = np::ones<double>({size});
            auto res = 2.5 - arr;
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
