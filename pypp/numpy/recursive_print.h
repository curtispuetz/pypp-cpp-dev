#pragma once
#include "py_list.h"
#include <functional>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string
np_as_str_recursive(const std::function<T(const std::vector<int> &)> &arr_at,
                    const PyList<int> &shape, const std::vector<int> &indices,
                    int indent) {
    std::ostringstream oss;
    if (indices.size() == shape.len() - 1) {
        oss << "[";
        for (int k = 0; k < shape[-1]; ++k) {
            if (k > 0)
                oss << " ";
            std::vector<int> idx = indices;
            idx.push_back(k);
            oss << arr_at(idx);
        }
        oss << "]";
        return oss.str();
    }
    oss << "[";
    for (int i = 0; i < shape[indices.size()]; ++i) {
        if (i > 0) {
            oss << "\n" << std::string(indent + 1, ' ');
        }
        std::vector<int> idx = indices;
        idx.push_back(i);
        oss << np_as_str_recursive<T>(arr_at, shape, idx, indent + 1);
    }
    oss << "]";
    return oss.str();
}

template <typename T>
std::string np_as_str(const std::function<T(const std::vector<int> &)> &arr_at,
                      const PyList<int> &shape) {
    return np_as_str_recursive<T>(arr_at, shape, {}, 0);
}
