#include "benchmark.h"
#include <chrono>
#include <iostream>
#include <numeric>

void benchmark(const std::string &label, const std::function<void()> &func,
               int runs) {
    // Warm up cache and JIT
    auto start = std::chrono::high_resolution_clock::now();
    func();
    std::cout << label << ":" << std::endl;
    std::cout << std::chrono::duration<double, std::milli>(
                     std::chrono::high_resolution_clock::now() - start)
                     .count()
              << "ms (first call)" << std::endl;

    std::vector<double> timings;

    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        timings.push_back(std::chrono::duration<double, std::milli>(
                              std::chrono::high_resolution_clock::now() - start)
                              .count());
    }

    double avg = std::accumulate(timings.begin(), timings.end(), 0.0) / runs;
    std::cout << avg << " ms (average over " << runs << " runs)" << std::endl;
}