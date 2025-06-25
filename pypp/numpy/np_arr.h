#pragma once

#include "numpy/index_calculator.h"
#include "numpy/iter_shape.h"
#include "numpy/recursive_print.h"
#include "numpy/util.h"
#include "numpy/view_info_calculator.h"
#include "py_list.h"
#include "pypp_util/dependency.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

template <typename T> class NpArr {
  private:
    PyppDependency<PyList<int>> self_shape;
    PyppDependency<std::vector<T>> self_data;
    PyppDependency<std::vector<int>> self_strides;
    PyppDependency<std::vector<ViewInfo>> self_all_view_info;
    NpArrIndexCalculator self_ic;

    // Helper to create a new NpArr with a function
    NpArr<T> _new_np_arr(std::function<T(const std::vector<int> &)> fn) const {
        auto new_shape = shape();
        auto new_data = new_np_arr_data<T>(fn, new_shape);
        return NpArr<T>::create(std::move(new_shape), std::move(new_data));
    }

    // Helper to apply a function to all data
    void _apply_all_data(std::function<void(int)> fn) {
        if (self_all_view_info.g().empty()) {
            for (int i = 0; i < static_cast<int>(self_data.g().size()); ++i) {
                fn(i);
            }
        } else {
            // Need to hold shape for iter_shape since it takes a reference.
            auto shape_ = shape();
            for (const auto &indices : iter_shape(shape_)) {
                fn(self_ic.calc_index(indices));
            }
        }
    }
    // TODO: remove the .g() calls everywhere and add more class members.
  public:
    NpArr(PyppDependency<PyList<int>> shape,
          PyppDependency<std::vector<T>> data,
          PyppDependency<std::vector<int>> strides,
          PyppDependency<std::vector<ViewInfo>> view_info)
        : self_shape(std::move(shape)), self_data(std::move(data)),
          self_strides(std::move(strides)),
          self_all_view_info(std::move(view_info)),
          self_ic(self_shape.g(), self_strides.g(), self_all_view_info.g()) {}

    NpArr<T> static create(
        PyList<int> shape, std::vector<T> data,
        std::vector<int> strides = std::vector<int>(),
        std::vector<ViewInfo> view_info = std::vector<ViewInfo>()) {
        std::vector<int> calculated_strides =
            strides.empty() ? calc_strides(shape) : std::move(strides);
        return NpArr<T>(std::move(shape), std::move(data),
                        std::move(calculated_strides), std::move(view_info));
    }

    // Indexing
    T operator[](int index) const { return self_data.g()[index]; }
    T operator[](const PyList<int> &indices) const {
        return self_data.g()[self_ic.calc_index(indices)];
    }
    T &operator[](int index) { return self_data.g()[index]; }
    T &operator[](const PyList<int> &indices) {
        return self_data.g()[self_ic.calc_index(indices)];
    }

    // Fill
    void fill(const T &val) {
        auto fn = [&](int i) { self_data.g()[i] = val; };
        _apply_all_data(fn);
    }

    // Shape
    // TODO: is it better to return a reference or constant reference here?
    PyList<int> shape() const {
        if (self_all_view_info.g().empty()) {
            return self_shape.g();
        }
        return self_all_view_info.g().back().shape;
    }

    // View
    NpArr<T> view(const ViewSAndI &view_s_and_i) {
        auto new_all_view_info = calc_all_view_info_for_view(
            shape(), self_shape.g(), self_all_view_info.g(), view_s_and_i);
        // TODO: still it is not working. I don't know what I am doing. I need a
        // simplified example.
        // I have no idea why this is failed. It is the exact same pattern in
        // testy.h and testy.h works.
        return NpArr<T>(self_shape.g(), self_data.g(), self_strides.g(),
                        std::move(new_all_view_info));
    }

    // Size
    int size() const {
        if (self_all_view_info.g().empty()) {
            return static_cast<int>(self_data.g().size());
        }
        return self_all_view_info.g().back().size;
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
        auto fn = [&](int i) { self_data.g()[i] *= val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator/=(const T &val) {
        auto fn = [&](int i) { self_data.g()[i] /= val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator+=(const T &val) {
        auto fn = [&](int i) { self_data.g()[i] += val; };
        _apply_all_data(fn);
        return *this;
    }
    NpArr<T> &operator-=(const T &val) {
        auto fn = [&](int i) { self_data.g()[i] -= val; };
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
};