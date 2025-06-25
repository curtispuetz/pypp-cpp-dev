#pragma once
#include "numpy/np_arr.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

// Helper to calculate total size from shape
inline std::size_t _calc_total_size(const PyList<int> &shape) {
    std::size_t ret = 1;
    for (int s : shape) {
        ret *= static_cast<std::size_t>(s);
    }
    return ret;
}

namespace np {
template <typename T> NpArr<T> full(PyList<int> shape, const T &val) {
    std::size_t total_size = _calc_total_size(shape);
    std::vector<T> data(total_size, val);
    return NpArr<T>::create(std::move(shape), std::move(data));
}

template <typename T = int> NpArr<T> zeros(PyList<int> shape) {
    return full<T>(std::move(shape), T{0});
}

template <typename T = int> NpArr<T> ones(PyList<int> shape) {
    return full<T>(std::move(shape), T{1});
}
} // namespace np