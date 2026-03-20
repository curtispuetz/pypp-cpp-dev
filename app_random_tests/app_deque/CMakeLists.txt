add_executable(deque_test main.cpp)
target_link_libraries(deque_test PRIVATE pypp_common GTest::gtest_main)
add_test(deque_test deque_test)
set_tests_properties(deque_test PROPERTIES LABELS "deque_test")
