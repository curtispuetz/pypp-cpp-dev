add_executable(benchmarking main.cpp ${PYPP_FILES} ${SRC_FILES})
target_include_directories(benchmarking PRIVATE ${CMAKE_SOURCE_DIR}/pypp ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(app_empty_vs_pre_init_list)
add_subdirectory(app_list)