add_executable(random_tests_test main.cpp ${PYPP_FILES} ${SRC_FILES})
target_include_directories(random_tests_test PRIVATE ${CMAKE_SOURCE_DIR}/pypp ${CMAKE_SOURCE_DIR}/src)