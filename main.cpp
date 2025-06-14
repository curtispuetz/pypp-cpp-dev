#include "py_np_dy_array_creation.h"
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    // Use an initializer list directly
    auto arr_zeros = np::zeros<int>({2, 3, 4});

    std::cout << "Created a zero-initialized array with shape: ";
    for (size_t dim : arr_zeros.shape()) {
        std::cout << dim << " ";
    }
    std::cout << std::endl;
    return 0;
}