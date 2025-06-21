#include "py_slice.h"

std::vector<int> PySlice::_compute_slice_indices(int start,
                                                 std::optional<int> stop,
                                                 int step,
                                                 int collection_length) const {
    if (step == 0) {
        throw PyppValueError("slice step cannot be zero");
    }
    std::vector<int> result;

    int stop_val = stop.value_or(collection_length);

    if (start < 0)
        start += collection_length;
    if (stop_val < 0)
        stop_val += collection_length;

    if (start < 0)
        start = 0;
    if (stop_val > collection_length)
        stop_val = collection_length;

    if (step > 0 && start < stop_val) {
        for (int i = start; i < stop_val; i += step)
            result.push_back(i);
    } else if (step < 0 && start > stop_val) {
        for (int i = start; i > stop_val; i += step)
            result.push_back(i);
    }

    return result;
}

int PySlice::compute_slice_length(int collection_length) const {
    if (m_step == 0) {
        throw PyppValueError("slice step cannot be zero");
    }

    int start = m_start < 0 ? m_start + collection_length : m_start;
    int stop = m_stop.value_or(collection_length);

    if (stop < 0) {
        stop += collection_length;
    }

    if (start < 0) {
        start = 0;
    }
    if (stop > collection_length) {
        stop = collection_length;
    }

    if (m_step > 0 && start < stop) {
        return (stop - start + m_step - 1) / m_step; // Ceiling division
    } else if (m_step < 0 && start > stop) {
        return (start - stop - m_step - 1) / -m_step; // Ceiling division
    }
    return 0; // No valid slice
}

void PySlice::print(std::ostream &os) const {
    os << "slice(" << m_start;
    if (m_stop.has_value()) {
        os << ", " << *m_stop;
    } else {
        os << ", None";
    }
    os << ", " << m_step << ")";
}

std::vector<int> PySlice::compute_slice_indices(int collection_length) const {
    return _compute_slice_indices(m_start, m_stop, m_step, collection_length);
}

bool PySlice::operator==(const PySlice &other) const {
    return m_start == other.m_start &&
           m_stop == other.m_stop && // std::optional has == defined
           m_step == other.m_step;
}

std::ostream &operator<<(std::ostream &os, const PySlice &pyslice) {
    pyslice.print(os);
    return os;
}