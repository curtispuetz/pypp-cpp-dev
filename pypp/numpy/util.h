#pragma once
#include "slice/py_slice.h"
#include <variant>
#include <vector>

// Equivalent of Python's Union[int, PySlice]
using ViewSAndIElem = std::variant<int, PySlice2>;
using ViewSAndI = std::vector<ViewSAndIElem>;

struct ViewInfo {
    ViewSAndI s_and_i;
    std::vector<int> shape;
    int size;

    ViewInfo(const ViewSAndI &s_and_i_, const std::vector<int> &shape_,
             int size_)
        : s_and_i(s_and_i_), shape(shape_), size(size_) {}
};

// Equivalent of calc_strides
std::vector<int> calc_strides(const std::vector<int> &shape);