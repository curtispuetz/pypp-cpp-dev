#pragma once
#include "numpy/util.h" // Assumed: defines ViewSAndI and PySlice
#include <vector>

void _handle_size_mismatch(size_t len_view_s_and_i, size_t len_shape);

std::pair<PyList<int>, int>
_calc_view_shape_and_size(const ViewSAndI &view_s_and_i,
                          const PyList<int> &outer_shape);

std::vector<ViewInfo>
calc_all_view_info_for_view(const PyList<int> &arr_shape,
                            const PyList<int> &arr_root_shape,
                            const std::vector<ViewInfo> current_all_view_info,
                            const ViewSAndI &view_s_and_i);