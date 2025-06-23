#pragma once
#include "numpy/index_calculator.h"
#include "numpy/iter_shape.h"
#include "numpy/recursive_print.h"
#include "numpy/util.h"
#include "numpy/view_info_calculator.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <typename T> class NpArr {
  public:
    using value_type = T;

    NpArr(const std::vector<int> &shape, const std::vector<T> &data,
          const std::vector<int> &strides = {},
          const std::vector<ViewInfo> &view_info = {})
        : self_shape(shape), self_data(data),
          self_strides(strides.empty() ? calc_strides(shape) : strides),
          self_all_view_info(view_info),
          self_ic(self_shape, self_strides, self_all_view_info) {}

    // Indexing
    T operator[](int index) const { return self_data[index]; }
    T operator[](const std::vector<int> &indices) const {
        return self_data[self_ic.calc_index(indices)];
    }
    T &operator[](int index) { return self_data[index]; }
    T &operator[](const std::vector<int> &indices) {
        return self_data[self_ic.calc_index(indices)];
    }

    // Fill
    void fill(const T &val) {
        auto fn = [&](int i) { self_data[i] = val; };
        _apply_all_data(fn);
    }

    // Shape
    std::vector<int> shape() const {
        if (self_all_view_info.empty()) {
            return self_shape;
        }
        return self_all_view_info.back().shape;
    }

    // View
    NpArr<T> view(const ViewSAndI &view_s_and_i) const {
        auto new_all_view_info = calc_all_view_info_for_view(
            shape(), self_shape, self_all_view_info, view_s_and_i);
        return NpArr<T>(self_shape, self_data, self_strides, new_all_view_info);
    }

    // Size
    int size() const {
        if (self_all_view_info.empty()) {
            return static_cast<int>(self_data.size());
        }
        return self_all_view_info.back().size;
    }

    // For string conversion
    T _at_for_str(const std::vector<int> &indices) const {
        return (*this)[indices];
    }

    // String representation
    std::string to_string() const {
        return np_as_str<T>(
            [this](const std::vector<int> &indices) {
                return (*this)[indices];
            },
            shape());
    }

    // Arithmetic operators (elementwise)
    NpArr<T> operator*(const T &val) const {
        return _new_np_arr([&](const std::vector<int> &indices) {
            return (*this)[indices] * val;
        });
    }
    NpArr<T> operator/(const T &val) const {
        return _new_np_arr([&](const std::vector<int> &indices) {
            return (*this)[indices] / val;
        });
    }
    NpArr<T> operator+(const T &val) const {
        return _new_np_arr([&](const std::vector<int> &indices) {
            return (*this)[indices] + val;
        });
    }
    NpArr<T> operator-(const T &val) const {
        return _new_np_arr([&](const std::vector<int> &indices) {
            return (*this)[indices] - val;
        });
    }

    // In-place arithmetic
    NpArr<T> &operator*=(const T &val) {
        auto fn = [&](int i) { self_data[i] *= val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator/=(const T &val) {
        auto fn = [&](int i) { self_data[i] /= val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator+=(const T &val) {
        auto fn = [&](int i) { self_data[i] += val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator-=(const T &val) {
        auto fn = [&](int i) { self_data[i] -= val; };
        _apply_all_data(fn);
        return *this;
    }

    // Reverse arithmetic (scalar op array)
    friend NpArr<T> operator*(const T &val, const NpArr<T> &arr) {
        return arr * val;
    }
    friend NpArr<T> operator/(const T &val, const NpArr<T> &arr) {
        return arr._new_np_arr([&](const std::vector<int> &indices) {
            return val / arr[indices];
        });
    }
    friend NpArr<T> operator+(const T &val, const NpArr<T> &arr) {
        return arr + val;
    }
    friend NpArr<T> operator-(const T &val, const NpArr<T> &arr) {
        return arr._new_np_arr([&](const std::vector<int> &indices) {
            return val - arr[indices];
        });
    }

  private:
    std::vector<int> self_shape;
    mutable std::vector<T> self_data;
    std::vector<int> self_strides;
    std::vector<ViewInfo> self_all_view_info;
    NpArrIndexCalculator self_ic;

    // Helper to create a new NpArr with a function
    NpArr<T> _new_np_arr(std::function<T(const std::vector<int> &)> fn) const {
        auto new_shape = shape();
        return NpArr<T>(new_shape, new_np_arr_data<T>(fn, new_shape));
    }

    // Helper to apply a function to all data
    void _apply_all_data(std::function<void(int)> fn) {
        if (self_all_view_info.empty()) {
            for (int i = 0; i < static_cast<int>(self_data.size()); ++i) {
                fn(i);
            }
        } else {
            for (const auto &indices : iter_shape(shape())) {
                fn(self_ic.calc_index(indices));
            }
        }
    }
};