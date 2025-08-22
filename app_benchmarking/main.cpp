#include "benchmark.h"
#include "compy_util/main_error_handler.h"
#include "py_range.h"
#include <array>
#include <iostream>
#include <vector>

int main() {
    try {
        const int size = 50000;

        // Testing std::array vs std::vector speed for initialization and
        // iteration.
        // Results: std::array is slightly faster for iteration and much faster
        // for initialization.
        // Conclusion: It is nice that std::array is so fast, but I just won't
        // use it because there is actually limited system wide memory you can
        // use for std::array since it is allocated on the stack. So its a
        // little risky unless it is small, and if it is small, then std::vector
        // is fast enough. There is probably cases where std::array is useful,
        // but I will ognore it for now.
        benchmark("std::vector initialization",
                  [=]() { std::vector<int> vec(size); });

        std::vector<int> vec2(size);
        benchmark("std::vector iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                vec2[i] = i;
            }
        });
        benchmark("std::vector range-based iteration", [&]() {
            for (auto &v : vec2) {
                vec2[0] = v; // just to use the variable
            }
        });
        benchmark("std::array initialization", [&]() {
            std::array<int, size> arr; // fine in main, watch for stack
        });

        // Test array iteration with a tranditional C++ for loop vs. a for loop
        // with PyRange.
        // Results: The PyRange is a little slower. But it adds at most 0.01ms
        // over 50,000 iterations. Which would be 0.2ms for 1 million
        // iterations. So, this very rarely would matter and therefore is fine
        // to use.
        std::array<int, size> arr2;
        benchmark("std::array iteration", [&]() {
            for (int i = 0; i < size; ++i) {
                arr2[i] = i;
            }
        });
        benchmark("std::array iteration with PyRange", [&]() {
            for (const auto &i : PyRange(size)) {
                arr2[i] = i;
            }
        });

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}