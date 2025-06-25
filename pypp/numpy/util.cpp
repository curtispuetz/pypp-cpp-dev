#include "numpy/util.h"

std::vector<int> calc_strides(const PyList<int> &shape) {
    std::vector<int> ret(shape.len(), 0);
    if (shape.len() == 0)
        return ret;
    ret.back() = 1;
    for (int i = static_cast<int>(shape.len()) - 2; i >= 0; --i) {
        ret[i] = ret[i + 1] * shape[i + 1];
    }
    return ret;
}