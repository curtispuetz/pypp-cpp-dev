#pragma once

#include "py_np_array.h"
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace nps {

// --- Helper to create a MultiArray of a given value ---
template <typename T, size_t... Dims>
void fill_value(MultiArray<T, Dims...> &arr, const T &value) {
    for (auto &element : arr.data) {
        if constexpr (std::is_same_v<decltype(element), T &>) {
            element = value;
        } else {
            fill_value(element, value);
        }
    }
}

// --- Zeros ---
template <typename T, size_t... Dims> MultiArray<T, Dims...> zeros() {
    MultiArray<T, Dims...> arr;
    fill_value(arr, T{0});
    return arr;
}

// --- Ones ---
template <typename T, size_t... Dims> MultiArray<T, Dims...> ones() {
    MultiArray<T, Dims...> arr;
    fill_value(arr, T{1});
    return arr;
}

// --- Full ---
template <typename T, size_t... Dims>
MultiArray<T, Dims...> full(const T &value) {
    MultiArray<T, Dims...> arr;
    fill_value(arr, value);
    return arr;
}

} // namespace nps