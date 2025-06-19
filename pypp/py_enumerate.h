#pragma once

#include <cstddef>  // For size_t
#include <iterator> // For std::begin, std::end, std::iterator_traits
#include <utility>  // For std::forward, std::pair

// Forward declaration of the iterator class
template <typename T> class py_enumerate_iterator;

// The main PyEnumerate class which acts as the "range"
template <typename T> class PyEnumerate {
  public:
    // The constructor uses a universal reference (T&&) to accept both
    // lvalues (e.g., existing vectors) and rvalues (e.g., temporary lists).
    // std::forward ensures that the iterable is perfectly forwarded,
    // meaning it's moved if it's a temporary and copied/referenced if it's not.
    PyEnumerate(T &&iterable) : _iterable(std::forward<T>(iterable)) {}

    // The begin() method of the range, returns our custom iterator.
    auto begin() { return py_enumerate_iterator<T>(std::begin(_iterable), 0); }

    // The end() method of the range.
    auto end() {
        // The end iterator's index is not used for comparison, so it can be 0.
        return py_enumerate_iterator<T>(std::end(_iterable), 0);
    }

  private:
    T _iterable;
};

// C++17 Deduction Guide: This allows you to write `PyEnumerate(my_vector)`
// instead of `PyEnumerate<decltype(my_vector)>(my_vector)`. The compiler
// automatically deduces the template argument `T`.
template <typename T> PyEnumerate(T &&) -> PyEnumerate<T>;

// The iterator class for PyEnumerate
template <typename T> class py_enumerate_iterator {
  private:
    // Deduces the type of the underlying container's iterator.
    using InnerIterator = decltype(std::begin(std::declval<T &>()));
    // Deduces the reference type of the container's elements (e.g., int&).
    using InnerReference =
        typename std::iterator_traits<InnerIterator>::reference;

  public:
    // --- C++ Standard Iterator Traits ---
    // These type definitions are necessary to make our iterator compatible
    // with the C++ standard library and range-based for loops.
    using iterator_category = std::input_iterator_tag;
    // The type returned when dereferencing the iterator.
    // It's a pair containing the index and a REFERENCE to the original value.
    using value_type = std::pair<size_t, InnerReference>;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = value_type;
    // --- End of Traits ---

    // Constructor for the iterator
    py_enumerate_iterator(InnerIterator it, size_t index)
        : _it(it), _index(index) {}

    // Dereference operator (*it)
    // Returns a std::pair by value. This pair contains the current index
    // and a reference to the element in the original container.
    reference operator*() const { return {_index, *_it}; }

    // Pre-increment operator (++it)
    py_enumerate_iterator &operator++() {
        ++_it;
        ++_index;
        return *this;
    }

    // Comparison operator (it != end())
    bool operator!=(const py_enumerate_iterator &other) const {
        // Iteration stops when the underlying container's iterator reaches its
        // end.
        return _it != other._it;
    }

  private:
    InnerIterator _it;
    size_t _index;
};