#include "compy_time.h"
#include <compy_util/main_error_handler.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>

int main() {
    try {

        auto time_start = compy_time::start();
        compy_time::sleep(0.01); // won't sleeps exactly this short.
        auto elapsed = compy_time::end(time_start);
        std::cout << "Elapsed time: " << elapsed << " seconds" << std::endl;

        auto perf_start = compy_time::perf_counter_start();
        compy_time::sleep(0.01);
        auto perf_elapsed = compy_time::perf_counter_end(perf_start);
        std::cout << "High-resolution elapsed time: " << perf_elapsed
                  << " seconds" << std::endl;

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
