#include "pypp_random.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <pypp_util/main_error_handler.h>

int main() {
    try {
        random::Random rng = random::Random(42);
        double r = rng.random();
        print(r);
        int i = rng.randint(1, 10);
        print(i);
        PyList<int> vec = PyList({1, 2, 3, 4, 5});
        rng.shuffle(vec);
        print(vec);
        int choice = rng.choice(vec);
        print(choice);
        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
