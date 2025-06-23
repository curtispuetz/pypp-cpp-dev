#pragma once
#include "numpy/util.h"
#include <functional>
#include <vector>

class NpArrIndexCalculator {
  public:
    NpArrIndexCalculator(const std::vector<int> &shape,
                         const std::vector<int> &strides,
                         const std::vector<ViewInfo> &view_info);

    int calc_index(const std::vector<int> &indices) const;

  private:
    int _calc_index_for_view(std::vector<int> indices) const;
    std::vector<int>
    _calc_indices_for_outer_view(int k, const std::vector<int> &shape,
                                 const std::vector<int> &indices) const;
    int _calc_index_for_root(const std::vector<int> &indices) const;
    // Shape
    std::vector<int> shape() const;

    std::vector<int> self_shape;
    std::vector<int> self_strides;
    std::vector<ViewInfo> self_all_view_info;
};