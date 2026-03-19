#include "py_list.h"
#include "pypp_util/print.h"
#include <cstdlib> // Required for EXIT_FAILURE
#include <gtest/gtest.h>
#include <pypp_util/main_error_handler.h>

TEST(ListTest, CreateListOfIntsWithMove) {
    int a = 1;
    pypp::PyList<int> numbers = pypp::PyList({std::move(a), 2, 3, 4, 5});
    pypp::print(a); // still prints 1, because move doesn't change basic
                    // types like an int.
    ASSERT_EQ(numbers.len(), 5);
    ASSERT_EQ(numbers[0], 1);
}

TEST(ListTest, CreateListOfLists) {
    pypp::PyList<int> inner_list1 = pypp::PyList({1, 2});
    pypp::PyList<pypp::PyList<int>> list_of_lists = {std::move(inner_list1),
                                                     pypp::PyList<int>({3, 4}),
                                                     pypp::PyList<int>({5, 6})};
    pypp::print(list_of_lists);
    pypp::print(inner_list1); // prints [] because it was moved
    ASSERT_EQ(list_of_lists.len(), 3);
    ASSERT_EQ(list_of_lists[0].len(), 2);
    ASSERT_EQ(inner_list1.len(), 0);
}

TEST(ListTest, AssignmentWithMove) {
    pypp::PyList<pypp::PyList<int>> b =
        pypp::PyList({pypp::PyList({1, 2}), pypp::PyList({3, 4})});
    pypp::print(b);

    pypp::PyList<int> inner_list2 = pypp::PyList({5, 6});
    b[0] = inner_list2;            // copy
    b[1] = std::move(inner_list2); // move
    pypp::print(b);
    pypp::print(inner_list2); // prints [] because it was moved
    ASSERT_EQ(b[0].len(), 2);
    ASSERT_EQ(b[1].len(), 2);
    ASSERT_EQ(inner_list2.len(), 0);
}
