#include "index_calculator.h"
#include "exceptions/stdexcept.h"
#include "slice/py_slice.h"
#include <variant>

NpArrIndexCalculator::NpArrIndexCalculator(
    const std::vector<int> &shape, const std::vector<int> &strides,
    const std::vector<ViewInfo> &view_info)
    : self_shape(shape), self_strides(strides), self_all_view_info(view_info) {}

int NpArrIndexCalculator::calc_index(const std::vector<int> &indices) const {
    int shape_len = static_cast<int>(shape().size());
    if (indices.size() != static_cast<size_t>(shape_len)) {
        throw PyppIndexError("indices mismatch: array is " +
                             std::to_string(shape_len) + "-dimensional, but " +
                             std::to_string(indices.size()) + " were indexed");
    }
    if (self_all_view_info.empty()) {
        return _calc_index_for_root(indices);
    }
    return _calc_index_for_view(indices);
}

int NpArrIndexCalculator::_calc_index_for_view(std::vector<int> indices) const {
    for (int i = static_cast<int>(self_all_view_info.size()) - 1; i >= 0; --i) {
        std::vector<int> shape;
        if (i - 1 < 0) {
            shape = self_shape;
        } else {
            shape = self_all_view_info[i - 1].shape;
        }
        indices = _calc_indices_for_outer_view(i, shape, indices);
    }
    return _calc_index_for_root(indices);
}

std::vector<int> NpArrIndexCalculator::_calc_indices_for_outer_view(
    int k, const std::vector<int> &shape,
    const std::vector<int> &indices) const {
    std::vector<int> outer_indices;
    int i = 0;
    const auto &s_and_i = self_all_view_info[k].s_and_i;
    for (size_t j = 0; j < s_and_i.size(); ++j) {
        // s_and_i[j] is either int or a struct/class with start_index(int) and
        // step
        if (std::holds_alternative<int>(s_and_i[j])) {
            outer_indices.push_back(std::get<int>(s_and_i[j]));
        } else {
            const auto &v = std::get<PySlice2>(s_and_i[j]);
            outer_indices.push_back(v.start_index(shape[j]) +
                                    indices[i] * v.step());
            ++i;
        }
    }
    return outer_indices;
}

int NpArrIndexCalculator::_calc_index_for_root(
    const std::vector<int> &indices) const {
    int ret = 0;
    for (size_t i = 0; i < indices.size(); ++i) {
        if (indices[i] >= self_shape[i]) {
            throw PyppIndexError("index " + std::to_string(indices[i]) +
                                 " is out of bounds for axis " +
                                 std::to_string(i) + " with size " +
                                 std::to_string(self_shape[i]));
        }
        ret += indices[i] * self_strides[i];
    }
    return ret;
}

// Shape
std::vector<int> NpArrIndexCalculator::shape() const {
    if (self_all_view_info.empty()) {
        return self_shape;
    }
    return self_all_view_info.back().shape;
}