add_executable(text_io_test main.cpp ${PYPP_FILES})
target_include_directories(text_io_test PRIVATE ${CMAKE_SOURCE_DIR}/pypp)