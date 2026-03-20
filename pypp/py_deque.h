#pragma once

#include "exceptions/common.h"
#include "py_dict.h"
#include "py_list.h"
#include "py_set.h"
#include "pypp_util/print_py_value.h"
#include <algorithm>
#include <deque>
#include <format>
#include <iostream>
#include <optional>
#include <sstream>

namespace pypp {

template <typename T> class PyDeque {
  private:
    std::deque<T> data;
    std::optional<int> maxlen_;

    // Helper: enforce maxlen after push_back
    void enforce_maxlen_right() {
        if (maxlen_.has_value() && static_cast<int>(data.size()) > *maxlen_) {
            data.pop_front();
        }
    }

    // Helper: enforce maxlen after push_front
    void enforce_maxlen_left() {
        if (maxlen_.has_value() && static_cast<int>(data.size()) > *maxlen_) {
            data.pop_back();
        }
    }

  public:
    using value_type = T;

    // ---- Constructors ----

    // Default constructor: empty deque, no maxlen
    PyDeque() = default;

    // Constructor with optional maxlen only
    explicit PyDeque(std::optional<int> maxlen) : maxlen_(maxlen) {}

    // Constructor from initializer_list with optional maxlen
    PyDeque(std::initializer_list<T> init,
            std::optional<int> maxlen = std::nullopt)
        : maxlen_(maxlen) {
        for (const auto &item : init) {
            append(item);
        }
    }

    // Constructor from PyList with optional maxlen
    PyDeque(const PyList<T> &list, std::optional<int> maxlen = std::nullopt)
        : maxlen_(maxlen) {
        for (const auto &item : list) {
            append(item);
        }
    }

    // Constructor from PySet with optional maxlen
    PyDeque(const PySet<T> &set, std::optional<int> maxlen = std::nullopt)
        : maxlen_(maxlen) {
        for (const auto &item : set) {
            append(item);
        }
    }

    // Constructor from PyDict (iterates over keys) with optional maxlen
    template <typename V>
    PyDeque(const PyDict<T, V> &dict, std::optional<int> maxlen = std::nullopt)
        : maxlen_(maxlen) {
        for (const auto &[key, val] : dict.data) {
            append(key);
        }
    }

    // ---- append / appendleft ----

    void append(const T &value) {
        if (maxlen_.has_value() && *maxlen_ == 0) {
            return; // maxlen=0 means never store anything
        }
        data.push_back(value);
        enforce_maxlen_right();
    }

    void append(T &&value) {
        if (maxlen_.has_value() && *maxlen_ == 0) {
            return;
        }
        data.push_back(std::move(value));
        enforce_maxlen_right();
    }

    void appendleft(const T &value) {
        if (maxlen_.has_value() && *maxlen_ == 0) {
            return;
        }
        data.push_front(value);
        enforce_maxlen_left();
    }

    void appendleft(T &&value) {
        if (maxlen_.has_value() && *maxlen_ == 0) {
            return;
        }
        data.push_front(std::move(value));
        enforce_maxlen_left();
    }

    // ---- pop / popleft ----

    T pop() {
        if (data.empty()) {
            throw IndexError("pop from an empty deque");
        }
        T value = std::move(data.back());
        data.pop_back();
        return value;
    }

    T popleft() {
        if (data.empty()) {
            throw IndexError("pop from an empty deque");
        }
        T value = std::move(data.front());
        data.pop_front();
        return value;
    }

    // ---- extend / extendleft ----

    void extend(const PyList<T> &other) {
        for (const auto &item : other) {
            append(item);
        }
    }

    void extend(PyList<T> &&other) {
        for (auto &item : other) {
            append(std::move(item));
        }
    }

    void extendleft(const PyList<T> &other) {
        for (const auto &item : other) {
            appendleft(item);
        }
    }

    void extendleft(PyList<T> &&other) {
        for (auto &item : other) {
            appendleft(std::move(item));
        }
    }

    // ---- insert ----

    void insert(int index, const T &value) {
        if (maxlen_.has_value() && static_cast<int>(data.size()) >= *maxlen_) {
            throw IndexError("deque already at its maximum size");
        }
        // Handle negative index
        if (index < 0) {
            index = static_cast<int>(data.size()) + index;
        }
        // Clamp to valid range
        if (index < 0)
            index = 0;
        if (index > static_cast<int>(data.size()))
            index = static_cast<int>(data.size());
        data.insert(data.begin() + index, value);
    }

    void insert(int index, T &&value) {
        if (maxlen_.has_value() && static_cast<int>(data.size()) >= *maxlen_) {
            throw IndexError("deque already at its maximum size");
        }
        if (index < 0) {
            index = static_cast<int>(data.size()) + index;
        }
        if (index < 0)
            index = 0;
        if (index > static_cast<int>(data.size()))
            index = static_cast<int>(data.size());
        data.insert(data.begin() + index, std::move(value));
    }

    // ---- remove ----

    void remove(const T &value) {
        auto it = std::find(data.begin(), data.end(), value);
        if (it == data.end()) {
            throw ValueError(std::string("deque.remove(x): x not in deque"));
        }
        data.erase(it);
    }

    // ---- rotate ----

    void rotate(int n = 1) {
        if (data.empty())
            return;
        int sz = static_cast<int>(data.size());
        // Normalize n into [0, sz)
        n = n % sz;
        if (n < 0)
            n += sz;
        if (n == 0)
            return;
        // Rotate right by n: move last n elements to front
        std::rotate(data.begin(), data.begin() + (sz - n), data.end());
    }

    // ---- reverse ----

    void reverse() { std::reverse(data.begin(), data.end()); }

    // ---- copy ----

    PyDeque<T> copy() const {
        PyDeque<T> new_deque;
        new_deque.data = data;
        new_deque.maxlen_ = maxlen_;
        return new_deque;
    }

    // ---- clear ----

    void clear() { data.clear(); }

    // ---- count ----

    int count(const T &value) const {
        return static_cast<int>(std::count(data.begin(), data.end(), value));
    }

    // ---- index ----
    // index(value, start=0, stop=len)

    int index(const T &value, int start = 0,
              std::optional<int> stop = std::nullopt) const {
        int sz = static_cast<int>(data.size());
        int end = stop.has_value() ? *stop : sz;

        // Normalize start
        if (start < 0)
            start += sz;
        if (start < 0)
            start = 0;

        // Normalize end
        if (end < 0)
            end += sz;
        if (end > sz)
            end = sz;

        for (int i = start; i < end; ++i) {
            if (data[i] == value) {
                return i;
            }
        }
        throw ValueError(std::string("deque.index(x): x not in deque"));
    }

    // ---- contains ----

    bool contains(const T &value) const {
        return std::find(data.begin(), data.end(), value) != data.end();
    }

    // ---- dg (deque get, supports negative indices) ----

    T &dg(int index) {
        if (index < 0)
            index += static_cast<int>(data.size());
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw IndexError("deque index out of range");
        }
        return data[index];
    }

