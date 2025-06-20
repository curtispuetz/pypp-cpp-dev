#pragma once
#include <functional>
#include <string>

void benchmark(const std::string &label, const std::function<void()> &func,
               int runs = 10);