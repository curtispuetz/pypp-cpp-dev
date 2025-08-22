#include "py_tuple.h"
#include "py_zip.h"
#include <array>
#include <compy_util/main_error_handler.h>
#include <cstdlib> // Required for EXIT_FAILURE
#include <vector>
int main() {
    try {
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::vector<std::string> words = {"one", "two", "three"};
        std::array<double, 4> decimals = {1.1, 2.2, 3.3, 4.4};

        std::cout
            << "Zipping three containers of different types and lengths.\n";
        std::cout << "The loop will stop when the shortest container (the list "
                     "of words) is exhausted.\n\n";

        // Use PyZip in a range-based for loop.
        // The type of 'item' will be a PyTup holding references to the
        // elements.
        for (const auto &[num, word, dec] : PyZip(numbers, words, decimals)) {

            // Print the values
            std::cout << "Number: " << num << ", Word: \"" << word
                      << "\", Decimal: " << dec << std::endl;

            // You can also modify the elements of the original containers if
            // they are not const For example, to prove we have a reference:
            // NOTE: do I care about this? I can't support it I think.
            if (num == 2) {
                word = "TWO (modified)";
            }
        }

        return 0;
    } catch (...) {
        handle_fatal_exception();
        return EXIT_FAILURE;
    }
}