    const T &dg(int index) const {
        if (index < 0)
            index += static_cast<int>(data.size());
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw IndexError("deque index out of range");
        }
        return data[index];
    }

    // ---- operator[] (supports negative indices, consistent with Python) ----

    T &operator[](int index) {
        if (index < 0)
            index += static_cast<int>(data.size());
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw IndexError("deque index out of range");
        }
        return data[index];
    }

    const T &operator[](int index) const {
        if (index < 0)
            index += static_cast<int>(data.size());
        if (index < 0 || index >= static_cast<int>(data.size())) {
            throw IndexError("deque index out of range");
        }
        return data[index];
    }

    // ---- Size ----

    int len() const { return static_cast<int>(data.size()); }

    // ---- maxlen accessor ----

    std::optional<int> maxlen() const { return maxlen_; }

    // ---- Comparison operators ----

    bool operator==(const PyDeque<T> &other) const {
        return data == other.data;
    }
    bool operator!=(const PyDeque<T> &other) const {
        return data != other.data;
    }
    bool operator<(const PyDeque<T> &other) const { return data < other.data; }
    bool operator<=(const PyDeque<T> &other) const {
        return data <= other.data;
    }
    bool operator>(const PyDeque<T> &other) const { return data > other.data; }
    bool operator>=(const PyDeque<T> &other) const {
        return data >= other.data;
    }

    // ---- Concatenation ----

    PyDeque<T> operator+(const PyDeque<T> &other) const {
        PyDeque<T> result;
        result.data = data;
        result.data.insert(result.data.end(), other.data.begin(),
                           other.data.end());
        return result;
    }

    PyDeque<T> &operator+=(const PyDeque<T> &other) {
        for (const auto &item : other.data) {
            append(item);
        }
        return *this;
    }

    // ---- Iterator support ----

    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto rbegin() { return data.rbegin(); }
    auto rend() { return data.rend(); }
    auto rbegin() const { return data.crbegin(); }
    auto rend() const { return data.crend(); }

    // ---- Print ----

    void print(std::ostream &os) const {
        os << "deque([";
        for (size_t i = 0; i < data.size(); ++i) {
            print_py_value(os, data[i]);
            if (i + 1 != data.size())
                os << ", ";
        }
        os << "]";
        if (maxlen_.has_value()) {
            os << ", maxlen=" << *maxlen_;
        }
        os << ")";
    }

    void print() const {
        print(std::cout);
        std::cout << std::endl;
    }

    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const PyDeque<U> &deque);
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const PyDeque<T> &deque) {
    deque.print(os);
    return os;
}

} // namespace pypp

namespace std {
// Hash function for usage as a key in PyDict and PySet
template <typename T> struct hash<pypp::PyDeque<T>> {
    std::size_t operator()(const pypp::PyDeque<T> &p) const noexcept {
        std::size_t seed = 0;
        for (const auto &item : p) {
            seed ^=
                std::hash<T>()(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
// Formatter for std::format
template <typename T> struct formatter<pypp::PyDeque<T>, char> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const pypp::PyDeque<T> &p, FormatContext &ctx) const {
        std::ostringstream oss;
        p.print(oss);
        return std::format_to(ctx.out(), "{}", oss.str());
    }
};
} // namespace std
