cmake_minimum_required(VERSION 4.0)
project(pypp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE PYPP_FILES pypp/*.cpp)

add_library(pypp_common STATIC ${SRC_FILES} ${PYPP_FILES})
target_include_directories(pypp_common PUBLIC ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/pypp)

add_executable(pyppDefaultExeName main.cpp)
target_link_libraries(pyppDefaultExeName PRIVATE pypp_common)

add_executable(test1 test.cpp)
target_link_libraries(test1 PRIVATE pypp_common)

add_subdirectory(app_benchmarking)
add_subdirectory(app_optional)
add_subdirectory(app_text_io)
add_subdirectory(app_random_tests)