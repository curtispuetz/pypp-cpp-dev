#pragma once

#include "slice/py_slice.h"
#include <optional>

inline PySlice2 py_slice_empty() { return PySlice2(0, std::nullopt, 1); }

inline PySlice2 py_slice_stop(int stop) { return PySlice2(0, stop, 1); }

inline PySlice2 py_slice(std::optional<int> start, std::optional<int> stop,
                         int step = 1) {
    return PySlice2(start, stop, step);
}