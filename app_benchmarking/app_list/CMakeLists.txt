add_executable(list_test main.cpp ${PYPP_FILES} ${SRC_FILES})
target_include_directories(list_test PRIVATE ${CMAKE_SOURCE_DIR}/pypp ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(app_creating)