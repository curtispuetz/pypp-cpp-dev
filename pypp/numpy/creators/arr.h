#pragma once
#include "numpy/np_arr.h"
#include <stdexcept>
#include <type_traits>
#include <variant>
#include <vector>

// Throw if ragged array (inconsistent subvector sizes)
template <typename T>
void throw_if_ragged_recursive(const std::vector<T> &data) {
    // Base case: not a vector of vectors, nothing to check
}

template <typename T>
void throw_if_ragged_recursive(const std::vector<std::vector<T>> &data) {
    if (data.empty())
        return;
    size_t first_len = data[0].size();
    for (const auto &sublist : data) {
        if (sublist.size() != first_len) {
            throw std::invalid_argument(
                "Inconsistent list lengths found when initializing numpy "
                "array; array is ragged.");
        }
        throw_if_ragged_recursive(sublist);
    }
}

// Deduce shape
template <typename T>
void deduce_shape_recursive(const std::vector<T> &data,
                            std::vector<size_t> &shape) {
    shape.push_back(data.size());
}

template <typename T>
void deduce_shape_recursive(const std::vector<std::vector<T>> &data,
                            std::vector<size_t> &shape) {
    shape.push_back(data.size());
    if (!data.empty()) {
        deduce_shape_recursive(data[0], shape);
    }
}

template <typename T>
std::vector<size_t> deduce_shape(const std::vector<T> &data) {
    std::vector<size_t> shape;
    deduce_shape_recursive(data, shape);
    return shape;
}

// Flatten
template <typename T>
void flatten_recursive(const std::vector<T> &data, std::vector<T> &flattened) {
    flattened.insert(flattened.end(), data.begin(), data.end());
}

template <typename T>
void flatten_recursive(const std::vector<std::vector<T>> &data,
                       std::vector<T> &flattened) {
    for (const auto &sublist : data) {
        flatten_recursive(sublist, flattened);
    }
}

template <typename T> std::vector<T> flatten(const std::vector<T> &data) {
    std::vector<T> flattened;
    flatten_recursive(data, flattened);
    return flattened;
}

// Main np_array function
template <typename T> NpArr<T> np_array(const std::vector<T> &data) {
    if (data.empty()) {
        throw std::invalid_argument("empty numpy array is not supported");
    }
    throw_if_ragged_recursive(data);
    auto shape = deduce_shape(data);
    auto flat_data = flatten(data);
    return NpArr<T>(shape, flat_data);
}

template <typename T>
NpArr<T> np_array(const std::vector<std::vector<T>> &data) {
    if (data.empty()) {
        throw std::invalid_argument("empty numpy array is not supported");
    }
    throw_if_ragged_recursive(data);
    auto shape = deduce_shape(data);
    auto flat_data = flatten(data);
    return NpArr<T>::create(std::move(shape), std::move(flat_data));
}