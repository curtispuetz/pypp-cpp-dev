add_executable(empty_vs_pre_init_list_test main.cpp ${PYPP_FILES} ${SRC_FILES})
target_include_directories(empty_vs_pre_init_list_test PRIVATE ${CMAKE_SOURCE_DIR}/pypp ${CMAKE_SOURCE_DIR}/src)