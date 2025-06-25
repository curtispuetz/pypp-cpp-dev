#pragma once
#include "numpy/util.h"
#include "pypp_util/generator.h"
#include <functional>
#include <vector>

// TODO: move implementations to cpp file
inline Generator<std::vector<int>>
iter_shape_recursive(const PyList<int> &shape, std::vector<int> indices) {
    if (indices.size() == shape.len()) {
        co_yield indices;
        co_return;
    }
    for (int i = 0; i < shape[indices.size()]; ++i) {
        auto next = indices;
        next.push_back(i);
        CO_YIELD_FROM(iter_shape_recursive(shape, next));
    }
}

inline Generator<std::vector<int>> iter_shape(const PyList<int> &arr_shape) {
    CO_YIELD_FROM(iter_shape_recursive(arr_shape, {}));
}

template <typename T>
inline std::vector<T>
new_np_arr_data(std::function<T(const std::vector<int> &)> fn,
                const PyList<int> &shape) {
    std::vector<T> data;
    data.reserve(_calc_total_size(shape));
    for (auto &&indices : iter_shape(shape)) {
        data.push_back(fn(indices));
    }
    return data;
}