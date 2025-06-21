#pragma once

#include "exceptions/stdexcept.h"
#include "exceptions/system_error.h"
#include "py_list.h"
#include "pypp_util/print_py_value.h"
#include <algorithm>
#include <format>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <type_traits> // Required for std::true_type, std::false_type
#include <utility>     // Required for std::index_sequence
#include <variant>
#include <vector>

template <typename T> class NpArr {
  private:
    std::vector<T> data_;
    PyList<int> shape_;
    std::vector<int> strides_;

    // Helper to calculate the flat index from multidimensional indices
    int get_index(PyList<int> indices) const {
        if (indices.len() != shape_.len()) {
            throw PyppIndexError(
                "indices mismatch: array is " + std::to_string(shape_.len()) +
                "-dimensional, but " + std::to_string(indices.len()) +
                " were indexed");
        }

        int index = 0;
        for (int k = 0; k < indices.len(); ++k) {
            if (indices[k] >= shape_[k]) {
                throw PyppIndexError("index " + std::to_string(indices[k]) +
                                     " is out of bounds for axis " +
                                     std::to_string(k) + " with size " +
                                     std::to_string(shape_[k]));
            }
            index += indices[k] * strides_[k];
        }
        return index;
    }

    // Recursive helper for printing
    void print_recursive(std::ostream &os, int dim_index,
                         std::vector<int> &current_indices,
                         int indent_level) const {
        if (dim_index == shape_.len() - 1) {
            // Last dimension, print elements
            os << "[";
            for (int i = 0; i < shape_[dim_index]; ++i) {
                current_indices[dim_index] = i;
                int flat_index = 0;
                for (int k = 0; k < current_indices.size(); ++k) {
                    flat_index += current_indices[k] * strides_[k];
                }
                print_py_value(os, data_[flat_index]);
                if (i < shape_[dim_index] - 1) {
                    os << " ";
                }
            }
            os << "]";
        } else {
            // Not the last dimension, print opening bracket
            os << "[";
            for (int i = 0; i < shape_[dim_index]; ++i) {
                current_indices[dim_index] = i;
                if (i > 0) {
                    os << std::string(indent_level,
                                      ' '); // Indent for subsequent rows
                }
                print_recursive(os, dim_index + 1, current_indices,
                                indent_level + 1);
                if (i < shape_[dim_index] - 1) {
                    os << "\n";
                }
            }
            os << "]";
        }
    }

  public:
    class NpArrView {
      private:
        NpArr<T> &original_;
        PyList<std::variant<int, PySlice>> view_indices_;
        int num_axes_;
        mutable std::optional<PyList<int>> cached_shape_;
        mutable std::optional<int> cached_size_;

        // TODO: change these names to 'calc'
        PyList<int> get_original_indicies(PyList<int> indices) const {
            if (indices.len() != num_axes_) {
                throw PyppIndexError(
                    "indices mismatch: view is " + std::to_string(num_axes_) +
                    "-dimensional, but " + std::to_string(indices.len()) +
                    " were indexed");
            }
            PyList<int> orig_indices;
            int i = 0;
            for (const auto &int_or_slice : view_indices_) {
                if (std::holds_alternative<int>(int_or_slice)) {
                    orig_indices.append(std::get<int>(int_or_slice));
                } else if (std::holds_alternative<PySlice>(int_or_slice)) {
                    const PySlice &slice = std::get<PySlice>(int_or_slice);
                    orig_indices.append(slice.m_start +
                                        indices[i] * slice.m_step);
                    i += 1;
                }
            }
            return orig_indices;
        }

        void fill_view_indicies() {
            // if view_indices_ length is less than original shape length,
            // fill the rest with PySlice(std::nullopt)
            int original_ndim = original_.shape().len();
            int view_ndim = view_indices_.len();
            if (view_ndim < original_ndim) {
                int diff = original_ndim - view_ndim;
                for (int i = 0; i < diff; ++i) {
                    view_indices_.append(PySlice(std::nullopt));
                }
            } else if (view_ndim > original_ndim) {
                std::cout << "Here2";
                throw PyppIndexError(
                    "too many indices for array: array is " +
                    std::to_string(original_ndim) + "-dimensional, but " +
                    std::to_string(view_ndim) + " were indexed");
            }
        }

        PySlice calc_slice_of_slice(const PySlice &slice,
                                    const PySlice &new_slice) const {
            // Calculate the new slice based on the original slice and the new
            // slice
            int start = slice.m_start + new_slice.m_start * slice.m_step;
            int step = new_slice.m_step * slice.m_step;
            std::optional<int> stop;
            if (new_slice.m_stop.has_value()) {
                stop = slice.m_start + new_slice.m_stop.value() * slice.m_step;
            } else {
                stop = slice.m_stop;
            }
            return PySlice(start, stop, step);
        }

      public:
        NpArrView(NpArr<T> &orig,
                  PyList<std::variant<int, PySlice>> view_indices)
            : original_(orig), view_indices_(view_indices) {
            // For now, require the right number of slices passed.
            // fill_view_indicies();
            num_axes_ = 0;
            for (const auto &index : view_indices_) {
                if (std::holds_alternative<PySlice>(index)) {
                    num_axes_++;
                }
            }
        }

        // TODO: I need to test thing thouroughly in python because it is too
        // hard to test it here in C++.
        NpArrView(NpArrView &other,
                  PyList<std::variant<int, PySlice>> view_indices)
            : original_(other.original_) {
            if (view_indices.len() > other.num_axes_) {
                std::cout << "Here";
                throw PyppIndexError(
                    "too many indices for array view: view is " +
                    std::to_string(other.num_axes_) + "-dimensional, but " +
                    std::to_string(view_indices.len()) + " were indexed");
            }
            // Need to calculate the new view indices and num_axes_
            num_axes_ = 0;
            view_indices_ = PyList<std::variant<int, PySlice>>();
            int s = 0;
            for (const auto &i : other.view_indices_) {
                if (std::holds_alternative<int>(i)) {
                    view_indices_.append(std::get<int>(i));
                } else if (std::holds_alternative<PySlice>(i)) {
                    const PySlice &slice = std::get<PySlice>(i);
                    std::variant<int, PySlice> new_i = view_indices[s];
                    if (std::holds_alternative<int>(new_i)) {
                        view_indices_.append(slice.m_start +
                                             std::get<int>(new_i));
                    } else if (std::holds_alternative<PySlice>(new_i)) {
                        // This is a slice of a slice.
                        const PySlice &new_slice = std::get<PySlice>(new_i);
                        view_indices_.append(
                            calc_slice_of_slice(slice, new_slice));
                        num_axes_++;
                    }
                    s += 1;
                }
            }
        }

        NpArrView operator[](PyList<std::variant<int, PySlice>> indices) {
            return NpArrView(*this, indices);
        }

        const NpArrView
        operator[](PyList<std::variant<int, PySlice>> indices) const {
            return NpArrView(*this, indices);
        }

        const T &at(PyList<int> indices) const {
            return original_.at(get_original_indicies(indices));
        }

        void set(const PyList<int> &i, const T &value) {
            original_.set(get_original_indicies(i), value);
        }

        // Don't support filling a view or printing a view for now.

        const PyList<int> shape() const {
            if (cached_shape_.has_value()) {
                return cached_shape_.value();
            }
            cached_shape_ = PyList<int>();
            PyList<int> original_shape = original_.shape();
            int i = 0;
            for (const auto &index : view_indices_) {
                if (std::holds_alternative<PySlice>(index)) {
                    const PySlice &slice = std::get<PySlice>(index);
                    cached_shape_.value().append(
                        slice.compute_slice_length(original_shape[i]));
                }
                i += 1;
            }
            return cached_shape_.value();
        }

        int size() const {
            if (cached_size_.has_value()) {
                return cached_size_.value();
            }
            PyList<int> s = shape();
            cached_size_ =
                std::accumulate(s.begin(), s.end(), 1, std::multiplies<int>());
            return cached_size_.value();
        }
    };

    // Constructor takes a vector for the shape
    NpArr(const PyList<int> &shape) : shape_(shape) {
        if (shape_.len() == 0) {
            throw PyppValueError("empty numpy array is not supported");
        }
        int total_size = std::accumulate(shape_.begin(), shape_.end(), 1,
                                         std::multiplies<int>());

        data_.resize(total_size);

        strides_.resize(shape_.len());
        strides_.back() = 1;
        for (int i = shape_.len() - 2; i >= 0; --i) {
            strides_[i] = strides_[i + 1] * shape_[i + 1];
        }
    }

    // Constructor that takes an initializer list for convenience
    NpArr(std::initializer_list<int> shape) : NpArr(PyList<int>(shape)) {}

    // Default constructor for an empty array
    NpArr() : NpArr({0}) {
        data_.clear(); // Make sure data is empty for shape (0,)
    }

    // Constructor for internal use by the `from_pylist` factory method.
    // It takes a shape and moves the already-flattened data vector.
    NpArr(const PyList<int> &shape, std::vector<T> &&data)
        : shape_(shape), data_(std::move(data)) {
        // This constructor assumes the provided data is already validated
        // and flattened. It calculates the strides based on the given
        // shape.
        strides_.resize(shape_.len());
        if (!(shape_.len() == 0)) {
            strides_.back() = 1;
            for (int i = shape_.len() - 2; i >= 0; --i) {
                strides_[i] = strides_[i + 1] * shape_[i + 1];
            }
        }

        // Final sanity check to ensure shape and data size match.
        int total_size = shape_.len() == 0
                             ? 0
                             : std::accumulate(shape_.begin(), shape_.end(), 1,
                                               std::multiplies<int>());
        if (data_.size() != total_size) {
            throw PyppSystemError(
                "(this could be a Pypp bug) Data size and "
                "shape mismatch during "
                "construction of numpy array. The list was "
                "likely ragged in a deeper "
                "dimension.",
                std::error_code(errno, std::system_category()));
        }
    }

    NpArrView operator[](PyList<std::variant<int, PySlice>> indices) {
        return NpArrView(*this, indices);
    }

    const NpArrView
    operator[](PyList<std::variant<int, PySlice>> indices) const {
        return NpArrView(*this, indices);
    }

    const T &at(PyList<int> indices) const { return data_[get_index(indices)]; }

    void set(const PyList<int> &i, const T &value) {
        // TODO: I don't really need this because I can set the value
        // with the () operator?
        data_[get_index(i)] = value;
    }

    // Fill the entire array with a value
    void fill(const T &value) { std::fill(data_.begin(), data_.end(), value); }

    const PyList<int> shape() const { return shape_; }

    int size() const { return data_.size(); }

    // Print method similar to NumPy
    void print(std::ostream &os) const {
        std::vector<int> current_indices(shape_.len());
        print_recursive(
            os, 0, current_indices,
            1); // Start recursion from dimension 0, initial indent 1
    }

    void print() const {
        print(std::cout);
        std::cout << std::endl;
    }

    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const NpArr<U> &arr);
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const NpArr<T> &arr) {
    arr.print(os);
    return os;
}

namespace std {
// Formatter for std::format
template <typename T> struct formatter<NpArr<T>, char> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const NpArr<T> &arr, FormatContext &ctx) const {
        std::ostringstream oss;
        arr.print(oss);
        return std::format_to(ctx.out(), "{}", oss.str());
    }
};
} // namespace std