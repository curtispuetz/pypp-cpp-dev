#pragma once
#include "numpy/util.h"
#include "py_list.h"
#include <functional>
#include <vector>

class NpArrIndexCalculator {
  public:
    NpArrIndexCalculator(const PyList<int> &shape,
                         const std::vector<int> &strides,
                         const std::vector<ViewInfo> &view_info);

    int calc_index(const PyList<int> &indices) const;

  private:
    int _calc_index_for_view(PyList<int> indices) const;
    PyList<int> _calc_indices_for_outer_view(int k, const PyList<int> &shape,
                                             const PyList<int> &indices) const;
    int _calc_index_for_root(const PyList<int> &indices) const;
    // Shape
    PyList<int> shape() const;

    PyList<int> self_shape;
    std::vector<int> self_strides;
    std::vector<ViewInfo> self_all_view_info;
};