#include "slice/py_slice.h"
#include "exceptions/stdexcept.h"
#include "pypp_util/floor_div.h"
#include "slice/index_calculator.h"

PySlice2::PySlice2(std::optional<int> start, std::optional<int> stop, int step)
    : _start(start), _stop(stop), _step(step) {
    if (step == 0) {
        throw PyppValueError("PySlice step cannot be zero");
    }
    if (start.has_value() && start.value() < 0) {
        throw PyppValueError("PySlice start cannot be less than zero");
    }
    if (stop.has_value() && stop.value() < 0) {
        throw PyppValueError("PySlice stop cannot be less than zero");
    }
}

int PySlice2::stop_index(int collection_size) const {
    return calc_stop_index(_stop, _step, collection_size);
}

int PySlice2::start_index(int collection_size) const {
    return calc_start_index(_start, _step, collection_size);
}

PyTup<int, int, int> PySlice2::indices(int collection_size) const {
    return PyTup(start_index(collection_size), stop_index(collection_size),
                 _step);
}

int PySlice2::calc_slice_length(int collection_size) const {
    const PyTup i = indices(collection_size);
    int start = i.get<0>();
    int stop = i.get<1>();
    int step = i.get<2>();
    if (step > 0) {
        return std::max(0, py_floor_div(stop - start + step - 1, step));
    } else {
        return std::max(0, py_floor_div(start - stop - step - 1, -step));
    }
}

void PySlice2::print(std::ostream &os) const {
    if (_start.has_value()) {
        os << "slice(" << *_start;
    } else {
        os << "slice(None";
    }
    if (_stop.has_value()) {
        os << ", " << *_stop;
    } else {
        os << ", None";
    }
    os << ", " << _step << ")";
}

bool PySlice2::operator==(const PySlice2 &other) const {
    return _start == other._start && // std::optional has == defined
           _stop == other._stop && _step == other._step;
}

std::ostream &operator<<(std::ostream &os, const PySlice2 &pyslice) {
    pyslice.print(os);
    return os;
}