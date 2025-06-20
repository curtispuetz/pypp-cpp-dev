add_executable(optional_test main.cpp ${PYPP_FILES} ${SRC_FILES})
target_include_directories(optional_test PRIVATE ${CMAKE_SOURCE_DIR}/pypp ${CMAKE_SOURCE_DIR}/src)