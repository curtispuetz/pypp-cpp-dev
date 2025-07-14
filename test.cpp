#include "pypp_util/main_error_handler.h"
#include <cstdint>
#include <cstdlib> // Required for EXIT_FAILURE

int main() {
    try {
        int8_t a = 1;
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}