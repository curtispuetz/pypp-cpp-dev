#pragma once

#include "py_np_dy_array.h"
#include <initializer_list>
#include <vector>

namespace np {

template <typename T>
DynamicMultiArray<T> zeros(const std::vector<size_t> &shape) {
    DynamicMultiArray<T> arr(shape);
    arr.fill(T{0});
    return arr;
}

// Overload for initializer_list
template <typename T>
DynamicMultiArray<T> zeros(std::initializer_list<size_t> shape) {
    return zeros<T>(std::vector<size_t>(shape));
}
// template <typename T>
// DynamicMultiArray<T> zeros(std::initializer_list<int> shape) {
//     // TODO: this is easy to implement by casting each int.
//     return zeros<T>(std::vector<size_t>(shape));
// }

template <typename T>
DynamicMultiArray<T> ones(const std::vector<size_t> &shape) {
    DynamicMultiArray<T> arr(shape);
    arr.fill(T{1});
    return arr;
}

// Overload for initializer_list
template <typename T>
DynamicMultiArray<T> ones(std::initializer_list<size_t> shape) {
    return ones<T>(std::vector<size_t>(shape));
}

template <typename T>
DynamicMultiArray<T> full(const std::vector<size_t> &shape, const T &value) {
    DynamicMultiArray<T> arr(shape);
    arr.fill(value);
    return arr;
}

// Overload for initializer_list
template <typename T>
DynamicMultiArray<T> full(std::initializer_list<size_t> shape, const T &value) {
    return full<T>(std::vector<size_t>(shape), value);
}

} // namespace np