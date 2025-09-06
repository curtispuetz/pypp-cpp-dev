#include "pypp_random.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        pypp::random::Random rng = pypp::random::Random(42);
        double r = rng.random();
        pypp::print(r);
        int i = rng.randint(1, 10);
        pypp::print(i);
        pypp::PyList<int> vec = pypp::PyList({1, 2, 3, 4, 5});
        rng.shuffle(vec);
        pypp::print(vec);
        int choice = rng.choice(vec);
        pypp::print(choice);
        return 0;
    } catch (...) {
        pypp::handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
