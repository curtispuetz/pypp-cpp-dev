#pragma once

#include <initializer_list>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>

template <typename T> class DynamicMultiArray {
  private:
    std::vector<T> data_;
    std::vector<size_t> shape_;
    std::vector<size_t> strides_;

    // Helper to calculate the flat index from multidimensional indices
    template <typename... Dims> size_t get_index(size_t i, Dims... dims) const {
        // Unpack indices into a vector
        std::vector<size_t> indices = {i, static_cast<size_t>(dims)...};

        if (indices.size() != shape_.size()) {
            throw std::out_of_range(
                "Incorrect number of dimensions for indexing.");
        }

        size_t index = 0;
        for (size_t k = 0; k < indices.size(); ++k) {
            if (indices[k] >= shape_[k]) {
                throw std::out_of_range("Index out of bounds.");
            }
            index += indices[k] * strides_[k];
        }
        return index;
    }

  public:
    // Constructor takes a vector for the shape
    DynamicMultiArray(const std::vector<size_t> &shape) : shape_(shape) {
        if (shape_.empty()) {
            throw std::invalid_argument(
                "Shape must have at least one dimension.");
        }
        size_t total_size = std::accumulate(shape_.begin(), shape_.end(), 1,
                                            std::multiplies<size_t>());
        data_.resize(total_size);

        strides_.resize(shape_.size());
        strides_.back() = 1;
        for (int i = shape_.size() - 2; i >= 0; --i) {
            strides_[i] = strides_[i + 1] * shape_[i + 1];
        }
    }

    // Constructor that takes an initializer list for convenience
    DynamicMultiArray(std::initializer_list<size_t> shape)
        : DynamicMultiArray(std::vector<size_t>(shape)) {}

    // Variadic template for direct access, e.g., arr(0, 1, 2)
    template <typename... Dims> T &operator()(size_t i, Dims... dims) {
        return data_[get_index(i, dims...)];
    }

    template <typename... Dims>
    const T &operator()(size_t i, Dims... dims) const {
        return data_[get_index(i, dims...)];
    }

    // Fill the entire array with a value
    void fill(const T &value) { std::fill(data_.begin(), data_.end(), value); }

    const std::vector<size_t> &shape() const { return shape_; }

    size_t size() const { return data_.size(); }
};