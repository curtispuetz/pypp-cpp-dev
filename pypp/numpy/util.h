#pragma once

#include "py_list.h"
#include "slice/py_slice.h"
#include <variant>
#include <vector>

// Equivalent of Python's Union[int, PySlice]
using ViewSAndIElem = std::variant<int, PySlice>;
using ViewSAndI = PyList<ViewSAndIElem>;

struct ViewInfo {
    ViewSAndI s_and_i;
    PyList<int> shape;
    int size;

    ViewInfo(const ViewSAndI &s_and_i_, const PyList<int> &shape_, int size_)
        : s_and_i(s_and_i_), shape(shape_), size(size_) {}
};

// Equivalent of calc_strides
std::vector<int> calc_strides(const PyList<int> &shape);

std::size_t _calc_total_size(const PyList<int> &shape);