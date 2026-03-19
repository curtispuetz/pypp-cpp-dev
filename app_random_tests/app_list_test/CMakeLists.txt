add_executable(list_test_test main.cpp)
target_link_libraries(list_test_test PRIVATE pypp_common GTest::gtest_main)
add_test(list_test_test list_test_test)
set_tests_properties(list_test_test PROPERTIES LABELS "unit_test")