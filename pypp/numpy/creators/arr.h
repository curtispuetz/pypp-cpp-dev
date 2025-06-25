#pragma once

#include "numpy/np_arr.h"
#include "py_list.h"
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Helper to check if a type is a vector
template <typename T> struct is_vector : std::false_type {};
template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename U>
void _throw_if_ragged_recursive(const std::vector<U> &data) {
    if (data.empty())
        return;
    if constexpr (is_vector<U>::value) {
        size_t first_len = data[0].size();
        for (const auto &sublist : data) {
            if (!is_vector<U>::value || sublist.size() != first_len) {
                throw std::invalid_argument(
                    "Inconsistent list lengths found when initializing numpy "
                    "array; array is ragged.");
            }
            _throw_if_ragged_recursive(sublist);
        }
    }
}

template <typename U>
void _deduce_shape_recursive(const std::vector<U> &data, PyList<int> &shape) {
    shape.append(static_cast<int>(data.size()));
    if constexpr (is_vector<U>::value) {
        _deduce_shape_recursive(data[0], shape);
    }
}

template <typename U> PyList<int> _deduce_shape(const std::vector<U> &data) {
    PyList<int> shape;
    _deduce_shape_recursive(data, shape);
    return shape;
}

template <typename T, typename U>
void _flatten_recursive(const std::vector<U> &data, std::vector<T> &flattened) {
    if constexpr (is_vector<U>::value) {
        for (const auto &sublist : data) {
            _flatten_recursive<T>(sublist, flattened);
        }
    } else {
        flattened.insert(flattened.end(), data.begin(), data.end());
    }
}

template <typename T, typename U>
std::vector<T> _flatten(const std::vector<U> &data) {
    std::vector<T> flattened;
    _flatten_recursive<T>(data, flattened);
    return flattened;
}

namespace np {
template <typename T, typename U> NpArr<T> array(const std::vector<U> data) {
    if (data.empty()) {
        throw std::invalid_argument("empty numpy array is not supported");
    }
    _throw_if_ragged_recursive(data);
    auto shape = _deduce_shape(data);
    auto flat_data = _flatten<T>(data);
    return NpArr<T>::create(std::move(shape), std::move(flat_data));
}
} // namespace np