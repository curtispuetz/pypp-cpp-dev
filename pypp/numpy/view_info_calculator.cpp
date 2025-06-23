#include "view_info_calculator.h"
#include "exceptions/stdexcept.h"
#include "slice/py_slice.h"

void _handle_size_mismatch(size_t len_view_s_and_i, size_t len_shape) {
    if (len_view_s_and_i != len_shape) {
        throw PyppIndexError(
            "indices mismatch when creating an array view: array is " +
            std::to_string(len_shape) + "-dimensional but " +
            std::to_string(len_view_s_and_i) +
            " slices or integers were provided");
    }
}

std::pair<std::vector<int>, int>
_calc_view_shape_and_size(const ViewSAndI &view_s_and_i,
                          const std::vector<int> &outer_shape) {
    std::vector<int> ret_shape;
    int ret_size = 1;
    for (size_t i = 0; i < view_s_and_i.size(); ++i) {
        auto v_i = view_s_and_i[i];
        if (std::holds_alternative<PySlice2>(v_i)) {
            int s = std::get<PySlice2>(v_i).calc_slice_length(outer_shape[i]);
            ret_shape.push_back(s);
            ret_size *= s;
        }
    }
    return {ret_shape, ret_size};
}

std::vector<ViewInfo>
calc_all_view_info_for_view(const std::vector<int> &arr_shape,
                            const std::vector<int> &arr_root_shape,
                            const std::vector<ViewInfo> current_all_view_info,
                            const ViewSAndI &view_s_and_i) {
    _handle_size_mismatch(view_s_and_i.size(), arr_shape.size());
    std::vector<ViewInfo> ret = current_all_view_info;
    const std::vector<int> &shape =
        ret.empty() ? arr_root_shape : ret.back().shape;
    auto [view_shape, view_size] =
        _calc_view_shape_and_size(view_s_and_i, shape);
    ret.emplace_back(view_s_and_i, view_shape, view_size);
    return ret;
}