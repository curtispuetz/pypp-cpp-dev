#pragma once

#include "py_tuple.h"
#include <optional>
#include <stdexcept>

class PySlice2 {
  public:
    PySlice2(std::optional<int> start, std::optional<int> stop, int step);

    int stop_index(int collection_size) const;
    int start_index(int collection_size) const;
    PyTup<int, int, int> indices(int collection_size) const;
    int calc_slice_length(int collection_size) const;

    std::optional<int> start() const { return _start; }
    std::optional<int> stop() const { return _stop; }
    int step() const { return _step; }

  private:
    std::optional<int> _start;
    std::optional<int> _stop;
    int _step;
};