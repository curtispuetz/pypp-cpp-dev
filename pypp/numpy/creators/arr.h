#pragma once

#include "numpy/np_arr.h"
#include "py_list.h"
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Primary template: false
template <typename T> struct is_pylist : std::false_type {};
// Specialization: true for PyList<T>
template <typename T> struct is_pylist<PyList<T>> : std::true_type {};

template <typename U> void _throw_if_ragged_recursive(const PyList<U> &data) {
    if (data.len() == 0)
        return;
    if constexpr (is_pylist<U>::value) {
        size_t first_len = data[0].len();
        for (const auto &sublist : data) {
            if (!is_pylist<U>::value || sublist.len() != first_len) {
                throw std::invalid_argument(
                    "Inconsistent list lengths found when initializing numpy "
                    "array; array is ragged.");
            }
            _throw_if_ragged_recursive(sublist);
        }
    }
}

template <typename U>
void _deduce_shape_recursive(const PyList<U> &data, PyList<int> &shape) {
    shape.append(static_cast<int>(data.len()));
    if constexpr (is_pylist<U>::value) {
        _deduce_shape_recursive(data[0], shape);
    }
}

template <typename U> PyList<int> _deduce_shape(const PyList<U> &data) {
    PyList<int> shape;
    _deduce_shape_recursive(data, shape);
    return shape;
}

template <typename T, typename U>
void _flatten_recursive(const PyList<U> &data, std::vector<T> &flattened) {
    if constexpr (is_pylist<U>::value) {
        for (const auto &sublist : data) {
            _flatten_recursive<T>(sublist, flattened);
        }
    } else {
        flattened.insert(flattened.end(), data.begin(), data.end());
    }
}

template <typename T, typename U>
std::vector<T> _flatten(const PyList<U> &data) {
    std::vector<T> flattened;
    _flatten_recursive<T>(data, flattened);
    return flattened;
}

namespace np {
template <typename T, typename U> NpArr<T> array(const PyList<U> &data) {
    if (data.len() == 0) {
        throw std::invalid_argument("empty numpy array is not supported");
    }
    _throw_if_ragged_recursive(data);
    auto shape = _deduce_shape(data);
    auto flat_data = _flatten<T>(data);
    return NpArr<T>::create(std::move(shape), std::move(flat_data));
}
} // namespace np