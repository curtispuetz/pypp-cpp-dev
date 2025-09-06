#include "pypp_time.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <iostream>
#include <pypp_util/main_error_handler.h>

int main() {
    try {

        auto time_start = pypp::time::start();
        pypp::time::sleep(0.01); // won't sleeps exactly this short.
        auto elapsed = pypp::time::end(time_start);
        std::cout << "Elapsed time: " << elapsed << " seconds" << std::endl;

        auto perf_start = pypp::time::perf_counter_start();
        pypp::time::sleep(0.01);
        auto perf_elapsed = pypp::time::perf_counter_end(perf_start);
        std::cout << "High-resolution elapsed time: " << perf_elapsed
                  << " seconds" << std::endl;

        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
