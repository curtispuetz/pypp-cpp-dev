#include "numpy/util.h"

std::vector<int> calc_strides(const std::vector<int> &shape) {
    std::vector<int> ret(shape.size(), 0);
    if (shape.empty())
        return ret;
    ret.back() = 1;
    for (int i = static_cast<int>(shape.size()) - 2; i >= 0; --i) {
        ret[i] = ret[i + 1] * shape[i + 1];
    }
    return ret;
}